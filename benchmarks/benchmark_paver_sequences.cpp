/***************************************************************************
 *            benchmarks/benchmark_paver_sequences.cpp
 *
 *  Copyright  2026  Luca Geretti
 *
 ****************************************************************************/

/*
 * This file is part of Ariadne Paving.
 *
 * Ariadne Paving is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ariadne Paving is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Ariadne Paving. If not, see <https://www.gnu.org/licenses/>.
 */

/** @file benchmarks/benchmark_paver_sequences.cpp
 *  @brief Compare Pavers on repeatable, SPRE22-inspired enclosure sequences.
 *
 * This is a controlled surrogate, NOT a replay of actual SPRE22 enclosures.
 * Construction, copy/union, and geometric checks are outside timed calls.
 */

#include <algorithm>
#include <chrono>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "algebra/vector.hpp"
#include "function/taylor_function.hpp"
#include "logging/logging.hpp"
#include "paving/binary_tree_paving.hpp"
#include "paving/paver.hpp"
#include "set/function_set.hpp"

#include "ariadne-benchmarking.hpp"

using namespace Ariadne;
namespace Bench = Ariadne::Benchmarking;

namespace {

struct Strategy {
    std::string name;
    Paver paver;
};

struct Outcome {
    std::string scenario;
    std::string mode;
    std::string strategy;
    unsigned fineness;
    unsigned steps;
    unsigned requested_samples;
    double median_ms;
    bool complete_samples;
    std::unique_ptr<BinaryTreePaving> paving;
};

bool contains(const BinaryTreePaving& outer, const BinaryTreePaving& inner) {
    for (auto it = inner.begin(), end = inner.end(); it != end; ++it)
        if (not outer.superset(*it))
            return false;
    return true;
}

std::string relation(const BinaryTreePaving& a, const BinaryTreePaving& b) {
    const bool ab = contains(a, b);
    const bool ba = contains(b, a);
    if (ab and ba)
        return "equal";
    if (ab)
        return "A strictly contains B";
    if (ba)
        return "B strictly contains A";
    return "incomparable";
}

std::vector<Strategy> parse_pavers(const String& selection) {
    const std::string input(selection);
    if (input == "all")
        return {{"subdivision", Paver(SubdivisionPaver())},
                {"constraint", Paver(ConstraintPaver())}};
    if (input.empty() or input.front() == ',' or input.back() == ',')
        throw std::invalid_argument("invalid paver selection");
    std::vector<Strategy> result;
    std::size_t start = 0u;
    while (true) {
        const std::size_t end = input.find(',', start);
        const std::string name = input.substr(
            start, end == std::string::npos ? end : end - start);
        if ((name != "subdivision" and name != "constraint") or
            std::any_of(result.begin(), result.end(),
                        [&name](const Strategy& s) { return s.name == name; }))
            throw std::invalid_argument("unknown or duplicate paver: " + name);
        result.push_back(name == "subdivision"
                             ? Strategy{"subdivision", Paver(SubdivisionPaver())}
                             : Strategy{"constraint", Paver(ConstraintPaver())});
        if (end == std::string::npos)
            break;
        start = end + 1u;
    }
    return result;
}

// SPRE22 uses t,x,y,vx,vy and state-grid lengths (1,200,200,20,20).
// These polynomial Taylor maps reproduce the dimension, scaling,
// nonlinear correlations, constraints, and substantial overlap, but are
// NOT obtained from the SPRE22 evolver. All constants are exact dyadics.
Grid scaled_grid() {
    return Grid(Vector<ApproximateDouble>{1.0, 200.0, 200.0, 20.0, 20.0});
}

std::vector<ValidatedConstrainedImageSet>
make_enclosures(const std::string& scenario, unsigned steps) {
    if (scenario != "overlap" and scenario != "sweep")
        throw std::invalid_argument("invalid scenario");
    if (steps == 0u or steps > 24u)
        throw std::invalid_argument("steps must be in 1..24");

    const auto u = EffectiveScalarMultivariateFunction::coordinates(5u);
    const ExactBoxType domain(5u, ExactIntervalType(-1, 1));
    std::vector<ValidatedConstrainedImageSet> sets;
    sets.reserve(steps);

    for (unsigned step = 0u; step < steps; ++step) {
        const int k = static_cast<int>(step);
        // One coarse-cell shift per step in the sweep scenario, but
        // substantially overlapping images in the overlap scenario.
        const int dx = (scenario == "overlap" ? 2 : 35) * k;
        const int dy = (scenario == "overlap" ? 1 : 25) * k;

        List<EffectiveScalarMultivariateFunction> components;
        // Use an exact symbolic constant: k * 0.125_x would be an
        // ApproximateDouble and cannot be added to an effective function.
        const auto time_offset =
            EffectiveScalarMultivariateFunction::constant(5u, Real(k) / Real(8));
        components.append(time_offset + u[0] * 0.0625_x);
        components.append(-900 + dx + 16 * u[0] + 8 * u[1] + 2 * u[0] * u[1]);
        components.append(-400 + dy + 12 * u[1] + 6 * u[2] + 2 * u[1] * u[2]);
        components.append(2 + u[2] * 0.5_x + u[3] * 0.25_x + u[0] * u[0] * 0.125_x);
        components.append(2 + u[3] * 0.5_x + u[4] * 0.25_x + u[1] * u[1] * 0.125_x);
        const EffectiveVectorMultivariateFunction map(components);
        EffectiveConstrainedImageSet effective(
            RealBox(5u, RealInterval(-1, 1)), map);
        // A central feasible point is known (all parameters zero).
        effective.new_parameter_constraint(u[0] + u[1] <= 0.5_x);
        effective.new_parameter_constraint(u[2] * u[2] + u[3] * u[3] <= 1.0_x);

        // Construct a validated Taylor-model map outside timed execution,
        // matching the function representation used by evolver enclosures.
        const ValidatedVectorMultivariateFunction validated_map(map);
        const ValidatedVectorMultivariateTaylorFunctionModelDP model(
            domain, validated_map, TrivialSweeper<FloatDP>(dp));
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(model),
                          effective.constraints());
    }
    return sets;
}

