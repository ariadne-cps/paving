/***************************************************************************
 *            benchmark_paving_backends.cpp
 *
 *  Comparative geometric-set workloads for BinaryTreePaving and RobddPaving.
 *
 *  This file is part of Ariadne Paving, GPL-3.0-or-later.
 ****************************************************************************/

#include "paving/binary_tree_paving.hpp"
#include "paving/grid_paving.hpp"
#include "paving/robdd_paving.hpp"
#include "paving/paving_cell_operations.hpp"
#include "set/function_set.hpp"
#include "function/functional.hpp"
#include "logging/logging.hpp"
#include "threading/threading_command_line_interface.hpp"
#include "threading/thread_manager.hpp"
#include "benchmark_command_line.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace Ariadne;
namespace CLI = Ariadne::PavingBenchmarkCommandLine;

namespace {

struct Selection {
    unsigned iterations=1u;
    unsigned samples=5u;
    unsigned max_depth_bits=20u;
    std::vector<unsigned> dimensions{2u,3u};
    std::vector<unsigned> finenesses{1u,2u};
    std::vector<std::string> shapes{
        "aligned-box","rotated-box","aligned-ellipsoid",
        "rotated-ellipsoid","ball"};
    bool summary_only=false;
};

std::vector<std::string> parse_shapes(String const& value) {
    std::string text(value);
    if (text.empty() or text.front()==',' or text.back()==',') {
        throw std::invalid_argument("invalid shape list");
    }
    std::vector<std::string> result;
    std::size_t begin=0u;
    do {
        std::size_t end=text.find(',',begin);
        auto name=text.substr(
            begin,end==std::string::npos?end:end-begin);
        constexpr char const* known[]={
            "aligned-box","rotated-box","aligned-ellipsoid",
            "rotated-ellipsoid","ball"};
        if (std::find(std::begin(known),std::end(known),name)
                ==std::end(known) or
            std::find(result.begin(),result.end(),name)!=result.end()) {
            throw std::invalid_argument("unknown or duplicate shape");
        }
        result.push_back(name);
        if (end==std::string::npos) { break; }
        begin=end+1u;
    } while (true);
    return result;
}

List<ArgumentParser> parsers(Selection& selection) {
    List<ArgumentParser> result;
    result.append(make_valued_argument_parser(
        "i","iterations","Positive repetitions per sample",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            if (n==0u) throw std::invalid_argument("iterations must be positive");
            return [&selection,n] { selection.iterations=n; };
        }));
    result.append(make_valued_argument_parser(
        "n","samples","Odd number of timing samples, at least three",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            if (n<3u or n%2u==0u)
                throw std::invalid_argument("samples must be odd and >=3");
            return [&selection,n] { selection.samples=n; };
        }));
    result.append(make_valued_argument_parser(
        "d","dimensions","Comma-separated space dimensions (>=2)",
        [&selection](String const& value)->VoidFunction {
            auto dims=CLI::parse_unique_unsigned_list(value,2u);
            return [&selection,dims] { selection.dimensions=dims; };
        }));
    result.append(make_valued_argument_parser(
        "f","finenesses","Comma-separated grid-relative fineness values",
        [&selection](String const& value)->VoidFunction {
            // zero is a meaningful coarsest approximation.
            auto finenesses=CLI::parse_unique_unsigned_list(value,0u);
            return [&selection,finenesses] { selection.finenesses=finenesses; };
        }));
    result.append(make_valued_argument_parser(
        "g","shapes","Comma-separated: aligned-box,rotated-box,aligned-ellipsoid,rotated-ellipsoid,ball",
        [&selection](String const& value)->VoidFunction {
            auto shapes=parse_shapes(value);
            return [&selection,shapes] { selection.shapes=shapes; };
        }));
    result.append(make_valued_argument_parser(
        "m","max-depth-bits","Maximum conservative subdivision-bit budget (positive)",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            if(n==0u) throw std::invalid_argument("max-depth-bits must be positive");
            return [&selection,n] { selection.max_depth_bits=n; };
        }));
    result.append(make_unvalued_argument_parser(
        "","summary-only","Suppress per-operation table; show speed ratios only",
        [&selection] { selection.summary_only=true; }));
    return result;
}

