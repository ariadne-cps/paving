/***************************************************************************
 *            benchmarks/benchmark_parallel_outer_paving.cpp
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

/** @file benchmarks/benchmark_parallel_outer_paving.cpp
 *  @brief Benchmark paving operations and backend implementations.
 */

#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>

#include "logging/logging.hpp"
#include "paving/binary_tree_paving.hpp"
#include "paving/robdd_paving.hpp"
#include "set/function_set.hpp"
#include "threading/thread_manager.hpp"

#include "ariadne-benchmarking.hpp"

using namespace Ariadne;
namespace Bench = Ariadne::Benchmarking;

namespace {

struct RestoreConcurrency {
    ThreadManager& threading;
    SizeType previous;
    ~RestoreConcurrency() {
        try {
            threading.set_concurrency(previous);
        } catch (...) {
        }
    }
};

EffectiveConstrainedImageSet make_set(unsigned dimension) {
    auto x = EffectiveScalarMultivariateFunction::coordinates(dimension);
    RealBox domain(dimension, RealInterval(0, 1));
    EffectiveConstrainedImageSet set(
        domain, EffectiveVectorMultivariateFunction::identity(dimension));
    auto quadratic = (x[0] - 0.5_x) * (x[0] - 0.5_x) + (x[1] - 0.5_x) * (x[1] - 0.5_x);
    for (unsigned k = 2u; k < dimension; ++k)
        quadratic = quadratic + (x[k] - 0.5_x) * (x[k] - 0.5_x);
    set.new_parameter_constraint(quadratic <= 0.125_x);
    return set;
}

// Compare geometric coverage without assuming the two ROBDDs share a manager.
// Checking every enabled leaf in both directions is exact even if their
// canonical subdivisions differ. Validation runs outside measured regions.
Bool same_geometry(const RobddPaving& first, const RobddPaving& second) {
    auto contained = [](const RobddPaving& candidate,
                        const RobddPaving& container) {
        Bool covered = true;
        candidate.for_each_enabled_leaf([&](const GridCell& cell) {
            if (covered and not container.superset(cell))
                covered = false;
        });
        return covered;
    };
    return contained(first, second) and contained(second, first);
}

void run(const Bench::CommandLine& cli, const Bench::Matrix& matrix,
         const Bench::Parameter<unsigned>& dims,
         const Bench::Parameter<unsigned>& fineness,
         const Bench::Parameter<unsigned>& concurrency,
         const Bench::Parameter<unsigned>& samples) {
    ThreadManager& threading = ThreadManager::instance();
    RestoreConcurrency restore{threading, threading.concurrency()};
    Bench::Runner runner(cli, matrix.combinations());
    Bench::Report report("ROBDD outer approximation: serial and parallel",
                         cli.summary_fields());
    report.set_time_unit(Bench::ReportTimeUnit::Milliseconds);
    report.set_dimension_columns({{"Dimension"}, {"Fineness"}, {"Concurrency"}, {"Mode"}, {"Leaves"}});
    matrix.for_each([&](const Bench::Configuration& conf) {
        const unsigned workers = conf.value(concurrency);
        if (workers > threading.maximum_concurrency()) {
            LOGGING_PRINTLN("Skipping concurrency=" << workers
                                                    << " (maximum=" << threading.maximum_concurrency() << ")")
            return;
        }
        threading.set_concurrency(workers);
        const unsigned d = conf.value(dims), f = conf.value(fineness);
        auto set = make_set(d);
        const EffectiveEuclideanCompactSetInterface& source = set;
        Grid grid(d);
        GridCell root(grid, 0u, BinaryWord{});

        // Reference, geometry and solver setup are not timed.
        RobddManager oracle_manager;
        RobddPaving oracle(oracle_manager, root,
                           oracle_manager.constant(false), 0u);
        oracle.adjoin_outer_approximation(source, f);
        runner.configuration_started(conf);
        for (bool parallel: {false, true}) {
            std::unique_ptr<RobddManager> manager;
            std::unique_ptr<RobddPaving> paving;
            auto measurements = runner.run(parallel ? "parallel" : "serial", conf.value(samples), [&] {
                    paving.reset();
                    manager=std::make_unique<RobddManager>();
                    paving=std::make_unique<RobddPaving>(
                        *manager,root,manager->constant(false),0u); }, [&] {
                    if(parallel)
                        paving->adjoin_outer_approximation_parallel(source,f);
                    else
                        paving->adjoin_outer_approximation(source,f); }, [&] {
                    if(not same_geometry(*paving,oracle))
                        throw std::runtime_error(
                            "Parallel and serial outer geometries differ");
                    Bench::keep_alive(paving->size()); });
            report.add_row({std::to_string(d), std::to_string(f),
                            std::to_string(workers), parallel ? "parallel" : "serial",
                            std::to_string(oracle.size())},
                           Bench::summarize(measurements));
            // The reference manager and measured manager are independent.
        }
        runner.configuration_completed(conf);
    });
    report.print();
}
} // namespace

int main(int argc, const char* argv[]) {
    try {
        Bench::CommandLine cli;
        Bench::Parameter<unsigned> dimension(
            "dimension", "Grid dimensions (at least two)", {2u, 3u});
        Bench::Parameter<unsigned> fineness(
            "fineness", "Grid relative fineness", {1u, 2u});
        Bench::Parameter<unsigned> concurrency(
            "concurrency", "ThreadManager worker count", {0u, 2u, 4u, 8u});
        Bench::Parameter<unsigned> samples(
            "samples", "Measured whole-analysis samples", {5u});
        Bench::Matrix matrix;
        matrix.add(dimension).add(fineness).add(concurrency).add(samples);
        cli.add_parameters(matrix);
        if (not cli.acquire(argc, argv))
            return 0;
        run(cli, matrix, dimension, fineness, concurrency, samples);
        return 0;
    } catch (const std::exception& e) {
        LOGGING_PRINTLN("benchmark_parallel_outer_paving: " << e.what())
        return 1;
    }
}
