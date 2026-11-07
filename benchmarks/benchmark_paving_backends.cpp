/***************************************************************************
 *            benchmark_paving_backends.cpp
 *
 *  Comparative geometric-set workloads for BinaryTreePaving and RobddPaving.
 *
 *  This file is part of Ariadne Paving, GPL-3.0-or-later.
 ****************************************************************************/

#include "paving/binary_tree_paving.hpp"
#include "paving/binary_tree_paving.hpp"
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
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using namespace Ariadne;
namespace CLI = Ariadne::PavingBenchmarkCommandLine;

namespace {

struct Selection {
    unsigned iterations=1u;
    unsigned samples=7u;
    unsigned min_sample_ms=12u;
    unsigned max_iterations=32768u;
    unsigned max_depth_bits=26u;
    unsigned min_leaves=2u;
    unsigned max_replay_cells=16384u;
    unsigned retention_steps=12u;
    unsigned retention_fineness=3u;
    std::vector<unsigned> dimensions{2u,3u,4u};
    std::vector<unsigned> finenesses{2u,3u};
    std::vector<std::string> shapes{
        "aligned-box","rotated-box","aligned-ellipsoid",
        "rotated-ellipsoid","ball"};
    bool summary_only=false;
    bool run_retention=true;
    bool tree_iterator_cost=false;
    bool outer_phases=false;
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
        "","min-sample-ms","Minimum calibration time per timed sample (ms, positive)",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            if(n==0u or n>10000u) throw std::invalid_argument("min-sample-ms out of range");
            return [&selection,n] { selection.min_sample_ms=n; };
        }));
    result.append(make_valued_argument_parser(
        "","max-iterations","Upper limit of calibrated repetitions per sample",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            if(n==0u) throw std::invalid_argument("max-iterations must be positive");
            return [&selection,n] { selection.max_iterations=n; };
        }));
    result.append(make_valued_argument_parser(
        "","min-leaves","Skip geometrically degenerate cases below this enabled leaf count",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            return [&selection,n] { selection.min_leaves=n; };
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
    result.append(make_valued_argument_parser(
        "","max-replay-cells","Maximum precomputed enabled cells to replay per scenario",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            if(n==0u) throw std::invalid_argument("max-replay-cells must be positive");
            return [&selection,n] { selection.max_replay_cells=n; };
        }));
    result.append(make_valued_argument_parser(
        "","retention-steps","Repeated/different compact sets per manager (1..64)",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            if(n<1u or n>64u) throw std::invalid_argument("retention-steps must be in [1,64]");
            return [&selection,n] { selection.retention_steps=n; };
        }));
    result.append(make_valued_argument_parser(
        "","retention-fineness","Grid fineness for the standalone retention experiment (1..6)",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            if(n<1u or n>6u) throw std::invalid_argument("retention-fineness must be in [1,6]");
            return [&selection,n] { selection.retention_fineness=n; };
        }));
    result.append(make_unvalued_argument_parser(
        "","summary-only","Suppress per-operation table; show speed ratios only",
        [&selection] { selection.summary_only=true; }));
    result.append(make_unvalued_argument_parser(
        "","no-retention","Skip shared-manager post-root-destruction experiment",
        [&selection] { selection.run_retention=false; }));
    result.append(make_unvalued_argument_parser(
        "","tree-iterator-cost","Time safe owner snapshots versus borrowed stable-tree iteration",
        [&selection] { selection.tree_iterator_cost=true; }));
    result.append(make_unvalued_argument_parser(
        "","outer-phases","Profile geometric outer classification versus predicate calls and cell traversal",
        [&selection] { selection.outer_phases=true; }));
    return result;
}