EffectiveConstrainedImageSet make_shape(
        unsigned dimension,std::string const& shape) {
    // One common exact geometry is passed to both approximation backends.
    // The domain [0,1]^d contains all five compact families. The first
    // coordinate pair is orthogonally rotated for diagonal counterparts.
    auto x=EffectiveScalarMultivariateFunction::coordinates(dimension);
    RealBox domain(dimension,RealInterval(0,1));
    EffectiveConstrainedImageSet result(
        domain,EffectiveVectorMultivariateFunction::identity(dimension));

    if (shape=="aligned-box") {
        result.new_parameter_constraint(x[0]>=0.2_x);
        result.new_parameter_constraint(x[0]<=0.8_x);
        result.new_parameter_constraint(x[1]>=0.35_x);
        result.new_parameter_constraint(x[1]<=0.65_x);
    } else if (shape=="rotated-box") {
        auto sum=x[0]+x[1]-1.0_x;
        auto difference=x[0]-x[1];
        // a=.30, b=.15; support in x0,x1 is within [0,1].
        // Bounds approximate sqrt(2)*a and sqrt(2)*b.
        result.new_parameter_constraint(
            sum>=-0.4242640687119285_x);
        result.new_parameter_constraint(
            sum<=0.4242640687119285_x);
        result.new_parameter_constraint(
            difference>=-0.2121320343559643_x);
        result.new_parameter_constraint(
            difference<=0.2121320343559643_x);
    } else {
        auto first=x[0]-0.5_x;
        auto second=x[1]-0.5_x;
        EffectiveScalarMultivariateFunction quadratic=first*first*16
            +second*second*16;
        if (shape=="aligned-ellipsoid") {
            quadratic=first*first*100/9+second*second*400/9;
        } else if (shape=="rotated-ellipsoid") {
            auto sum=x[0]+x[1]-1.0_x;
            auto difference=x[0]-x[1];
            quadratic=sum*sum*50/9+difference*difference*200/9;
        } else if (shape!="ball") {
            throw std::invalid_argument("unknown shape");
        }
        for (unsigned i=2u;i<dimension;++i) {
            auto centered=x[i]-0.5_x;
            quadratic=quadratic+centered*centered*16;
        }
        result.new_parameter_constraint(quadratic<=1.0_x);
    }
    if (shape=="aligned-box" or shape=="rotated-box") {
        for (unsigned i=2u;i<dimension;++i) {
            result.new_parameter_constraint(x[i]>=0.25_x);
            result.new_parameter_constraint(x[i]<=0.75_x);
        }
    }
    return result;
}

template<class T>
void keep_alive(T const& value) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("" : : "g"(&value) : "memory");
#else
    static void const* volatile sink=nullptr;
    sink=&value;
#endif
}

template<class Operation>
double median_ms(Operation&& operation,unsigned iterations,unsigned samples) {
    std::vector<double> elapsed;
    elapsed.reserve(samples);
    for (unsigned sample=0u;sample<=samples;++sample) {
        auto start=std::chrono::steady_clock::now();
        for (unsigned i=0u;i<iterations;++i) {
            auto value=operation(i);
            keep_alive(value);
        }
        auto finish=std::chrono::steady_clock::now();
        if (sample!=0u) {
            elapsed.push_back(
                std::chrono::duration<double,std::milli>(
                    finish-start).count()/iterations);
        }
    }
    std::sort(elapsed.begin(),elapsed.end());
    double result=elapsed[samples/2u];
    if(not std::isfinite(result) or result<=0.0) {
        throw std::runtime_error("nonpositive or nonfinite benchmark time");
    }
    return result;
}

SizeType count_tree_nodes(BinaryTreeNode const& node) {
    if (node.is_leaf()) { return 1u; }
    return 1u+count_tree_nodes(*node.left_node())
              +count_tree_nodes(*node.right_node());
}

SizeType visit_leaves(SubPavingInterface const& paving) {
    SizeType checksum=0u;
    for (auto it=paving.begin(),end=paving.end();it!=end;++it) {
        checksum+=it->word().size()+1u;
    }
    return checksum;
}