// Exactly the same sequence is presented to both Pavers. In "isolated",
// every enclosure starts with an empty paving; their union is constructed
// only in verify(). In "accumulating", the same paving is reused, allowing
// the Paver to exploit already-enabled cells within timed calls.
class SequenceRun {
    std::vector<std::unique_ptr<BinaryTreePaving>> _pavings;
    bool _accumulating = false;

  public:
    void prepare(const Grid& grid, std::size_t steps, bool accumulating) {
        _accumulating = accumulating;
        _pavings.clear();
        _pavings.reserve(accumulating ? 1u : steps);
        for (std::size_t i = 0u; i < (accumulating ? 1u : steps); ++i)
            _pavings.push_back(std::make_unique<BinaryTreePaving>(grid));
    }

    BinaryTreePaving& stage(std::size_t i) {
        return *_pavings[_accumulating ? 0u : i];
    }

    void execute(const std::vector<ValidatedConstrainedImageSet>& sets,
                 const Strategy& strategy, unsigned fineness) {
        for (std::size_t i = 0u; i < sets.size(); ++i)
            strategy.paver.adjoin_outer_approximation(stage(i), sets[i], fineness);
    }

    std::unique_ptr<BinaryTreePaving> merged(const Grid& grid) const {
        auto result = std::make_unique<BinaryTreePaving>(grid);
        for (const auto& paving: _pavings)
            result->adjoin(*paving);
        result->recombine();
        return result;
    }
};