EffectiveConstrainedImageSet make_shape(
        unsigned dimension,std::string const& shape) {
    // Exact dyadic parameters avoid ExactDouble rejecting inexact decimals.
    // Rotation uses the rational orthogonal matrix [3/5,4/5;-4/5,3/5].
    // The paired boxes and ellipsoids have identical volumes and radii.
    auto x=EffectiveScalarMultivariateFunction::coordinates(dimension);
    RealBox domain(dimension,RealInterval(0,1));
    EffectiveConstrainedImageSet result(
        domain,EffectiveVectorMultivariateFunction::identity(dimension));

    auto first=x[0]-0.5_x;
    auto second=x[1]-0.5_x;
    auto rotated_first=3*first+4*second;
    auto rotated_second=-4*first+3*second;

    if (shape=="aligned-box") {
        result.new_parameter_constraint(first>=-0.25_x);
        result.new_parameter_constraint(first<=0.25_x);
        result.new_parameter_constraint(second>=-0.125_x);
        result.new_parameter_constraint(second<=0.125_x);
    } else if (shape=="rotated-box") {
        // Multiplication by five makes the rotated constraints exact.
        result.new_parameter_constraint(rotated_first>=-1.25_x);
        result.new_parameter_constraint(rotated_first<=1.25_x);
        result.new_parameter_constraint(rotated_second>=-0.625_x);
        result.new_parameter_constraint(rotated_second<=0.625_x);
    } else {
        EffectiveScalarMultivariateFunction quadratic=first*first*16
            +second*second*16;
        if (shape=="aligned-ellipsoid") {
            quadratic=first*first*16+second*second*64;
        } else if (shape=="rotated-ellipsoid") {
            // 16*(u/5)^2 + 64*(v/5)^2 <= 1 iff 16*u^2+64*v^2 <= 25.
            quadratic=rotated_first*rotated_first*16
                +rotated_second*rotated_second*64;
        } else if (shape!="ball") {
            throw std::invalid_argument("unknown shape");
        }
        for (unsigned i=2u;i<dimension;++i) {
            auto centered=x[i]-0.5_x;
            quadratic=quadratic+centered*centered*
                (shape=="rotated-ellipsoid"?400:16);
        }
        result.new_parameter_constraint(
            quadratic<=(shape=="rotated-ellipsoid"?25.0_x:1.0_x));
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

struct Timing {
    double milliseconds;
    unsigned iterations;
};

// A timed sample is long enough to amortize clock precision and dispatch.
// Calibration is independent for each backend and operation. Each candidate
// run is a complete operation with a visible result (not a baseline subtraction).
template<class Operation>
Timing median_ms(Operation&& operation,unsigned min_iterations,
                 unsigned samples,unsigned minimum_ms,unsigned max_iterations) {
    using Clock=std::chrono::steady_clock;
    unsigned iterations=min_iterations;
    auto run=[&](unsigned count) {
        auto start=Clock::now();
        for(unsigned i=0u;i<count;++i) {
            auto value=operation(i);
            keep_alive(value);
        }
        return std::chrono::duration<double,std::milli>(
            Clock::now()-start).count();
    };
    // Perform an unmeasured warm-up. For cold-manager mode, each operation
    // has a fresh manager even though the CPU/cache gets warmed up.
    double elapsed=run(iterations);
    while(elapsed<static_cast<double>(minimum_ms) and
          iterations<max_iterations) {
        // Increase progressively without exceeding the user's safety budget.
        unsigned next=iterations>max_iterations/2u
            ?max_iterations:iterations*2u;
        if(next==iterations) break;
        iterations=next;
        elapsed=run(iterations);
    }
    std::vector<double> measurements;
    measurements.reserve(samples);
    for(unsigned sample=0u;sample<samples;++sample) {
        double ms=run(iterations)/static_cast<double>(iterations);
        if(not std::isfinite(ms) or ms<=0.0) {
            throw std::runtime_error("nonpositive/nonfinite benchmark timing");
        }
        measurements.push_back(ms);
    }
    std::sort(measurements.begin(),measurements.end());
    return {measurements[samples/2u],iterations};
}

// Only insertion/recombination occurs inside timed batches. All input
// paving copies are constructed outside the timed interval.
template<class Factory,class Mutate>
Timing median_mutation_ms(Factory&& factory,Mutate&& mutate,
        unsigned min_iterations,unsigned samples,unsigned minimum_ms,
        unsigned max_iterations) {
    using Clock=std::chrono::steady_clock;
    // Allocate independent copies before each timed chunk, then destroy
    // them afterwards. Repeated chunks accumulate an adequately long
    // timing sample without retaining thousands of copies simultaneously.
    constexpr unsigned maximum_live_copies=256u;
    unsigned iterations=min_iterations;
    auto run=[&](unsigned count) {
        double elapsed_ms=0.0;
        while(count!=0u) {
            unsigned chunk=std::min(count,maximum_live_copies);
            std::vector<decltype(factory())> copies;
            copies.reserve(chunk);
            for(unsigned i=0u;i<chunk;++i) {
                copies.push_back(factory());
            }
            auto start=Clock::now();
            for(auto& copy:copies) {
                mutate(*copy);
                keep_alive(*copy);
            }
            elapsed_ms+=std::chrono::duration<double,std::milli>(
                Clock::now()-start).count();
            count-=chunk;
        }
        return elapsed_ms;
    };
    double elapsed=run(iterations);
    while(elapsed<static_cast<double>(minimum_ms) and
          iterations<max_iterations) {
        iterations=iterations>max_iterations/2u
            ?max_iterations:iterations*2u;
        elapsed=run(iterations);
    }
    std::vector<double> measured;
    measured.reserve(samples);
    for(unsigned sample=0u;sample<samples;++sample) {
        double per_operation=run(iterations)/static_cast<double>(iterations);
        if(not std::isfinite(per_operation) or per_operation<=0.0) {
            throw std::runtime_error("invalid isolated mutation timing");
        }
        measured.push_back(per_operation);
    }
    std::sort(measured.begin(),measured.end());
    return {measured[samples/2u],iterations};
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

// This is a deliberately borrowed traversal, valid ONLY while the owning
// tree is stable. It is not a substitute for concurrent-safe owner iterators.
SizeType visit_borrowed_tree_leaves(BinaryTreeSubpaving const& paving) {
    SizeType checksum=0u;
    for(auto it=paving.BinaryTreeSubpaving::begin(),
             end=paving.BinaryTreeSubpaving::end();it!=end;++it) {
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

// RobddManager::Node is private, so sizeof() cannot be queried directly.
// This mirror has precisely the same declared fields and order in bdd/robdd.hpp.
// Its sizeof() is an ABI-dependent *estimate* (not a manager allocation metric).
struct RobddDecisionNodeLayoutEstimate {
    BddVariable variable;
    SizeType low;
    SizeType high;
};

// Recover physical DAG identity from the public BDD cofactors. Complemented
// edges must refer to the SAME physical decision node: normalize the low
// branch to regular polarity, matching the manager's canonical representation.
// Terminal references are 0/1 and cost no stored decision-node payload.
// This intentionally runs OUTSIDE all benchmark timings.
SizeType reachable_robdd_node_count(
        RobddManager const& manager,Robdd const& root) {
    using Key=std::tuple<SizeType,SizeType,SizeType>;
    std::map<Key,SizeType> unique_nodes;
    std::function<SizeType(Robdd const&)> visit=
        [&](Robdd const& value)->SizeType {
            if (value.is_terminal()) {
                return value.is_true()?1u:0u;
            }
            auto var=manager.top_variable(value);
            if (not var) {
                throw std::logic_error("nonterminal ROBDD without variable");
            }
            SizeType low=visit(manager.low(value));
            SizeType high=visit(manager.high(value));
            SizeType complement=low & 1u;
            if (complement!=0u) {
                low^=1u;
                high^=1u;
            }
            Key key{var->index(),low,high};
            auto found=unique_nodes.find(key);
            if (found==unique_nodes.end()) {
                SizeType id=unique_nodes.size()+1u;
                found=unique_nodes.emplace(key,id).first;
            }
            return (found->second<<1u)|complement;
        };
    visit(root);
    return unique_nodes.size();
}

void verify_reachable_node_oracle() {
    RobddManager manager;
    Robdd false_bdd=manager.constant(false);
    Robdd true_bdd=manager.constant(true);
    Robdd x=manager.variable(0u);
    Robdd y=manager.variable(1u);
    Robdd xor_bdd=manager.exclusive_or(x,y);
    if (reachable_robdd_node_count(manager,false_bdd)!=0u
        or reachable_robdd_node_count(manager,true_bdd)!=0u
        or reachable_robdd_node_count(manager,x)!=1u
        or reachable_robdd_node_count(manager,manager.negation(x))!=1u
        or reachable_robdd_node_count(manager,xor_bdd)!=2u
        or reachable_robdd_node_count(
            manager,manager.negation(xor_bdd))!=2u) {
        throw std::runtime_error("incorrect physical ROBDD DAG node accounting");
    }
}

SizeType checked_node_bytes(SizeType count,SizeType bytes_per_node) {
    if (count>std::numeric_limits<SizeType>::max()/bytes_per_node) {
        throw std::overflow_error("node payload estimate exceeds SizeType");
    }
    return count*bytes_per_node;
}

struct MemoryDatum {
    std::string shape;
    unsigned dimension;
    unsigned fineness;
    SizeType tree_nodes;
    SizeType robdd_reachable_nodes;
    SizeType robdd_manager_nodes;
    SizeType tree_payload_bytes;
    SizeType robdd_reachable_bytes;
    SizeType robdd_manager_payload_bytes;
    SizeType robdd_manager_after_nodes;
    RobddManagerStatistics fresh;
    RobddManagerStatistics after_warm;
};

void print_memory_results(std::vector<MemoryDatum> const& memory) {
    constexpr int width=140;
    LOGGING_PRINTLN("Estimated physical node payload (not RSS/heap footprint):")
    LOGGING_PRINTLN("sizeof(BinaryTreeNode)="<<sizeof(BinaryTreeNode)
        <<"  estimated sizeof(ROBDD decision node)="
        <<sizeof(RobddDecisionNodeLayoutEstimate)
        <<"  sizeof(BinaryTreePaving)="<<sizeof(BinaryTreePaving)
        <<"  sizeof(RobddPaving)="<<sizeof(RobddPaving)
        <<"  sizeof(RobddManager)="<<sizeof(RobddManager))
    LOGGING_PRINTLN("Tree B~=TreeNodes*sizeof(BinaryTreeNode);"
        <<" BDD B~=reachable distinct nonterminal nodes*estimated node size.")
    LOGGING_PRINTLN("Mgr N is a fresh manager after construction;"
        <<" Mgr warm is after warm timings. BDD N counts root-reachable nodes.")
    LOGGING_PRINTLN("Mgr B~=fresh manager node count*estimated node size;"
        <<" not per-paving memory; ITE and unique tables excluded.")
    LOGGING_PRINTLN("All estimates exclude allocator overhead, vector spare"
        <<" capacity, unique/computed tables, and dynamically owned metadata.")
    LOGGING_PRINTLN(std::string(width,'='))
    LOGGING_PRINTLN(std::left
        <<std::setw(19)<<"Shape"<<"  "
        <<std::setw(3)<<"D"<<"  "
        <<std::setw(3)<<"F"<<"  "
        <<std::right<<std::setw(9)<<"Tree N"
        <<"  "<<std::setw(9)<<"BDD N"
        <<"  "<<std::setw(9)<<"Mgr N"
        <<"  "<<std::setw(9)<<"Mgr warm"
        <<"  "<<std::setw(12)<<"Tree B~"
        <<"  "<<std::setw(12)<<"BDD B~"
        <<"  "<<std::setw(12)<<"Mgr B~"
        <<"  "<<std::setw(12)<<"BDD/Tree")
    LOGGING_PRINTLN(std::string(width,'-'))
    for (MemoryDatum const& datum:memory) {
        double ratio=static_cast<double>(datum.robdd_reachable_bytes)
                    /static_cast<double>(datum.tree_payload_bytes);
        LOGGING_PRINTLN(std::left
            <<std::setw(19)<<datum.shape<<"  "
            <<std::setw(3)<<datum.dimension<<"  "
            <<std::setw(3)<<datum.fineness<<"  "
            <<std::right<<std::setw(9)<<datum.tree_nodes
            <<"  "<<std::setw(9)<<datum.robdd_reachable_nodes
            <<"  "<<std::setw(9)<<datum.robdd_manager_nodes
            <<"  "<<std::setw(9)<<datum.robdd_manager_after_nodes
            <<"  "<<std::setw(12)<<datum.tree_payload_bytes
            <<"  "<<std::setw(12)<<datum.robdd_reachable_bytes
            <<"  "<<std::setw(12)<<datum.robdd_manager_payload_bytes
            <<"  "<<std::fixed<<std::setprecision(3)
            <<std::setw(11)<<ratio<<"x")
    }
    LOGGING_PRINTLN(std::string(width,'='))
}

SizeType manager_payload_lower_bound(RobddManagerStatistics const& stats) {
    SizeType nodes=checked_node_bytes(stats.node_capacity,stats.node_size_bytes);
    SizeType unique=checked_node_bytes(
        stats.unique_table_entries,stats.unique_table_entry_payload_bytes);
    SizeType computed=checked_node_bytes(
        stats.ite_cache_entries,stats.ite_cache_entry_payload_bytes);
    if(nodes>std::numeric_limits<SizeType>::max()-unique or
       nodes+unique>std::numeric_limits<SizeType>::max()-computed) {
        throw std::overflow_error("estimated manager payload overflow");
    }
    return nodes+unique+computed;
}

void print_manager_statistics(std::vector<MemoryDatum> const& memory) {
    LOGGING_PRINTLN("ROBDD manager statistics (fresh vs after workload)")
    LOGGING_PRINTLN("Payload B>= is a lower bound excluding std::map node"
        " links, allocator overhead and all other runtime allocations.")
    LOGGING_PRINTLN(std::string(105,'='))
    LOGGING_PRINTLN(std::left<<std::setw(19)<<"Shape"
        <<"  "<<std::setw(3)<<"D"<<"  "<<std::setw(3)<<"F"
        <<"  "<<std::setw(9)<<"Nodes"
        <<"  "<<std::setw(9)<<"Capacity"
        <<"  "<<std::setw(9)<<"Unique"
        <<"  "<<std::setw(9)<<"ITE"
        <<"  "<<std::setw(14)<<"Payload B>="
        <<"  "<<std::setw(7)<<"Stage")
    LOGGING_PRINTLN(std::string(105,'-'))
    for(auto const& datum:memory) {
        auto line=[&](RobddManagerStatistics const& stats,char const* stage) {
            LOGGING_PRINTLN(std::left<<std::setw(19)<<datum.shape
                <<"  "<<std::setw(3)<<datum.dimension
                <<"  "<<std::setw(3)<<datum.fineness
                <<"  "<<std::setw(9)<<stats.node_count
                <<"  "<<std::setw(9)<<stats.node_capacity
                <<"  "<<std::setw(9)<<stats.unique_table_entries
                <<"  "<<std::setw(9)<<stats.ite_cache_entries
                <<"  "<<std::setw(14)<<manager_payload_lower_bound(stats)
                <<"  "<<std::setw(7)<<stage)
        };
        line(datum.fresh,"fresh");
        line(datum.after_warm,"after");
    }
    LOGGING_PRINTLN(std::string(105,'='))
}

EffectiveConstrainedImageSet make_shifted_box(unsigned variant) {
    // Distinct exact compact inputs on an 8x8 lattice of translations:
    // all bounds have a power-of-two denominator and remain in [0,1]^2.
    // The first geometry is exactly the same in both retention runs.
    unsigned translation_column=variant%8u;
    unsigned translation_row=(variant/8u)%8u;
    double first_lower=0.125+static_cast<double>(translation_column)/16.0;
    double second_lower=0.125+static_cast<double>(translation_row)/16.0;
    auto x=EffectiveScalarMultivariateFunction::coordinates(2u);
    RealBox domain(2u,RealInterval(0,1));
    EffectiveConstrainedImageSet compact(
        domain,EffectiveVectorMultivariateFunction::identity(2u));
    compact.new_parameter_constraint(x[0]>=ExactDouble(first_lower));
    compact.new_parameter_constraint(x[0]<=ExactDouble(first_lower+0.25));
    compact.new_parameter_constraint(x[1]>=ExactDouble(second_lower));
    compact.new_parameter_constraint(x[1]<=ExactDouble(second_lower+0.25));
    return compact;
}

void print_manager_retention(Selection const& selection) {
    // Compare two *independent* managers:
    // SAME: repeatedly construct identical compact geometry;
    // VARIED: translate compact boxes by distinct dyadic offsets.
    // Both use identical operation counts and fineness. Every paving
    // and all root handles are destroyed BEFORE the corresponding row.
    Grid grid(2u);
    GridCell primary(grid,0u,BinaryWord{});
    LOGGING_PRINTLN("Manager retention after destroying every paving root"
        <<" (d=2, fineness="<<selection.retention_fineness
        <<", steps="<<selection.retention_steps<<")")
    LOGGING_PRINTLN("SAME repeats one exact box; VARIED uses distinct"
        " translated exact boxes (discretizations can coincide).")
    LOGGING_PRINTLN(std::string(128,'='))
    LOGGING_PRINTLN(std::left<<std::setw(9)<<"Scenario"
        <<"  "<<std::setw(5)<<"Step"
        <<"  "<<std::setw(8)<<"Variant"
        <<"  "<<std::setw(8)<<"Root N"
        <<"  "<<std::setw(9)<<"Mgr N"
        <<"  "<<std::setw(9)<<"Capacity"
        <<"  "<<std::setw(9)<<"Unique"
        <<"  "<<std::setw(9)<<"ITE"
        <<"  "<<std::setw(13)<<"Payload B>=")
    LOGGING_PRINTLN(std::string(128,'-'))
    for(unsigned scenario=0u;scenario<2u;++scenario) {
        RobddManager manager;
        char const* label=scenario==0u?"SAME":"VARIED";
        auto emit=[&](unsigned step,unsigned variant,SizeType reachable) {
            RobddManagerStatistics stats=manager.statistics();
            LOGGING_PRINTLN(std::left<<std::setw(9)<<label
                <<"  "<<std::setw(5)<<step
                <<"  "<<std::setw(8)<<variant
                <<"  "<<std::setw(8)<<reachable
                <<"  "<<std::setw(9)<<stats.node_count
                <<"  "<<std::setw(9)<<stats.node_capacity
                <<"  "<<std::setw(9)<<stats.unique_table_entries
                <<"  "<<std::setw(9)<<stats.ite_cache_entries
                <<"  "<<std::setw(13)<<manager_payload_lower_bound(stats))
        };
        emit(0u,0u,0u);
        for(unsigned step=1u;step<=selection.retention_steps;++step) {
            unsigned variant=scenario==0u?0u:step-1u;
            SizeType reachable=0u;
            {
                EffectiveConstrainedImageSet compact=make_shifted_box(variant);
                EffectiveEuclideanCompactSetInterface const& set=compact;
                RobddPaving paving(
                    manager,primary,manager.constant(false),0u);
                paving.adjoin_outer_approximation(
                    set,selection.retention_fineness);
                paving.recombine();
                reachable=reachable_robdd_node_count(
                    manager,paving.robdd());
                keep_alive(paving);
            }
            emit(step,variant,reachable);
        }
        if(scenario==0u) {
            LOGGING_PRINTLN(std::string(128,'-'))
        }
    }
    LOGGING_PRINTLN(std::string(128,'='))
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
    unsigned iterations;
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


// Opt-in comparison for the *common* cost of approximating the same set.
// The empty ROBDD backend enumerates candidate cells using these exact
// separated/covers/stop decisions, then encodes them in its manager.
// No production logic or performance-critical code is instrumented.
struct OuterTraceEntry {
    ExactBoxType box;
    BinaryWord path;
    Bool check_covers;
    Bool separated;
    Bool covered;
    Bool subdivide;
};

struct OuterSelection {
    GridCell enclosing_cell;
    std::vector<GridCell> selected;
    std::vector<OuterTraceEntry> trace;
    SizeType visited=0u;
    SizeType separated_calls=0u;
    SizeType covers_calls=0u;
    SizeType subdivisions=0u;
};

OuterSelection select_outer_cells(
        EffectiveEuclideanCompactSetInterface const& set,
        Grid const& grid,unsigned fineness,Bool keep_trace) {
    Nat extent=GridCell::smallest_enclosing_primary_cell_extent(
        set.bounding_box(),grid);
    Nat dimensions=static_cast<Nat>(grid.dimension());
    constexpr Nat nat_limit=std::numeric_limits<Nat>::max();
    constexpr SizeType size_limit=std::numeric_limits<SizeType>::max();
    if(dimensions==0u or extent>nat_limit-dimensions
       or extent+dimensions>nat_limit-fineness) {
        throw std::overflow_error("invalid outer diagnostic extent");
    }
    Nat root_extent=extent+dimensions;
    SizeType levels=static_cast<SizeType>(root_extent)+fineness;
    if(levels>size_limit/dimensions) {
        throw std::overflow_error("outer diagnostic depth exceeds SizeType");
    }
    SizeType target_depth=levels*dimensions;
    GridCell root(grid,root_extent,BinaryWord{});
    OuterSelection result{root, {}, {}, 0u, 0u, 0u, 0u};
    auto const* open_set=dynamic_cast<
        EffectiveEuclideanOpenSetInterface const*>(
            static_cast<EuclideanSetInterfaceBase const*>(&set));

    std::function<void(GridCell const&,SizeType)> visit=
        [&](GridCell const& cell,SizeType depth) {
            ++result.visited;
            ++result.separated_calls;
            Bool separated=definitely(set.separated(cell.box()));
            Bool check_cover=not separated and open_set!=nullptr;
            Bool covered=false;
            if(check_cover) {
                ++result.covers_calls;
                covered=definitely(open_set->covers(cell.box()));
            }
            Bool subdivide=not separated and not covered and depth<target_depth;
            if(keep_trace) {
                result.trace.push_back({
                    cell.box(),cell.word(),check_cover,separated,covered,subdivide});
            }
            if(separated) { return; }
            if(not subdivide) {
                result.selected.push_back(cell);
                return;
            }
            ++result.subdivisions;
            visit(cell.split(false),depth+1u);
            visit(cell.split(true),depth+1u);
        };
    visit(root,0u);
    return result;
}

// Re-run exactly those geometric predicates whose results determined the
// recorded topology; boxes are precomputed, so no GridCell split occurs.
SizeType replay_outer_predicates(
        EffectiveEuclideanCompactSetInterface const& set,
        std::vector<OuterTraceEntry> const& trace) {
    auto const* open_set=dynamic_cast<
        EffectiveEuclideanOpenSetInterface const*>(
            static_cast<EuclideanSetInterfaceBase const*>(&set));
    SizeType checksum=0u;
    for(OuterTraceEntry const& entry:trace) {
        Bool separated=definitely(set.separated(entry.box));
        checksum+=static_cast<SizeType>(separated);
        if(entry.check_covers) {
            if(open_set==nullptr) throw std::logic_error(
                "outer predicate trace expects a missing open set");
            checksum+=static_cast<SizeType>(
                definitely(open_set->covers(entry.box)));
        }
    }
    return checksum;
}

// Replay the identical recorded branch topology without predicates.
// This still constructs all GridCells and calculates their exact boxes.
SizeType replay_outer_geometry(OuterSelection const& reference,
                               Bool check_paths) {
    SizeType index=0u;
    SizeType checksum=0u;
    std::function<void(GridCell const&)> visit=
        [&](GridCell const& cell) {
            if(index>=reference.trace.size())
                throw std::logic_error("outer traversal trace exhausted");
            OuterTraceEntry const& entry=reference.trace[index++];
            if(check_paths and cell.word()!=entry.path)
                throw std::logic_error("outer traversal path mismatch");
            checksum+=cell.word().size()+cell.box().dimension()+1u;
            if(entry.subdivide) {
                visit(cell.split(false));
                visit(cell.split(true));
            }
        };
    visit(reference.enclosing_cell);
    if(index!=reference.trace.size())
        throw std::logic_error("outer traversal trace not fully consumed");
    return checksum;
}

void profile_outer_phases(
        Selection const& selection,std::string const& shape,
        unsigned dimension,unsigned fineness,
        EffectiveEuclideanCompactSetInterface const& set,Grid const& grid,
        BinaryTreePaving const& tree,RobddPaving const& robdd) {
    OuterSelection recorded=select_outer_cells(set,grid,fineness,true);
    if(recorded.visited!=recorded.trace.size()
       or recorded.separated_calls!=recorded.visited
       or recorded.selected.empty()) {
        throw std::logic_error("inconsistent outer selection counters");
    }
    // Check the candidate set against *both* production backends before
    // attributing time to this diagnostic traversal.
    BinaryTreePaving oracle(grid);
    for(GridCell const& cell:recorded.selected) oracle.adjoin(cell);
    oracle.recombine();
    validate_geometry(tree,oracle);
    validate_geometry(robdd,oracle);

    SizeType expected_predicates=0u;
    for(OuterTraceEntry const& entry:recorded.trace) {
        expected_predicates+=static_cast<SizeType>(entry.separated)
                            +static_cast<SizeType>(entry.covered);
    }
    if(replay_outer_predicates(set,recorded.trace)!=expected_predicates) {
        throw std::logic_error("geometric predicates changed on replay");
    }
    SizeType geometry_checksum=replay_outer_geometry(recorded,true);

    Timing selection_time=median_ms(
        [&](unsigned) {
            OuterSelection result=select_outer_cells(set,grid,fineness,false);
            if(result.visited!=recorded.visited
               or result.selected.size()!=recorded.selected.size()) {
                throw std::logic_error("unstable outer candidate selection");
            }
            keep_alive(result);
            return result.visited+result.selected.size();
        },selection.iterations,selection.samples,
        selection.min_sample_ms,selection.max_iterations);
    Timing predicate_time=median_ms(
        [&](unsigned) { return replay_outer_predicates(set,recorded.trace); },
        selection.iterations,selection.samples,
        selection.min_sample_ms,selection.max_iterations);
    Timing geometry_time=median_ms(
        [&](unsigned) { return replay_outer_geometry(recorded,false); },
        selection.iterations,selection.samples,
        selection.min_sample_ms,selection.max_iterations);

    LOGGING_PRINTLN("OUTER_PHASES shape="<<shape
        <<" d="<<dimension<<" f="<<fineness
        <<" visited="<<recorded.visited
        <<" selected="<<recorded.selected.size()
        <<" separated_calls="<<recorded.separated_calls
        <<" covers_calls="<<recorded.covers_calls
        <<" subdivisions="<<recorded.subdivisions
        <<" candidate_checksum="<<geometry_checksum
        <<" selection_ms="<<std::setprecision(9)<<selection_time.milliseconds
        <<" predicates_ms="<<predicate_time.milliseconds
        <<" geometry_walk_ms="<<geometry_time.milliseconds
        <<" predicate_over_selection="
        <<predicate_time.milliseconds/selection_time.milliseconds)
    LOGGING_PRINTLN("OUTER_PHASES are independently timed overlapping workloads,"
        <<" not an additive cost decomposition; the selected cells were"
        <<" semantically checked against tree and ROBDD before timing.")
}

void append_case(std::vector<Datum>& data,std::vector<MemoryDatum>& memory,
                 Selection const& selection,std::string const& shape,
                 unsigned dimension,unsigned fineness) {
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

    // Snapshot node counts BEFORE geometric validation or timed operations,
    // both of which may allocate additional manager nodes / cache entries.
    SizeType tree_nodes=count_tree_nodes(*tree.binary_tree());
    SizeType reachable_nodes=reachable_robdd_node_count(
        manager,robdd.robdd());
    SizeType manager_nodes=manager.node_count();
    if (reachable_nodes>manager_nodes) {
        throw std::logic_error("reachable nodes exceed allocated manager nodes");
    }
    memory.push_back({shape,dimension,fineness,tree_nodes,
        reachable_nodes,manager_nodes,
        checked_node_bytes(tree_nodes,sizeof(BinaryTreeNode)),
        checked_node_bytes(reachable_nodes,sizeof(RobddDecisionNodeLayoutEstimate)),
        checked_node_bytes(manager_nodes,sizeof(RobddDecisionNodeLayoutEstimate)),
        manager_nodes,manager.statistics(),manager.statistics()});

    validate_geometry(tree,robdd);
    SizeType tree_leaves=tree.size();
    SizeType robdd_leaves=robdd.size();
    if(tree_leaves<selection.min_leaves) {
        LOGGING_PRINTLN("SKIP degenerate geometry: "<<shape
            <<" d="<<dimension<<" f="<<fineness
            <<" leaves="<<tree_leaves
            <<" < min-leaves="<<selection.min_leaves)
        memory.pop_back();
        return;
    }
    if(selection.outer_phases) {
        profile_outer_phases(selection,shape,dimension,fineness,set,grid,
                             tree,robdd);
    }
    auto probes=make_probes(grid,dimension,fineness);

    auto record=[&](char const* name,auto&& tree_operation,
                    auto&& robdd_operation) {
        Timing tree_time=median_ms(tree_operation,selection.iterations,
            selection.samples,selection.min_sample_ms,selection.max_iterations);
        Timing robdd_time=median_ms(robdd_operation,selection.iterations,
            selection.samples,selection.min_sample_ms,selection.max_iterations);
        data.push_back({shape,name,"tree",dimension,fineness,
                        tree_time.milliseconds,tree_leaves,tree_nodes,
                        tree_time.iterations});
        data.push_back({shape,name,"robdd",dimension,fineness,
                        robdd_time.milliseconds,robdd_leaves,reachable_nodes,
                        robdd_time.iterations});
    };
    // The BDD manager is reused for all timed operations in this case.
    // In particular, repeated construction benefits from its unique table;
    // startup/manager allocation is NOT included (warm-manager mode).
    record("outer_warm",
        [&](unsigned) {
            BinaryTreePaving fresh(grid);
            fresh.adjoin_outer_approximation(set,fineness);
            fresh.recombine();
            return fresh;
        },
        [&](unsigned) {
            RobddPaving fresh(
                manager,primary,manager.constant(false),0u);
            fresh.adjoin_outer_approximation(set,fineness);
            fresh.recombine();
            return fresh;
        });

    record("outer_cold",
        [&](unsigned) {
            BinaryTreePaving fresh(grid);
            fresh.adjoin_outer_approximation(set,fineness);
            fresh.recombine();
            return fresh.size();
        },
        [&](unsigned) {
            RobddManager cold_manager;
            RobddPaving fresh(cold_manager,primary,
                cold_manager.constant(false),0u);
            fresh.adjoin_outer_approximation(set,fineness);
            fresh.recombine();
            return fresh.size();
        });

    // Replay only the production bulk construction path. Extract and
    // validate cells before timing so geometric set solving and iteration
    // are not included in the cell-insertion cost.
    if(tree_leaves<=selection.max_replay_cells) {
        std::vector<GridCell> prepared;
        prepared.reserve(tree_leaves);
        for(auto it=tree.begin(),end=tree.end();it!=end;++it) {
            prepared.push_back(*it);
        }
        if(prepared.size()!=tree_leaves) {
            throw std::runtime_error("inconsistent prepared-cell count");
        }
        {
            // The tree is an independent geometric oracle for replay.
            // Use a separate manager so the checks cannot affect timings.
            BinaryTreePaving verification_tree(grid);
            RobddManager verification_manager;
            RobddPaving verification_bdd(
                verification_manager,primary,
                verification_manager.constant(false),0u);
            for(GridCell const& cell:prepared) {
                verification_tree.adjoin(cell);
            }
            verification_bdd.adjoin(prepared);
            verification_tree.recombine();
            verification_bdd.recombine();
            validate_geometry(tree,verification_tree);
            validate_geometry(tree,verification_bdd);
        }
        record("replay_bulk",
            [&](unsigned) {
                BinaryTreePaving rebuilt(grid);
                for(GridCell const& cell:prepared) rebuilt.adjoin(cell);
                rebuilt.recombine();
                return rebuilt;
            },
            [&](unsigned) {
                RobddPaving rebuilt(
                    manager,primary,manager.constant(false),0u);
                rebuilt.adjoin(prepared);
                rebuilt.recombine();
                return rebuilt;
            });
    } else {
        LOGGING_PRINTLN("SKIP replay_bulk: "<<shape
            <<" d="<<dimension<<" f="<<fineness
            <<" leaves="<<tree_leaves
            <<" > max-replay-cells="<<selection.max_replay_cells)
    }

    record("copy",
        [&](unsigned) { return BinaryTreePaving(tree); },
        [&](unsigned) { return RobddPaving(robdd); });

    record("iterate_leaves",
        [&](unsigned) { return visit_leaves(tree); },
        [&](unsigned) { return visit_leaves(robdd); });

    record("iterate_min",
        [&](unsigned) { return visit_minimum(tree,fineness); },
        [&](unsigned) { return visit_minimum(robdd,fineness); });

    if(selection.tree_iterator_cost) {
        // Compare only representations of the same immutable tree. Ordinary
        // BinaryTreePaving iterators own a deep copy; borrowed base iterators
        // are valid here because this scenario has no concurrent writers.
        BinaryTreeSubpaving const& stable_tree=tree;
        SizeType safe_checksum=visit_leaves(tree);
        SizeType borrowed_checksum=visit_borrowed_tree_leaves(stable_tree);
        if(safe_checksum!=borrowed_checksum) {
            throw std::runtime_error(
                "stable borrowed-tree traversal disagrees with owner snapshot");
        }

        Timing owner_begin=median_ms(
            [&](unsigned) { return tree.begin(); },
            selection.iterations,selection.samples,
            selection.min_sample_ms,selection.max_iterations);
        Timing borrowed_begin=median_ms(
            [&](unsigned) { return stable_tree.BinaryTreeSubpaving::begin(); },
            selection.iterations,selection.samples,
            selection.min_sample_ms,selection.max_iterations);
        Timing owner_visit=median_ms(
            [&](unsigned) { return visit_leaves(tree); },
            selection.iterations,selection.samples,
            selection.min_sample_ms,selection.max_iterations);
        Timing borrowed_visit=median_ms(
            [&](unsigned) { return visit_borrowed_tree_leaves(stable_tree); },
            selection.iterations,selection.samples,
            selection.min_sample_ms,selection.max_iterations);

        LOGGING_PRINTLN("TREE_ITERATOR_COST shape="<<shape
            <<" d="<<dimension<<" f="<<fineness
            <<" leaves="<<tree_leaves<<" nodes="<<tree_nodes
            <<" owner_begin_ms="<<std::setprecision(9)<<owner_begin.milliseconds
            <<" borrowed_begin_ms="<<borrowed_begin.milliseconds
            <<" owner_visit_ms="<<owner_visit.milliseconds
            <<" borrowed_visit_ms="<<borrowed_visit.milliseconds
            <<" begin_ratio_owner_over_borrowed="
            <<owner_begin.milliseconds/borrowed_begin.milliseconds
            <<" visit_ratio_owner_over_borrowed="
            <<owner_visit.milliseconds/borrowed_visit.milliseconds);
    }

    record("superset_16",
        [&](unsigned) { return probe_coverage(tree,probes); },
        [&](unsigned) { return probe_coverage(robdd,probes); });

    GridCell right_half=primary.split(true);
    Timing tree_adjoin=median_mutation_ms(
        [&] { return std::make_unique<BinaryTreePaving>(tree); },
        [&](BinaryTreePaving& copy) {
            copy.adjoin(right_half);
            copy.recombine();
        },selection.iterations,selection.samples,selection.min_sample_ms,
        selection.max_iterations);
    Timing robdd_adjoin=median_mutation_ms(
        [&] { return std::make_unique<RobddPaving>(robdd); },
        [&](RobddPaving& copy) {
            copy.adjoin(right_half);
            copy.recombine();
        },selection.iterations,selection.samples,selection.min_sample_ms,
        selection.max_iterations);
    data.push_back({shape,"adjoin_recombine","tree",dimension,fineness,
        tree_adjoin.milliseconds,tree_leaves,tree_nodes,
        tree_adjoin.iterations});
    data.push_back({shape,"adjoin_recombine","robdd",dimension,fineness,
        robdd_adjoin.milliseconds,robdd_leaves,reachable_nodes,
        robdd_adjoin.iterations});
    memory.back().robdd_manager_after_nodes=manager.node_count();
    memory.back().after_warm=manager.statistics();
}

void report(std::vector<Datum> const& data,
            std::vector<MemoryDatum> const& memory,
            Selection const& selection,unsigned skipped) {
    constexpr int width=132;
    LOGGING_PRINTLN("BinaryTree / ROBDD compact-set benchmark")
    LOGGING_PRINTLN("Minimum iterations="<<selection.iterations
        <<"  samples="<<selection.samples
        <<"  min_sample_ms="<<selection.min_sample_ms
        <<"  max_iterations="<<selection.max_iterations
        <<"  min_leaves="<<selection.min_leaves
        <<"  manager=warm/shared  skipped_budget="<<skipped
        <<"  concurrency="<<ThreadManager::instance().concurrency()) 
    LOGGING_PRINTLN("BDDs are still sequential: --concurrency configures"
                   " the threading runtime but does not parallelize BDD operations.")
    LOGGING_PRINTLN("Node column: tree physical nodes; ROBDD root-reachable"
                   " distinct nodes (not manager total).")
    if (not selection.summary_only) {
        LOGGING_PRINTLN(std::string(width,'='))
        LOGGING_PRINTLN(std::left
            <<std::setw(19)<<"Shape"<<"  "
            <<std::setw(5)<<"D"<<"  "<<std::setw(5)<<"F"<<"  "
            <<std::setw(8)<<"Backend"<<"  "
            <<std::setw(21)<<"Operation"<<"  "
            <<std::right<<std::setw(16)<<"Median (ms)"
            <<"  "<<std::setw(10)<<"Leaves"
            <<"  "<<std::setw(12)<<"Nodes"
            <<"  "<<std::setw(8)<<"Iter")
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
                <<"  "<<std::setw(12)<<datum.nodes
                <<"  "<<std::setw(8)<<datum.iterations)
        }
        LOGGING_PRINTLN(std::string(width,'='))
    }

    print_memory_results(memory);
    print_manager_statistics(memory);

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
        verify_reachable_node_oracle();
        ThreadingCommandLineInterface command_line;
        command_line.add(CommandLineContribution(
            "paving-backends-benchmark",parsers(selection)));
        List<String> args;
        for (int i=0;i<argc;++i) { args.append(String(argv[i])); }
        if(not command_line.acquire(args)) {
            return CLI::requests_help(args)?0:1;
        }

        std::vector<Datum> data;
        std::vector<MemoryDatum> memory;
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
                    append_case(data,memory,selection,shape,d,f);
                }
            }
        }
        if (data.empty()) {
            throw std::runtime_error("no nondegenerate cases within the selection"
                " (lower --min-leaves or increase --finenesses)");
        }
        report(data,memory,selection,skipped);
        if(selection.run_retention) print_manager_retention(selection);
        return 0;
    } catch(std::exception const& error) {
        LOGGING_PRINTLN("benchmark_paving_backends: "<<error.what())
        return 1;
    }
}