SizeType visit_minimum(SubPavingInterface const& paving,Nat fineness) {
    SizeType checksum=0u;
    for_each_enabled_cell_at_fineness(
        paving,fineness,[&](GridCell const& cell) {
            checksum+=cell.word().size()+1u;
        });
    return checksum;
}

std::vector<GridCell> make_probes(Grid const& grid,unsigned dimension,
                                  unsigned fineness) {
    std::vector<GridCell> result;
    unsigned depth=std::min(8u,dimension*fineness);
    for (unsigned i=0u;i<16u;++i) {
        BinaryWord word;
        for (unsigned j=0u;j<depth;++j) {
            word.push_back(((i>>(j%4u))&1u)!=0u);
        }
        result.emplace_back(grid,0u,word);
    }
    return result;
}

SizeType probe_coverage(SubPavingInterface const& paving,
                        std::vector<GridCell> const& queries) {
    SizeType result=0u;
    for (GridCell const& query:queries) {
        if (paving.superset(query)) { ++result; }
    }
    return result;
}

struct Datum {
    std::string shape;
    std::string operation;
    std::string backend;
    unsigned dimension;
    unsigned fineness;
    double milliseconds;
    SizeType leaves;
    SizeType nodes;
};

struct Aggregate {
    unsigned count=0u;
    double log_sum=0.0;
    double worst=0.0;
    std::string worst_case;
    void add(double ratio,std::string const& name) {
        if (not std::isfinite(ratio) or ratio<=0.0) {
            throw std::runtime_error("invalid benchmark speed ratio");
        }
        ++count;
        log_sum+=std::log(ratio);
        if (ratio>worst) { worst=ratio; worst_case=name; }
    }
    double geometric_mean() const { return std::exp(log_sum/count); }
};

void validate_geometry(
        SubPavingInterface const& tree,SubPavingInterface const& robdd) {
    // Never pair timing rows unless both algorithms produce the same
    // geometric set. This check runs outside the timed measurements.
    for (auto it=tree.begin(),end=tree.end();it!=end;++it) {
        if(not robdd.superset(*it)) {
            throw std::runtime_error("ROBDD lacks a cell from tree approximation");
        }
    }
    for (auto it=robdd.begin(),end=robdd.end();it!=end;++it) {
        if(not tree.superset(*it)) {
            throw std::runtime_error("Tree lacks a cell from ROBDD approximation");
        }
    }
}

void append_case(std::vector<Datum>& data, Selection const& selection,
                 std::string const& shape, unsigned dimension,unsigned fineness) {
    Grid grid(dimension);
    GridCell primary(grid,0u,BinaryWord{});
    EffectiveConstrainedImageSet compact=make_shape(dimension,shape);
    EffectiveEuclideanCompactSetInterface const& set=compact;
    RobddManager manager;

    BinaryTreePaving tree(grid);
    RobddPaving robdd(manager,primary,manager.constant(false),0u);
    tree.adjoin_outer_approximation(set,fineness);
    tree.recombine();
    robdd.adjoin_outer_approximation(set,fineness);
    robdd.recombine();
    validate_geometry(tree,robdd);

    SizeType tree_leaves=tree.size();
    SizeType robdd_leaves=robdd.size();
    SizeType tree_nodes=count_tree_nodes(*tree.binary_tree());
    // Manager nodes include reusable nodes from all constructions and
    // cannot be attributed to one Boolean root. This initial count is
    // reported as shared-manager state, never as exclusive ROBDD memory.
    SizeType manager_nodes=manager.node_count();
    auto probes=make_probes(grid,dimension,fineness);

    auto record=[&](char const* name,auto&& tree_operation,
                    auto&& robdd_operation) {
        double tree_ms=median_ms(
            tree_operation,selection.iterations,selection.samples);
        double robdd_ms=median_ms(
            robdd_operation,selection.iterations,selection.samples);
        data.push_back({shape,name,"tree",dimension,fineness,
                        tree_ms,tree_leaves,tree_nodes});
        data.push_back({shape,name,"robdd",dimension,fineness,
                        robdd_ms,robdd_leaves,manager_nodes});
    };
    // The BDD manager is reused for all timed operations in this case.
    // In particular, repeated construction benefits from its unique table;
    // startup/manager allocation is NOT included (warm-manager mode).
    record("outer_compact",
        [&](unsigned) {
            BinaryTreePaving fresh(grid);
            fresh.adjoin_outer_approximation(set,fineness);
            fresh.recombine();
            return fresh.size();
        },
        [&](unsigned) {
            RobddPaving fresh(
                manager,primary,manager.constant(false),0u);
            fresh.adjoin_outer_approximation(set,fineness);
            fresh.recombine();
            return fresh.size();
        });

    record("copy",
        [&](unsigned) { BinaryTreePaving copy(tree); return copy.size(); },
        [&](unsigned) { RobddPaving copy(robdd); return copy.size(); });

    record("iterate_leaves",
        [&](unsigned) { return visit_leaves(tree); },
        [&](unsigned) { return visit_leaves(robdd); });

    record("iterate_min",
        [&](unsigned) { return visit_minimum(tree,fineness); },
        [&](unsigned) { return visit_minimum(robdd,fineness); });

    record("superset_16",
        [&](unsigned) { return probe_coverage(tree,probes); },
        [&](unsigned) { return probe_coverage(robdd,probes); });

    GridCell right_half=primary.split(true);
    record("copy_adjoin_cell",
        [&](unsigned) {
            BinaryTreePaving copy(tree);
            copy.adjoin(right_half);
            copy.recombine();
            return copy.size();
        },
        [&](unsigned) {
            RobddPaving copy(robdd);
            copy.adjoin(right_half);
            copy.recombine();
            return copy.size();
        });
}

