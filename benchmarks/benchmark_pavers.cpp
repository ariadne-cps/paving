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
#include <cmath>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <string>
#include <vector>

#include "logging/logging.hpp"
#include "paving/binary_tree_paving.hpp"
#include "paving/paver.hpp"

#include "ariadne-benchmarking.hpp"
#include "paving_benchmark_shapes.hpp"

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

// The geometries are fully contained in the unit box; in higher dimensions
// boxes have additional width 1/2, ellipsoids and balls have radius 1/4.
// Rotation is orthogonal, so it preserves their Euclidean volume.
double analytical_volume(unsigned dimension, const std::string& shape) {
    const double half_dimensions = static_cast<double>(dimension) / 2.0;
    if (shape == "aligned-box" or shape == "rotated-box")
        return 0.125 * std::pow(0.5, static_cast<double>(dimension - 2u));
    const double unit_ball_volume =
        std::pow(std::acos(-1.0), half_dimensions) /
        std::tgamma(half_dimensions + 1.0);
    if (shape == "ball")
        return unit_ball_volume * std::pow(0.25, static_cast<double>(dimension));
    if (shape == "aligned-ellipsoid" or shape == "rotated-ellipsoid")
        return unit_ball_volume * 0.5 *
               std::pow(0.25, static_cast<double>(dimension));
    throw std::invalid_argument("unknown benchmark geometry");
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

// These exact dyadic boxes lie strictly inside every benchmark shape,
// including rotated ellipsoids. This verifies a known interior region rather
// than demanding equality between two different valid outer approximations.
void verify_result(const BinaryTreePaving& paving, unsigned dimension,
                   const std::string& shape, const std::string& strategy) {
    const ExactBoxType interior(
        dimension, ExactIntervalType(0.49609375_x, 0.50390625_x));
    if (paving.is_empty() or not paving.superset(interior))
        throw std::runtime_error(
            "Missing certified interior: shape=" + shape +
            " paver=" + strategy + " dimension=" + std::to_string(dimension));
}

double measured_volume(const BinaryTreePaving& paving) {
    return paving.measure().raw().get_d();
}

void run(const Bench::CommandLine& cli, const Bench::Matrix& matrix,
         const Bench::Parameter<unsigned>& dimensions,
         const Bench::Parameter<unsigned>& finenesses,
         const Bench::Parameter<std::string>& shapes,
         const Bench::Parameter<unsigned>& samples,
         bool verify_only) {
    Bench::Runner runner(cli, matrix.combinations());
    Bench::SummaryFields fields = cli.summary_fields();
    if (fields == Bench::default_summary_fields())
        fields.insert(fields.begin(), Bench::SummaryField::Median);
    Bench::Report report("Paver outer approximations: time, tightness and depth limits",
                         fields);
    report.set_time_unit(Bench::ReportTimeUnit::Milliseconds);
    report.set_dimension_columns({
        {"Shape"}, {"Dimension"}, {"Fineness"}, {"Paver"},
        {"Cells"}, {"Outer volume"}, {"Shape volume"}, {"Outer/shape"},
        {"Depth cap"}, {"Unresolved"}, {"Splits"}
    });
    report.set_parameter_header(matrix.header());
    Bench::Table geometry({
        {"Shape"}, {"Dimension"}, {"Fineness"},
        {"A"}, {"B"}, {"Geometric relation"}
    });

    const auto implementations = strategies();
    matrix.for_each([&](const Bench::Configuration& configuration) {
        const unsigned d = configuration.value(dimensions);
        const unsigned f = configuration.value(finenesses);
        const std::string& shape = configuration.value(shapes);
        const unsigned n = configuration.value(samples);

        if (d < 2u or d > 6u or f > 4u or n == 0u)
            throw std::invalid_argument(
                "dimensions must be 2..6, finenesses 0..4, samples positive");

        // Not measured: geometry, constraints and the validated input.
        const ValidatedConstrainedImageSet set =
            PavingBenchmarkCases::make_validated_shape(d, shape);
        const Grid grid(d);
        const double shape_volume = analytical_volume(d, shape);
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
                verify_result(*paving, d, shape, strategy.name);
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
                LOGGING_PRINTLN("PAVER_VERIFY shape=" << shape
                                << " d=" << d << " f=" << f
                                << " paver=" << strategy.name
                                << " cells=" << expected_cells
                                << " volume=" << expected_volume
                                << " volume_ratio=" << expected_volume / shape_volume
                                << " depth_capped=" << (strategy.affine and expected_diagnostics.depth_limit_reached())
                                << " unresolved=" << (strategy.affine ? expected_diagnostics.unresolved_domains : 0u))
            } else {
                auto measured = runner.run(
                    strategy.name, n, prepare, operate, verify);
                const Bench::Statistics stats = Bench::summarize(measured);
                if (not std::isfinite(stats.median) or stats.median <= 0.0)
                    throw std::runtime_error("Invalid Paver timing");
                report.add_row({
                    shape, std::to_string(d), std::to_string(f),
                    strategy.name, std::to_string(expected_cells),
                    Bench::Table::number(expected_volume, 6u),
                    Bench::Table::number(shape_volume, 6u),
                    Bench::Table::number(expected_volume / shape_volume, 3u),
                    strategy.affine ? (expected_diagnostics.depth_limit_reached() ? "HIT" : "no") : "n/a",
                    strategy.affine ? std::to_string(expected_diagnostics.unresolved_domains) : "n/a",
                    strategy.affine ? std::to_string(expected_diagnostics.split_domains) : "n/a"
                }, stats);
            }
            outcomes.push_back({strategy.name, std::move(paving)});
        }
        for (std::size_t i = 0u; i != outcomes.size(); ++i) {
            for (std::size_t j = i + 1u; j != outcomes.size(); ++j) {
                geometry.add_row({
                    shape, std::to_string(d), std::to_string(f),
                    outcomes[i].name, outcomes[j].name,
                    relation(*outcomes[i].paving, *outcomes[j].paving)
                });
            }
        }
        runner.configuration_completed(configuration);
    });
    if (not verify_only)
        report.print();
    LOGGING_PRINTLN("Pairwise geometric containment (exact cell coverage):")
    LOGGING_PRINTLN(geometry.str())
}
} // namespace

