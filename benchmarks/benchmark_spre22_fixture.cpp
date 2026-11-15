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
#include <cstdint>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

#include "constraint-solving/constraint_solver.hpp"
#include "function/box_function.hpp"
#include "function/procedure.hpp"
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
    std::vector<unsigned> inspected_indices;
    unsigned inspect_limit = 12u;
    unsigned inspect_depth = 6u;
    bool audit_all_cells = false;
    bool audit_verbose = false;
    bool preflight = false;
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
            std::cout << "--fineness 4 --samples 5 --max-enclosures 48 --hybrid-switch 16 "
                      << "[--analyze-enclosures] [--inspect-cells 1,17] "
                      << "[--inspect-limit 12] [--inspect-depth 6] "
                      << "[--audit-all-cells] [--audit-verbose] "
                      << "[--preflight]\n";
            result.samples = 0u;
            return result;
        }
        if (flag == "--analyze-enclosures") {
            result.analyze_enclosures = true;
            continue;
        }
        if (flag == "--audit-all-cells") {
            result.audit_all_cells = true;
            continue;
        }
        if (flag == "--audit-verbose") {
            result.audit_verbose = true;
            continue;
        }
        if (flag == "--preflight") {
            result.preflight = true;
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
        else if (flag == "--inspect-cells") {
            if (not result.inspected_indices.empty())
                throw std::invalid_argument("--inspect-cells may occur only once");
            std::istringstream indices(value);
            std::string token;
            while (std::getline(indices, token, ','))
                result.inspected_indices.push_back(positive(token, "--inspect-cells"));
            if (result.inspected_indices.empty() or value.back() == ',')
                throw std::invalid_argument("empty --inspect-cells list");
        }
        else if (flag == "--inspect-limit")
            result.inspect_limit = positive(value, "--inspect-limit");
        else if (flag == "--inspect-depth")
            result.inspect_depth = positive(value, "--inspect-depth");
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


// Diagnostic of genuine output cells, not a surrogate or a fitted classifier.
// The exact difference is enumerated at a common grid fineness so internal
// tree recombination does not affect which cell boxes are compared.
struct CellHullProbe {
    SizeType calls = 0u;
    bool proved_empty = false;
    std::string first_empty_target = "none";
    SizeType narrowed_parameters = 0u;
};

// One sweep of the same compiled hull contractors used by ReducePaver.
// A non-empty result is INCONCLUSIVE: the real Paver may repeat/split later.
CellHullProbe probe_hull_once(
    const ValidatedConstrainedImageSet& set, const GridCell& cell,
    const List<ValidatedProcedure>& procedures) {
    const auto f = set.function();
    const auto g = set.constraint_function();
    const auto& bounds = set.constraint_bounds();
    const auto& original_domain = set.reduced_domain();
    UpperBoxType reduced(original_domain);
    ConstraintSolver solver;
    CellHullProbe result;
    for (SizeType j = 0u; j < f.result_size(); ++j) {
        ++result.calls;
        solver.hull_reduce(reduced, procedures[j], cell.box()[j]);
        if (definitely(reduced.is_empty())) {
            result.proved_empty = true;
            result.first_empty_target = "state_" + std::to_string(j);
            return result;
        }
    }
    for (SizeType j = 0u; j < g.result_size(); ++j) {
        ++result.calls;
        solver.hull_reduce(reduced, procedures[f.result_size()+j], bounds[j]);
        if (definitely(reduced.is_empty())) {
            result.proved_empty = true;
            result.first_empty_target = "constraint_" + std::to_string(j);
            return result;
        }
    }
    const UpperBoxType initial(original_domain);
    for (SizeType j = 0u; j < reduced.size(); ++j) {
        if (reduced[j].lower_bound().raw() > initial[j].lower_bound().raw() or
            reduced[j].upper_bound().raw() < initial[j].upper_bound().raw())
            ++result.narrowed_parameters;
    }
    return result;
}

// Independent validated set-disjointness test on the SAME cell and model.
// Failure to prove means unknown, never "the set intersects the cell".
// Skip zero-width parameters and cycle among remaining parameters.
bool certify_disjoint_by_subdivision(
    const ValidatedVectorMultivariateFunction& combined,
    const ExactBoxType& target, const ExactBoxType& domain,
    unsigned remaining_depth, SizeType next_axis, SizeType& evaluations) {
    ++evaluations;
    if (definitely(disjoint(apply(combined, domain), target)))
        return true;
    if (remaining_depth == 0u or domain.size() == 0u)
        return false;
    SizeType axis = domain.size();
    for (SizeType offset = 0u; offset < domain.size(); ++offset) {
        const SizeType candidate = (next_axis + offset) % domain.size();
        if (domain[candidate].upper_bound().get_d() >
            domain[candidate].lower_bound().get_d()) {
            axis = candidate;
            break;
        }
    }
    if (axis == domain.size())
        return false;
    const auto halves = domain.split(axis);
    return certify_disjoint_by_subdivision(
               combined, target, halves.first, remaining_depth,
               axis + 1u, evaluations) &&
           certify_disjoint_by_subdivision(
               combined, target, halves.second, remaining_depth,
               axis + 1u, evaluations);
}

// Recompute only two selected isolated pavings: no timing and no changes
// to the Paver implementation. Also inspect the explicit boundary cell
// previously proposed from static Taylor-model analysis.
int inspect_cell_differences(
    const Options& options, const std::vector<ValidatedConstrainedImageSet>& sets,
    const Grid& grid) {
    const Paver constraint{ConstraintPaver()};
    const Paver hull{ReducePaver(ReducePaverReductionMode::HullOnly)};
    const ExactBoxType previously_proposed{
        {0.0_x, 0.0625_x}, {-937.5_x, -925.0_x},
        {-425.0_x, -412.5_x}, {1.25_x, 2.5_x},
        {1.25_x, 2.5_x}
    };
    std::cout << std::fixed << std::setprecision(6);
    SizeType all_common = 0u;
    SizeType all_exclusive = 0u;
    SizeType all_root_certified = 0u;
    SizeType all_refined_certified = 0u;
    SizeType all_unresolved = 0u;
    SizeType all_first_sweep = 0u;
    SizeType all_range_evaluations = 0u;
    SizeType equal_enclosures = 0u;
    SizeType tighter_enclosures = 0u;
    SizeType inspected_enclosures = 0u;
    for (const unsigned index: options.inspected_indices) {
        if (index > sets.size() or index > options.max_enclosures)
            throw std::invalid_argument("inspect index is outside selected enclosures");
        const auto& set = sets[index-1u];

        BinaryTreePaving constraint_paving(grid), hull_paving(grid);
        constraint.adjoin_outer_approximation(constraint_paving, set, options.fineness);
        hull.adjoin_outer_approximation(hull_paving, set, options.fineness);
        constraint_paving.recombine();
        hull_paving.recombine();
        if (not contains(constraint_paving, hull_paving))
            throw std::runtime_error("HullOnly not geometrically contained in Constraint");
        const bool same_geometry = contains(hull_paving, constraint_paving);
        ++inspected_enclosures;
        if (same_geometry) ++equal_enclosures;
        else ++tighter_enclosures;

        BinaryTreePaving difference_paving =
            difference(constraint_paving, hull_paving);
        difference_paving.recombine();
        std::vector<GridCell> extra;
        difference_paving.for_each_enabled_cell_at_exact_fineness(
            options.fineness, [&](const GridCell& cell) { extra.push_back(cell); });

        // Exhaustively partition the finite grid-cell universe at the
        // requested fineness. This is a reference label, NOT a cheap
        // online decision criterion for choosing a Paver.
        SizeType constraint_exact = 0u, shared_exact = 0u;
        constraint_paving.for_each_enabled_cell_at_exact_fineness(
            options.fineness, [&](const GridCell&) { ++constraint_exact; });
        hull_paving.for_each_enabled_cell_at_exact_fineness(
            options.fineness, [&](const GridCell&) { ++shared_exact; });
        if (constraint_exact != shared_exact + extra.size() or
            same_geometry != extra.empty()) {
            throw std::runtime_error(
                "exact-fineness partition differs from geometric containment at enclosure " +
                std::to_string(index));
        }
        all_common += shared_exact;
        all_exclusive += extra.size();
        if (options.audit_all_cells) {
            const auto features = describe_enclosure(set);
            std::cout << "SPRE22_CELL_AUDIT_LABEL index=" << index
                      << " f=" << options.fineness
                      << " label=" << (same_geometry ? "equal" : "hull_tighter")
                      << " constraint_exact_cells=" << constraint_exact
                      << " shared_exact_cells=" << shared_exact
                      << " exclusive_exact_cells=" << extra.size()
                      << " hull_only_exact_cells=0"
                      << " state_terms=" << features.state_terms
                      << " nonlinear_terms=" << features.nonlinear_terms
                      << " max_degree=" << features.maximum_degree
                      << " total_taylor_error=" << features.total_taylor_error
                      << " mean_parameter_width=" << features.mean_parameter_width
                      << "\n";
        }

        std::cout << "SPRE22_CELL_DIFFERENCE index=" << index
                  << " f=" << options.fineness
                  << " relation=" << relation(constraint_paving, hull_paving)
                  << " constraint_cells=" << constraint_paving.size()
                  << " hull_cells=" << hull_paving.size()
                  << " difference_tree_cells=" << difference_paving.size()
                  << " difference_exact_cells=" << extra.size()
                  << " difference_volume="
                  << difference_paving.measure().raw().get_d() << "\n";

        // A deliberately explicit falsification check of our prior
        // candidate: overlapping an OUTPUT difference is a stricter
        // observation than proving only that the true image is disjoint.
        if (index == 1u or index == 17u) {
            std::cout << "SPRE22_CELL_PREVIOUS_CANDIDATE index=" << index
                      << " cell=" << previously_proposed
                      << " constraint_intersects=" << constraint_paving.intersects(previously_proposed)
                      << " hull_intersects=" << hull_paving.intersects(previously_proposed)
                      << " difference_intersects=" << difference_paving.intersects(previously_proposed)
                      << " constraint_covers=" << constraint_paving.superset(previously_proposed)
                      << " hull_covers=" << hull_paving.superset(previously_proposed)
                      << "\n";
        }

        if (extra.empty()) {
            if (options.audit_all_cells)
                std::cout << "SPRE22_CELL_AUDIT_CERTIFICATES index=" << index
                          << " inspected=0 root_disjoint=0 refined_disjoint=0"
                          << " unresolved=0 first_sweep_hull_rejections=0\n";
            continue;
        }
        const auto f = set.function();
        const auto g = set.constraint_function();
        const auto combined = join(f, g);
        const auto domain = set.reduced_domain();
        const auto bounds = set.constraint_bounds();

        List<ValidatedProcedure> procedures;
        procedures.reserve(f.result_size() + g.result_size());
        for (SizeType j = 0u; j < f.result_size(); ++j)
            procedures.append(make_procedure(f[j]));
        for (SizeType j = 0u; j < g.result_size(); ++j)
            procedures.append(make_procedure(g[j]));

        const SizeType shown = options.audit_all_cells
            ? extra.size()
            : std::min(extra.size(), static_cast<SizeType>(options.inspect_limit));
        SizeType root_certified = 0u, refined_certified = 0u;
        SizeType unresolved = 0u, immediate_hull_rejections = 0u;
        for (SizeType k = 0u; k < shown; ++k) {
            const SizeType sample_position =
                shown == 1u ? 0u : k * (extra.size()-1u) / (shown-1u);
            const GridCell& cell = extra[sample_position];
            const auto target = product(cell.box(), bounds);
            SizeType evaluations = 0u;
            // The combined range is validated; true proves the cell is
            // disjoint from the original constrained image.
            ++evaluations;
            const bool root_separated =
                definitely(disjoint(apply(combined, domain), target));
            bool separated = root_separated;
            if (not separated) {
                // Full depth includes the initial check above; the recursive
                // helper includes its own root range check as its first call.
                evaluations = 0u;
                separated = certify_disjoint_by_subdivision(
                    combined, target, domain, options.inspect_depth, 0u, evaluations);
            }
            if (root_separated) ++root_certified;
            else if (separated) ++refined_certified;
            else ++unresolved;
            all_range_evaluations += evaluations;

            const auto hull_probe = probe_hull_once(set, cell, procedures);
            if (hull_probe.proved_empty) ++immediate_hull_rejections;

            if (not options.audit_all_cells or options.audit_verbose)
                std::cout << "SPRE22_CELL_WITNESS index=" << index
                      << " sample=" << k+1u
                      << " exact_cell_index=" << sample_position+1u
                      << " cell_extent=" << cell.root_extent()
                      << " cell_word=" << cell.word()
                      << " box=" << cell.box()
                      << " root_interval_disjoint=" << root_separated
                      << " split_validated_disjoint=" << separated
                      << " interval_evaluations=" << evaluations
                      << " hull_one_sweep_empty=" << hull_probe.proved_empty
                      << " hull_first_empty=" << hull_probe.first_empty_target
                      << " hull_calls=" << hull_probe.calls
                      << " narrowed_parameters=" << hull_probe.narrowed_parameters
                      << "\n";
        }
        all_root_certified += root_certified;
        all_refined_certified += refined_certified;
        all_unresolved += unresolved;
        all_first_sweep += immediate_hull_rejections;
        if (options.audit_all_cells)
            std::cout << "SPRE22_CELL_AUDIT_CERTIFICATES index=" << index
                      << " inspected=" << shown
                      << " root_disjoint=" << root_certified
                      << " refined_disjoint=" << refined_certified
                      << " unresolved=" << unresolved
                      << " first_sweep_hull_rejections=" << immediate_hull_rejections
                      << "\n";
        std::cout << "SPRE22_CELL_DIFFERENCE_SUMMARY index=" << index
                  << " f=" << options.fineness
                  << " difference_exact_cells=" << extra.size()
                  << " inspected=" << shown
                  << " root_disjoint=" << root_certified
                  << " refined_disjoint=" << refined_certified
                  << " unresolved=" << unresolved
                  << " first_sweep_hull_rejections=" << immediate_hull_rejections
                  << " max_split_depth=" << options.inspect_depth
                  << "\n";
    }
    if (options.audit_all_cells) {
        if (equal_enclosures + tighter_enclosures != inspected_enclosures or
            all_root_certified + all_refined_certified + all_unresolved !=
                all_exclusive)
            throw std::runtime_error("exhaustive audit count invariant failed");
        std::cout << "SPRE22_CELL_AUDIT_SUMMARY"
                  << " selected=" << inspected_enclosures
                  << " f=" << options.fineness
                  << " equal=" << equal_enclosures
                  << " hull_tighter=" << tighter_enclosures
                  << " shared_exact_cells=" << all_common
                  << " exclusive_exact_cells=" << all_exclusive
                  << " certified_root=" << all_root_certified
                  << " certified_split=" << all_refined_certified
                  << " unresolved=" << all_unresolved
                  << " first_sweep_hull_rejections=" << all_first_sweep
                  << " total_interval_evaluations=" << all_range_evaluations
                  << " max_split_depth=" << options.inspect_depth
                  << "\n";
    }
    return 0;
}



// Input-only, cell-aware witness: the ordinary interval image overlaps a
// real grid boundary, while a tight validated Bernstein bound excludes it.
// This does NOT prove the two Pavers return different geometry.
struct BoundaryPreflight {
    enum class Decision { Witness, NoWitness, Unknown };
    Decision decision = Decision::NoWitness;
    unsigned tested_faces = 0u;
    SizeType witness_axis = 0u;
    double witness_face = 0.0;
    double witness_margin = 0.0;
};

std::uint64_t preflight_binomial(unsigned n, unsigned k) {
    if (k > n) return 0u;
    k = std::min(k, n-k);
    std::uint64_t v = 1u;
    for (unsigned i = 1u; i <= k; ++i)
        v = (v * (n-k+i)) / i;
    return v;
}

// Validated Bernstein coefficient of (2u-1)^exponent, in degree d,
// at basis position b. Every rational division uses directed bounds.
FloatDPBounds preflight_weight(unsigned exponent, unsigned d, unsigned b) {
    FloatDPBounds result(0u, dp);
    for (unsigned k = 0u; k <= std::min(exponent,b); ++k) {
        const std::uint64_t n = preflight_binomial(exponent,k)
            * (std::uint64_t{1} << k) * preflight_binomial(b,k);
        const std::uint64_t denominator = preflight_binomial(d,k);
        const FloatDPBounds term =
            FloatDPBounds(n, dp) / FloatDPBounds(denominator, dp);
        if ((exponent-k) % 2u) result -= term;
        else result += term;
    }
    return result;
}

// On normalized [-1,1]^n, the tensor Bernstein polynomial has range
// contained in the convex hull of its coefficients. Include every DP
// rounding error with Bounds and subtract the original uniform error.
// The 4096-basis computational limit is NOT an empirical class threshold:
// if exceeded, the preflight returns Unknown and uses HullOnly.
std::optional<FloatDP> preflight_bernstein_lower(
    const ValidatedTaylorModelDP& model, SizeType max_basis = 4096u) {
    const SizeType dimensions = model.argument_size();
    std::vector<unsigned> degrees(dimensions, 0u);
    for (auto it = model.begin(); it != model.end(); ++it)
        for (SizeType j = 0u; j < dimensions; ++j)
            degrees[j] = std::max(degrees[j],
                static_cast<unsigned>(it->index()[j]));

    SizeType count = 1u;
    for (const unsigned d : degrees) {
        if (d > 8u or count > max_basis / (d+1u))
            return std::nullopt;
        count *= d+1u;
    }
    std::vector<std::vector<FloatDPBounds>> weights;
    weights.reserve(dimensions);
    for (const unsigned d : degrees) {
        std::vector<FloatDPBounds> table;
        table.reserve(static_cast<std::size_t>(d+1u)*(d+1u));
        for (unsigned p = 0u; p <= d; ++p)
            for (unsigned b = 0u; b <= d; ++b)
                table.emplace_back(preflight_weight(p,d,b));
        weights.push_back(std::move(table));
    }

    std::vector<unsigned> indices(dimensions, 0u);
    FloatDP minimum(0u, dp);
    bool initialized = false;
    for (SizeType position = 0u; position < count; ++position) {
        SizeType remaining = position;
        for (SizeType j = 0u; j < dimensions; ++j) {
            indices[j] = static_cast<unsigned>(remaining % (degrees[j]+1u));
            remaining /= degrees[j]+1u;
        }
        FloatDPBounds coefficient(0u, dp);
        for (auto it = model.begin(); it != model.end(); ++it) {
            FloatDPBounds term(it->coefficient());
            for (SizeType j = 0u; j < dimensions; ++j) {
                const unsigned p = static_cast<unsigned>(it->index()[j]);
                term *= weights[j][p*(degrees[j]+1u)+indices[j]];
            }
            coefficient += term;
        }
        if (not initialized or coefficient.lower_raw() < minimum) {
            minimum = coefficient.lower_raw();
            initialized = true;
        }
    }
    if (not initialized) return std::nullopt;
    return sub(down, minimum, model.error().raw());
}

BoundaryPreflight inspect_boundary_witness(
    const ValidatedConstrainedImageSet& set, const Grid& grid, unsigned fineness) {
    BoundaryPreflight result;
    const auto* patch =
        dynamic_cast<const ValidatedVectorMultivariateTaylorFunctionModelDP*>(
            set.function().raw_pointer());
    if (patch == nullptr or fineness > 24u) {
        result.decision = BoundaryPreflight::Decision::Unknown;
        return result;
    }
    const auto range = apply(set.function(), set.reduced_domain());
    if (range.dimension() != grid.dimension() or
        patch->result_size() != range.dimension()) {
        result.decision = BoundaryPreflight::Decision::Unknown;
        return result;
    }
    for (SizeType j = 0u; j < range.dimension(); ++j) {
        const double step = std::ldexp(grid.lengths()[j].get_d(),
                                       -static_cast<int>(fineness));
        const double origin = grid.origin()[j].get_d();
        const double lower = range[j].lower_bound().raw().get_d();
        const double upper = range[j].upper_bound().raw().get_d();
        if (not (std::isfinite(step) and step > 0.0 and
                 std::isfinite(lower) and std::isfinite(upper))) {
            result.decision = BoundaryPreflight::Decision::Unknown;
            return result;
        }
        const double near_index = std::ceil((lower-origin)/step);
        if (not std::isfinite(near_index) or
            std::abs(near_index) > 1000000000.0) {
            result.decision = BoundaryPreflight::Decision::Unknown;
            return result;
        }
        const FloatDP face = grid.subdivision_coordinate(
            j, Dyadic(Integer(static_cast<std::int64_t>(near_index)),
                      static_cast<std::size_t>(fineness)));
        if (not (face.get_d() > lower and face.get_d() < upper))
            continue;
        ++result.tested_faces;
        const auto certified = preflight_bernstein_lower(patch->model(j));
        if (not certified) {
            result.decision = BoundaryPreflight::Decision::Unknown;
            return result;
        }
        if (*certified > face) {
            result.decision = BoundaryPreflight::Decision::Witness;
            result.witness_axis = j;
            result.witness_face = face.get_d();
            result.witness_margin = sub(down,*certified,face).get_d();
            return result;
        }
    }
    return result;
}

const char* preflight_name(BoundaryPreflight::Decision decision) {
    if (decision == BoundaryPreflight::Decision::Witness) return "witness";
    if (decision == BoundaryPreflight::Decision::NoWitness) return "no_witness";
    return "unknown";
}


int evaluate_preflight(const Options& options,
                       const std::vector<ValidatedConstrainedImageSet>& sets,
                       const Grid& grid) {
    const Paver constraint{ConstraintPaver()};
    const Paver hull{ReducePaver(ReducePaverReductionMode::HullOnly)};
    unsigned tp = 0u, fp = 0u, tn = 0u, fn = 0u, unknown = 0u;
    std::vector<double> classifier_costs;
    classifier_costs.reserve(options.max_enclosures);
    std::cout << std::fixed << std::setprecision(6);

    // Geometric labels are computed only AFTER the individual preflight
    // samples have finished and are never supplied to the preflight.
    for (unsigned i = 0u; i < options.max_enclosures; ++i) {
        std::vector<double> times;
        times.reserve(options.samples);
        BoundaryPreflight first_decision;
        for (unsigned sample = 0u; sample < options.samples; ++sample) {
            const auto start = std::chrono::steady_clock::now();
            const auto result =
                inspect_boundary_witness(sets[i], grid, options.fineness);
            times.push_back(std::chrono::duration<double, std::milli>(
                std::chrono::steady_clock::now() - start).count());
            if (sample == 0u) first_decision = result;
            else if (result.decision != first_decision.decision or
                     result.witness_axis != first_decision.witness_axis)
                throw std::runtime_error("nondeterministic preflight decision");
        }
        const double cost = median(times);
        classifier_costs.push_back(cost);

        BinaryTreePaving a(grid), b(grid);
        constraint.adjoin_outer_approximation(a, sets[i], options.fineness);
        hull.adjoin_outer_approximation(b, sets[i], options.fineness);
        a.recombine();
        b.recombine();
        if (not contains(a,b))
            throw std::runtime_error("HullOnly not contained in Constraint");
        const bool tighter = not contains(b,a);
        if (first_decision.decision == BoundaryPreflight::Decision::Unknown) {
            ++unknown;
        } else if (first_decision.decision == BoundaryPreflight::Decision::Witness) {
            if (tighter) ++tp; else ++fp;
        } else {
            if (tighter) ++fn; else ++tn;
        }
        std::cout << "SPRE22_PREFLIGHT index=" << i+1u
                  << " decision=" << preflight_name(first_decision.decision)
                  << " reference=" << (tighter ? "hull_tighter" : "equal")
                  << " tested_faces=" << first_decision.tested_faces
                  << " witness_axis=" << first_decision.witness_axis
                  << " witness_face=" << first_decision.witness_face
                  << " witness_margin=" << first_decision.witness_margin
                  << " median_ms=" << cost
                  << " mad_ms=" << mad(times)
                  << "\n";
    }
    std::cout << "SPRE22_PREFLIGHT_CLASSIFICATION selected="
              << options.max_enclosures
              << " f=" << options.fineness
              << " true_positive=" << tp
              << " false_positive=" << fp
              << " true_negative=" << tn
              << " false_negative=" << fn
              << " unknown=" << unknown
              << " median_preflight_ms=" << median(classifier_costs)
              << "\n";

    // Preflight and selected Paver are timed together. Full sequence
    // geometries are compared by bidirectional containment, separately.
    const std::string names[3]{
        "constraint", "reduce-hull", "preflight-adaptive"
    };
    std::unique_ptr<BinaryTreePaving> references[3];
    double medians[3]{0.0,0.0,0.0};
    for (unsigned strategy = 0u; strategy < 3u; ++strategy) {
        std::vector<double> samples;
        samples.reserve(options.samples);
        for (unsigned sample = 0u; sample < options.samples; ++sample) {
            BinaryTreePaving paving(grid);
            const auto start = std::chrono::steady_clock::now();
            for (unsigned i = 0u; i < options.max_enclosures; ++i) {
                bool use_hull = strategy == 1u;
                if (strategy == 2u) {
                    const auto result =
                        inspect_boundary_witness(sets[i], grid, options.fineness);
                    // Unknown: retain HullOnly. NoWitness: experimental
                    // prediction, not a proof of equal output geometry.
                    use_hull =
                        result.decision != BoundaryPreflight::Decision::NoWitness;
                }
                (use_hull ? hull : constraint).adjoin_outer_approximation(
                    paving, sets[i], options.fineness);
            }
            samples.push_back(std::chrono::duration<double,std::milli>(
                std::chrono::steady_clock::now() - start).count());
            paving.recombine();
            if (not references[strategy])
                references[strategy] = std::make_unique<BinaryTreePaving>(paving);
            else if (not contains(*references[strategy], paving) or
                     not contains(paving, *references[strategy]))
                throw std::runtime_error("nondeterministic preflight strategy geometry");
        }
        medians[strategy] = median(samples);
        std::cout << "SPRE22_PREFLIGHT_SEQUENCE strategy=" << names[strategy]
                  << " median_ms=" << medians[strategy]
                  << " mad_ms=" << mad(samples)
                  << " cells=" << references[strategy]->size()
                  << " outer_volume=" << references[strategy]->measure().raw().get_d()
                  << "\n";
    }
    const auto geometry = relation(*references[2], *references[1]);
    std::cout << "SPRE22_PREFLIGHT_RESULT"
              << " selected=" << options.max_enclosures
              << " f=" << options.fineness
              << " geometry_vs_hull=" << geometry
              << " hull_over_adaptive=" << medians[1] / medians[2]
              << " adaptive_over_constraint=" << medians[2] / medians[0]
              << " classifier_included_in_timing=true"
              << "\n";
    // Explicitly fail geometric regressions; never silently declare an
    // experimentally fitted approximation equivalent to HullOnly.
    return geometry == "equal" ? 0 : 2;
}

int run(int argc, const char* argv[]) {
    auto options = parse(argc, argv);
    if (options.samples == 0u) return 0;
    if (options.audit_verbose and not options.audit_all_cells)
        throw std::invalid_argument("--audit-verbose requires --audit-all-cells");
    if (options.preflight and
        (options.audit_all_cells or options.analyze_enclosures or
         not options.inspected_indices.empty()))
        throw std::invalid_argument("--preflight is exclusive with other inspection modes");
    if (options.audit_all_cells) {
        if (options.analyze_enclosures or not options.inspected_indices.empty())
            throw std::invalid_argument(
                "--audit-all-cells cannot be combined with --analyze-enclosures or --inspect-cells");
        options.inspected_indices.reserve(options.max_enclosures);
        for (unsigned i = 1u; i <= options.max_enclosures; ++i)
            options.inspected_indices.push_back(i);
    }
    const auto sets = SPRE22Fixture::real_enclosures();
    if (sets.size() != 48u)
        throw std::runtime_error("fixture must contain all 48 real enclosures");
    if (options.max_enclosures > sets.size() or
        (not options.preflight and not options.analyze_enclosures and
         options.inspected_indices.empty() and
         options.switch_after >= options.max_enclosures))
        throw std::invalid_argument("invalid enclosure count or hybrid switch");

    const Grid grid(Vector<ApproximateDouble>{1.0, 200.0, 200.0, 20.0, 20.0});
    if (options.preflight)
        return evaluate_preflight(options, sets, grid);
    if (options.analyze_enclosures) {
        if (not options.inspected_indices.empty())
            throw std::invalid_argument("--analyze-enclosures and --inspect-cells are exclusive");
        return analyze_enclosures(options, sets, grid);
    }
    if (not options.inspected_indices.empty())
        return inspect_cell_differences(options, sets, grid);
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
