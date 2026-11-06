/***************************************************************************
 *            benchmark_paving_superset.cpp
 *
 *  Read-only GridCell containment: tree, reference alignment and direct ROBDD.
 *  Part of Ariadne Paving, GPL-3.0-or-later.
 ****************************************************************************/

#include "paving/binary_tree_paving.hpp"
#include "paving/robdd_paving.hpp"
#include "paving/grid_cell_alignment.hpp"
#include "logging/logging.hpp"
#include "threading/threading_command_line_interface.hpp"
#include "threading/thread_manager.hpp"
#include "benchmark_command_line.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace Ariadne;
namespace CLI=Ariadne::PavingBenchmarkCommandLine;

namespace {

struct Selection {
    unsigned dimension=2u;
    unsigned probes=64u;
    unsigned iterations=1u;
    unsigned samples=7u;
    unsigned min_sample_ms=12u;
    unsigned max_iterations=32768u;
};

List<ArgumentParser> parsers(Selection& selection) {
    List<ArgumentParser> result;
    result.append(make_valued_argument_parser(
        "d","dimension","Grid dimension (1..6)",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            if(n<1u or n>6u) throw std::invalid_argument("dimension must be in [1,6]");
            return [&selection,n] { selection.dimension=n; };
        }));
    result.append(make_valued_argument_parser(
        "","probes-per-class","Number of prepared queries per class (4..1024)",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            if(n<4u or n>1024u) throw std::invalid_argument("probes-per-class must be in [4,1024]");
            return [&selection,n] { selection.probes=n; };
        }));
    result.append(make_valued_argument_parser(
        "i","iterations","Minimum complete batches per timing sample",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            if(n==0u) throw std::invalid_argument("iterations must be positive");
            return [&selection,n] { selection.iterations=n; };
        }));
    result.append(make_valued_argument_parser(
        "n","samples","Odd number of samples (>=3)",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            if(n<3u or n%2u==0u) throw std::invalid_argument("samples must be odd and >=3");
            return [&selection,n] { selection.samples=n; };
        }));
    result.append(make_valued_argument_parser(
        "","min-sample-ms","Minimum calibration time (ms)",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            if(n==0u or n>10000u) throw std::invalid_argument("min-sample-ms out of range");
            return [&selection,n] { selection.min_sample_ms=n; };
        }));
    result.append(make_valued_argument_parser(
        "","max-iterations","Maximum complete batches per sample",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            if(n==0u) throw std::invalid_argument("max-iterations must be positive");
            return [&selection,n] { selection.max_iterations=n; };
        }));
    return result;
}

template<class T> void keep_alive(T const& value) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("" : : "g"(&value) : "memory");
#else
    static void const* volatile sink=nullptr;
    sink=&value;
#endif
}

// This is the *previous* geometry-prefix lookup, retained here ONLY as
// a comparative measurement. The production method is RobddPaving::superset.
Bool reference_alignment_superset(
        RobddManager const& manager,RobddPaving const& paving,
        GridCell const& cell) {
    if(paving.grid()!=cell.grid()) {
        throw std::invalid_argument("reference query grid mismatch");
    }
    Robdd value=paving.robdd();
    if(value.is_false()) { return false; }
    GridCell common=common_root_cell(paving.root_cell(),cell);
    if(not relative_cell_path(common,paving.root_cell()).empty()) {
        return false;
    }
    BinaryWord path=relative_cell_path(common,cell);
    for(SizeType i=0u;i<path.size();++i) {
        if(value.is_terminal()) { return value.is_true(); }
        auto top=manager.top_variable(value);
        if(not top.has_value() or top->index()<i) {
            throw std::logic_error("invalid ROBDD variable ordering");
        }
        if(top->index()==i) {
            value=path[i]?manager.high(value):manager.low(value);
        }
    }
    return value.is_true();
}

GridCell at_index(GridCell const& root,unsigned index,unsigned depth) {
    BinaryWord bits=root.word();
    for(unsigned bit=depth;bit!=0u;--bit) {
        bits.push_back((index&(1u<<(bit-1u)))!=0u);
    }
    return GridCell(root.grid(),root.root_extent(),bits);
}

GridCell extend(GridCell cell,unsigned length,unsigned seed) {
    for(unsigned i=0u;i<length;++i) {
        cell=cell.split((seed&(1u<<(i%8u)))!=0u);
    }
    return cell;
}

struct ProbeGroup {
    std::string name;
    std::vector<GridCell> queries;
    Bool expected;
};