void report(std::vector<Datum> const& data, Selection const& selection,
            unsigned skipped) {
    constexpr int width=117;
    LOGGING_PRINTLN("BinaryTree / ROBDD compact-set benchmark")
    LOGGING_PRINTLN("Iterations="<<selection.iterations
        <<"  samples="<<selection.samples
        <<"  manager=warm/shared  skipped_budget="<<skipped
        <<"  concurrency="<<ThreadManager::instance().concurrency()) 
    LOGGING_PRINTLN("BDDs are still sequential: --concurrency configures"
                   " the threading runtime but does not parallelize BDD operations.")
    LOGGING_PRINTLN("Node column: tree = physical nodes; ROBDD = manager total"
                   " at baseline (not exclusive memory).")
    if (not selection.summary_only) {
        LOGGING_PRINTLN(std::string(width,'='))
        LOGGING_PRINTLN(std::left
            <<std::setw(19)<<"Shape"<<"  "
            <<std::setw(5)<<"D"<<"  "<<std::setw(5)<<"F"<<"  "
            <<std::setw(8)<<"Backend"<<"  "
            <<std::setw(21)<<"Operation"<<"  "
            <<std::right<<std::setw(16)<<"Median (ms)"
            <<"  "<<std::setw(10)<<"Leaves"
            <<"  "<<std::setw(12)<<"Nodes")
        LOGGING_PRINTLN(std::string(width,'-'))
        for (std::size_t i=0u;i<data.size();++i) {
            Datum const& datum=data[i];
            if (i!=0u and (datum.dimension!=data[i-1u].dimension
                    or datum.fineness!=data[i-1u].fineness
                    or datum.shape!=data[i-1u].shape)) {
                LOGGING_PRINTLN(std::string(width,'-'))
            }
            LOGGING_PRINTLN(std::left
                <<std::setw(19)<<datum.shape<<"  "
                <<std::setw(5)<<datum.dimension<<"  "
                <<std::setw(5)<<datum.fineness<<"  "
                <<std::setw(8)<<datum.backend<<"  "
                <<std::setw(21)<<datum.operation<<"  "
                <<std::right<<std::fixed<<std::setprecision(4)
                <<std::setw(16)<<datum.milliseconds
                <<"  "<<std::setw(10)<<datum.leaves
                <<"  "<<std::setw(12)<<datum.nodes)
        }
        LOGGING_PRINTLN(std::string(width,'='))
    }

    std::map<std::string,Aggregate> by_operation;
    std::map<std::string,Aggregate> by_shape;
    Aggregate overall;
    for (std::size_t i=0u;i<data.size();i+=2u) {
        if (i+1u==data.size() or data[i].backend!="tree"
                or data[i+1u].backend!="robdd"
                or data[i].operation!=data[i+1u].operation
                or data[i].shape!=data[i+1u].shape
                or data[i].dimension!=data[i+1u].dimension
                or data[i].fineness!=data[i+1u].fineness) {
            throw std::runtime_error("unpaired tree/ROBDD benchmark rows");
        }
        double ratio=data[i+1u].milliseconds/data[i].milliseconds;
        std::string case_name=data[i].shape+"/"+data[i].operation
            +"/d"+std::to_string(data[i].dimension)
            +"/f"+std::to_string(data[i].fineness);
        by_operation[data[i].operation].add(ratio,case_name);
        by_shape[data[i].shape].add(ratio,case_name);
        overall.add(ratio,case_name);
    }
    LOGGING_PRINTLN("Speed ratios: ROBDD median / tree median"
        <<" (>1 means tree is faster; <1 means ROBDD is faster).")
    LOGGING_PRINTLN(std::string(68,'='))
    LOGGING_PRINTLN(std::left<<std::setw(24)<<"Group"
        <<"  "<<std::right<<std::setw(6)<<"Cases"
        <<"  "<<std::setw(13)<<"Geomean"
        <<"  "<<std::setw(13)<<"Worst")
    LOGGING_PRINTLN(std::string(68,'-'))
    auto line=[](std::string const& name,Aggregate const& a) {
        LOGGING_PRINTLN(std::left<<std::setw(24)<<name
            <<"  "<<std::right<<std::setw(6)<<a.count
            <<"  "<<std::fixed<<std::setprecision(3)
            <<std::setw(12)<<a.geometric_mean()<<"x"
            <<"  "<<std::setw(12)<<a.worst<<"x")
    };
    for (auto const& entry:by_operation) { line(entry.first,entry.second); }
    LOGGING_PRINTLN(std::string(68,'-'))
    for (auto const& entry:by_shape) { line(entry.first,entry.second); }
    LOGGING_PRINTLN(std::string(68,'-'))
    if (overall.count!=0u) {
        line("ALL",overall);
        LOGGING_PRINTLN("Worst case: "<<overall.worst_case)
    }
    LOGGING_PRINTLN(std::string(68,'='))
    LOGGING_PRINTLN("Each statistic is a geometric mean of paired ratios,"
        <<" not a sum of unrelated operation times.")
}

} // namespace