int main(int argc, const char* argv[]) {
    try {
        Bench::Parameter<unsigned> dimensions(
            "dimensions", "Space dimensions (2..6)", {2u});
        Bench::Parameter<unsigned> finenesses(
            "finenesses", "Grid fineness (0..4)", {1u});
        Bench::Parameter<std::string> shapes(
            "shapes", "Geometry (aligned-box, rotated-box, aligned-ellipsoid, rotated-ellipsoid, ball)",
            {"aligned-box", "rotated-box", "aligned-ellipsoid",
             "rotated-ellipsoid", "ball"},
            [](const std::string& name) {
                const auto accepted = PavingBenchmarkCases::parse_shapes(String(name));
                if (accepted.size() != 1u)
                    throw std::invalid_argument("one shape per matrix value");
                return accepted.front();
            });
        Bench::Parameter<unsigned> samples(
            "samples", "Measured samples per strategy", {5u});
        Bench::Matrix matrix;
        matrix.add(dimensions).add(finenesses).add(shapes).add(samples);

        bool verify_only = false;
        Bench::CommandLine cli;
        cli.add_parameters(matrix);
        List<ArgumentParser> options;
        options.append(make_unvalued_argument_parser(
            "", "verify-only", "Validate all selected cases without timing",
            [&verify_only] { verify_only = true; }));
        cli.add(CommandLineContribution("pavers-benchmark", options));
        if (not cli.acquire(argc, argv))
            return 0;

        run(cli, matrix, dimensions, finenesses, shapes, samples, verify_only);
        return 0;
    } catch (const std::exception& error) {
        LOGGING_PRINTLN("benchmark_pavers: " << error.what())
        return 1;
    }
}