void run(const Bench::CommandLine& cli, const Bench::Matrix& matrix,
         const Bench::Parameter<unsigned>& finenesses,
         const Bench::Parameter<std::string>& scenarios,
         const Bench::Parameter<std::string>& modes,
         const Bench::Parameter<unsigned>& steps,
         const Bench::Parameter<unsigned>& samples,
         const std::vector<Strategy>& strategies,
         bool diagnostics, bool verify_only, unsigned max_case_ms) {
    Bench::Runner runner(cli, matrix.combinations());
    Bench::Runner sample_runner(0u, matrix.combinations());
    auto fields = cli.summary_fields();
    if (fields == Bench::default_summary_fields())
        fields.insert(fields.begin(), Bench::SummaryField::Median);
    Bench::Report report("Paver enclosure sequences: SPRE22-inspired surrogate",
                         fields);
    report.set_time_unit(Bench::ReportTimeUnit::Milliseconds);
    report.set_dimension_columns({{"Scenario"}, {"Mode"}, {"Fineness"},
                                  {"Steps"}, {"Paver"}, {"Cells"},
                                  {"Outer volume"}, {"Samples"}, {"Budget"}});
    std::string header = matrix.header() + " | pavers=[";
    for (const auto& strategy: strategies) {
        if (&strategy != &strategies.front())
            header += ",";
        header += strategy.name;
    }
    report.set_parameter_header(header + "]");

    Bench::Table pairwise({{"Scenario"}, {"Mode"}, {"Fineness"}, {"Steps"},
                           {"A"}, {"B"}, {"Geometric relation"}});
    Bench::Table mode_geometry({{"Scenario"}, {"Fineness"}, {"Steps"}, {"Paver"},
                                {"A (isolated)"}, {"B (accumulating)"}, {"Geometric relation"}});
    Bench::Table per_step({{"Scenario"}, {"Mode"}, {"Fineness"}, {"Paver"},
                           {"Step"}, {"Cells before"}, {"Cells after"},
                           {"Visits"}, {"Covered"}, {"Param splits"},
                           {"Map evals"}, {"Sensitivity"}, {"Paver (ms)"}});
    // Different finenesses must be compared geometrically, not only by
    // outer volume or equal numeric fineness settings.
    Bench::Table across_fineness({{"Scenario"}, {"Mode"}, {"Steps"}, {"A"}, {"B"},
                                  {"Geometric relation"}});
    Bench::Table frontier({{"Scenario"}, {"Mode"}, {"Steps"}, {"Samples"},
                           {"Fineness"}, {"Paver"}, {"Median (ms)"}, {"Cells"},
                           {"Outer volume"}, {"Volume / tightest"}, {"Status"}});

    const Grid grid = scaled_grid();
    std::vector<Outcome> finished;

    matrix.for_each([&](const Bench::Configuration& c) {
        const unsigned f = c.value(finenesses);
        const auto& scenario = c.value(scenarios);
        const auto& mode = c.value(modes);
        const unsigned count = c.value(steps);
        const unsigned n = c.value(samples);
        if (n == 0u)
            throw std::invalid_argument("samples must be positive");
        const bool accumulating = mode == "accumulating";
        if (not accumulating and mode != "isolated")
            throw std::invalid_argument("invalid mode");

        // Domain/functions/constraints and input sequence are shared by
        // every strategy and constructed entirely outside the timer.
        const auto sets = make_enclosures(scenario, count);
        std::vector<Outcome> case_outcomes;
        runner.configuration_started(c);

        for (const auto& strategy: strategies) {
            SequenceRun sequence;
            std::unique_ptr<BinaryTreePaving> reference;
            double median_ms = 0.0;
            bool complete_samples = false;
            auto prepare = [&] {
                sequence.prepare(grid, sets.size(), accumulating);
            };
            auto execute = [&] {
                sequence.execute(sets, strategy, f);
            };
            auto verify = [&] {
                auto actual = sequence.merged(grid);
                if (actual->is_empty())
                    throw std::runtime_error("unexpected empty paving");
                if (reference == nullptr) {
                    reference = std::move(actual);
                } else if (not contains(*reference, *actual) or
                           not contains(*actual, *reference)) {
                    throw std::runtime_error(
                        "Paver geometry differs across identical samples: " +
                        scenario + "/" + mode + "/" + strategy.name);
                }
                Bench::keep_alive(reference->size());
            };

            if (verify_only) {
                prepare();
                execute();
                verify();
                LOGGING_PRINTLN("SEQUENCE_VERIFY scenario=" << scenario
                               << " mode=" << mode << " f=" << f
                               << " steps=" << count << " paver=" << strategy.name
                               << " cells=" << reference->size());
            } else {
                for (unsigned warm = 0u; warm < cli.warmups(); ++warm) {
                    prepare();
                    execute();
                    verify();
                }
                Bench::Samples measured;
                double elapsed_ms = 0.0;
                bool budget_hit = false;
                for (unsigned sample = 0u; sample < n; ++sample) {
                    const auto value = sample_runner.run(
                        strategy.name, 1u, prepare, execute, verify);
                    const double nanoseconds = value.values().front();
                    measured.add(nanoseconds);
                    elapsed_ms += nanoseconds / 1e6;
                    if (max_case_ms and elapsed_ms >= max_case_ms and
                        sample + 1u < n) {
                        budget_hit = true;
                        break;
                    }
                }
                const auto stats = Bench::summarize(measured);
                if (not std::isfinite(stats.median) or stats.median <= 0.0)
                    throw std::runtime_error("invalid sequence timing");
                // Runner samples are nanoseconds; report medians in ms.
                median_ms = stats.median / 1e6;
                complete_samples = not budget_hit and measured.size() == n;
                report.add_row({scenario, mode, std::to_string(f),
                                std::to_string(count), strategy.name,
                                std::to_string(reference->size()),
                                Bench::Table::number(reference->measure().raw().get_d(), 6u),
                                std::to_string(measured.size()) + "/" + std::to_string(n),
                                budget_hit ? "SOFT LIMIT" : "complete"},
                               stats);
            }

            if (diagnostics) {
                // Independent, untimed replay: instrument only Paver calls;
                // its geometry must equal the measured uninstrumented result.
                SequenceRun instrumented;
                instrumented.prepare(grid, sets.size(), accumulating);
                for (std::size_t i = 0u; i < sets.size(); ++i) {
                    auto& paving = instrumented.stage(i);
                    const auto before = paving.size();
                    PaverDiagnostics counts;
                    const auto begin = std::chrono::steady_clock::now();
                    if (strategy.name == "subdivision")
                        SubdivisionPaver().adjoin_outer_approximation(
                            paving, sets[i], f, counts);
                    else
                        ConstraintPaver().adjoin_outer_approximation(
                            paving, sets[i], f, counts);
                    const auto elapsed = std::chrono::duration_cast<
                        std::chrono::nanoseconds>(
                            std::chrono::steady_clock::now() - begin).count();
                    per_step.add_row({scenario, mode, std::to_string(f),
                                      strategy.name, std::to_string(i),
                                      std::to_string(before),
                                      std::to_string(paving.size()),
                                      std::to_string(counts.visited_nodes),
                                      std::to_string(counts.already_covered),
                                      std::to_string(counts.parameter_splits),
                                      std::to_string(counts.map_evaluations),
                                      std::to_string(counts.sensitivity_evaluations),
                                      Bench::Table::number(
                                          static_cast<double>(elapsed) / 1e6, 3u)});
                }
                const auto replay = instrumented.merged(grid);
                if (not contains(*reference, *replay) or
                    not contains(*replay, *reference))
                    throw std::runtime_error(
                        "Instrumented sequence changed geometric result: " +
                        scenario + "/" + mode + "/" + strategy.name);
            }
            case_outcomes.push_back({scenario, mode, strategy.name, f, count,
                                     n, median_ms, complete_samples,
                                     std::move(reference)});
        }

        for (std::size_t i = 0u; i < case_outcomes.size(); ++i)
            for (std::size_t j = i + 1u; j < case_outcomes.size(); ++j)
                pairwise.add_row({scenario, mode, std::to_string(f),
                                  std::to_string(count),
                                  case_outcomes[i].strategy,
                                  case_outcomes[j].strategy,
                                  relation(*case_outcomes[i].paving,
                                           *case_outcomes[j].paving)});

        for (auto& outcome: case_outcomes)
            finished.push_back(std::move(outcome));
        runner.configuration_completed(c);
    });

    for (std::size_t i = 0u; i < finished.size(); ++i)
        for (std::size_t j = i + 1u; j < finished.size(); ++j) {
            const auto& a = finished[i];
            const auto& b = finished[j];
            if (a.scenario == b.scenario and a.fineness == b.fineness and
                a.steps == b.steps and a.strategy == b.strategy and
                a.mode == "isolated" and b.mode == "accumulating")
                mode_geometry.add_row({a.scenario, std::to_string(a.fineness),
                                       std::to_string(a.steps), a.strategy,
                                       a.mode, b.mode,
                                       relation(*a.paving, *b.paving)});
        }

    // A descriptive time/tightness Pareto comparison. Rank only complete
    // sample sets from the same generated input sequence and occupancy mode.
    // A candidate is dominated only when another result is no slower
    // AND its entire paving is geometrically contained (not just smaller
    // in total volume). Median ordering alone is not a significance test.
    auto same_workload = [](const Outcome& a, const Outcome& b) {
        return a.scenario == b.scenario and a.mode == b.mode and
               a.steps == b.steps and a.requested_samples == b.requested_samples;
    };
    if (not verify_only) {
        for (const auto& candidate: finished) {
            double tightest_volume = candidate.paving->measure().raw().get_d();
            const Outcome* fastest_dominator = nullptr;
            for (const auto& other: finished) {
                if (not same_workload(candidate, other))
                    continue;
                tightest_volume = std::min(
                    tightest_volume, other.paving->measure().raw().get_d());
                if (&other == &candidate or not candidate.complete_samples or
                    not other.complete_samples or other.median_ms > candidate.median_ms)
                    continue;
                if (not contains(*candidate.paving, *other.paving))
                    continue;
                if (other.median_ms == candidate.median_ms and
                    contains(*other.paving, *candidate.paving))
                    continue;
                if (fastest_dominator == nullptr or
                    other.median_ms < fastest_dominator->median_ms)
                    fastest_dominator = &other;
            }
            const double volume = candidate.paving->measure().raw().get_d();
            const std::string status = not candidate.complete_samples
                ? "incomplete (not ranked)"
                : fastest_dominator == nullptr
                    ? "frontier"
                    : "dominated by " + fastest_dominator->strategy +
                      " f=" + std::to_string(fastest_dominator->fineness);
            frontier.add_row({candidate.scenario, candidate.mode,
                              std::to_string(candidate.steps),
                              std::to_string(candidate.requested_samples),
                              std::to_string(candidate.fineness),
                              candidate.strategy,
                              Bench::Table::number(candidate.median_ms, 3u),
                              std::to_string(candidate.paving->size()),
                              Bench::Table::number(volume, 6u),
                              tightest_volume > 0.0
                                  ? Bench::Table::number(volume / tightest_volume, 3u)
                                  : "n/a",
                              status});
        }
    }

    // Full exact cross-fineness comparisons (outside timed samples).
    for (std::size_t i = 0u; i < finished.size(); ++i)
        for (std::size_t j = i + 1u; j < finished.size(); ++j) {
            const auto& a = finished[i];
            const auto& b = finished[j];
            if (same_workload(a, b) and a.fineness != b.fineness)
                across_fineness.add_row({a.scenario, a.mode,
                                         std::to_string(a.steps),
                                         a.strategy + " f=" + std::to_string(a.fineness),
                                         b.strategy + " f=" + std::to_string(b.fineness),
                                         relation(*a.paving, *b.paving)});
        }

    if (not verify_only) {
        report.print();
        LOGGING_PRINTLN("Time/tightness frontier (complete samples, exact cell containment):")
        LOGGING_PRINTLN(frontier.str())
    }
    LOGGING_PRINTLN("Cross-fineness geometric comparisons (exact cell coverage):")
    LOGGING_PRINTLN(across_fineness.str())
    LOGGING_PRINTLN("Paver pairwise geometry (bidirectional cell coverage):")
    LOGGING_PRINTLN(pairwise.str())
    LOGGING_PRINTLN("Isolated versus accumulating geometry (same Paver):")
    LOGGING_PRINTLN(mode_geometry.str())
    if (diagnostics) {
        LOGGING_PRINTLN("Per-step diagnostic replay (not timed samples):")
        LOGGING_PRINTLN(per_step.str())
    }
}

} // namespace