// Distinct categories deliberately exercise different prefix relationships.
// All objects and both backend pavings are prepared before measurement.
std::vector<ProbeGroup> make_probes(
        GridCell const& root,unsigned count,
        std::vector<GridCell> const& enabled,
        std::vector<GridCell> const& disabled) {
    std::vector<ProbeGroup> groups{
        {"internal",{},true},
        {"external",{},false},
        {"ancestor",{},false},
        {"descendant",{},true}};
    for(auto& group:groups) group.queries.reserve(count);

    BinaryWord wider_root_path=GridCell::primary_cell_path(
        root.grid().dimension(),root.root_extent()+1u,root.root_extent());
    for(unsigned i=0u;i<count;++i) {
        // Occupied cells at the actual binary depth of the representation.
        groups[0].queries.push_back(enabled[i%enabled.size()]);
        // Both internal holes and cells disjoint from the paving's root.
        groups[1].queries.push_back(i%4u==0u
            ?GridCell(root.grid(),root.root_extent(),make_binary_word("00"))
            :disabled[i%disabled.size()]);
        // Cells containing some enabled and disabled descendants, including
        // ancestors represented at a larger primary-cell extent.
        if(i%8u==0u) {
            groups[2].queries.push_back(root);
        } else if(i%8u==1u) {
            groups[2].queries.emplace_back(
                root.grid(),root.root_extent(),make_binary_word("0"));
        } else if(i%8u==2u) {
            groups[2].queries.emplace_back(
                root.grid(),root.root_extent()+1u,BinaryWord{});
        } else {
            unsigned bits=2u+(i%4u);
            BinaryWord prefix=root.word();
            BinaryWord const& word=enabled[i%enabled.size()].word();
            for(unsigned k=0u;k<bits;++k) {
                prefix.push_back(word[root.word().size()+k]);
            }
            groups[2].queries.emplace_back(
                root.grid(),root.root_extent(),prefix);
        }
        // Strict descendants of a fully enabled leaf, including alternate
        // primary-extents representing the same geometric descendants.
        GridCell deep=extend(
            enabled[i%enabled.size()],3u+(i%4u),i*53u+7u);
        if(i%7u==0u) {
            BinaryWord word=wider_root_path;
            word.append(deep.word());
            groups[3].queries.emplace_back(
                root.grid(),root.root_extent()+1u,word);
        } else {
            groups[3].queries.push_back(deep);
        }
    }
    return groups;
}

struct Timing {
    double nanoseconds_per_query;
    unsigned batches;
};

template<class Operation>
Timing median_query_ns(Operation&& operation,
        std::vector<GridCell> const& queries,Selection const& selection) {
    using Clock=std::chrono::steady_clock;
    unsigned batches=selection.iterations;
    auto run=[&](unsigned count) {
        SizeType count_true=0u;
        auto start=Clock::now();
        for(unsigned batch=0u;batch<count;++batch) {
            for(GridCell const& cell:queries) {
                if(operation(cell)) { ++count_true; }
            }
        }
        auto elapsed=std::chrono::duration<double,std::milli>(
            Clock::now()-start).count();
        keep_alive(count_true);
        return elapsed;
    };
    double elapsed=run(batches);
    while(elapsed<selection.min_sample_ms and
          batches<selection.max_iterations) {
        unsigned next=batches>selection.max_iterations/2u
            ?selection.max_iterations:batches*2u;
        if(next==batches) break;
        batches=next;
        elapsed=run(batches);
    }
    std::vector<double> samples;
    samples.reserve(selection.samples);
    for(unsigned sample=0u;sample<selection.samples;++sample) {
        double milliseconds=run(batches);
        double nanoseconds=milliseconds*1.0e6/
            (static_cast<double>(batches)*queries.size());
        if(not std::isfinite(nanoseconds) or nanoseconds<=0.0) {
            throw std::runtime_error("invalid query timing");
        }
        samples.push_back(nanoseconds);
    }
    std::sort(samples.begin(),samples.end());
    return {samples[selection.samples/2u],batches};
}

bool same_manager_statistics(
        RobddManagerStatistics const& x,RobddManagerStatistics const& y) {
    return x.node_count==y.node_count
        and x.node_capacity==y.node_capacity
        and x.unique_table_entries==y.unique_table_entries
        and x.ite_cache_entries==y.ite_cache_entries;
}

