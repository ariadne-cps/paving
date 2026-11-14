/***************************************************************************
 *            benchmarks/benchmark_spre22_fixture.cpp
 *
 *  Copyright  2026  Luca Geretti
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne Paving.
 *  Ariadne Paving is free software under the GPL version 3 or later.
 */

/** @file benchmarks/benchmark_spre22_fixture.cpp
 *  @brief Replay actual SPRE22 validated enclosure fixtures without ARCH.
 */

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "function/taylor_function.hpp"
#include "paving/binary_tree_paving.hpp"
#include "paving/paver.hpp"
#include "set/function_set.hpp"
#include "fixtures/spre22_enclosures.generated.hpp"

namespace {

using namespace Ariadne;

struct Options {
    unsigned fineness = 4u;
    unsigned samples = 5u;
    unsigned max_enclosures = 48u;
    unsigned switch_after = 16u;
    bool analyze_enclosures = false;
};

unsigned positive(const std::string& s, const char* flag) {
    std::size_t pos = 0u;
    const auto n = std::stoul(s, &pos);
    if (pos != s.size() or n == 0ul or n > 100000ul)
        throw std::invalid_argument(std::string("invalid ") + flag);
    return static_cast<unsigned>(n);
}

Options parse(int argc, const char* argv[]) {
    Options result;
    for (int i = 1; i < argc; ++i) {
        const std::string flag(argv[i]);
        if (flag == "--help") {
            std::cout << "--fineness 4 --samples 5 --max-enclosures 48 --hybrid-switch 16 [--analyze-enclosures]\n";
            result.samples = 0u;
            return result;
        }
        if (flag == "--analyze-enclosures") {
            result.analyze_enclosures = true;
            continue;
        }
        if (i + 1 >= argc)
            throw std::invalid_argument("missing value for " + flag);
        const std::string value(argv[++i]);
        if (flag == "--fineness") result.fineness = positive(value, "--fineness");
        else if (flag == "--samples") result.samples = positive(value, "--samples");
        else if (flag == "--max-enclosures")
            result.max_enclosures = positive(value, "--max-enclosures");
        else if (flag == "--hybrid-switch")
            result.switch_after = positive(value, "--hybrid-switch");
        else throw std::invalid_argument("unknown option " + flag);
    }
    return result;
}

bool contains(const BinaryTreePaving& outer, const BinaryTreePaving& inner) {
    for (auto it = inner.begin(), end = inner.end(); it != end; ++it)
        if (not outer.superset(*it)) return false;
    return true;
}

std::string relation(const BinaryTreePaving& a, const BinaryTreePaving& b) {
    const bool ab = contains(a, b), ba = contains(b, a);
    if (ab and ba) return "equal";
    if (ab) return "A strictly contains B";
    if (ba) return "B strictly contains A";
    return "incomparable";
}

double median(std::vector<double> v) {
    if (v.empty()) throw std::logic_error("no timings");
    std::sort(v.begin(), v.end());
    const auto n = v.size();
    return (n % 2u) ? v[n / 2u] : (v[n / 2u - 1u] + v[n / 2u]) / 2.0;
}

double mad(const std::vector<double>& values) {
    const double centre = median(values);
    std::vector<double> deviations;
    for (const auto v: values) deviations.push_back(std::abs(v - centre));
    return median(deviations);
}

struct Outcome {
    std::string name;
    double median_ms;
    std::unique_ptr<BinaryTreePaving> paving;
};


// The features are measured on the INPUT before paving and do not use
// enclosure indices or output geometry as predictive features.
struct EnclosureFeatures {
    SizeType state_terms = 0u;
    SizeType constraint_terms = 0u;
    SizeType nonlinear_terms = 0u;
    unsigned maximum_degree = 0u;
    double total_taylor_error = 0.0;
    double maximum_taylor_error = 0.0;
    double mean_parameter_width = 0.0;
};

void accumulate_model_features(EnclosureFeatures& result,
                               const ValidatedTaylorModelDP& model,
                               bool is_constraint) {
    const SizeType terms = model.number_of_terms();
    if (is_constraint) result.constraint_terms += terms;
    else result.state_terms += terms;
    result.maximum_degree = std::max(
        result.maximum_degree, static_cast<unsigned>(model.degree()));
    const double error = model.error().raw().get_d();
    result.total_taylor_error += error;
    result.maximum_taylor_error = std::max(result.maximum_taylor_error, error);
    for (auto it = model.begin(); it != model.end(); ++it)
        if (it->index().degree() > 1u)
            ++result.nonlinear_terms;
}

EnclosureFeatures describe_enclosure(const ValidatedConstrainedImageSet& set) {
    EnclosureFeatures features;
    const auto* mapping =
        dynamic_cast<const ValidatedVectorMultivariateTaylorFunctionModelDP*>(
            set.function().raw_pointer());
    if (mapping == nullptr)
        throw std::runtime_error("the captured state map is not a validated DP Taylor model");
    for (SizeType i = 0u; i < mapping->result_size(); ++i)
        accumulate_model_features(features, mapping->model(i), false);

    for (SizeType i = 0u; i < set.number_of_constraints(); ++i) {
        const auto constraint = set.constraint(i);
        const auto* scalar =
            dynamic_cast<const ValidatedScalarMultivariateTaylorFunctionModelDP*>(
                constraint.function().raw_pointer());
        if (scalar == nullptr)
            throw std::runtime_error("the captured constraint is not a validated DP Taylor model");
        accumulate_model_features(features, scalar->model(), true);
    }

    const auto& domain = set.domain();
    for (SizeType j = 0u; j < domain.dimension(); ++j)
        features.mean_parameter_width +=
            domain[j].upper_bound().get_d() - domain[j].lower_bound().get_d();
    if (domain.dimension() != 0u)
        features.mean_parameter_width /= domain.dimension();
    return features;
}

struct EnclosureMeasurement {
    std::vector<double> timings[2];
    std::unique_ptr<BinaryTreePaving> geometry[2];
};

// Compare the two Pavers separately on each real enclosure, then compare
// their marginal cost and prefix geometry during a complete accumulation.
// All snapshots and geometry comparisons are OUTSIDE timed sections.
int analyze_enclosures(const Options& options,
                       const std::vector<ValidatedConstrainedImageSet>& sets,
                       const Grid& grid) {
    const Paver pavers[2]{
        Paver(ConstraintPaver()),
        Paver(ReducePaver(ReducePaverReductionMode::HullOnly))
    };
    std::vector<EnclosureFeatures> features;
    features.reserve(options.max_enclosures);
    for (unsigned i = 0u; i < options.max_enclosures; ++i)
        features.push_back(describe_enclosure(sets[i]));

    std::cout << std::fixed << std::setprecision(6);
    for (unsigned mode = 0u; mode != 2u; ++mode) {
        const bool accumulating = mode == 1u;
        const std::string mode_name = accumulating ? "accumulating" : "isolated";
        std::vector<EnclosureMeasurement> records(options.max_enclosures);

        for (unsigned sample = 0u; sample < options.samples; ++sample) {
            BinaryTreePaving accumulated[2]{
                BinaryTreePaving(grid), BinaryTreePaving(grid)
            };
            for (unsigned i = 0u; i < options.max_enclosures; ++i) {
                BinaryTreePaving isolated[2]{
                    BinaryTreePaving(grid), BinaryTreePaving(grid)
                };
                // Reverse the order for successive samples and enclosures.
                for (unsigned position = 0u; position != 2u; ++position) {
                    const unsigned which = (position + sample + i) % 2u;
                    BinaryTreePaving& destination =
                        accumulating ? accumulated[which] : isolated[which];
                    const auto start = std::chrono::steady_clock::now();
                    pavers[which].adjoin_outer_approximation(
                        destination, sets[i], options.fineness);
                    const double millis = std::chrono::duration<double, std::milli>(
                        std::chrono::steady_clock::now() - start).count();
                    records[i].timings[which].push_back(millis);

                    BinaryTreePaving snapshot(destination);
                    snapshot.recombine();
                    auto& expected = records[i].geometry[which];
                    if (not expected)
                        expected = std::make_unique<BinaryTreePaving>(snapshot);
                    else if (not contains(*expected, snapshot) or
                             not contains(snapshot, *expected))
                        throw std::runtime_error(
                            "non-deterministic output: mode=" + mode_name +
                            " enclosure=" + std::to_string(i + 1u) +
                            " strategy=" + (which == 0u ? "constraint" : "reduce-hull"));
                }
            }
        }

        std::vector<double> ratios;
        unsigned equal = 0u, hull_tighter = 0u;
        unsigned constraint_tighter = 0u, incomparable = 0u;
        for (unsigned i = 0u; i < options.max_enclosures; ++i) {
            const auto& measurement = records[i];
            const auto& feature = features[i];
            const double constraint_ms = median(measurement.timings[0]);
            const double hull_ms = median(measurement.timings[1]);
            if (constraint_ms <= 0.0)
                throw std::runtime_error("nonpositive ConstraintPaver timing");
            const double ratio = hull_ms / constraint_ms;
            ratios.push_back(ratio);
            const std::string geometry =
                relation(*measurement.geometry[0], *measurement.geometry[1]);
            if (geometry == "equal") ++equal;
            else if (geometry == "A strictly contains B") ++hull_tighter;
            else if (geometry == "B strictly contains A") ++constraint_tighter;
            else ++incomparable;

            std::cout << "SPRE22_ENCLOSURE_ANALYSIS mode=" << mode_name
                      << " index=" << i + 1u
                      << " f=" << options.fineness
                      << " samples=" << options.samples
                      << " parameters=" << sets[i].number_of_parameters()
                      << " constraints=" << sets[i].number_of_constraints()
                      << " state_terms=" << feature.state_terms
                      << " constraint_terms=" << feature.constraint_terms
                      << " nonlinear_terms=" << feature.nonlinear_terms
                      << " maximum_degree=" << feature.maximum_degree
                      << " total_taylor_error=" << feature.total_taylor_error
                      << " max_taylor_error=" << feature.maximum_taylor_error
                      << " mean_parameter_width=" << feature.mean_parameter_width
                      << " constraint_ms=" << constraint_ms
                      << " constraint_mad_ms=" << mad(measurement.timings[0])
                      << " hull_ms=" << hull_ms
                      << " hull_mad_ms=" << mad(measurement.timings[1])
                      << " hull_over_constraint=" << ratio
                      << " geometry=" << geometry
                      << " constraint_cells=" << measurement.geometry[0]->size()
                      << " hull_cells=" << measurement.geometry[1]->size()
                      << " constraint_volume="
                      << measurement.geometry[0]->measure().raw().get_d()
                      << " hull_volume="
                      << measurement.geometry[1]->measure().raw().get_d()
                      << "\n";
        }

        const double ratio_median = median(ratios);
        const auto extrema = std::minmax_element(ratios.begin(), ratios.end());
        std::cout << "SPRE22_ENCLOSURE_ANALYSIS_SUMMARY mode=" << mode_name
                  << " selected=" << options.max_enclosures
                  << " f=" << options.fineness
                  << " ratio_median=" << ratio_median
                  << " ratio_mad=" << mad(ratios)
                  << " ratio_min=" << *extrema.first
                  << " ratio_max=" << *extrema.second
                  << " equal=" << equal
                  << " hull_tighter=" << hull_tighter
                  << " constraint_tighter=" << constraint_tighter
                  << " incomparable=" << incomparable
                  << "\n";
    }
    return 0;
}

int run(int argc, const char* argv[]) {
    const auto options = parse(argc, argv);
    if (options.samples == 0u) return 0;
    const auto sets = SPRE22Fixture::real_enclosures();
    if (sets.size() != 48u)
        throw std::runtime_error("fixture must contain all 48 real enclosures");
    if (options.max_enclosures > sets.size() or
        (not options.analyze_enclosures and options.switch_after >= options.max_enclosures))
        throw std::invalid_argument("invalid enclosure count or hybrid switch");

    const Grid grid(Vector<ApproximateDouble>{1.0, 200.0, 200.0, 20.0, 20.0});
    if (options.analyze_enclosures)
        return analyze_enclosures(options, sets, grid);
    const Paver constraint{ConstraintPaver()};
    const Paver hull{ReducePaver(ReducePaverReductionMode::HullOnly)};
    const std::string names[3]{"constraint", "reduce-hull", "hybrid-constraint-hull"};
    std::vector<Outcome> results;
    std::cout << std::fixed << std::setprecision(6);
    for (unsigned strategy = 0u; strategy != 3u; ++strategy) {
        std::vector<double> samples;
        std::unique_ptr<BinaryTreePaving> reference;
        for (unsigned sample = 0u; sample < options.samples; ++sample) {
            BinaryTreePaving paving(grid);
            const auto start = std::chrono::steady_clock::now();
            for (unsigned i = 0u; i < options.max_enclosures; ++i) {
                const Paver& selected =
                    strategy == 1u or (strategy == 2u and i >= options.switch_after)
                    ? hull : constraint;
                selected.adjoin_outer_approximation(paving, sets[i], options.fineness);
            }
            samples.push_back(std::chrono::duration<double, std::milli>(
                std::chrono::steady_clock::now() - start).count());
            paving.recombine(); // Untimed, as in ARCH.
            if (not reference) reference = std::make_unique<BinaryTreePaving>(paving);
            else if (not contains(*reference, paving) or not contains(paving, *reference))
                throw std::runtime_error("non-deterministic geometry for " + names[strategy]);
        }
        const double time = median(samples);
        std::cout << "SPRE22_FIXTURE_PAVER f=" << options.fineness
                  << " selected=" << options.max_enclosures
                  << " strategy=" << names[strategy]
                  << " samples=" << options.samples
                  << " median_ms=" << time << " mad_ms=" << mad(samples)
                  << " cells=" << reference->size()
                  << " outer_volume=" << reference->measure().raw().get_d() << "\n";
        results.push_back({names[strategy], time, std::move(reference)});
    }
    for (unsigned i = 0u; i != 3u; ++i)
        for (unsigned j = i + 1u; j != 3u; ++j)
            std::cout << "SPRE22_FIXTURE_GEOMETRY A=" << results[i].name
                      << " B=" << results[j].name
                      << " relation=" << relation(*results[i].paving, *results[j].paving)
                      << "\n";

    const auto geometry = relation(*results[2].paving, *results[1].paving);
    std::cout << "SPRE22_FIXTURE_SUMMARY selected=" << options.max_enclosures
              << " f=" << options.fineness
              << " hybrid_over_constraint=" << results[2].median_ms / results[0].median_ms
              << " hull_over_hybrid=" << results[1].median_ms / results[2].median_ms
              << " geometry_vs_hull=" << geometry << "\n";

    if (options.fineness == 4u and options.max_enclosures == 48u and
        options.switch_after == 16u) {
        // User-measured ARCH baseline. Count and volume are necessary,
        // not sufficient, evidence of exact cross-repository equality.
        bool match = results[0].paving->size() == 1624u &&
                     results[1].paving->size() == 1523u &&
                     results[2].paving->size() == 1523u &&
                     std::abs(results[0].paving->measure().raw().get_d() - 56030.273438) < 0.002 &&
                     std::abs(results[1].paving->measure().raw().get_d() - 47561.645508) < 0.002 &&
                     std::abs(results[2].paving->measure().raw().get_d() - 47561.645508) < 0.002;
        std::cout << "SPRE22_FIXTURE_REFERENCE counts_and_volumes_match=" << match
                  << " hybrid_equals_hull=" << (geometry == "equal") << "\n";
        if (not match or geometry != "equal") return 2;
    }
    return 0;
}

} // namespace

int main(int argc, const char* argv[]) {
    try { return run(argc, argv); }
    catch (const std::exception& e) {
        std::cerr << "SPRE22_FIXTURE_ERROR: " << e.what() << "\n";
        return 1;
    }
}