int main(int argc, const char* argv[]) {
    try {
        Bench::Parameter<unsigned> finenesses(
            "finenesses", "Output grid fineness (unsigned)", {2u});
        Bench::Parameter<std::string> scenarios(
            "scenarios", "overlap (repeated regions) or sweep (new regions)",
            {"overlap", "sweep"},
            [](const std::string& name) {
                if (name != "overlap" and name != "sweep")
                    throw std::invalid_argument("unknown scenario");
                return name;
            });
        Bench::Parameter<std::string> modes(
            "modes", "isolated (fresh paving) or accumulating (one shared paving)",
            {"isolated", "accumulating"},
            [](const std::string& name) {
                if (name != "isolated" and name != "accumulating")
                    throw std::invalid_argument("unknown mode");
                return name;
            });
        Bench::Parameter<unsigned> steps(
            "steps", "Enclosures per sequence (1..24)", {6u});
        Bench::Parameter<unsigned> samples(
            "samples", "Measured complete sequences per Paver", {5u});
        Bench::Matrix matrix;
        matrix.add(finenesses).add(scenarios).add(modes).add(steps).add(samples);

        bool diagnostics = false;
        bool verify_only = false;
        unsigned max_case_ms = 5000u;
        auto selected = parse_pavers(String("all"));
        Bench::CommandLine cli;
        cli.add_parameters(matrix);
        List<ArgumentParser> options;
        options.append(make_valued_argument_parser(
            "", "pavers", "subdivision,constraint or all (default: all)",
            [&selected](const String& s) -> VoidFunction {
                auto parsed = parse_pavers(s);
                return [&selected, parsed = std::move(parsed)]() mutable {
                    selected = std::move(parsed);
                };
            }));
        options.append(make_unvalued_argument_parser(
            "", "diagnostics", "Per-stage instrumented replay (outside timed samples)",
            [&diagnostics] { diagnostics = true; }));
        options.append(make_unvalued_argument_parser(
            "", "verify-only", "Validate sequence geometry without timing",
            [&verify_only] { verify_only = true; }));
        options.append(make_valued_argument_parser(
            "", "max-case-ms", "Soft time budget per configuration/Paver (0 disables)",
            [&max_case_ms](const String& value) -> VoidFunction {
                const unsigned ms = Bench::parse_unsigned<unsigned>(value);
                return [&max_case_ms, ms] { max_case_ms = ms; };
            }));
        cli.add(CommandLineContribution("paver-sequence-benchmark", options));
        if (not cli.acquire(argc, argv))
            return 0;
        run(cli, matrix, finenesses, scenarios, modes, steps, samples,
            selected, diagnostics, verify_only, max_case_ms);
        return 0;
    } catch (const std::exception& exception) {
        LOGGING_PRINTLN("benchmark_paver_sequences: " << exception.what())
        return 1;
    }
}