void benchmark(Selection const& selection) {
    Grid grid(selection.dimension);
    GridCell root(grid,1u,make_binary_word("01"));
    std::vector<GridCell> enabled,disabled;
    enabled.reserve(256u);
    disabled.reserve(256u);
    for(unsigned i=0u;i<256u;++i) {
        GridCell cell=at_index(root,i,8u);
        if(i%3u==0u or i%11u==0u) enabled.push_back(cell);
        else disabled.push_back(cell);
    }

    // Both pavings encode the same exact union, not an approximation.
    BinaryTreePaving tree(grid);
    for(GridCell const& cell:enabled) tree.adjoin(cell);
    tree.recombine();
    RobddManager manager;
    RobddPaving robdd(manager,root,manager.constant(false),0u);
    robdd.adjoin(enabled);
    robdd.recombine();
    auto groups=make_probes(root,selection.probes,enabled,disabled);
    LOGGING_PRINTLN("Read-only GridCell superset microbenchmark"
        <<" | dimension="<<selection.dimension
        <<" | queries/class="<<selection.probes
        <<" | samples="<<selection.samples
        <<" | min-sample-ms="<<selection.min_sample_ms
        <<" | concurrency="<<ThreadManager::instance().concurrency())
    LOGGING_PRINTLN("Tree vs reference prefix alignment vs optimized"
        " production ROBDD, all using identical precomputed queries.")
    LOGGING_PRINTLN("Time is median nanoseconds PER query. Probe generation,"
        " geometric validation and manager snapshots are never timed.")
    LOGGING_PRINTLN(std::string(117,'='))
    LOGGING_PRINTLN(std::left<<std::setw(13)<<"Category"
        <<"  "<<std::right<<std::setw(7)<<"Probes"
        <<"  "<<std::setw(10)<<"Tree ns"
        <<"  "<<std::setw(11)<<"Reference"
        <<"  "<<std::setw(11)<<"Optimized"
        <<"  "<<std::setw(12)<<"Opt/ref"
        <<"  "<<std::setw(12)<<"Opt/tree"
        <<"  "<<std::setw(9)<<"Tree bat"
        <<"  "<<std::setw(9)<<"Ref bat"
        <<"  "<<std::setw(9)<<"Opt bat")
    LOGGING_PRINTLN(std::string(117,'-'))
    for(ProbeGroup const& group:groups) {
        for(GridCell const& query:group.queries) {
            Bool expected=tree.superset(query);
            if(expected!=group.expected
                or reference_alignment_superset(manager,robdd,query)!=expected
                or robdd.superset(query)!=expected) {
                throw std::runtime_error(
                    "query classification or backend equivalence failure for "
                    +group.name);
            }
        }
        RobddManagerStatistics before=manager.statistics();
        Timing tree_t=median_query_ns(
            [&](GridCell const& q) { return tree.superset(q); },
            group.queries,selection);
        Timing ref_t=median_query_ns(
            [&](GridCell const& q) {
                return reference_alignment_superset(manager,robdd,q);
            },group.queries,selection);
        Timing opt_t=median_query_ns(
            [&](GridCell const& q) { return robdd.superset(q); },
            group.queries,selection);
        RobddManagerStatistics after=manager.statistics();
        if(not same_manager_statistics(before,after)) {
            throw std::runtime_error("superset query modified ROBDD manager");
        }
        LOGGING_PRINTLN(std::left<<std::setw(13)<<group.name
            <<"  "<<std::right<<std::setw(7)<<group.queries.size()
            <<std::fixed<<std::setprecision(2)
            <<"  "<<std::setw(10)<<tree_t.nanoseconds_per_query
            <<"  "<<std::setw(11)<<ref_t.nanoseconds_per_query
            <<"  "<<std::setw(11)<<opt_t.nanoseconds_per_query
            <<"  "<<std::setw(11)<<opt_t.nanoseconds_per_query/ref_t.nanoseconds_per_query<<"x"
            <<"  "<<std::setw(11)<<opt_t.nanoseconds_per_query/tree_t.nanoseconds_per_query<<"x"
            <<"  "<<std::setw(9)<<tree_t.batches
            <<"  "<<std::setw(9)<<ref_t.batches
            <<"  "<<std::setw(9)<<opt_t.batches)
    }
    LOGGING_PRINTLN(std::string(117,'='))
}

} // namespace

int main(int argc,char const* argv[]) {
    try {
        Logger::instance().configuration().set_verbosity(1u);
        Selection selection;
        ThreadingCommandLineInterface command_line;
        command_line.add(CommandLineContribution(
            "paving-superset-benchmark",parsers(selection)));
        List<String> args;
        for(int i=0;i<argc;++i) args.append(String(argv[i]));
        if(not command_line.acquire(args)) {
            return CLI::requests_help(args)?0:1;
        }
        if(selection.iterations>selection.max_iterations) {
            throw std::invalid_argument(
                "--iterations must not exceed --max-iterations");
        }
        benchmark(selection);
        return 0;
    } catch(std::exception const& error) {
        LOGGING_PRINTLN("benchmark_paving_superset: "<<error.what())
        return 1;
    }
}
