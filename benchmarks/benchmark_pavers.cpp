/***************************************************************************
 *            benchmarks/benchmark_pavers.cpp
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

/** @file benchmarks/benchmark_pavers.cpp
 *  @brief Compare certified outer approximation Paver strategies on common sets.
 */

#include <algorithm>
#include <chrono>
#include <cmath>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "logging/logging.hpp"
#include "paving/binary_tree_paving.hpp"
#include "paving/paver.hpp"
#include "paving_benchmark_shapes.hpp"

#include "ariadne-benchmarking.hpp"

using namespace Ariadne;
namespace Bench = Ariadne::Benchmarking;

namespace {

struct Strategy {
    std::string name;
    Paver implementation;
    bool affine = false;
};

struct Outcome {
    std::string name;
    std::unique_ptr<BinaryTreePaving> paving;
};

// A geometry comparison, not a check of equal cell encodings.
// Each tree's enabled cells must be completely covered by the other tree.
bool contains(const BinaryTreePaving& outer, const BinaryTreePaving& inner) {
    for (auto it = inner.begin(), end = inner.end(); it != end; ++it) {
        if (not outer.superset(*it))
            return false;
    }
    return true;
}

std::string relation(const BinaryTreePaving& first,
                     const BinaryTreePaving& second) {
    const bool first_contains_second = contains(first, second);
    const bool second_contains_first = contains(second, first);
    if (first_contains_second and second_contains_first)
        return "equal";
    if (first_contains_second)
        return "A strictly contains B";
    if (second_contains_first)
        return "B strictly contains A";
    return "incomparable";
}

std::vector<Strategy> strategies() {
    return {
        {"affine", Paver(AffinePaver()), true},
        {"subdivision", Paver(SubdivisionPaver())},
        {"reduce", Paver(ReducePaver())},
        {"constraint", Paver(ConstraintPaver())}
    };
}

// Preserve user-selected order in timing and pairwise comparison rows.
// Reject unknown names, duplicates and empty entries before running cases.
std::vector<Strategy> parse_strategies(const String& value) {
    const std::string input(value);
    const auto available = strategies();
    if (input == "all")
        return available;
    if (input.empty() or input.front() == ',' or input.back() == ',')
        throw std::invalid_argument("invalid Paver selection");

    std::vector<Strategy> selected;
    std::size_t start = 0u;
    do {
        const std::size_t end = input.find(',', start);
        const std::string name = input.substr(
            start, end == std::string::npos ? end : end - start);
        const auto found = std::find_if(
            available.begin(), available.end(),
            [&name](const Strategy& strategy) { return strategy.name == name; });
        const bool duplicate = std::any_of(
            selected.begin(), selected.end(),
            [&name](const Strategy& strategy) { return strategy.name == name; });
        if (found == available.end() or duplicate)
            throw std::invalid_argument("unknown or duplicate Paver: " + name);
        selected.push_back(*found);
        if (end == std::string::npos)
            break;
        start = end + 1u;
    } while (true);
    return selected;
}

// Independently verified interior regions are optional: curves have no
// full-dimensional interior, and empty sets must stay empty.
void verify_result(const BinaryTreePaving& paving,
                   const PavingBenchmarkCases::PaverBenchmarkCase& test_case,
                   const std::string& name, const std::string& strategy) {
    if (test_case.expected_empty) {
        if (not paving.is_empty())
            throw std::runtime_error(
                "Nonempty approximation of a proven empty set: " +
                name + "/" + strategy);
    } else {
        if (paving.is_empty())
            throw std::runtime_error(
                "Empty approximation of a nonempty set: " +
                name + "/" + strategy);
        if (test_case.interior and not paving.superset(*test_case.interior))
            throw std::runtime_error(
                "Missing certified interior: " + name + "/" + strategy);
    }
}

double measured_volume(const BinaryTreePaving& paving) {
    return paving.measure().raw().get_d();
}

void run(const Bench::CommandLine& cli, const Bench::Matrix& matrix,
         const Bench::Parameter<unsigned>& dimensions,
         const Bench::Parameter<unsigned>& finenesses,
         const Bench::Parameter<std::string>& shapes,
         const Bench::Parameter<unsigned>& samples,
         bool verify_only, unsigned max_case_ms,
         const std::vector<Strategy>& implementations,
         bool show_diagnostics) {
    if (implementations.empty())
        throw std::invalid_argument("at least one Paver must be selected");
    Bench::Runner runner(cli, matrix.combinations());
    Bench::Runner sample_runner(0u, matrix.combinations());
    Bench::SummaryFields fields = cli.summary_fields();
    if (fields == Bench::default_summary_fields())
        fields.insert(fields.begin(), Bench::SummaryField::Median);
    Bench::Report report("Paver outer approximations: time, tightness and depth limits",
                         fields);
    report.set_time_unit(Bench::ReportTimeUnit::Milliseconds);
    report.set_dimension_columns({{"Case"}, {"Dimension"}, {"Fineness"}, {"Paver"}, {"Cells"}, {"Outer volume"}, {"Case volume"}, {"Outer/case"}, {"Depth cap"}, {"Unresolved"}, {"Splits"}, {"Samples"}, {"Budget"}});
    std::string report_header = matrix.header() + " | pavers=[";
    for (const Strategy& strategy: implementations) {
        if (&strategy != &implementations.front())
            report_header += ",";
        report_header += strategy.name;
    }
    report_header += "]";
    report.set_parameter_header(report_header);
    Bench::Table geometry({{"Case"}, {"Dimension"}, {"Fineness"}, {"A"}, {"B"}, {"Geometric relation"}});
    Bench::Table work({{"Case"}, {"Dim"}, {"Fine"}, {"Paver"}, {"Visits"}, {"Param splits"}, {"Cell splits"}, {"Map evals"}, {"Constraint evals"}, {"Sensitivity"}, {"Reductions"}, {"Constraint reductions"}, {"Box reducers"}, {"Hull reducers"}, {"Joins"}, {"Rejected"}, {"Covered"}, {"Adjoins"}});
    Bench::Table timings({{"Case"}, {"Dim"}, {"Fine"}, {"Paver"}, {"Map (ms)"}, {"Constraints (ms)"}, {"Joint (ms)"}, {"Sensitivity (ms)"}, {"Box red. (ms)"}, {"Hull red. (ms)"}, {"Join (ms)"}, {"Adjoin (ms)"}, {"Other (ms)"}, {"Profiled (ms)"}});
    // ConstraintPaver-specific decomposition of the prior "Other" bucket.
    Bench::Table traversal({{"Case"}, {"Dim"}, {"Fine"}, {"Coverage"}, {"Cell boxes"}, {"Products"}, {"Disjoint tests"}, {"Superset (ms)"}, {"Cell box (ms)"}, {"Product (ms)"}, {"Disjoint (ms)"}});

    matrix.for_each([&](const Bench::Configuration& configuration) {
        const unsigned d = configuration.value(dimensions);
        const unsigned f = configuration.value(finenesses);
        const std::string& shape = configuration.value(shapes);
        const unsigned n = configuration.value(samples);

        if (d < 2u or d > 6u or f > 4u or n == 0u)
            throw std::invalid_argument(
                "dimensions must be 2..6, finenesses 0..4, samples positive");

        // Geometry and validation data are constructed outside the timer.
        const auto test_case = PavingBenchmarkCases::make_paver_case(d, shape);
        const auto& set = test_case.set;
        const Grid grid(d);
        const double shape_volume = test_case.exact_volume;
        std::vector<Outcome> outcomes;
        runner.configuration_started(configuration);

        for (const Strategy& strategy: implementations) {
            std::unique_ptr<BinaryTreePaving> paving;
            SizeType expected_cells = 0u;
            double expected_volume = 0.0;
            AffinePaverDiagnostics diagnostics;
            AffinePaverDiagnostics expected_diagnostics;
            bool reference_recorded = false;

            auto prepare = [&] {
                paving = std::make_unique<BinaryTreePaving>(grid);
            };
            auto operate = [&] {
                if (strategy.affine) {
                    AffinePaver().adjoin_outer_approximation(
                        *paving, set, f, diagnostics);
                } else {
                    strategy.implementation.adjoin_outer_approximation(
                        *paving, set, f);
                }
            };
            auto verify = [&] {
                // Canonicalization, correctness checks and size/volume
                // accounting are outside the Paver execution timer.
                paving->recombine();
                verify_result(*paving, test_case, shape, strategy.name);
                const SizeType cells = paving->size();
                const double volume = measured_volume(*paving);
                if (not std::isfinite(volume) or
                    volume < shape_volume * (1.0 - 1e-10)) {
                    throw std::runtime_error(
                        "Outer volume smaller than theoretical set volume: " +
                        shape + "/" + strategy.name);
                }
                if (not reference_recorded) {
                    expected_cells = cells;
                    expected_volume = volume;
                    expected_diagnostics = diagnostics;
                    reference_recorded = true;
                } else if (cells != expected_cells or volume != expected_volume or
                           (strategy.affine and
                            (diagnostics.depth_limit_reached() != expected_diagnostics.depth_limit_reached() or
                             diagnostics.unresolved_domains != expected_diagnostics.unresolved_domains or
                             diagnostics.split_domains != expected_diagnostics.split_domains))) {
                    throw std::runtime_error(
                        "Paver output or diagnostics differ between identical runs: " +
                        shape + "/" + strategy.name);
                }
                Bench::keep_alive(cells);
            };

            if (verify_only) {
                prepare();
                operate();
                verify();
                LOGGING_PRINTLN("PAVER_VERIFY case=" << shape
                                                     << " d=" << d << " f=" << f
                                                     << " paver=" << strategy.name
                                                     << " cells=" << expected_cells
                                                     << " volume=" << expected_volume
                                                     << " ratio=" << (shape_volume > 0.0 ? Bench::Table::number(expected_volume / shape_volume, 3u) : "n/a")
                                                     << " depth_capped=" << (strategy.affine and expected_diagnostics.depth_limit_reached())
                                                     << " unresolved=" << (strategy.affine ? expected_diagnostics.unresolved_domains : 0u))
            } else {
                // The framework Runner executes one sample at a time so
                // the soft budget can be checked between completed calls.
                // No safe cancellation of a Paver already running is possible.
                for (unsigned i = 0u; i < cli.warmups(); ++i) {
                    prepare();
                    operate();
                    verify();
                }
                Bench::Samples measured;
                double measured_ms = 0.0;
                bool budget_hit = false;
                for (unsigned i = 0u; i < n; ++i) {
                    const Bench::Samples sample = sample_runner.run(
                        strategy.name, 1u, prepare, operate, verify);
                    const double nanoseconds = sample.values().front();
                    measured.add(nanoseconds);
                    measured_ms += nanoseconds / 1e6;
                    if (max_case_ms != 0u and measured_ms >= max_case_ms and
                        i + 1u < n) {
                        budget_hit = true;
                        break;
                    }
                }
                const Bench::Statistics stats = Bench::summarize(measured);
                if (not std::isfinite(stats.median) or stats.median <= 0.0)
                    throw std::runtime_error("Invalid Paver timing");
                const std::string ratio = shape_volume > 0.0
                                            ? Bench::Table::number(expected_volume / shape_volume, 3u)
                                            : "n/a";
                report.add_row({shape, std::to_string(d), std::to_string(f),
                                strategy.name, std::to_string(expected_cells),
                                Bench::Table::number(expected_volume, 6u),
                                Bench::Table::number(shape_volume, 6u),
                                ratio,
                                strategy.affine ? (expected_diagnostics.depth_limit_reached() ? "HIT" : "no") : "n/a",
                                strategy.affine ? std::to_string(expected_diagnostics.unresolved_domains) : "n/a",
                                strategy.affine ? std::to_string(expected_diagnostics.split_domains) : "n/a",
                                std::to_string(measured.size()) + "/" + std::to_string(n),
                                budget_hit ? "SOFT LIMIT" : "complete"},
                               stats);
            }
            if (show_diagnostics and not strategy.affine) {
                // Instrumentation is deliberately outside every timed sample.
                // Compare the complete cell geometry, not only count/volume.
                BinaryTreePaving instrumented(grid);
                PaverDiagnostics counters;
                const auto profile_begin = std::chrono::steady_clock::now();
                if (strategy.name == "subdivision") {
                    SubdivisionPaver().adjoin_outer_approximation(instrumented, set, f, counters);
                } else if (strategy.name == "reduce") {
                    ReducePaver().adjoin_outer_approximation(instrumented, set, f, counters);
                } else {
                    ConstraintPaver().adjoin_outer_approximation(instrumented, set, f, counters);
                }
                const auto profile_elapsed = std::chrono::duration_cast<
                                                 std::chrono::nanoseconds>(std::chrono::steady_clock::now() -
                                                                           profile_begin)
                                                 .count();
                instrumented.recombine();
                verify_result(instrumented, test_case, shape, strategy.name);
                if (not contains(instrumented, *paving) or
                    not contains(*paving, instrumented))
                    throw std::runtime_error(
                        "Instrumentation changed the paving geometry: " +
                        shape + "/" + strategy.name);
                work.add_row({shape, std::to_string(d), std::to_string(f), strategy.name,
                              std::to_string(counters.visited_nodes),
                              std::to_string(counters.parameter_splits),
                              std::to_string(counters.cell_splits),
                              std::to_string(counters.map_evaluations),
                              std::to_string(counters.constraint_evaluations),
                              std::to_string(counters.sensitivity_evaluations),
                              std::to_string(counters.reduction_calls),
                              std::to_string(counters.constraint_reduction_calls),
                              std::to_string(counters.box_reduction_calls),
                              std::to_string(counters.hull_reduction_calls),
                              std::to_string(counters.join_constructions),
                              std::to_string(counters.disjoint_rejections),
                              std::to_string(counters.already_covered),
                              std::to_string(counters.adjoin_calls)});
                const std::uint64_t accounted_ns =
                    counters.map_range_ns + counters.constraint_range_ns +
                    counters.joint_range_ns + counters.sensitivity_ns +
                    counters.box_reduction_ns + counters.hull_reduction_ns +
                    counters.join_ns + counters.adjoin_ns +
                    counters.coverage_ns + counters.cell_box_ns +
                    counters.product_box_ns + counters.disjoint_ns;
                const auto ms = [](std::uint64_t ns) {
                    return Bench::Table::number(
                        static_cast<double>(ns) / 1000000.0, 3u);
                };
                const std::uint64_t total_ns =
                    static_cast<std::uint64_t>(profile_elapsed);
                // Residual after all individually measured operations:
                // recursion, splitting, solver setup, and profiling overhead.
                // The ConstraintPaver traversal costs are shown separately,
                // not double-counted in this residual.
                const std::uint64_t other_ns =
                    total_ns > accounted_ns ? total_ns - accounted_ns : 0u;
                timings.add_row({shape, std::to_string(d), std::to_string(f), strategy.name,
                                 ms(counters.map_range_ns),
                                 ms(counters.constraint_range_ns),
                                 ms(counters.joint_range_ns),
                                 ms(counters.sensitivity_ns),
                                 ms(counters.box_reduction_ns),
                                 ms(counters.hull_reduction_ns),
                                 ms(counters.join_ns),
                                 ms(counters.adjoin_ns),
                                 ms(other_ns), ms(total_ns)});
                if (strategy.name == "constraint") {
                    traversal.add_row({shape, std::to_string(d), std::to_string(f),
                                       std::to_string(counters.coverage_checks),
                                       std::to_string(counters.cell_box_constructions),
                                       std::to_string(counters.product_box_constructions),
                                       std::to_string(counters.disjoint_checks),
                                       ms(counters.coverage_ns), ms(counters.cell_box_ns),
                                       ms(counters.product_box_ns), ms(counters.disjoint_ns)});
                }
            }
            outcomes.push_back({strategy.name, std::move(paving)});
        }
        for (std::size_t i = 0u; i != outcomes.size(); ++i) {
            for (std::size_t j = i + 1u; j != outcomes.size(); ++j) {
                geometry.add_row({shape, std::to_string(d), std::to_string(f),
                                  outcomes[i].name, outcomes[j].name,
                                  relation(*outcomes[i].paving, *outcomes[j].paving)});
            }
        }
        runner.configuration_completed(configuration);
    });
    if (not verify_only)
        report.print();
    if (show_diagnostics) {
        LOGGING_PRINTLN("Paver operation counts (separate, untimed execution):")
        LOGGING_PRINTLN(work.str())
        LOGGING_PRINTLN("Paver operation timings (instrumented replay only):")
        LOGGING_PRINTLN(timings.str())
        for (const Strategy& strategy: implementations) {
            if (strategy.name == "constraint") {
                LOGGING_PRINTLN("ConstraintPaver traversal breakdown (instrumented replay only):")
                LOGGING_PRINTLN(traversal.str())
                break;
            }
        }
    }
    if (implementations.size() > 1u) {
        LOGGING_PRINTLN("Pairwise geometric containment (exact cell coverage):")
        LOGGING_PRINTLN(geometry.str())
    } else {
        LOGGING_PRINTLN("Pairwise comparison skipped: one Paver selected.")
    }
}
} // namespace

