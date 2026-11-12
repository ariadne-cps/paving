/***************************************************************************
 *            benchmarks/benchmark_paving_superset.cpp
 *
 *  Copyright  2026  Luca Geretti
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne Paving.
 *
 *  Ariadne Paving is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne Paving is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne Paving. If not, see <https://www.gnu.org/licenses/>.
 */

/** @file benchmarks/benchmark_paving_superset.cpp
 *  @brief Benchmark paving operations and backend implementations.
 */

#include "paving/binary_tree_paving.hpp"
#include "paving/robdd_paving.hpp"
#include "paving/grid_cell_alignment.hpp"
#include "logging/logging.hpp"
#include "threading/thread_manager.hpp"
#include "threading/threading_command_line_interface.hpp"
#include "ariadne-benchmarking.hpp"
#include "threading/thread_manager.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace Ariadne;
namespace Bench=Ariadne::Benchmarking;

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
            unsigned n=Bench::parse_unsigned<unsigned>(value);
            if(n<1u or n>6u) throw std::invalid_argument("dimension must be in [1,6]");
            return [&selection,n] { selection.dimension=n; };
        }));
    result.append(make_valued_argument_parser(
        "","probes-per-class","Number of prepared queries per class (4..1024)",
        [&selection](String const& value)->VoidFunction {
            unsigned n=Bench::parse_unsigned<unsigned>(value);
            if(n<4u or n>1024u) throw std::invalid_argument("probes-per-class must be in [4,1024]");
            return [&selection,n] { selection.probes=n; };
        }));
    result.append(make_valued_argument_parser(
        "i","iterations","Minimum complete batches per timing sample",
        [&selection](String const& value)->VoidFunction {
            unsigned n=Bench::parse_unsigned<unsigned>(value);
            if(n==0u) throw std::invalid_argument("iterations must be positive");
            return [&selection,n] { selection.iterations=n; };
        }));
    result.append(make_valued_argument_parser(
        "n","samples","Odd number of samples (>=3)",
        [&selection](String const& value)->VoidFunction {
            unsigned n=Bench::parse_unsigned<unsigned>(value);
            if(n<3u or n%2u==0u) throw std::invalid_argument("samples must be odd and >=3");
            return [&selection,n] { selection.samples=n; };
        }));
    result.append(make_valued_argument_parser(
        "","min-sample-ms","Minimum calibration time (ms)",
        [&selection](String const& value)->VoidFunction {
            unsigned n=Bench::parse_unsigned<unsigned>(value);
            if(n==0u or n>10000u) throw std::invalid_argument("min-sample-ms out of range");
            return [&selection,n] { selection.min_sample_ms=n; };
        }));
    result.append(make_valued_argument_parser(
        "","max-iterations","Maximum complete batches per sample",
        [&selection](String const& value)->VoidFunction {
            unsigned n=Bench::parse_unsigned<unsigned>(value);
            if(n==0u) throw std::invalid_argument("max-iterations must be positive");
            return [&selection,n] { selection.max_iterations=n; };
        }));
    return result;
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
    Bench::Statistics statistics;
};

template<class Operation>
Timing median_query_ns(Operation&& operation,
        std::vector<GridCell> const& queries,Selection const& selection,
        unsigned warmups) {
    unsigned batches=selection.iterations;
    auto batch=[&](unsigned count) {
        SizeType true_count=0u;
        for(unsigned i=0u;i<count;++i) {
            for(GridCell const& cell:queries)
                true_count+=static_cast<SizeType>(operation(cell));
        }
        Bench::keep_alive(true_count);
    };
    // A pilot is not part of reported samples; calibrate to the same
    // minimum batch duration without including probe construction.
    double elapsed=Bench::measure_ns([&]{batch(batches);});
    double const minimum_ns=static_cast<double>(selection.min_sample_ms)*1e6;
    while(elapsed<minimum_ns && batches<selection.max_iterations) {
        unsigned next=batches>selection.max_iterations/2u
            ?selection.max_iterations:batches*2u;
        if(next==batches)break;
        batches=next;
        elapsed=Bench::measure_ns([&]{batch(batches);});
    }
    Bench::Runner runner(warmups,1u);
    auto samples=runner.run("superset-queries",selection.samples,
        []{},[&]{batch(batches);},[]{},
        static_cast<double>(batches)*queries.size());
    auto stats=Bench::summarize(samples);
    if(not std::isfinite(stats.median)||stats.median<=0.0)
        throw std::runtime_error("invalid query timing");
    return {stats.median,batches,stats};
}

bool same_manager_statistics(
        RobddManagerStatistics const& x,RobddManagerStatistics const& y) {
    return x.node_count==y.node_count
        and x.node_capacity==y.node_capacity
        and x.unique_table_entries==y.unique_table_entries
        and x.ite_cache_entries==y.ite_cache_entries;
}

void benchmark(Selection const& selection,unsigned warmups) {
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
    Bench::Table table({
        {"Category"},{"Probes",0u,Bench::TableAlignment::Right},
        {"Tree ns",0u,Bench::TableAlignment::Right},
        {"Reference ns",0u,Bench::TableAlignment::Right},
        {"Optimized ns",0u,Bench::TableAlignment::Right},
        {"Opt/ref",0u,Bench::TableAlignment::Right},
        {"Opt/tree",0u,Bench::TableAlignment::Right},
        {"Tree bat",0u,Bench::TableAlignment::Right},
        {"Ref bat",0u,Bench::TableAlignment::Right},
        {"Opt bat",0u,Bench::TableAlignment::Right}
    },Bench::TableOptions::ruled());
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
            group.queries,selection,warmups);
        Timing ref_t=median_query_ns(
            [&](GridCell const& q) {
                return reference_alignment_superset(manager,robdd,q);
            },group.queries,selection,warmups);
        Timing opt_t=median_query_ns(
            [&](GridCell const& q) { return robdd.superset(q); },
            group.queries,selection,warmups);
        RobddManagerStatistics after=manager.statistics();
        if(not same_manager_statistics(before,after)) {
            throw std::runtime_error("superset query modified ROBDD manager");
        }
        table.add_row({
            group.name,std::to_string(group.queries.size()),
            Bench::Table::number(tree_t.nanoseconds_per_query,2u),
            Bench::Table::number(ref_t.nanoseconds_per_query,2u),
            Bench::Table::number(opt_t.nanoseconds_per_query,2u),
            Bench::Table::number(opt_t.nanoseconds_per_query/
                                 ref_t.nanoseconds_per_query,2u,"x"),
            Bench::Table::number(opt_t.nanoseconds_per_query/
                                 tree_t.nanoseconds_per_query,2u,"x"),
            std::to_string(tree_t.batches),std::to_string(ref_t.batches),
            std::to_string(opt_t.batches)
        });
    }
    LOGGING_PRINTLN(table.str())
}

} // namespace

int main(int argc,char const* argv[]) {
    try {
        Selection selection;
        Bench::CommandLine command_line;
        ThreadManager::instance();
        command_line.add(threading_command_line_contribution());
        command_line.add(CommandLineContribution(
            "paving-superset-benchmark",parsers(selection)));
        if(not command_line.acquire(argc,argv))return 0;
        if(selection.iterations>selection.max_iterations) {
            throw std::invalid_argument(
                "--iterations must not exceed --max-iterations");
        }
        benchmark(selection,command_line.warmups());
        return 0;
    } catch(std::exception const& error) {
        LOGGING_PRINTLN("benchmark_paving_superset: "<<error.what())
        return 1;
    }
}