int main(int argc,char const* argv[]) {
    // ThreadingCommandLineInterface initializes ThreadManager and its
    // Logger thread registry. The measured BDD work remains sequential.
    try {
        Logger::instance().configuration().set_verbosity(1u);
        Selection selection;
        ThreadingCommandLineInterface command_line;
        command_line.add(CommandLineContribution(
            "paving-backends-benchmark",parsers(selection)));
        List<String> args;
        for (int i=0;i<argc;++i) { args.append(String(argv[i])); }
        if(not command_line.acquire(args)) {
            return CLI::requests_help(args)?0:1;
        }

        std::vector<Datum> data;
        unsigned skipped=0u;
        for (unsigned d:selection.dimensions) {
            for (unsigned f:selection.finenesses) {
                // The generic outer-approximation implementation can
                // explore up to roughly (d+f)*d binary decisions even
                // for small compact sets. Do not silently attempt the
                // infeasible full cartesian experiment matrix.
                const unsigned long long estimated=
                    (static_cast<unsigned long long>(d)+f)*d;
                if (estimated>selection.max_depth_bits) {
                    skipped+=static_cast<unsigned>(selection.shapes.size());
                    LOGGING_PRINTLN("SKIP dimension="<<d<<" fineness="<<f
                        <<" estimated_bits="<<estimated
                        <<" > max-depth-bits="<<selection.max_depth_bits)
                    continue;
                }
                for (auto const& shape:selection.shapes) {
                    LOGGING_PRINTLN("Measuring "<<shape<<" | d="<<d
                        <<" | fineness="<<f)
                    append_case(data,selection,shape,d,f);
                }
            }
        }
        if (data.empty()) {
            throw std::runtime_error("no cases within max-depth-bits budget");
        }
        report(data,selection,skipped);
        return 0;
    } catch(std::exception const& error) {
        LOGGING_PRINTLN("benchmark_paving_backends: "<<error.what())
        return 1;
    }
}
