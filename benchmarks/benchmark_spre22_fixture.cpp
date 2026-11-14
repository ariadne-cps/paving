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
    bool certified_skip = false;
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
            std::cout << "--fineness 4 --samples 5 --max-enclosures 48 --hybrid-switch 16 [--certified-skip]\n";
            result.samples = 0u;
            return result;
        }
        if (flag == "--certified-skip") {
            result.certified_skip = true;
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

int run(int argc, const char* argv[]) {
    const auto options = parse(argc, argv);
    if (options.samples == 0u) return 0;
    const auto sets = SPRE22Fixture::real_enclosures();
    if (sets.size() != 48u)
        throw std::runtime_error("fixture must contain all 48 real enclosures");
    if (options.max_enclosures > sets.size() or
        options.switch_after >= options.max_enclosures)
        throw std::invalid_argument("invalid enclosure count or hybrid switch");

    const Grid grid(Vector<ApproximateDouble>{1.0, 200.0, 200.0, 20.0, 20.0});
    const Paver constraint{ConstraintPaver()};
    const Paver hull{ReducePaver(ReducePaverReductionMode::HullOnly)};
    const Paver certified{ReducePaver(ReducePaverReductionMode::HullOnlyCertifiedSkip)};
    const std::string names[4]{"constraint", "reduce-hull", "hybrid-constraint-hull", "reduce-hull-certified-skip"};
    std::vector<Outcome> results;
    std::cout << std::fixed << std::setprecision(6);
    for (unsigned strategy = 0u; strategy != (options.certified_skip ? 4u : 3u); ++strategy) {
        std::vector<double> samples;
        std::unique_ptr<BinaryTreePaving> reference;
        for (unsigned sample = 0u; sample < options.samples; ++sample) {
            BinaryTreePaving paving(grid);
            const auto start = std::chrono::steady_clock::now();
            for (unsigned i = 0u; i < options.max_enclosures; ++i) {
                const Paver& selected =
                    strategy == 3u ? certified :
                    (strategy == 1u or (strategy == 2u and i >= options.switch_after))
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
    for (unsigned i = 0u; i != results.size(); ++i)
        for (unsigned j = i + 1u; j != results.size(); ++j)
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

    if (options.certified_skip) {
        const std::string certified_geometry =
            relation(*results[3].paving, *results[1].paving);
        std::cout << "SPRE22_FIXTURE_CERTIFIED f=" << options.fineness
                  << " selected=" << options.max_enclosures
                  << " hull_over_certified=" << results[1].median_ms / results[3].median_ms
                  << " geometry_vs_hull=" << certified_geometry << "\n";
        if (certified_geometry != "equal")
            return 3;

        // Separate untimed diagnostic pass: instrumented runs have a different
        // cost profile and must not contaminate the timing medians.
        BinaryTreePaving instrumented(grid);
        PaverDiagnostics diagnostics;
        const ReducePaver candidate(ReducePaverReductionMode::HullOnlyCertifiedSkip);
        for (unsigned i = 0u; i < options.max_enclosures; ++i)
            candidate.adjoin_outer_approximation(
                instrumented, sets[i], options.fineness, diagnostics);
        instrumented.recombine();
        if (not contains(instrumented, *results[3].paving) or
            not contains(*results[3].paving, instrumented))
            throw std::runtime_error("instrumented certified replay changed geometry");
        std::cout << "SPRE22_FIXTURE_CERTIFIED_DIAGNOSTICS f=" << options.fineness
                  << " selected=" << options.max_enclosures
                  << " skipped_nodes=" << diagnostics.certified_hull_skips
                  << " hull_reduction_calls=" << diagnostics.hull_reduction_calls
                  << " visited_nodes=" << diagnostics.visited_nodes << "\n";
    }
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
