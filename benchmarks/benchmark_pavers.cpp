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

#include <cmath>
#include <memory>
#include <sstream>
#include <stdexcept>
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
};

std::vector<Strategy> strategies() {
    return {
        {"affine", Paver(AffinePaver())},
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

std::string measured_volume(const BinaryTreePaving& paving) {
    std::ostringstream out;
    out << paving.measure().raw();
    return out.str();
}

void run(const Bench::CommandLine& cli, const Bench::Matrix& matrix,
         const Bench::Parameter<unsigned>& dimensions,
         const Bench::Parameter<unsigned>& finenesses,
         const Bench::Parameter<std::string>& shapes,
         const Bench::Parameter<unsigned>& samples,
         bool verify_only) {
    Bench::Runner runner(cli, matrix.combinations());
    Bench::Report report("Paver outer approximations: time and enclosure size",
                         cli.summary_fields());
    report.set_time_unit(Bench::ReportTimeUnit::Milliseconds);
    report.set_dimension_columns({
        {"Shape"}, {"Dimension"}, {"Fineness"}, {"Paver"},
        {"Cells"}, {"Enclosure volume (approx.)"}
    });
    report.set_parameter_header(matrix.header());

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
        runner.configuration_started(configuration);

        for (const Strategy& strategy: implementations) {
            std::unique_ptr<BinaryTreePaving> paving;
            SizeType expected_cells = 0u;
            std::string expected_volume;
            bool reference_recorded = false;

            auto prepare = [&] {
                paving = std::make_unique<BinaryTreePaving>(grid);
            };
            auto operate = [&] {
                strategy.implementation.adjoin_outer_approximation(
                    *paving, set, f);
            };
            auto verify = [&] {
                // Canonicalization, correctness checks and size/volume
                // accounting are outside the Paver execution timer.
                paving->recombine();
                verify_result(*paving, d, shape, strategy.name);
                const SizeType cells = paving->size();
                const std::string volume = measured_volume(*paving);
                if (not reference_recorded) {
                    expected_cells = cells;
                    expected_volume = volume;
                    reference_recorded = true;
                } else if (cells != expected_cells or volume != expected_volume) {
                    throw std::runtime_error(
                        "Paver output differs between identical runs: " +
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
                                << " volume=" << expected_volume)
            } else {
                auto measured = runner.run(
                    strategy.name, n, prepare, operate, verify);
                const Bench::Statistics stats = Bench::summarize(measured);
                if (not std::isfinite(stats.median) or stats.median <= 0.0)
                    throw std::runtime_error("Invalid Paver timing");
                report.add_row({
                    shape, std::to_string(d), std::to_string(f),
                    strategy.name, std::to_string(expected_cells),
                    expected_volume
                }, stats);
            }
        }
        runner.configuration_completed(configuration);
    });
    if (not verify_only)
        report.print();
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