int main(int argc, const char* argv[]) {
    try {
        Bench::Parameter<unsigned> dimensions(
            "dimensions", "Space dimensions (2..6)", {2u});
        Bench::Parameter<unsigned> finenesses(
            "finenesses", "Grid fineness (0..4)", {1u});
        Bench::Parameter<std::string> shapes(
            "shapes", "Paver cases: five geometric, affine-shear, quadratic-shear,"
                      " folded-quadratic, uv-triangle, affine-projection, correlated-strip,"
                      " product-cap, parabolic-curve, empty-constraints",
            {"aligned-box", "rotated-box", "aligned-ellipsoid",
             "rotated-ellipsoid", "ball", "affine-shear", "quadratic-shear",
             "folded-quadratic", "uv-triangle", "affine-projection",
             "correlated-strip", "product-cap", "parabolic-curve",
             "empty-constraints"},
            [](const std::string& name) {
                const auto accepted = PavingBenchmarkCases::parse_paver_cases(String(name));
                if (accepted.size() != 1u)
                    throw std::invalid_argument("one case per matrix value");
                return accepted.front();
            });
        Bench::Parameter<unsigned> samples(
            "samples", "Measured samples per strategy", {5u});
        Bench::Matrix matrix;
        matrix.add(dimensions).add(finenesses).add(shapes).add(samples);

        bool verify_only = false;
        bool show_diagnostics = false;
        unsigned max_case_ms = 2000u;
        std::vector<Strategy> selected_pavers = strategies();
        Bench::CommandLine cli;
        cli.add_parameters(matrix);
        List<ArgumentParser> options;
        options.append(make_valued_argument_parser(
            "", "pavers",
            "Comma-separated Pavers: affine,subdivision,reduce,constraint (default: all)",
            [&selected_pavers](const String& value) -> VoidFunction {
                auto parsed = parse_strategies(value);
                return [&selected_pavers, parsed = std::move(parsed)]() mutable {
                    selected_pavers = std::move(parsed);
                };
            }));
        options.append(make_unvalued_argument_parser(
            "", "verify-only", "Validate all selected cases without timing",
            [&verify_only] { verify_only = true; }));
        options.append(make_unvalued_argument_parser(
            "", "diagnostics",
            "Collect Paver work counters in a separate untimed run",
            [&show_diagnostics] { show_diagnostics = true; }));
        options.append(make_valued_argument_parser(
            "", "max-case-ms",
            "Soft per-strategy timed budget in ms (0 disables; default 2000)",
            [&max_case_ms](const String& value) -> VoidFunction {
                const unsigned ms = Bench::parse_unsigned<unsigned>(value);
                return [&max_case_ms, ms] { max_case_ms = ms; };
            }));
        cli.add(CommandLineContribution("pavers-benchmark", options));
        if (not cli.acquire(argc, argv))
            return 0;

        run(cli, matrix, dimensions, finenesses, shapes, samples,
            verify_only, max_case_ms, selected_pavers, show_diagnostics);
        return 0;
    } catch (const std::exception& error) {
        LOGGING_PRINTLN("benchmark_pavers: " << error.what())
        return 1;
    }
}
