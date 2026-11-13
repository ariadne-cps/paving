/***************************************************************************
 *            benchmarks/benchmark_paving_backends.cpp
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

/** @file benchmarks/benchmark_paving_backends.cpp
 *  @brief Benchmark paving operations and backend implementations.
 */

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "function/functional.hpp"
#include "function/procedure.hpp"
#include "logging/logging.hpp"
#include "paving/binary_tree_paving.hpp"
#include "paving/cached_binary_tree_paving.hpp"
#include "paving/paving_cell_operations.hpp"
#include "paving/robdd_paving.hpp"
#include "set/function_set.hpp"
#include "paving_benchmark_shapes.hpp"
#include "threading/thread_manager.hpp"
#include "threading/threading_command_line_interface.hpp"

#include "ariadne-benchmarking.hpp"
#include "constraint-solving/constraint_solver.hpp"

// This existing set-module conversion is also declared by
// paving/src/set_approximation.cpp; set/function_set.hpp does not expose it.
namespace Ariadne {
ExactBoxType over_approximation(const RealBox&);
}

using namespace Ariadne;
namespace Bench = Ariadne::Benchmarking;
using PavingBenchmarkCases::make_shape;
using PavingBenchmarkCases::parse_shapes;

namespace {

struct Selection {
    unsigned iterations = 1u;
    unsigned samples = 7u;
    unsigned warmups = 0u;
    unsigned min_sample_ms = 12u;
    unsigned max_iterations = 32768u;
    unsigned max_depth_bits = 26u;
    unsigned min_leaves = 2u;
    unsigned max_replay_cells = 16384u;
    unsigned retention_steps = 12u;
    unsigned retention_fineness = 3u;
    std::vector<unsigned> dimensions{2u, 3u, 4u};
    std::vector<unsigned> finenesses{2u, 3u};
    std::vector<std::string> shapes{
        "aligned-box", "rotated-box", "aligned-ellipsoid",
        "rotated-ellipsoid", "ball"};
    bool summary_only = false;
    bool include_cached_tree = false;
    bool run_retention = true;
    bool tree_iterator_cost = false;
    bool outer_phases = false;
    bool compare_outer_preparation = false;
    bool compare_outer_overlap = false;
    bool outer_preparation_verify_only = false;
    std::string outer_trace_prefix;
};

List<ArgumentParser> parsers(Selection& selection) {
    List<ArgumentParser> result;
    result.append(make_valued_argument_parser(
        "i", "iterations", "Positive repetitions per sample",
        [&selection](const String& value) -> VoidFunction {
            unsigned n = Bench::parse_unsigned<unsigned>(value);
            if (n == 0u)
                throw std::invalid_argument("iterations must be positive");
            return [&selection, n] { selection.iterations = n; };
        }));
    result.append(make_valued_argument_parser(
        "n", "samples", "Odd number of timing samples, at least three",
        [&selection](const String& value) -> VoidFunction {
            unsigned n = Bench::parse_unsigned<unsigned>(value);
            if (n < 3u or n % 2u == 0u)
                throw std::invalid_argument("samples must be odd and >=3");
            return [&selection, n] { selection.samples = n; };
        }));
    result.append(make_valued_argument_parser(
        "", "min-sample-ms", "Minimum calibration time per timed sample (ms, positive)",
        [&selection](const String& value) -> VoidFunction {
            unsigned n = Bench::parse_unsigned<unsigned>(value);
            if (n == 0u or n > 10000u)
                throw std::invalid_argument("min-sample-ms out of range");
            return [&selection, n] { selection.min_sample_ms = n; };
        }));
    result.append(make_valued_argument_parser(
        "", "max-iterations", "Upper limit of calibrated repetitions per sample",
        [&selection](const String& value) -> VoidFunction {
            unsigned n = Bench::parse_unsigned<unsigned>(value);
            if (n == 0u)
                throw std::invalid_argument("max-iterations must be positive");
            return [&selection, n] { selection.max_iterations = n; };
        }));
    result.append(make_valued_argument_parser(
        "", "min-leaves", "Skip geometrically degenerate cases below this enabled leaf count",
        [&selection](const String& value) -> VoidFunction {
            unsigned n = Bench::parse_unsigned<unsigned>(value);
            return [&selection, n] { selection.min_leaves = n; };
        }));
    result.append(make_valued_argument_parser(
        "d", "dimensions", "Comma-separated space dimensions (>=2)",
        [&selection](const String& value) -> VoidFunction {
            auto dims = Bench::parse_unique_unsigned_list(value, 2u);
            return [&selection, dims] { selection.dimensions = dims; };
        }));
    result.append(make_valued_argument_parser(
        "f", "finenesses", "Comma-separated grid-relative fineness values",
        [&selection](const String& value) -> VoidFunction {
            // zero is a meaningful coarsest approximation.
            auto finenesses = Bench::parse_unique_unsigned_list(value, 0u);
            return [&selection, finenesses] { selection.finenesses = finenesses; };
        }));
    result.append(make_valued_argument_parser(
        "g", "shapes", "Comma-separated: aligned-box,rotated-box,aligned-ellipsoid,rotated-ellipsoid,ball",
        [&selection](const String& value) -> VoidFunction {
            auto shapes = parse_shapes(value);
            return [&selection, shapes] { selection.shapes = shapes; };
        }));
    result.append(make_valued_argument_parser(
        "m", "max-depth-bits", "Maximum conservative subdivision-bit budget (positive)",
        [&selection](const String& value) -> VoidFunction {
            unsigned n = Bench::parse_unsigned<unsigned>(value);
            if (n == 0u)
                throw std::invalid_argument("max-depth-bits must be positive");
            return [&selection, n] { selection.max_depth_bits = n; };
        }));
    result.append(make_valued_argument_parser(
        "", "max-replay-cells", "Maximum precomputed enabled cells to replay per scenario",
        [&selection](const String& value) -> VoidFunction {
            unsigned n = Bench::parse_unsigned<unsigned>(value);
            if (n == 0u)
                throw std::invalid_argument("max-replay-cells must be positive");
            return [&selection, n] { selection.max_replay_cells = n; };
        }));
    result.append(make_valued_argument_parser(
        "", "retention-steps", "Repeated/different compact sets per manager (1..64)",
        [&selection](const String& value) -> VoidFunction {
            unsigned n = Bench::parse_unsigned<unsigned>(value);
            if (n < 1u or n > 64u)
                throw std::invalid_argument("retention-steps must be in [1,64]");
            return [&selection, n] { selection.retention_steps = n; };
        }));
    result.append(make_valued_argument_parser(
        "", "retention-fineness", "Grid fineness for the standalone retention experiment (1..6)",
        [&selection](const String& value) -> VoidFunction {
            unsigned n = Bench::parse_unsigned<unsigned>(value);
            if (n < 1u or n > 6u)
                throw std::invalid_argument("retention-fineness must be in [1,6]");
            return [&selection, n] { selection.retention_fineness = n; };
        }));
    result.append(make_unvalued_argument_parser(
        "", "summary-only", "Suppress per-operation table; show speed ratios only",
        [&selection] { selection.summary_only = true; }));
    result.append(make_unvalued_argument_parser(
        "", "include-cached-tree", "Add CachedTree as third backend",
        [&selection] { selection.include_cached_tree = true; }));
    result.append(make_unvalued_argument_parser(
        "", "no-retention", "Skip shared-manager post-root-destruction experiment",
        [&selection] { selection.run_retention = false; }));
    result.append(make_unvalued_argument_parser(
        "", "tree-iterator-cost", "Time safe owner snapshots versus borrowed stable-tree iteration",
        [&selection] { selection.tree_iterator_cost = true; }));
    result.append(make_unvalued_argument_parser(
        "", "outer-phases", "Profile geometric outer classification versus predicate calls and cell traversal",
        [&selection] { selection.outer_phases = true; }));
    result.append(make_unvalued_argument_parser(
        "", "compare-outer-preparation",
        "Compare full separated(), prepared join and compiled outer predicates",
        [&selection] { selection.compare_outer_preparation = true; }));
    result.append(make_unvalued_argument_parser(
        "", "compare-outer-overlap",
        "Compare outer approximation on empty, partially covered and full pavings",
        [&selection] { selection.compare_outer_overlap = true; }));
    result.append(make_unvalued_argument_parser(
        "", "verify-only", "Check opt-in outer workload correctness without timing",
        [&selection] { selection.outer_preparation_verify_only = true; }));
    result.append(make_valued_argument_parser(
        "", "outer-trace-prefix", "Export exact adaptive d4/f3 query boxes to PREFIX-SHAPE-d4-f3.trace",
        [&selection](const String& value) -> VoidFunction {
            std::string prefix(value);
            if (prefix.empty())
                throw std::invalid_argument("outer-trace-prefix must not be empty");
            return [&selection, prefix] { selection.outer_trace_prefix = prefix; };
        }));
    return result;
}


using Bench::keep_alive;
// Calibration can stop at the iteration cap before the requested duration.
struct Timing {
    double milliseconds;
    unsigned iterations;
    Bench::Statistics statistics;
    bool calibration_met = true;
};

template<class Operation>
Timing median_ms(Operation&& operation, unsigned iterations, unsigned samples,
                 unsigned minimum_ms, unsigned maximum, unsigned warmups) {
    auto execute = [&](unsigned count) {
        for (unsigned k = 0u; k < count; ++k) {
            auto result = operation(k);
            Bench::keep_alive(result);
        }
    };
    double elapsed = Bench::measure_ns([&] { execute(iterations); });
    while (elapsed < static_cast<double>(minimum_ms) * 1e6 && iterations < maximum) {
        unsigned next = iterations > maximum / 2u ? maximum : iterations * 2u;
        if (next == iterations)
            break;
        iterations = next;
        elapsed = Bench::measure_ns([&] { execute(iterations); });
    }
    Bench::Runner runner(warmups);
    auto durations = runner.run("paving-operation", samples, [] {}, [&] { execute(iterations); }, [] {}, static_cast<double>(iterations));
    auto stats = Bench::summarize(durations);
    if (!std::isfinite(stats.median) || stats.median <= 0.0)
        throw std::runtime_error("nonpositive/nonfinite benchmark timing");
    return {stats.median / 1e6, iterations, stats};
}

// Preserve mutation-only timing: copies are constructed outside each timed
// chunk and no more than 256 copies are resident at once.
template<class Factory, class Mutate>
Timing median_mutation_ms(Factory&& factory, Mutate&& mutate,
                          unsigned iterations, unsigned samples, unsigned minimum_ms,
                          unsigned maximum, unsigned warmups) {
    constexpr unsigned limit = 256u;
    auto execute = [&](unsigned count) {
        double total = 0.0;
        while (count > 0u) {
            unsigned chunk = std::min(count, limit);
            std::vector<decltype(factory())> copies;
            copies.reserve(chunk);
            for (unsigned k = 0u; k < chunk; ++k)
                copies.push_back(factory());
            total += Bench::measure_ns([&] {
                for (auto& copy: copies) {
                    mutate(*copy);
                    Bench::keep_alive(*copy);
                }
            });
            count -= chunk;
        }
        return total;
    };
    double elapsed = execute(iterations);
    while (elapsed < static_cast<double>(minimum_ms) * 1e6 && iterations < maximum) {
        unsigned next = iterations > maximum / 2u ? maximum : iterations * 2u;
        if (next == iterations)
            break;
        iterations = next;
        elapsed = execute(iterations);
    }
    for (unsigned k = 0u; k < warmups; ++k)
        (void)execute(iterations);
    Bench::Samples measured;
    for (unsigned k = 0u; k < samples; ++k)
        measured.add(execute(iterations) / static_cast<double>(iterations));
    auto stats = Bench::summarize(measured);
    if (!std::isfinite(stats.median) || stats.median <= 0.0)
        throw std::runtime_error("invalid isolated mutation timing");
    return {stats.median / 1e6, iterations, stats,
            elapsed >= static_cast<double>(minimum_ms) * 1e6};
}

SizeType count_tree_nodes(const BinaryTreeNode& node) {
    if (node.is_leaf()) {
        return 1u;
    }
    return 1u + count_tree_nodes(*node.left_node()) + count_tree_nodes(*node.right_node());
}

SizeType visit_leaves(const SubPavingInterface& paving) {
    SizeType checksum = 0u;
    for (auto it = paving.begin(), end = paving.end(); it != end; ++it) {
        checksum += it->word().size() + 1u;
    }
    return checksum;
}

// This is a deliberately borrowed traversal, valid ONLY while the owning
// tree is stable. It is not a substitute for concurrent-safe owner iterators.
SizeType visit_borrowed_tree_leaves(const BinaryTreeSubpaving& paving) {
    SizeType checksum = 0u;
    for (auto it = paving.BinaryTreeSubpaving::begin(),
              end = paving.BinaryTreeSubpaving::end();
         it != end; ++it) {
        checksum += it->word().size() + 1u;
    }
    return checksum;
}

SizeType visit_minimum(const SubPavingInterface& paving, Nat fineness) {
    SizeType checksum = 0u;
    for_each_enabled_cell_at_fineness(
        paving, fineness, [&](const GridCell& cell) {
            checksum += cell.word().size() + 1u;
        });
    return checksum;
}

std::vector<GridCell> make_probes(const Grid& grid, unsigned dimension,
                                  unsigned fineness) {
    std::vector<GridCell> result;
    unsigned depth = std::min(8u, dimension * fineness);
    for (unsigned i = 0u; i < 16u; ++i) {
        BinaryWord word;
        for (unsigned j = 0u; j < depth; ++j) {
            word.push_back(((i >> (j % 4u)) & 1u) != 0u);
        }
        result.emplace_back(grid, 0u, word);
    }
    return result;
}

SizeType probe_coverage(const SubPavingInterface& paving,
                        const std::vector<GridCell>& queries) {
    SizeType result = 0u;
    for (const GridCell& query: queries) {
        if (paving.superset(query)) {
            ++result;
        }
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
    const RobddManager& manager, const Robdd& root) {
    using Key = std::tuple<SizeType, SizeType, SizeType>;
    std::map<Key, SizeType> unique_nodes;
    std::function<SizeType(const Robdd&)> visit =
        [&](const Robdd& value) -> SizeType {
        if (value.is_terminal()) {
            return value.is_true() ? 1u : 0u;
        }
        auto var = manager.top_variable(value);
        if (not var) {
            throw std::logic_error("nonterminal ROBDD without variable");
        }
        SizeType low = visit(manager.low(value));
        SizeType high = visit(manager.high(value));
        SizeType complement = low & 1u;
        if (complement != 0u) {
            low ^= 1u;
            high ^= 1u;
        }
        Key key{var->index(), low, high};
        auto found = unique_nodes.find(key);
        if (found == unique_nodes.end()) {
            SizeType id = unique_nodes.size() + 1u;
            found = unique_nodes.emplace(key, id).first;
        }
        return (found->second << 1u) | complement;
    };
    visit(root);
    return unique_nodes.size();
}

void verify_reachable_node_oracle() {
    RobddManager manager;
    Robdd false_bdd = manager.constant(false);
    Robdd true_bdd = manager.constant(true);
    Robdd x = manager.variable(0u);
    Robdd y = manager.variable(1u);
    Robdd xor_bdd = manager.exclusive_or(x, y);
    if (reachable_robdd_node_count(manager, false_bdd) != 0u or reachable_robdd_node_count(manager, true_bdd) != 0u or reachable_robdd_node_count(manager, x) != 1u or reachable_robdd_node_count(manager, manager.negation(x)) != 1u or reachable_robdd_node_count(manager, xor_bdd) != 2u or reachable_robdd_node_count(manager, manager.negation(xor_bdd)) != 2u) {
        throw std::runtime_error("incorrect physical ROBDD DAG node accounting");
    }
}

SizeType checked_node_bytes(SizeType count, SizeType bytes_per_node) {
    if (count > std::numeric_limits<SizeType>::max() / bytes_per_node) {
        throw std::overflow_error("node payload estimate exceeds SizeType");
    }
    return count * bytes_per_node;
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

void print_memory_results(const std::vector<MemoryDatum>& memory) {
    constexpr int width = 140;
    LOGGING_PRINTLN("Estimated physical node payload (not RSS/heap footprint):")
    LOGGING_PRINTLN("sizeof(BinaryTreeNode)=" << sizeof(BinaryTreeNode)
                                              << "  estimated sizeof(ROBDD decision node)="
                                              << sizeof(RobddDecisionNodeLayoutEstimate)
                                              << "  sizeof(BinaryTreePaving)=" << sizeof(BinaryTreePaving)
                                              << "  sizeof(RobddPaving)=" << sizeof(RobddPaving)
                                              << "  sizeof(RobddManager)=" << sizeof(RobddManager))
    LOGGING_PRINTLN("Tree B~=TreeNodes*sizeof(BinaryTreeNode);"
                    << " BDD B~=reachable distinct nonterminal nodes*estimated node size.")
    LOGGING_PRINTLN("Mgr N is a fresh manager after construction;"
                    << " Mgr warm is after warm timings. BDD N counts root-reachable nodes.")
    LOGGING_PRINTLN("Mgr B~=fresh manager node count*estimated node size;"
                    << " not per-paving memory; ITE and unique tables excluded.")
    LOGGING_PRINTLN("All estimates exclude allocator overhead, vector spare"
                    << " capacity, unique/computed tables, and dynamically owned metadata.")
    LOGGING_PRINTLN(std::string(width, '='))
    LOGGING_PRINTLN(std::left
                    << std::setw(19) << "Shape" << "  "
                    << std::setw(3) << "D" << "  "
                    << std::setw(3) << "F" << "  "
                    << std::right << std::setw(9) << "Tree N"
                    << "  " << std::setw(9) << "BDD N"
                    << "  " << std::setw(9) << "Mgr N"
                    << "  " << std::setw(9) << "Mgr warm"
                    << "  " << std::setw(12) << "Tree B~"
                    << "  " << std::setw(12) << "BDD B~"
                    << "  " << std::setw(12) << "Mgr B~"
                    << "  " << std::setw(12) << "BDD/Tree")
    LOGGING_PRINTLN(std::string(width, '-'))
    for (const MemoryDatum& datum: memory) {
        double ratio = static_cast<double>(datum.robdd_reachable_bytes) / static_cast<double>(datum.tree_payload_bytes);
        LOGGING_PRINTLN(std::left
                        << std::setw(19) << datum.shape << "  "
                        << std::setw(3) << datum.dimension << "  "
                        << std::setw(3) << datum.fineness << "  "
                        << std::right << std::setw(9) << datum.tree_nodes
                        << "  " << std::setw(9) << datum.robdd_reachable_nodes
                        << "  " << std::setw(9) << datum.robdd_manager_nodes
                        << "  " << std::setw(9) << datum.robdd_manager_after_nodes
                        << "  " << std::setw(12) << datum.tree_payload_bytes
                        << "  " << std::setw(12) << datum.robdd_reachable_bytes
                        << "  " << std::setw(12) << datum.robdd_manager_payload_bytes
                        << "  " << std::fixed << std::setprecision(3)
                        << std::setw(11) << ratio << "x")
    }
    LOGGING_PRINTLN(std::string(width, '='))
}

SizeType manager_payload_lower_bound(const RobddManagerStatistics& stats) {
    SizeType nodes = checked_node_bytes(stats.node_capacity, stats.node_size_bytes);
    SizeType unique = checked_node_bytes(
        stats.unique_table_entries, stats.unique_table_entry_payload_bytes);
    SizeType computed = checked_node_bytes(
        stats.ite_cache_entries, stats.ite_cache_entry_payload_bytes);
    if (nodes > std::numeric_limits<SizeType>::max() - unique or
        nodes + unique > std::numeric_limits<SizeType>::max() - computed) {
        throw std::overflow_error("estimated manager payload overflow");
    }
    return nodes + unique + computed;
}

void print_manager_statistics(const std::vector<MemoryDatum>& memory) {
    LOGGING_PRINTLN("ROBDD manager statistics (fresh vs after workload)")
    LOGGING_PRINTLN("Payload B>= is a lower bound excluding std::map node"
                    " links, allocator overhead and all other runtime allocations.")
    LOGGING_PRINTLN(std::string(105, '='))
    LOGGING_PRINTLN(std::left << std::setw(19) << "Shape"
                              << "  " << std::setw(3) << "D" << "  " << std::setw(3) << "F"
                              << "  " << std::setw(9) << "Nodes"
                              << "  " << std::setw(9) << "Capacity"
                              << "  " << std::setw(9) << "Unique"
                              << "  " << std::setw(9) << "ITE"
                              << "  " << std::setw(14) << "Payload B>="
                              << "  " << std::setw(7) << "Stage")
    LOGGING_PRINTLN(std::string(105, '-'))
    for (const auto& datum: memory) {
        auto line = [&](const RobddManagerStatistics& stats, const char* stage) {
            LOGGING_PRINTLN(std::left << std::setw(19) << datum.shape
                                      << "  " << std::setw(3) << datum.dimension
                                      << "  " << std::setw(3) << datum.fineness
                                      << "  " << std::setw(9) << stats.node_count
                                      << "  " << std::setw(9) << stats.node_capacity
                                      << "  " << std::setw(9) << stats.unique_table_entries
                                      << "  " << std::setw(9) << stats.ite_cache_entries
                                      << "  " << std::setw(14) << manager_payload_lower_bound(stats)
                                      << "  " << std::setw(7) << stage)
        };
        line(datum.fresh, "fresh");
        line(datum.after_warm, "after");
    }
    LOGGING_PRINTLN(std::string(105, '='))
}

EffectiveConstrainedImageSet make_shifted_box(unsigned variant) {
    // Distinct exact compact inputs on an 8x8 lattice of translations:
    // all bounds have a power-of-two denominator and remain in [0,1]^2.
    // The first geometry is exactly the same in both retention runs.
    unsigned translation_column = variant % 8u;
    unsigned translation_row = (variant / 8u) % 8u;
    double first_lower = 0.125 + static_cast<double>(translation_column) / 16.0;
    double second_lower = 0.125 + static_cast<double>(translation_row) / 16.0;
    auto x = EffectiveScalarMultivariateFunction::coordinates(2u);
    RealBox domain(2u, RealInterval(0, 1));
    EffectiveConstrainedImageSet compact(
        domain, EffectiveVectorMultivariateFunction::identity(2u));
    compact.new_parameter_constraint(x[0] >= ExactDouble(first_lower));
    compact.new_parameter_constraint(x[0] <= ExactDouble(first_lower + 0.25));
    compact.new_parameter_constraint(x[1] >= ExactDouble(second_lower));
    compact.new_parameter_constraint(x[1] <= ExactDouble(second_lower + 0.25));
    return compact;
}

void print_manager_retention(const Selection& selection) {
    // Compare two *independent* managers:
    // SAME: repeatedly construct identical compact geometry;
    // VARIED: translate compact boxes by distinct dyadic offsets.
    // Both use identical operation counts and fineness. Every paving
    // and all root handles are destroyed BEFORE the corresponding row.
    Grid grid(2u);
    GridCell primary(grid, 0u, BinaryWord{});
    LOGGING_PRINTLN("Manager retention after destroying every paving root"
                    << " (d=2, fineness=" << selection.retention_fineness
                    << ", steps=" << selection.retention_steps << ")")
    LOGGING_PRINTLN("SAME repeats one exact box; VARIED uses distinct"
                    " translated exact boxes (discretizations can coincide).")
    LOGGING_PRINTLN(std::string(128, '='))
    LOGGING_PRINTLN(std::left << std::setw(9) << "Scenario"
                              << "  " << std::setw(5) << "Step"
                              << "  " << std::setw(8) << "Variant"
                              << "  " << std::setw(8) << "Root N"
                              << "  " << std::setw(9) << "Mgr N"
                              << "  " << std::setw(9) << "Capacity"
                              << "  " << std::setw(9) << "Unique"
                              << "  " << std::setw(9) << "ITE"
                              << "  " << std::setw(13) << "Payload B>=")
    LOGGING_PRINTLN(std::string(128, '-'))
    for (unsigned scenario = 0u; scenario < 2u; ++scenario) {
        RobddManager manager;
        const char* label = scenario == 0u ? "SAME" : "VARIED";
        auto emit = [&](unsigned step, unsigned variant, SizeType reachable) {
            RobddManagerStatistics stats = manager.statistics();
            LOGGING_PRINTLN(std::left << std::setw(9) << label
                                      << "  " << std::setw(5) << step
                                      << "  " << std::setw(8) << variant
                                      << "  " << std::setw(8) << reachable
                                      << "  " << std::setw(9) << stats.node_count
                                      << "  " << std::setw(9) << stats.node_capacity
                                      << "  " << std::setw(9) << stats.unique_table_entries
                                      << "  " << std::setw(9) << stats.ite_cache_entries
                                      << "  " << std::setw(13) << manager_payload_lower_bound(stats))
        };
        emit(0u, 0u, 0u);
        for (unsigned step = 1u; step <= selection.retention_steps; ++step) {
            unsigned variant = scenario == 0u ? 0u : step - 1u;
            SizeType reachable = 0u;
            {
                EffectiveConstrainedImageSet compact = make_shifted_box(variant);
                const EffectiveEuclideanCompactSetInterface& set = compact;
                RobddPaving paving(
                    manager, primary, manager.constant(false), 0u);
                paving.adjoin_outer_approximation(
                    set, selection.retention_fineness);
                paving.recombine();
                reachable = reachable_robdd_node_count(
                    manager, paving.robdd());
                keep_alive(paving);
            }
            emit(step, variant, reachable);
        }
        if (scenario == 0u) {
            LOGGING_PRINTLN(std::string(128, '-'))
        }
    }
    LOGGING_PRINTLN(std::string(128, '='))
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
    Bench::Statistics statistics;
};

struct Aggregate {
    unsigned count = 0u;
    double log_sum = 0.0;
    double worst = 0.0;
    std::string worst_case;
    void add(double ratio, const std::string& name) {
        if (not std::isfinite(ratio) or ratio <= 0.0) {
            throw std::runtime_error("invalid benchmark speed ratio");
        }
        ++count;
        log_sum += std::log(ratio);
        if (ratio > worst) {
            worst = ratio;
            worst_case = name;
        }
    }
    double geometric_mean() const { return std::exp(log_sum / count); }
};

void validate_geometry(
    const SubPavingInterface& tree, const SubPavingInterface& robdd) {
    // Never pair timing rows unless both algorithms produce the same
    // geometric set. This check runs outside the timed measurements.
    for (auto it = tree.begin(), end = tree.end(); it != end; ++it) {
        if (not robdd.superset(*it)) {
            throw std::runtime_error("ROBDD lacks a cell from tree approximation");
        }
    }
    for (auto it = robdd.begin(), end = robdd.end(); it != end; ++it) {
        if (not tree.superset(*it)) {
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
    SizeType visited = 0u;
    SizeType separated_calls = 0u;
    SizeType covers_calls = 0u;
    SizeType subdivisions = 0u;
};

OuterSelection select_outer_cells(
    const EffectiveEuclideanCompactSetInterface& set,
    const Grid& grid, unsigned fineness, Bool keep_trace) {
    Nat extent = GridCell::smallest_enclosing_primary_cell_extent(
        set.bounding_box(), grid);
    Nat dimensions = static_cast<Nat>(grid.dimension());
    constexpr Nat nat_limit = std::numeric_limits<Nat>::max();
    constexpr SizeType size_limit = std::numeric_limits<SizeType>::max();
    if (dimensions == 0u or extent > nat_limit - dimensions or extent + dimensions > nat_limit - fineness) {
        throw std::overflow_error("invalid outer diagnostic extent");
    }
    Nat root_extent = extent + dimensions;
    SizeType levels = static_cast<SizeType>(root_extent) + fineness;
    if (levels > size_limit / dimensions) {
        throw std::overflow_error("outer diagnostic depth exceeds SizeType");
    }
    SizeType target_depth = levels * dimensions;
    GridCell root(grid, root_extent, BinaryWord{});
    OuterSelection result{root, {}, {}, 0u, 0u, 0u, 0u};
    const auto* open_set = dynamic_cast<
        const EffectiveEuclideanOpenSetInterface*>(
        static_cast<const EuclideanSetInterfaceBase*>(&set));

    std::function<void(const GridCell&, SizeType)> visit =
        [&](const GridCell& cell, SizeType depth) {
            ++result.visited;
            ++result.separated_calls;
            Bool separated = definitely(set.separated(cell.box()));
            Bool check_cover = not separated and open_set != nullptr;
            Bool covered = false;
            if (check_cover) {
                ++result.covers_calls;
                covered = definitely(open_set->covers(cell.box()));
            }
            Bool subdivide = not separated and not covered and depth < target_depth;
            if (keep_trace) {
                result.trace.push_back({cell.box(), cell.word(), check_cover, separated, covered, subdivide});
            }
            if (separated) {
                return;
            }
            if (not subdivide) {
                result.selected.push_back(cell);
                return;
            }
            ++result.subdivisions;
            visit(cell.split(false), depth + 1u);
            visit(cell.split(true), depth + 1u);
        };
    visit(root, 0u);
    return result;
}

// Re-run exactly those geometric predicates whose results determined the
// recorded topology; boxes are precomputed, so no GridCell split occurs.
SizeType replay_outer_predicates(
    const EffectiveEuclideanCompactSetInterface& set,
    const std::vector<OuterTraceEntry>& trace) {
    const auto* open_set = dynamic_cast<
        const EffectiveEuclideanOpenSetInterface*>(
        static_cast<const EuclideanSetInterfaceBase*>(&set));
    SizeType checksum = 0u;
    for (const OuterTraceEntry& entry: trace) {
        Bool separated = definitely(set.separated(entry.box));
        checksum += static_cast<SizeType>(separated);
        if (entry.check_covers) {
            if (open_set == nullptr)
                throw std::logic_error(
                    "outer predicate trace expects a missing open set");
            checksum += static_cast<SizeType>(
                definitely(open_set->covers(entry.box)));
        }
    }
    return checksum;
}

// Replay the identical recorded branch topology without predicates.
// This still constructs all GridCells and calculates their exact boxes.
SizeType replay_outer_geometry(const OuterSelection& reference,
                               Bool check_paths) {
    SizeType index = 0u;
    SizeType checksum = 0u;
    std::function<void(const GridCell&)> visit =
        [&](const GridCell& cell) {
            if (index >= reference.trace.size())
                throw std::logic_error("outer traversal trace exhausted");
            const OuterTraceEntry& entry = reference.trace[index++];
            if (check_paths and cell.word() != entry.path)
                throw std::logic_error("outer traversal path mismatch");
            checksum += cell.word().size() + cell.box().dimension() + 1u;
            if (entry.subdivide) {
                visit(cell.split(false));
                visit(cell.split(true));
            }
        };
    visit(reference.enclosing_cell);
    if (index != reference.trace.size())
        throw std::logic_error("outer traversal trace not fully consumed");
    return checksum;
}

// Experimental preparation comparison at the set/paving boundary.
// Compiled mode now uses the *real*, centralized set/solver prepared API.
// A new context
// is constructed *inside every timed full-trace sweep* so preparation
// cost is accounted for, rather than hidden in a warm-cache estimate.
struct OuterPredicateContext {
    UpperBoxType domain;
    ValidatedVectorMultivariateFunction function;
    ExactBoxType bounds;
    explicit OuterPredicateContext(const EffectiveConstrainedImageSet& set)
        : domain(over_approximation(set.domain())), function(join(set.function(), set.constraint_function())), bounds(ExactBoxType(over_approximation(set.constraint_bounds()))) {}

    ExactBoxType codomain(const ExactBoxType& box) const {
        return product(box, bounds);
    }
};

SizeType replay_outer_joined(const OuterPredicateContext& context,
                             const std::vector<OuterTraceEntry>& trace) {
    ConstraintSolver solver;
    SizeType checksum = 0u;
    for (const OuterTraceEntry& entry: trace) {
        UpperBoxType domain = context.domain;
        solver.reduce(domain, context.function, context.codomain(entry.box));
        checksum += static_cast<SizeType>(definitely(domain.is_empty()));
    }
    return checksum;
}

SizeType replay_outer_compiled(
    const PreparedConstrainedImageSetSeparation& context,
    const std::vector<OuterTraceEntry>& trace) {
    SizeType checksum = 0u;
    for (const OuterTraceEntry& entry: trace)
        checksum += static_cast<SizeType>(
            definitely(context.separated(entry.box)));
    return checksum;
}

struct OuterPreparationResult {
    std::string shape;
    SizeType queries = 0u;
    SizeType separated = 0u;
    double baseline = 0.0;
    double joined = 0.0;
    double compiled = 0.0;
};

OuterPreparationResult benchmark_outer_preparation(
    const Selection& selection, const std::string& shape) {
    constexpr unsigned dimension = 4u;
    constexpr unsigned fineness = 3u;
    Grid grid(dimension);
    EffectiveConstrainedImageSet compact = make_shape(dimension, shape);
    const EffectiveEuclideanCompactSetInterface& set = compact;
    const OuterSelection reference = select_outer_cells(
        set, grid, fineness, true);
    if (reference.visited != reference.trace.size() or reference.covers_calls != 0u or reference.selected.empty())
        throw std::runtime_error("inconsistent prepared outer trace");

    // Full geometric oracle: the old virtual set.separated() path
    // selected the reference cells, independently of either backend's
    // newly prepared contractor. Check the actual backend results.
    BinaryTreePaving oracle(grid);
    for (const GridCell& cell: reference.selected)
        oracle.adjoin(cell);
    oracle.recombine();
    BinaryTreePaving actual_tree(grid);
    actual_tree.adjoin_outer_approximation(set, fineness);
    actual_tree.recombine();
    RobddManager manager;
    const GridCell primary(grid, 0u, BinaryWord{});
    RobddPaving actual_robdd(
        manager, primary, manager.constant(false), 0u);
    actual_robdd.adjoin_outer_approximation(set, fineness);
    actual_robdd.recombine();
    validate_geometry(oracle, actual_tree);
    validate_geometry(oracle, actual_robdd);
    LOGGING_PRINTLN("OUTER_PREPARATION_GEOMETRY shape=" << shape
                                                        << " d=4 f=3 selected=" << reference.selected.size()
                                                        << " tree_matches_oracle=true robdd_matches_oracle=true")

    const OuterPredicateContext context(compact);
    const PreparedConstrainedImageSetSeparation procedure(
        compact.prepare_separation());
    ConstraintSolver solver;
    SizeType separated = 0u;
    for (SizeType q = 0u; q < reference.trace.size(); ++q) {
        const OuterTraceEntry& entry = reference.trace[q];
        const Bool original = definitely(set.separated(entry.box));
        const ExactBoxType codomain = context.codomain(entry.box);
        UpperBoxType joined = context.domain;
        solver.reduce(joined, context.function, codomain);
        UpperBoxType compiled = procedure.reduced_domain(entry.box);
        if (original != entry.separated or original != definitely(joined.is_empty()) or original != definitely(compiled.is_empty()) or not same(joined, compiled))
            throw std::runtime_error(
                "outer prepared-predicate mismatch shape=" + shape + " query=" + std::to_string(q));
        separated += static_cast<SizeType>(original);
    }
    LOGGING_PRINTLN("OUTER_PREPARATION_VERIFIED shape=" << shape
                                                        << " d=4 f=3 queries=" << reference.visited
                                                        << " selected=" << reference.selected.size()
                                                        << " separated=" << separated
                                                        << " joined_equal=true compiled_equal=true"
                                                        << " exact_contracted_boxes_equal=true")
    OuterPreparationResult row;
    row.shape = shape;
    row.queries = reference.visited;
    row.separated = separated;
    if (selection.outer_preparation_verify_only)
        return row;

    auto baseline = [&](unsigned) {
        return replay_outer_predicates(set, reference.trace);
    };
    auto joined = [&](unsigned) {
        const OuterPredicateContext data(compact);
        return replay_outer_joined(data, reference.trace);
    };
    auto compiled = [&](unsigned) {
        const PreparedConstrainedImageSetSeparation data(
            compact.prepare_separation());
        return replay_outer_compiled(data, reference.trace);
    };
    if (baseline(0u) != separated or joined(0u) != separated or compiled(0u) != separated)
        throw std::runtime_error("outer preparation timing checksum mismatch");

    const Timing t0 = median_ms(baseline, selection.iterations,
                                selection.samples, selection.min_sample_ms, selection.max_iterations, selection.warmups);
    const Timing t1 = median_ms(joined, selection.iterations,
                                selection.samples, selection.min_sample_ms, selection.max_iterations, selection.warmups);
    const Timing t2 = median_ms(compiled, selection.iterations,
                                selection.samples, selection.min_sample_ms, selection.max_iterations, selection.warmups);
    row.baseline = t0.milliseconds;
    row.joined = t1.milliseconds;
    row.compiled = t2.milliseconds;
    for (double ms: {row.baseline, row.joined, row.compiled})
        if (not std::isfinite(ms) or ms <= 0.0)
            throw std::runtime_error("invalid outer preparation timing");
    LOGGING_PRINTLN("OUTER_PREPARATION_COMPARE shape=" << shape
                                                       << " queries=" << row.queries
                                                       << " baseline_ms=" << std::fixed << std::setprecision(6) << row.baseline
                                                       << " joined_ms=" << row.joined
                                                       << " compiled_ms=" << row.compiled
                                                       << " joined_speedup=" << (row.baseline / row.joined)
                                                       << " compiled_speedup=" << (row.baseline / row.compiled)
                                                       << " preparation_in_timed_sweep=true")
    return row;
}

void report_outer_preparation(
    const Selection& selection,
    const std::vector<OuterPreparationResult>& rows) {
    if (rows.empty())
        throw std::runtime_error("no outer preparation shapes");
    LOGGING_PRINTLN(" ")
    LOGGING_PRINTLN("Paving separated() preparation | d=4 f=3"
                    << " | samples=" << selection.samples)
    LOGGING_PRINTLN("Baseline uses actual set.separated(); Joined reuses"
                    " the combined function and constraint bounds; Compiled also"
                    " reuses hull instructions. Preparation INCLUDED per sweep.")
    if (selection.outer_preparation_verify_only) {
        LOGGING_PRINTLN("Correctness-only replay; timing not collected.")
        return;
    }
    LOGGING_PRINTLN(std::string(102u, '='))
    LOGGING_PRINTLN(std::left << std::setw(22) << "Shape"
                              << std::right << std::setw(16) << "Baseline (ms)"
                              << std::setw(16) << "Joined (ms)"
                              << std::setw(16) << "Compiled (ms)"
                              << std::setw(16) << "Joined/base"
                              << std::setw(16) << "Compiled/base")
    LOGGING_PRINTLN(std::string(102u, '-'))
    double sum_base = 0.0, sum_join = 0.0, sum_comp = 0.0;
    double log_join = 0.0, log_comp = 0.0;
    SizeType all_queries = 0u;
    for (const auto& row: rows) {
        sum_base += row.baseline;
        sum_join += row.joined;
        sum_comp += row.compiled;
        log_join += std::log(row.baseline / row.joined);
        log_comp += std::log(row.baseline / row.compiled);
        all_queries += row.queries;
        LOGGING_PRINTLN(std::left << std::setw(22) << row.shape
                                  << std::right << std::fixed << std::setprecision(4)
                                  << std::setw(16) << row.baseline
                                  << std::setw(16) << row.joined
                                  << std::setw(16) << row.compiled
                                  << std::setw(15) << (row.baseline / row.joined) << "x"
                                  << std::setw(15) << (row.baseline / row.compiled) << "x")
    }
    LOGGING_PRINTLN(std::string(102u, '='))
    LOGGING_PRINTLN("OUTER_PREPARATION_AGGREGATE shapes=" << rows.size()
                                                          << " queries=" << all_queries
                                                          << " baseline_sum_ms=" << std::fixed << std::setprecision(6) << sum_base
                                                          << " joined_sum_ms=" << sum_join
                                                          << " compiled_sum_ms=" << sum_comp
                                                          << " joined_total_speedup=" << (sum_base / sum_join)
                                                          << " compiled_total_speedup=" << (sum_base / sum_comp)
                                                          << " joined_geomean_speedup="
                                                          << std::exp(log_join / static_cast<double>(rows.size()))
                                                          << " compiled_geomean_speedup="
                                                          << std::exp(log_comp / static_cast<double>(rows.size()))
                                                          << " preparation_in_timed_sweep=true")
    LOGGING_PRINTLN("Input trace box construction and grid traversal"
                    " are excluded equally from all three timed predicate sweeps."
                    " Medians are independently calibrated, not paired.")
}

struct OuterOverlapTiming {
    std::string scenario;
    Timing tree;
    Timing robdd;
};

struct OuterOverlapResult {
    std::string shape;
    unsigned dimension;
    unsigned fineness;
    SizeType reference_leaves;
    SizeType partial_seed_leaves;
    std::vector<OuterOverlapTiming> timings;
};

void benchmark_outer_overlap(
    const Selection& selection, const std::string& shape,
    unsigned dimension, unsigned fineness,
    std::vector<OuterOverlapResult>& results) {
    Grid grid(dimension);
    EffectiveConstrainedImageSet compact = make_shape(dimension, shape);
    const EffectiveEuclideanCompactSetInterface& set = compact;

    // Independent geometric oracle: classify every candidate using the
    // original virtual separated/covers path, NOT either paving backend.
    OuterSelection reference_cells = select_outer_cells(set, grid, fineness, false);
    BinaryTreePaving reference(grid);
    reference.adjoin(reference_cells.selected);
    reference.recombine();
    const SizeType reference_leaves = reference.size();
    const SizeType minimum = std::max<SizeType>(selection.min_leaves, 2u);
    if (reference_leaves < minimum) {
        LOGGING_PRINTLN("OUTER_OVERLAP_SKIP shape=" << shape
                                                    << " d=" << dimension << " f=" << fineness
                                                    << " leaves=" << reference_leaves << " min_leaves=" << minimum)
        return;
    }

    // Alternating *compact* leaves yields an actual nonempty proper subset
    // of the same reference approximation, not a shifted/different set.
    std::vector<GridCell> partial_cells;
    partial_cells.reserve((reference_leaves + 1u) / 2u);
    SizeType ordinal = 0u;
    for (auto it = reference.begin(), end = reference.end(); it != end; ++it) {
        if ((ordinal++ % 2u) == 0u)
            partial_cells.push_back(*it);
    }
    if (partial_cells.empty() or partial_cells.size() >= reference_leaves)
        throw std::runtime_error("partial outer seed is not a proper subset");

    RobddManager manager;
    GridCell primary(grid, 0u, BinaryWord{});
    OuterOverlapResult result{shape, dimension, fineness, reference_leaves, partial_cells.size(), {}};
    for (unsigned scenario = 0u; scenario < 3u; ++scenario) {
        const char* name = scenario == 0u ? "empty" : scenario == 1u ? "partial"
                                                                     : "full";
        BinaryTreePaving initial_tree(grid);
        RobddPaving initial_robdd(
            manager, primary, manager.constant(false), 0u);
        if (scenario == 1u) {
            initial_tree.adjoin(partial_cells);
            initial_robdd.adjoin(partial_cells);
        } else if (scenario == 2u) {
            // Exactly the geometric search root used by BOTH backends.
            initial_tree.adjoin(reference_cells.enclosing_cell);
            initial_robdd.adjoin(reference_cells.enclosing_cell);
        }

        // Verify that the two independently seeded representations agree.
        validate_geometry(initial_tree, initial_robdd);
        BinaryTreePaving expected(reference);
        if (scenario == 2u)
            expected.adjoin(reference_cells.enclosing_cell);
        expected.recombine();

        // Compute both candidate unions once outside all timed samples.
        BinaryTreePaving actual_tree(initial_tree);
        RobddPaving actual_robdd(initial_robdd);
        actual_tree.adjoin_outer_approximation(set, fineness);
        actual_robdd.adjoin_outer_approximation(set, fineness);
        actual_tree.recombine();
        actual_robdd.recombine();

        // Each of the three checks is bidirectional geometric inclusion.
        // The expected result comes from an independently classified
        // reference, so common backend mistakes cannot self-validate.
        validate_geometry(expected, actual_tree);
        validate_geometry(expected, actual_robdd);
        validate_geometry(actual_tree, actual_robdd);
        LOGGING_PRINTLN("OUTER_OVERLAP_VERIFIED shape=" << shape
                                                        << " d=" << dimension << " f=" << fineness
                                                        << " scenario=" << name
                                                        << " reference_leaves=" << reference_leaves
                                                        << " partial_seed_leaves=" << partial_cells.size()
                                                        << " geometry_matches=true")
        if (selection.outer_preparation_verify_only)
            continue;

        // Factories deep-copy the prepared destinations OUTSIDE each
        // timed chunk. Only adjoin_outer_approximation is timed, not
        // setup, destruction, reference construction or recombination.
        // The shared manager is warm, as in existing backend workloads.
        Timing tree_time = median_mutation_ms(
            [&] { return std::make_unique<BinaryTreePaving>(initial_tree); },
            [&](BinaryTreePaving& value) {
                value.adjoin_outer_approximation(set, fineness);
            },
            selection.iterations, selection.samples,
            selection.min_sample_ms, selection.max_iterations,
            selection.warmups);
        Timing robdd_time = median_mutation_ms(
            [&] { return std::make_unique<RobddPaving>(initial_robdd); },
            [&](RobddPaving& value) {
                value.adjoin_outer_approximation(set, fineness);
            },
            selection.iterations, selection.samples,
            selection.min_sample_ms, selection.max_iterations,
            selection.warmups);
        if (not tree_time.calibration_met or not robdd_time.calibration_met)
            LOGGING_PRINTLN("OUTER_OVERLAP_CALIBRATION_WARNING shape=" << shape
                                                                       << " d=" << dimension << " f=" << fineness
                                                                       << " scenario=" << name
                                                                       << " max_iterations=" << selection.max_iterations
                                                                       << " min_sample_ms=" << selection.min_sample_ms
                                                                       << " tree_met=" << (tree_time.calibration_met ? "true" : "false")
                                                                       << " robdd_met=" << (robdd_time.calibration_met ? "true" : "false"))
        result.timings.push_back({name, tree_time, robdd_time});
    }
    results.push_back(std::move(result));
}

struct OuterOverlapRatioAggregate {
    SizeType cases = 0u;
    SizeType tree_wins = 0u;
    SizeType robdd_wins = 0u;
    SizeType ties = 0u;
    SizeType uncalibrated = 0u;
    double sum_log = 0.0;
    double minimum = std::numeric_limits<double>::infinity();
    double maximum = 0.0;
    std::string min_case;
    std::string max_case;

    void add(double ratio, const std::string& case_name, bool calibrated) {
        if (not std::isfinite(ratio) or ratio <= 0.0)
            throw std::runtime_error("invalid overlap backend ratio");
        ++cases;
        sum_log += std::log(ratio);
        if (ratio < minimum) {
            minimum = ratio;
            min_case = case_name;
        }
        if (ratio > maximum) {
            maximum = ratio;
            max_case = case_name;
        }
        if (ratio > 1.0)
            ++tree_wins;
        else if (ratio < 1.0)
            ++robdd_wins;
        else
            ++ties;
        if (not calibrated)
            ++uncalibrated;
    }
    double geomean() const {
        if (cases == 0u)
            throw std::runtime_error("empty overlap aggregate");
        return std::exp(sum_log / static_cast<double>(cases));
    }
};

void report_outer_overlap_backend_ratios(
    const std::vector<OuterOverlapResult>& results) {
    constexpr std::array<const char*, 3> categories{"empty", "partial", "full"};
    std::array<OuterOverlapRatioAggregate, 3> by_coverage{};
    std::map<std::string, OuterOverlapRatioAggregate> by_geometry;
    OuterOverlapRatioAggregate overall;
    // Retain the descriptive, all-case aggregates but build independent
    // quality-filtered summaries from pairs where BOTH sides calibrated.
    std::array<OuterOverlapRatioAggregate, 3> calibrated_coverage{};
    std::map<std::string, OuterOverlapRatioAggregate> calibrated_geometry;
    OuterOverlapRatioAggregate calibrated_overall;

    LOGGING_PRINTLN(" ")
    LOGGING_PRINTLN("OUTER OVERLAP BACKEND COMPARISON (ROBDD / BinaryTree)")
    LOGGING_PRINTLN("Ratio >1: tree faster; <1: ROBDD faster. Ratios use"
                    " matched initial states, not historical code versions.")
    LOGGING_PRINTLN("Geometric means weight each shape/dimension/fineness/"
                    "coverage combination equally.")
    LOGGING_PRINTLN(std::string(99, '='))
    LOGGING_PRINTLN(std::left << std::setw(23) << "Geometry"
                              << std::right << std::setw(4) << "D" << std::setw(4) << "F"
                              << std::setw(13) << "Empty"
                              << std::setw(13) << "Partial"
                              << std::setw(13) << "Full"
                              << std::setw(13) << "Geomean"
                              << std::setw(8) << "Tree"
                              << std::setw(8) << "ROBDD")
    LOGGING_PRINTLN(std::string(99, '-'))
    for (const auto& result: results) {
        if (result.timings.size() != categories.size())
            throw std::runtime_error("missing overlap backend timing");
        std::array<double, 3> ratios{};
        OuterOverlapRatioAggregate row;
        for (std::size_t i = 0u; i < categories.size(); ++i) {
            const OuterOverlapTiming& datum = result.timings[i];
            if (datum.scenario != categories[i] or not std::isfinite(datum.tree.milliseconds) or not std::isfinite(datum.robdd.milliseconds) or datum.tree.milliseconds <= 0.0 or datum.robdd.milliseconds <= 0.0)
                throw std::runtime_error("invalid overlap backend timing");
            double ratio = datum.robdd.milliseconds / datum.tree.milliseconds;
            std::string case_name = result.shape + "/d" +
                                    std::to_string(result.dimension) + "/f" +
                                    std::to_string(result.fineness) + "/" + datum.scenario;
            bool calibrated = datum.tree.calibration_met and datum.robdd.calibration_met;
            ratios[i] = ratio;
            row.add(ratio, case_name, calibrated);
            by_geometry[result.shape].add(ratio, case_name, calibrated);
            by_coverage[i].add(ratio, case_name, calibrated);
            overall.add(ratio, case_name, calibrated);
            if (calibrated) {
                calibrated_geometry[result.shape].add(ratio, case_name, true);
                calibrated_coverage[i].add(ratio, case_name, true);
                calibrated_overall.add(ratio, case_name, true);
            }
        }
        LOGGING_PRINTLN(std::left << std::setw(23) << result.shape
                                  << std::right << std::setw(4) << result.dimension
                                  << std::setw(4) << result.fineness
                                  << std::fixed << std::setprecision(4)
                                  << std::setw(13) << ratios[0]
                                  << std::setw(13) << ratios[1]
                                  << std::setw(13) << ratios[2]
                                  << std::setw(13) << row.geomean()
                                  << std::setw(8) << row.tree_wins
                                  << std::setw(8) << row.robdd_wins)
        LOGGING_PRINTLN("OUTER_OVERLAP_BACKEND_CASE shape=" << result.shape
                                                            << " d=" << result.dimension << " f=" << result.fineness
                                                            << " empty_ratio=" << std::setprecision(9) << ratios[0]
                                                            << " partial_ratio=" << ratios[1]
                                                            << " full_ratio=" << ratios[2]
                                                            << " geomean_ratio=" << row.geomean()
                                                            << " tree_wins=" << row.tree_wins
                                                            << " robdd_wins=" << row.robdd_wins
                                                            << " ties=" << row.ties
                                                            << " uncalibrated=" << row.uncalibrated)
    }

    auto header = [&](const char* title) {
        LOGGING_PRINTLN(" ")
        LOGGING_PRINTLN(title)
        LOGGING_PRINTLN(std::string(99, '='))
        LOGGING_PRINTLN(std::left << std::setw(23) << "Group"
                                  << std::right << std::setw(7) << "Cases"
                                  << std::setw(13) << "Geomean"
                                  << std::setw(13) << "Min"
                                  << std::setw(13) << "Max"
                                  << std::setw(10) << "Tree"
                                  << std::setw(10) << "ROBDD"
                                  << std::setw(10) << "Ties"
                                  << std::setw(10) << "Uncal.")
        LOGGING_PRINTLN(std::string(99, '-'))
    };
    auto emit = [&](const char* marker, const char* key,
                    const std::string& group, const OuterOverlapRatioAggregate& a) {
        LOGGING_PRINTLN(std::left << std::setw(23) << group
                                  << std::right << std::setw(7) << a.cases
                                  << std::fixed << std::setprecision(4)
                                  << std::setw(13) << a.geomean()
                                  << std::setw(13) << a.minimum
                                  << std::setw(13) << a.maximum
                                  << std::setw(10) << a.tree_wins
                                  << std::setw(10) << a.robdd_wins
                                  << std::setw(10) << a.ties
                                  << std::setw(10) << a.uncalibrated)
        LOGGING_PRINTLN(marker << " " << key << "=" << group
                               << " cases=" << a.cases
                               << " geomean_ratio=" << std::setprecision(9) << a.geomean()
                               << " min_ratio=" << a.minimum << " min_case=" << a.min_case
                               << " max_ratio=" << a.maximum << " max_case=" << a.max_case
                               << " tree_wins=" << a.tree_wins
                               << " robdd_wins=" << a.robdd_wins
                               << " ties=" << a.ties
                               << " uncalibrated=" << a.uncalibrated)
    };
    LOGGING_PRINTLN("ALL-CASE AGGREGATES ARE DESCRIPTIVE: they may include"
                    " ratios with insufficient sample calibration.")
    header("AGGREGATE BY GEOMETRY (all cases, including uncalibrated)");
    for (const auto& entry: by_geometry)
        emit("OUTER_OVERLAP_BACKEND_GEOMETRY", "shape", entry.first, entry.second);
    header("AGGREGATE BY INITIAL COVERAGE (all selected geometries, d, f)");
    for (std::size_t i = 0u; i < categories.size(); ++i)
        if (by_coverage[i].cases != 0u)
            emit("OUTER_OVERLAP_BACKEND_COVERAGE", "scenario",
                 categories[i], by_coverage[i]);
    header("OVERALL (all cases, including uncalibrated)");
    emit("OUTER_OVERLAP_BACKEND_OVERALL", "scope", "all", overall);

    // A group's geomean is meaningful here only when both measured
    // backend samples reached the requested minimum batch duration.
    header("CALIBRATED ONLY: AGGREGATE BY GEOMETRY");
    for (const auto& entry: calibrated_geometry)
        emit("OUTER_OVERLAP_BACKEND_CALIBRATED_GEOMETRY",
             "shape", entry.first, entry.second);
    header("CALIBRATED ONLY: AGGREGATE BY INITIAL COVERAGE");
    for (std::size_t i = 0u; i < categories.size(); ++i) {
        if (calibrated_coverage[i].cases != 0u)
            emit("OUTER_OVERLAP_BACKEND_CALIBRATED_COVERAGE",
                 "scenario", categories[i], calibrated_coverage[i]);
        else {
            LOGGING_PRINTLN(categories[i] << ": no calibrated backend pairs")
            LOGGING_PRINTLN("OUTER_OVERLAP_BACKEND_CALIBRATED_COVERAGE"
                            " scenario="
                            << categories[i] << " cases=0 geomean_ratio=NA")
        }
    }
    header("CALIBRATED ONLY: OVERALL (equal case weighting)");
    if (calibrated_overall.cases != 0u)
        emit("OUTER_OVERLAP_BACKEND_CALIBRATED_OVERALL",
             "scope", "calibrated_only", calibrated_overall);
    else
        LOGGING_PRINTLN("OUTER_OVERLAP_BACKEND_CALIBRATED_OVERALL"
                        " scope=calibrated_only cases=0 geomean_ratio=NA")
    LOGGING_PRINTLN("Calibrated-only aggregates omit each whole ROBDD/tree"
                    " pair unless both sides met the requested sample duration.")
    LOGGING_PRINTLN("Reported ratios use independent medians and are not"
                    " estimates of statistical significance.")
}

void report_outer_overlap(const std::vector<OuterOverlapResult>& results,
                          const Selection& selection) {
    if (results.empty())
        throw std::runtime_error("no nondegenerate outer-overlap cases");
    if (selection.outer_preparation_verify_only) {
        LOGGING_PRINTLN("OUTER_OVERLAP_VERIFICATION_COMPLETE cases="
                        << results.size() << " timing=false")
        return;
    }
    LOGGING_PRINTLN("Outer approximation of already populated pavings"
                    " | independently calibrated median ms per mutation")
    LOGGING_PRINTLN("Empty vs partial vs full reflects INITIAL COVERAGE,"
                    " not speedup relative to the previous implementation.")
    LOGGING_PRINTLN("Each sample times only the approximation call;"
                    " cloning, reference classification and recombination excluded.")
    LOGGING_PRINTLN(std::left << std::setw(21) << "Shape"
                              << std::right << std::setw(4) << "D" << std::setw(4) << "F"
                              << std::setw(11) << "Coverage" << std::setw(14) << "Tree (ms)"
                              << std::setw(14) << "ROBDD (ms)"
                              << std::setw(13) << "Tree Iters" << std::setw(13) << "BDD Iters")
    for (const auto& result: results) {
        if (result.timings.size() != 3u)
            throw std::runtime_error("missing outer overlap timings");
        for (const auto& row: result.timings) {
            LOGGING_PRINTLN(std::left << std::setw(21) << result.shape
                                      << std::right << std::setw(4) << result.dimension
                                      << std::setw(4) << result.fineness
                                      << std::setw(11) << row.scenario
                                      << std::fixed << std::setprecision(6)
                                      << std::setw(14) << row.tree.milliseconds
                                      << std::setw(14) << row.robdd.milliseconds
                                      << std::setw(13) << row.tree.iterations
                                      << std::setw(13) << row.robdd.iterations)
            LOGGING_PRINTLN("OUTER_OVERLAP_TIMING shape=" << result.shape
                                                          << " d=" << result.dimension << " f=" << result.fineness
                                                          << " scenario=" << row.scenario
                                                          << " tree_ms=" << std::setprecision(9) << row.tree.milliseconds
                                                          << " robdd_ms=" << row.robdd.milliseconds
                                                          << " tree_iterations=" << row.tree.iterations
                                                          << " robdd_iterations=" << row.robdd.iterations
                                                          << " tree_calibration_met=" << (row.tree.calibration_met ? "true" : "false")
                                                          << " robdd_calibration_met=" << (row.robdd.calibration_met ? "true" : "false")
                                                          << " tree_vs_robdd=" << (row.robdd.milliseconds / row.tree.milliseconds)
                                                          << " reference_leaves=" << result.reference_leaves
                                                          << " partial_seed_leaves=" << result.partial_seed_leaves)
        }
        const auto& empty = result.timings[0];
        for (unsigned i = 1u; i < 3u; ++i) {
            const auto& row = result.timings[i];
            LOGGING_PRINTLN("OUTER_OVERLAP_RELATIVE shape=" << result.shape
                                                            << " d=" << result.dimension << " f=" << result.fineness
                                                            << " scenario=" << row.scenario
                                                            << " empty_over_tree=" << (empty.tree.milliseconds / row.tree.milliseconds)
                                                            << " empty_over_robdd=" << (empty.robdd.milliseconds / row.robdd.milliseconds))
        }
    }
    report_outer_overlap_backend_ratios(results);
}

void export_outer_solver_trace(
    const std::string& prefix, const std::string& shape,
    unsigned dimension, unsigned fineness,
    const EffectiveEuclideanCompactSetInterface& set, const Grid& grid) {
    // Export the same adaptive candidate sequence as --outer-phases.
    // Keep the output in a primitive, versioned format: solver tools in
    // constraint-solving must not depend on paving types.
    OuterSelection recorded = select_outer_cells(set, grid, fineness, true);
    if (recorded.trace.size() != recorded.visited or recorded.separated_calls != recorded.visited or recorded.covers_calls != 0u) {
        throw std::logic_error("unsupported or inconsistent outer solver trace");
    }
    std::string path = prefix + "-" + shape + "-d" + std::to_string(dimension) + "-f" + std::to_string(fineness) + ".trace";
    std::ofstream out(path);
    if (not out)
        throw std::runtime_error("cannot open outer trace: " + path);
    out << "ARIADNE_PAVING_SOLVER_TRACE_V1 "
        << shape << " " << dimension << " " << fineness
        << " " << recorded.trace.size() << "\n";
    // Hexadecimal floating-point preserves the exact dyadic endpoints
    // produced by GridCell::box(), without decimal-rounding ambiguity.
    out << std::hexfloat;
    SizeType separated_count = 0u;
    for (const OuterTraceEntry& entry: recorded.trace) {
        separated_count += static_cast<SizeType>(entry.separated);
        out << static_cast<int>(entry.separated);
        for (SizeType i = 0u; i < entry.box.dimension(); ++i) {
            out << " " << entry.box[i].lower_bound().get_d()
                << " " << entry.box[i].upper_bound().get_d();
        }
        out << "\n";
    }
    out.flush();
    if (not out)
        throw std::runtime_error("cannot write outer trace: " + path);
    LOGGING_PRINTLN("OUTER_SOLVER_TRACE shape=" << shape
                                                << " d=" << dimension << " f=" << fineness
                                                << " path=" << path << " queries=" << recorded.visited
                                                << " separated=" << separated_count)
}

void profile_outer_phases(
    const Selection& selection, const std::string& shape,
    unsigned dimension, unsigned fineness,
    const EffectiveEuclideanCompactSetInterface& set, const Grid& grid,
    const BinaryTreePaving& tree, const RobddPaving& robdd) {
    OuterSelection recorded = select_outer_cells(set, grid, fineness, true);
    if (recorded.visited != recorded.trace.size() or recorded.separated_calls != recorded.visited or recorded.selected.empty()) {
        throw std::logic_error("inconsistent outer selection counters");
    }
    // Check the candidate set against *both* production backends before
    // attributing time to this diagnostic traversal.
    BinaryTreePaving oracle(grid);
    for (const GridCell& cell: recorded.selected)
        oracle.adjoin(cell);
    oracle.recombine();
    validate_geometry(tree, oracle);
    validate_geometry(robdd, oracle);

    SizeType expected_predicates = 0u;
    for (const OuterTraceEntry& entry: recorded.trace) {
        expected_predicates += static_cast<SizeType>(entry.separated) + static_cast<SizeType>(entry.covered);
    }
    if (replay_outer_predicates(set, recorded.trace) != expected_predicates) {
        throw std::logic_error("geometric predicates changed on replay");
    }
    SizeType geometry_checksum = replay_outer_geometry(recorded, true);

    Timing selection_time = median_ms(
        [&](unsigned) {
            OuterSelection result = select_outer_cells(set, grid, fineness, false);
            if (result.visited != recorded.visited or result.selected.size() != recorded.selected.size()) {
                throw std::logic_error("unstable outer candidate selection");
            }
            keep_alive(result);
            return result.visited + result.selected.size();
        },
        selection.iterations, selection.samples,
        selection.min_sample_ms, selection.max_iterations, selection.warmups);
    Timing predicate_time = median_ms(
        [&](unsigned) { return replay_outer_predicates(set, recorded.trace); },
        selection.iterations, selection.samples,
        selection.min_sample_ms, selection.max_iterations, selection.warmups);
    Timing geometry_time = median_ms(
        [&](unsigned) { return replay_outer_geometry(recorded, false); },
        selection.iterations, selection.samples,
        selection.min_sample_ms, selection.max_iterations, selection.warmups);

    LOGGING_PRINTLN("OUTER_PHASES shape=" << shape
                                          << " d=" << dimension << " f=" << fineness
                                          << " visited=" << recorded.visited
                                          << " selected=" << recorded.selected.size()
                                          << " separated_calls=" << recorded.separated_calls
                                          << " covers_calls=" << recorded.covers_calls
                                          << " subdivisions=" << recorded.subdivisions
                                          << " candidate_checksum=" << geometry_checksum
                                          << " selection_ms=" << std::setprecision(9) << selection_time.milliseconds
                                          << " predicates_ms=" << predicate_time.milliseconds
                                          << " geometry_walk_ms=" << geometry_time.milliseconds
                                          << " predicate_over_selection="
                                          << predicate_time.milliseconds / selection_time.milliseconds)
    LOGGING_PRINTLN("OUTER_PHASES are independently timed overlapping workloads,"
                    << " not an additive cost decomposition; the selected cells were"
                    << " semantically checked against tree and ROBDD before timing.")
}

void append_case(std::vector<Datum>& data, std::vector<MemoryDatum>& memory,
                 const Selection& selection, const std::string& shape,
                 unsigned dimension, unsigned fineness) {
    Grid grid(dimension);
    GridCell primary(grid, 0u, BinaryWord{});
    EffectiveConstrainedImageSet compact = make_shape(dimension, shape);
    const EffectiveEuclideanCompactSetInterface& set = compact;
    RobddManager manager;

    BinaryTreePaving tree(grid);
    RobddPaving robdd(manager, primary, manager.constant(false), 0u);
    tree.adjoin_outer_approximation(set, fineness);
    tree.recombine();
    robdd.adjoin_outer_approximation(set, fineness);
    robdd.recombine();

    // Snapshot node counts BEFORE geometric validation or timed operations,
    // both of which may allocate additional manager nodes / cache entries.
    SizeType tree_nodes = count_tree_nodes(*tree.binary_tree());
    SizeType reachable_nodes = reachable_robdd_node_count(
        manager, robdd.robdd());
    SizeType manager_nodes = manager.node_count();
    if (reachable_nodes > manager_nodes) {
        throw std::logic_error("reachable nodes exceed allocated manager nodes");
    }
    memory.push_back({shape, dimension, fineness, tree_nodes,
                      reachable_nodes, manager_nodes,
                      checked_node_bytes(tree_nodes, sizeof(BinaryTreeNode)),
                      checked_node_bytes(reachable_nodes, sizeof(RobddDecisionNodeLayoutEstimate)),
                      checked_node_bytes(manager_nodes, sizeof(RobddDecisionNodeLayoutEstimate)),
                      manager_nodes, manager.statistics(), manager.statistics()});

    validate_geometry(tree, robdd);
    std::unique_ptr<CachedBinaryTreePaving> cached;
    if (selection.include_cached_tree) {
        cached = std::make_unique<CachedBinaryTreePaving>(tree);
        if (not cached->compile_cache() or not cached->has_cache())
            throw std::runtime_error("Unable to compile CachedTree cache");
        validate_geometry(tree, *cached);
    }
    SizeType tree_leaves = tree.size();
    SizeType robdd_leaves = robdd.size();
    if (tree_leaves < selection.min_leaves) {
        LOGGING_PRINTLN("SKIP degenerate geometry: " << shape
                                                     << " d=" << dimension << " f=" << fineness
                                                     << " leaves=" << tree_leaves
                                                     << " < min-leaves=" << selection.min_leaves)
        memory.pop_back();
        return;
    }
    if (selection.outer_phases) {
        profile_outer_phases(selection, shape, dimension, fineness, set, grid,
                             tree, robdd);
    }
    auto probes = make_probes(grid, dimension, fineness);

    auto record = [&](const char* name, auto&& tree_operation,
                      auto&& robdd_operation, auto&& cached_operation) {
        Timing tree_time = median_ms(tree_operation, selection.iterations,
                                     selection.samples, selection.min_sample_ms, selection.max_iterations, selection.warmups);
        Timing robdd_time = median_ms(robdd_operation, selection.iterations,
                                      selection.samples, selection.min_sample_ms, selection.max_iterations, selection.warmups);
        data.push_back({shape, name, "tree", dimension, fineness,
                        tree_time.milliseconds, tree_leaves, tree_nodes,
                        tree_time.iterations, tree_time.statistics});
        data.push_back({shape, name, "robdd", dimension, fineness,
                        robdd_time.milliseconds, robdd_leaves, reachable_nodes,
                        robdd_time.iterations, robdd_time.statistics});
        if (selection.include_cached_tree) {
            Timing cached_time = median_ms(cached_operation, selection.iterations,
                                           selection.samples, selection.min_sample_ms,
                                           selection.max_iterations, selection.warmups);
            data.push_back({shape, name, "cached-tree", dimension, fineness,
                            cached_time.milliseconds, tree_leaves, tree_nodes,
                            cached_time.iterations, cached_time.statistics});
        }
    };
    // The BDD manager is reused for all timed operations in this case.
    // In particular, repeated construction benefits from its unique table;
    // startup/manager allocation is NOT included (warm-manager mode).
    record("outer_warm", [&](unsigned) {
            BinaryTreePaving fresh(grid);
            fresh.adjoin_outer_approximation(set,fineness);
            fresh.recombine();
            return fresh; }, [&](unsigned) {
            RobddPaving fresh(
                manager,primary,manager.constant(false),0u);
            fresh.adjoin_outer_approximation(set,fineness);
            fresh.recombine();
            return fresh; }, [&](unsigned) {
            CachedBinaryTreePaving fresh(grid);
            fresh.adjoin_outer_approximation(set,fineness);
            fresh.recombine();
            return fresh; });

    record("outer_cold", [&](unsigned) {
            BinaryTreePaving fresh(grid);
            fresh.adjoin_outer_approximation(set,fineness);
            fresh.recombine();
            return fresh.size(); }, [&](unsigned) {
            RobddManager cold_manager;
            RobddPaving fresh(cold_manager,primary,
                cold_manager.constant(false),0u);
            fresh.adjoin_outer_approximation(set,fineness);
            fresh.recombine();
            return fresh.size(); }, [&](unsigned) {
            CachedBinaryTreePaving fresh(grid);
            fresh.adjoin_outer_approximation(set,fineness);
            fresh.recombine();
            return fresh.size(); });

    // Replay only the production bulk construction path. Extract and
    // validate cells before timing so geometric set solving and iteration
    // are not included in the cell-insertion cost.
    if (tree_leaves <= selection.max_replay_cells) {
        std::vector<GridCell> prepared;
        prepared.reserve(tree_leaves);
        for (auto it = tree.begin(), end = tree.end(); it != end; ++it) {
            prepared.push_back(*it);
        }
        if (prepared.size() != tree_leaves) {
            throw std::runtime_error("inconsistent prepared-cell count");
        }
        {
            // The tree is an independent geometric oracle for replay.
            // Use a separate manager so the checks cannot affect timings.
            BinaryTreePaving verification_tree(grid);
            RobddManager verification_manager;
            RobddPaving verification_bdd(
                verification_manager, primary,
                verification_manager.constant(false), 0u);
            for (const GridCell& cell: prepared) {
                verification_tree.adjoin(cell);
            }
            verification_bdd.adjoin(prepared);
            verification_tree.recombine();
            verification_bdd.recombine();
            validate_geometry(tree, verification_tree);
            validate_geometry(tree, verification_bdd);
        }
        record("replay_bulk", [&](unsigned) {
                BinaryTreePaving rebuilt(grid);
                for( const GridCell & cell:prepared) rebuilt.adjoin(cell);
                rebuilt.recombine();
                return rebuilt; }, [&](unsigned) {
                RobddPaving rebuilt(
                    manager,primary,manager.constant(false),0u);
                rebuilt.adjoin(prepared);
                rebuilt.recombine();
                return rebuilt; }, [&](unsigned) {
                CachedBinaryTreePaving rebuilt(grid);
                rebuilt.adjoin(prepared);
                rebuilt.recombine();
                return rebuilt; });
    } else {
        LOGGING_PRINTLN("SKIP replay_bulk: " << shape
                                             << " d=" << dimension << " f=" << fineness
                                             << " leaves=" << tree_leaves
                                             << " > max-replay-cells=" << selection.max_replay_cells)
    }

    record("copy", [&](unsigned) { return BinaryTreePaving(tree); }, [&](unsigned) { return RobddPaving(robdd); }, [&](unsigned) { return CachedBinaryTreePaving(*cached); });

    record("iterate_leaves", [&](unsigned) { return visit_leaves(tree); }, [&](unsigned) { return visit_leaves(robdd); }, [&](unsigned) { return visit_leaves(*cached); });

    record("iterate_min", [&](unsigned) { return visit_minimum(tree, fineness); }, [&](unsigned) { return visit_minimum(robdd, fineness); }, [&](unsigned) { return visit_minimum(*cached, fineness); });

    if (selection.tree_iterator_cost) {
        // Compare only representations of the same immutable tree. Ordinary
        // BinaryTreePaving iterators own a deep copy; borrowed base iterators
        // are valid here because this scenario has no concurrent writers.
        const BinaryTreeSubpaving& stable_tree = tree;
        SizeType safe_checksum = visit_leaves(tree);
        SizeType borrowed_checksum = visit_borrowed_tree_leaves(stable_tree);
        if (safe_checksum != borrowed_checksum) {
            throw std::runtime_error(
                "stable borrowed-tree traversal disagrees with owner snapshot");
        }

        Timing owner_begin = median_ms(
            [&](unsigned) { return tree.begin(); },
            selection.iterations, selection.samples,
            selection.min_sample_ms, selection.max_iterations, selection.warmups);
        Timing borrowed_begin = median_ms(
            [&](unsigned) { return stable_tree.BinaryTreeSubpaving::begin(); },
            selection.iterations, selection.samples,
            selection.min_sample_ms, selection.max_iterations, selection.warmups);
        Timing owner_visit = median_ms(
            [&](unsigned) { return visit_leaves(tree); },
            selection.iterations, selection.samples,
            selection.min_sample_ms, selection.max_iterations, selection.warmups);
        Timing borrowed_visit = median_ms(
            [&](unsigned) { return visit_borrowed_tree_leaves(stable_tree); },
            selection.iterations, selection.samples,
            selection.min_sample_ms, selection.max_iterations, selection.warmups);

        LOGGING_PRINTLN("TREE_ITERATOR_COST shape=" << shape
                                                    << " d=" << dimension << " f=" << fineness
                                                    << " leaves=" << tree_leaves << " nodes=" << tree_nodes
                                                    << " owner_begin_ms=" << std::setprecision(9) << owner_begin.milliseconds
                                                    << " borrowed_begin_ms=" << borrowed_begin.milliseconds
                                                    << " owner_visit_ms=" << owner_visit.milliseconds
                                                    << " borrowed_visit_ms=" << borrowed_visit.milliseconds
                                                    << " begin_ratio_owner_over_borrowed="
                                                    << owner_begin.milliseconds / borrowed_begin.milliseconds
                                                    << " visit_ratio_owner_over_borrowed="
                                                    << owner_visit.milliseconds / borrowed_visit.milliseconds);
    }

    record("superset_16", [&](unsigned) { return probe_coverage(tree, probes); }, [&](unsigned) { return probe_coverage(robdd, probes); }, [&](unsigned) { return probe_coverage(*cached, probes); });

    GridCell right_half = primary.split(true);
    Timing tree_adjoin = median_mutation_ms(
        [&] { return std::make_unique<BinaryTreePaving>(tree); },
        [&](BinaryTreePaving& copy) {
            copy.adjoin(right_half);
            copy.recombine();
        },
        selection.iterations, selection.samples, selection.min_sample_ms,
        selection.max_iterations, selection.warmups);
    Timing robdd_adjoin = median_mutation_ms(
        [&] { return std::make_unique<RobddPaving>(robdd); },
        [&](RobddPaving& copy) {
            copy.adjoin(right_half);
            copy.recombine();
        },
        selection.iterations, selection.samples, selection.min_sample_ms,
        selection.max_iterations, selection.warmups);
    data.push_back({shape, "adjoin_recombine", "tree", dimension, fineness,
                    tree_adjoin.milliseconds, tree_leaves, tree_nodes,
                    tree_adjoin.iterations, tree_adjoin.statistics});
    data.push_back({shape, "adjoin_recombine", "robdd", dimension, fineness,
                    robdd_adjoin.milliseconds, robdd_leaves, reachable_nodes,
                    robdd_adjoin.iterations, robdd_adjoin.statistics});
    if (selection.include_cached_tree) {
        Timing cached_adjoin = median_mutation_ms(
            [&] {
                auto snapshot = std::make_unique<CachedBinaryTreePaving>(*cached);
                if (not snapshot->compile_cache())
                    throw std::runtime_error("Failed preparing CachedTree mutation");
                return snapshot;
            },
            [&](CachedBinaryTreePaving& copy) {
                copy.adjoin(right_half);
                copy.recombine();
            },
            selection.iterations, selection.samples, selection.min_sample_ms,
            selection.max_iterations, selection.warmups);
        data.push_back({shape, "adjoin_recombine", "cached-tree",
                        dimension, fineness, cached_adjoin.milliseconds, tree_leaves, tree_nodes,
                        cached_adjoin.iterations, cached_adjoin.statistics});
    }
    memory.back().robdd_manager_after_nodes = manager.node_count();
    memory.back().after_warm = manager.statistics();
}

void report(const std::vector<Datum>& data,
            const std::vector<MemoryDatum>& memory,
            const Selection& selection, unsigned skipped,
            const Bench::SummaryFields& fields) {
    LOGGING_PRINTLN("BinaryTree / ROBDD"
                    << (selection.include_cached_tree ? " / CachedTree" : "")
                    << " compact-set benchmark")
    LOGGING_PRINTLN("Minimum iterations=" << selection.iterations
                                          << "  samples=" << selection.samples
                                          << "  min_sample_ms=" << selection.min_sample_ms
                                          << "  max_iterations=" << selection.max_iterations
                                          << "  min_leaves=" << selection.min_leaves
                                          << "  manager=warm/shared  skipped_budget=" << skipped
                                          << "  concurrency=" << ThreadManager::instance().concurrency())
    LOGGING_PRINTLN("BDDs are still sequential: --concurrency configures"
                    " the threading runtime but does not parallelize BDD operations.")
    LOGGING_PRINTLN("Node column: tree physical nodes; ROBDD root-reachable"
                    " distinct nodes (not manager total).")
    if (not selection.summary_only) {
        Bench::Report report("Paving operation timings (nanoseconds)", fields);
        report.set_dimension_columns({
            {"Shape"},
            {"D", 0u, Bench::TableAlignment::Right},
            {"F", 0u, Bench::TableAlignment::Right},
            {"Backend"},
            {"Operation"},
            {"Median (ms)", 0u, Bench::TableAlignment::Right},
            {"Leaves", 0u, Bench::TableAlignment::Right},
            {"Nodes", 0u, Bench::TableAlignment::Right},
            {"Iterations", 0u, Bench::TableAlignment::Right}
        });
        auto options = Bench::TableOptions::ruled();
        options.group_columns = {0u, 1u, 2u};
        report.set_table_options(options);
        for (const auto& datum: data)
            report.add_row({datum.shape, std::to_string(datum.dimension),
                            std::to_string(datum.fineness), datum.backend, datum.operation,
                            Bench::Table::number(datum.milliseconds, 4u),
                            std::to_string(datum.leaves), std::to_string(datum.nodes),
                            std::to_string(datum.iterations)},
                           datum.statistics);
        report.print();
    }

    print_memory_results(memory);
    print_manager_statistics(memory);

    std::map<std::string, Aggregate> by_operation;
    std::map<std::string, Aggregate> by_shape;
    Aggregate overall;
    std::map<std::string, Aggregate> cached_by_operation;
    std::map<std::string, Aggregate> cached_by_shape;
    Aggregate cached_overall;
    std::size_t stride = selection.include_cached_tree ? 3u : 2u;
    for (std::size_t i = 0u; i < data.size(); i += stride) {
        if (i + 1u == data.size() or data[i].backend != "tree" or data[i + 1u].backend != "robdd" or data[i].operation != data[i + 1u].operation or data[i].shape != data[i + 1u].shape or data[i].dimension != data[i + 1u].dimension or data[i].fineness != data[i + 1u].fineness) {
            throw std::runtime_error("unpaired tree/ROBDD benchmark rows");
        }
        double ratio = data[i + 1u].milliseconds / data[i].milliseconds;
        std::string case_name = data[i].shape + "/" + data[i].operation + "/d" + std::to_string(data[i].dimension) + "/f" + std::to_string(data[i].fineness);
        by_operation[data[i].operation].add(ratio, case_name);
        by_shape[data[i].shape].add(ratio, case_name);
        overall.add(ratio, case_name);
        if (selection.include_cached_tree) {
            if (i + 2u >= data.size() or data[i + 2u].backend != "cached-tree" or data[i + 2u].operation != data[i].operation or data[i + 2u].shape != data[i].shape or data[i + 2u].dimension != data[i].dimension or data[i + 2u].fineness != data[i].fineness)
                throw std::runtime_error("unpaired CachedTree benchmark row");
            double cache_ratio = data[i + 2u].milliseconds / data[i].milliseconds;
            cached_by_operation[data[i].operation].add(cache_ratio, case_name);
            cached_by_shape[data[i].shape].add(cache_ratio, case_name);
            cached_overall.add(cache_ratio, case_name);
        }
    }
    LOGGING_PRINTLN("Speed ratios: ROBDD median / tree median"
                    << " (>1 means tree is faster; <1 means ROBDD is faster).")
    LOGGING_PRINTLN(std::string(68, '='))
    LOGGING_PRINTLN(std::left << std::setw(24) << "Group"
                              << "  " << std::right << std::setw(6) << "Cases"
                              << "  " << std::setw(13) << "Geomean"
                              << "  " << std::setw(13) << "Worst")
    LOGGING_PRINTLN(std::string(68, '-'))
    auto line = [](const std::string& name, const Aggregate& a) {
        LOGGING_PRINTLN(std::left << std::setw(24) << name
                                  << "  " << std::right << std::setw(6) << a.count
                                  << "  " << std::fixed << std::setprecision(3)
                                  << std::setw(12) << a.geometric_mean() << "x"
                                  << "  " << std::setw(12) << a.worst << "x")
    };
    for (const auto& entry: by_operation) {
        line(entry.first, entry.second);
    }
    LOGGING_PRINTLN(std::string(68, '-'))
    for (const auto& entry: by_shape) {
        line(entry.first, entry.second);
    }
    LOGGING_PRINTLN(std::string(68, '-'))
    if (overall.count != 0u) {
        line("ALL", overall);
        LOGGING_PRINTLN("Worst case: " << overall.worst_case)
    }
    LOGGING_PRINTLN(std::string(68, '='))
    LOGGING_PRINTLN("Each statistic is a geometric mean of paired ratios,"
                    << " not a sum of unrelated operation times.")
    if (selection.include_cached_tree) {
        LOGGING_PRINTLN("Speed ratios: CachedTree median / Tree median"
                        << " (>1 means Tree is faster; <1 means CachedTree is faster).")
        LOGGING_PRINTLN(std::string(68, '='))
        for (const auto& entry: cached_by_operation)
            line(entry.first, entry.second);
        LOGGING_PRINTLN(std::string(68, '-'))
        for (const auto& entry: cached_by_shape)
            line(entry.first, entry.second);
        LOGGING_PRINTLN(std::string(68, '-'))
        if (cached_overall.count != 0u) {
            line("ALL", cached_overall);
            LOGGING_PRINTLN("Worst CachedTree case: " << cached_overall.worst_case)
        }
        LOGGING_PRINTLN("CachedTree superset_16 uses a precompiled ROBDD index;"
                        << " other operations use the authoritative Tree backend."
                        << " CachedTree Nodes count excludes the separate ROBDD cache.")
    }
}

} // namespace

int main(int argc, const char* argv[]) {
    try {
        Selection selection;
        verify_reachable_node_oracle();
        Bench::CommandLine command_line;
        ThreadManager::instance();
        command_line.add(threading_command_line_contribution());
        command_line.add(CommandLineContribution(
            "paving-backends-benchmark", parsers(selection)));
        if (not command_line.acquire(argc, argv))
            return 0;
        selection.warmups = command_line.warmups();

        if (selection.outer_preparation_verify_only and not(selection.compare_outer_preparation or
                                                            selection.compare_outer_overlap))
            throw std::invalid_argument(
                "--verify-only requires an opt-in outer comparison");
        if (selection.compare_outer_overlap) {
            if (selection.compare_outer_preparation or selection.outer_phases or not selection.outer_trace_prefix.empty() or selection.tree_iterator_cost)
                throw std::invalid_argument(
                    "--compare-outer-overlap cannot combine with other"
                    " opt-in outer comparisons or --tree-iterator-cost");
            std::vector<OuterOverlapResult> rows;
            for (unsigned d: selection.dimensions) {
                for (unsigned f: selection.finenesses) {
                    unsigned long long bits =
                        (static_cast<unsigned long long>(d) + f) * d;
                    if (bits > selection.max_depth_bits) {
                        LOGGING_PRINTLN("OUTER_OVERLAP_SKIP d=" << d << " f=" << f
                                                                << " bits=" << bits << " max_bits=" << selection.max_depth_bits)
                        continue;
                    }
                    for (const auto& shape: selection.shapes)
                        benchmark_outer_overlap(selection, shape, d, f, rows);
                }
            }
            report_outer_overlap(rows, selection);
            return 0;
        }
        if (selection.compare_outer_preparation) {
            if (selection.outer_phases or not selection.outer_trace_prefix.empty() or selection.dimensions != std::vector<unsigned>{4u} or selection.finenesses != std::vector<unsigned>{3u} or selection.max_depth_bits < 28u)
                throw std::invalid_argument(
                    "--compare-outer-preparation requires --dimensions 4"
                    " --finenesses 3 --max-depth-bits 28; cannot combine"
                    " with --outer-phases or --outer-trace-prefix");
            std::vector<OuterPreparationResult> rows;
            rows.reserve(selection.shapes.size());
            for (const auto& shape: selection.shapes) {
                LOGGING_PRINTLN("Measuring outer preparation: " << shape << " ...")
                rows.push_back(benchmark_outer_preparation(selection, shape));
                LOGGING_PRINTLN("Completed outer preparation: " << shape)
            }
            report_outer_preparation(selection, rows);
            return 0;
        }

        if (not selection.outer_trace_prefix.empty()) {
            if (selection.dimensions.size() != 1u or selection.dimensions[0] != 4u or selection.finenesses.size() != 1u or selection.finenesses[0] != 3u) {
                throw std::invalid_argument(
                    "--outer-trace-prefix currently requires --dimensions 4 --finenesses 3");
            }
            Grid grid(4u);
            for (const std::string& shape: selection.shapes) {
                EffectiveConstrainedImageSet compact = make_shape(4u, shape);
                const EffectiveEuclideanCompactSetInterface& set = compact;
                export_outer_solver_trace(
                    selection.outer_trace_prefix, shape, 4u, 3u, set, grid);
            }
            return 0;
        }

        std::vector<Datum> data;
        std::vector<MemoryDatum> memory;
        unsigned skipped = 0u;
        for (unsigned d: selection.dimensions) {
            for (unsigned f: selection.finenesses) {
                // The generic outer-approximation implementation can
                // explore up to roughly (d+f)*d binary decisions even
                // for small compact sets. Do not silently attempt the
                // infeasible full cartesian experiment matrix.
                const unsigned long long estimated =
                    (static_cast<unsigned long long>(d) + f) * d;
                if (estimated > selection.max_depth_bits) {
                    skipped += static_cast<unsigned>(selection.shapes.size());
                    LOGGING_PRINTLN("SKIP dimension=" << d << " fineness=" << f
                                                      << " estimated_bits=" << estimated
                                                      << " > max-depth-bits=" << selection.max_depth_bits)
                    continue;
                }
                for (const auto& shape: selection.shapes) {
                    LOGGING_PRINTLN("Measuring " << shape << " | d=" << d
                                                 << " | fineness=" << f)
                    append_case(data, memory, selection, shape, d, f);
                }
            }
        }
        if (data.empty()) {
            throw std::runtime_error("no nondegenerate cases within the selection"
                                     " (lower --min-leaves or increase --finenesses)");
        }
        report(data, memory, selection, skipped, command_line.summary_fields());
        if (selection.run_retention)
            print_manager_retention(selection);
        return 0;
    } catch (const std::exception& error) {
        LOGGING_PRINTLN("benchmark_paving_backends: " << error.what())
        return 1;
    }
}
