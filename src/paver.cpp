/***************************************************************************
 *            src/paver.cpp
 *
 *  Copyright  2011-20  Pieter Collins
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

/** @file src/paver.cpp
 *  @brief Implement paver operations.
 */

#include "paving/paver.hpp"

#include <algorithm>
#include <chrono>
#include <type_traits>
#include <vector>

#include "algebra/algebra.hpp"
#include "algebra/polynomial.hpp"
#include "function/function.hpp"
#include "function/functional.hpp"
#include "function/procedure.hpp"
#include "function/taylor_function.hpp"
#include "logging/logging.hpp"
#include "paving/binary_tree_paving.hpp"
#include "paving/paving_interface.hpp"
#include "paving/set_approximation.hpp"
#include "set/affine_set.hpp"
#include "set/function_set.hpp"
#include "threading/task_group.hpp"
#include "threading/thread_manager.hpp"
#include "utility/macros.hpp"

#include "constraint-solving/constraint_solver.hpp"
#include "nonlinear-programming/nonlinear_interior_point_method.hpp"
#include "nonlinear-programming/nonlinear_programming.hpp"

namespace Ariadne {

Pair<SizeType, FloatDPError> nonlinearity_index_and_error(const ValidatedVectorMultivariateFunction& function, const ExactBoxType& domain);
Pair<SizeType, FloatDPError> lipschitz_index_and_error(const ValidatedVectorMultivariateFunction& function, const ExactBoxType& domain);
inline Pair<SizeType, FloatDPError> lipschitz_index_and_error(const ValidatedVectorMultivariateFunction& function, const UpperBoxType& domain) {
    return lipschitz_index_and_error(function, cast_exact_box(domain));
}

namespace {

// Time only the operation itself, in the explicitly instrumented replay.
// The ordinary non-diagnostic path avoids reading the clock entirely.
template<class Operation>
auto profile_paver_operation(PaverDiagnostics* diagnostics,
                             std::uint64_t PaverDiagnostics::* nanoseconds,
                             Operation&& operation) -> decltype(operation()) {
    if (diagnostics == nullptr)
        return operation();
    const auto begin = std::chrono::steady_clock::now();
    if constexpr (std::is_void_v<decltype(operation())>) {
        operation();
        diagnostics->*nanoseconds += static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now() - begin)
                .count());
    } else {
        auto result = operation();
        diagnostics->*nanoseconds += static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now() - begin)
                .count());
        return result;
    }
}

// Ignore zero-output constraint functions when counting substantive work,
// but still evaluate them if the original algorithm does so.
template<class Domain>
auto profile_constraint_range(const ValidatedVectorMultivariateFunction& constraints,
                              const Domain& domain, PaverDiagnostics* diagnostics) {
    if (constraints.result_size() == 0u)
        return apply(constraints, domain);
    if (diagnostics != nullptr)
        ++diagnostics->constraint_evaluations;
    return profile_paver_operation(diagnostics, &PaverDiagnostics::constraint_range_ns,
                                   [&] { return apply(constraints, domain); });
}

PositiveFloatDPUpperBound average_width(const UpperBoxType& bx) {
    PositiveFloatDPUpperBound res(0u, double_precision);
    for (SizeType i = 0; i != bx.size(); ++i) {
        res += bx[i].width();
    }
    // ReducePaver delegates parameter-free maps before entering reduction.
    return res / bx.size();
}

PositiveFloatDPUpperBound maximum_scaled_width(const UpperBoxType& bx, const Vector<PositiveFloatDP>& sf) {
    PositiveFloatDPUpperBound res(0u, double_precision);
    for (SizeType i = 0; i != bx.size(); ++i) {
        res = max(bx[i].width() / sf[i], res);
    }
    return res;
}

PositiveFloatDPUpperBound average_scaled_width(const UpperBoxType& bx, const Vector<PositiveFloatDP>& sf) {
    PositiveFloatDPUpperBound res(0u, double_precision);
    for (SizeType i = 0; i != bx.size(); ++i) {
        res += (bx[i].width() / sf[i]);
    }
    return bx.size() == 0u ? res : res / bx.size();
}

FloatDP average_scaled_width(const UpperBoxType& bx, const Vector<FloatDP>& sf) {
    Vector<PositiveFloatDP> positive_sf(sf.size(), [&](SizeType i) { return cast_positive(sf[i]); }, dp);
    return average_scaled_width(bx, positive_sf).raw();
}

// Each task owns its counters, so aggregation happens only after joining.
void accumulate_paver_diagnostics(PaverDiagnostics& total, const PaverDiagnostics& part) {
    total.visited_nodes += part.visited_nodes;
    total.parameter_splits += part.parameter_splits;
    total.cell_splits += part.cell_splits;
    total.map_evaluations += part.map_evaluations;
    total.constraint_evaluations += part.constraint_evaluations;
    total.sensitivity_evaluations += part.sensitivity_evaluations;
    total.reduction_calls += part.reduction_calls;
    total.box_reduction_calls += part.box_reduction_calls;
    total.hull_reduction_calls += part.hull_reduction_calls;
    total.constraint_reduction_calls += part.constraint_reduction_calls;
    total.join_constructions += part.join_constructions;
    total.disjoint_rejections += part.disjoint_rejections;
    total.already_covered += part.already_covered;
    total.adjoin_calls += part.adjoin_calls;
    total.map_range_ns += part.map_range_ns;
    total.constraint_range_ns += part.constraint_range_ns;
    total.joint_range_ns += part.joint_range_ns;
    total.sensitivity_ns += part.sensitivity_ns;
    total.contractor_ns += part.contractor_ns;
    total.box_reduction_ns += part.box_reduction_ns;
    total.hull_reduction_ns += part.hull_reduction_ns;
    total.join_ns += part.join_ns;
    total.adjoin_ns += part.adjoin_ns;
}

} // namespace

OutputStream& AffinePaver::_write(OutputStream& os) const { return os << "AffinePaver()"; }
OutputStream& SubdivisionPaver::_write(OutputStream& os) const { return os << "SubdivisionPaver()"; }
OutputStream& ReducePaver::_write(OutputStream& os) const { return os << "ReducePaver()"; }
OutputStream& ConstraintPaver::_write(OutputStream& os) const { return os << "ConstraintPaver()"; }

Void SubdivisionPaver::adjoin_outer_approximation(PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness) const {
    this->_adjoin_outer_approximation(paving, set, fineness, nullptr);
}

Void SubdivisionPaver::adjoin_outer_approximation(PavingInterface& paving,
                                                  const ValidatedConstrainedImageSet& set,
                                                  Nat fineness,
                                                  PaverDiagnostics& diagnostics) const {
    diagnostics = {};
    this->_adjoin_outer_approximation(paving, set, fineness, &diagnostics);
}

Void SubdivisionPaver::_adjoin_outer_approximation(PavingInterface& paving,
                                                   const ValidatedConstrainedImageSet& set,
                                                   Nat fineness,
                                                   PaverDiagnostics* diagnostics) const {
    Vector<FloatDP> max_errors(paving.dimension(), dp);
    for (SizeType i = 0; i != max_errors.size(); ++i) {
        max_errors[i] = shft(static_cast<FloatDP>(paving.grid().lengths()[i]), -static_cast<int>(fineness));
    }

    ThreadManager& manager = ThreadManager::instance();
    const SizeType concurrency = manager.concurrency();
    if (concurrency == 0u) {
        this->_adjoin_outer_approximation_recursion(
            paving, set, fineness, max_errors, diagnostics);
        return;
    }

    // Bound the number of independent subdomains submitted to the shared
    // ThreadManager pool. Build the frontier with the *same* stopping and
    // splitting rules as the serial recursion; do not enqueue a task for
    // every recursive node.
    constexpr SizeType max_frontier_size = 32u;
    const SizeType target = std::min(max_frontier_size, concurrency * 2u);
    Nat frontier_depth = 0u;
    SizeType frontier_capacity = 1u;
    while (frontier_capacity < target) {
        frontier_capacity *= 2u;
        ++frontier_depth;
    }
    std::vector<ValidatedConstrainedImageSet> frontier;
    frontier.reserve(frontier_capacity);
    this->_adjoin_outer_approximation_recursion(
        paving, set, fineness, max_errors, diagnostics, &frontier, frontier_depth);

    if (frontier.empty())
        return;
    if (frontier.size() == 1u) {
        this->_adjoin_outer_approximation_recursion(
            paving, frontier.front(), fineness, max_errors, diagnostics);
        return;
    }

    // Public paving insertion is synchronized by its owner. The expensive
    // parameter-box evaluations are independent, while all task exceptions
    // are observed only after every submitted child has completed.
    std::vector<PaverDiagnostics> local_diagnostics(frontier.size());
    auto process = [&](SizeType index) {
        this->_adjoin_outer_approximation_recursion(
            paving, frontier[index], fineness, max_errors,
            diagnostics == nullptr ? nullptr : &local_diagnostics[index]);
    };
    TaskGroup<ThreadManager> group(manager);
    for (SizeType index = 1u; index != frontier.size(); ++index)
        group.fork([&, index] { process(index); });
    std::exception_ptr caller_error;
    try {
        process(0u);
    } catch (...) {
        caller_error = std::current_exception();
    }
    std::exception_ptr worker_error;
    try {
        group.join();
    } catch (...) {
        worker_error = std::current_exception();
    }
    if (caller_error)
        std::rethrow_exception(caller_error);
    if (worker_error)
        std::rethrow_exception(worker_error);
    if (diagnostics != nullptr) {
        for (const auto& part: local_diagnostics)
            accumulate_paver_diagnostics(*diagnostics, part);
    }
}

Void SubdivisionPaver::adjoin_outer_approximation_recursion(PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness, const Vector<FloatDP>& max_errors) const {
    this->_adjoin_outer_approximation_recursion(paving, set, fineness, max_errors, nullptr);
}

Void SubdivisionPaver::_adjoin_outer_approximation_recursion(
    PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness,
    const Vector<FloatDP>& max_errors, PaverDiagnostics* diagnostics,
    std::vector<ValidatedConstrainedImageSet>* frontier,
    Nat remaining_frontier_depth) const {
    // Frontier leaves are evaluated later, exactly once by one worker.
    if (frontier != nullptr && remaining_frontier_depth == 0u) {
        frontier->push_back(set);
        return;
    }
    if (diagnostics != nullptr)
        ++diagnostics->visited_nodes;
    // How small an over-approximating box needs to be relative to the cell size
    static const ExactDouble RELATIVE_SMALLNESS = 0.5_x;

    ARIADNE_PRECONDITION(paving.dimension() == set.dimension());
    ARIADNE_PRECONDITION(max_errors.size() == paving.dimension());
    for (SizeType i = 0; i != max_errors.size(); ++i) {
        ARIADNE_PRECONDITION(max_errors[i] > FloatDP(0, dp));
    }
    const ExactBoxType& subdomain = set.reduced_domain();
    if (definitely(subdomain.is_empty())) {
        return;
    }
    const ValidatedVectorMultivariateFunction& function = set.function();
    const List<ValidatedConstraint>& constraints = set.constraints();

    for (List<ValidatedConstraint>::ConstIterator iter = constraints.begin();
         iter != constraints.end(); ++iter) {
        if (diagnostics != nullptr)
            ++diagnostics->constraint_evaluations;
        UpperIntervalType constraint_range = profile_paver_operation(
            diagnostics, &PaverDiagnostics::constraint_range_ns,
            [&] { return apply(iter->function(), subdomain); });
        if (definitely(constraint_range.lower_bound() > iter->bounds().upper_bound() || constraint_range.upper_bound() < iter->bounds().lower_bound())) {
            if (diagnostics != nullptr)
                ++diagnostics->disjoint_rejections;
            return;
        }
    }

    if (diagnostics != nullptr)
        ++diagnostics->map_evaluations;
    UpperBoxType range = profile_paver_operation(
        diagnostics, &PaverDiagnostics::map_range_ns,
        [&] { return apply(function, subdomain); });
    Bool small = true;
    for (SizeType i = 0; i != range.size(); ++i) {
        if (possibly(hlf(range[i].width()) > max_errors[i] * RELATIVE_SMALLNESS)) {
            small = false;
            break;
        }
    }

    if (small) {
        if (diagnostics != nullptr)
            ++diagnostics->adjoin_calls;
        profile_paver_operation(
            diagnostics, &PaverDiagnostics::adjoin_ns,
            [&] { paving.adjoin_outer_approximation(cast_exact_box(range), fineness); });
    } else {
        if (diagnostics != nullptr)
            ++diagnostics->sensitivity_evaluations;
        const auto sensitivity = profile_paver_operation(
            diagnostics, &PaverDiagnostics::sensitivity_ns,
            [&] { return lipschitz_index_and_error(function, subdomain); });
        FloatDP minimum_error = max_errors[0];
        for (SizeType i = 1u; i != max_errors.size(); ++i) {
            minimum_error = min(minimum_error, max_errors[i]);
        }
        if (sensitivity.first == subdomain.dimension() || sensitivity.second.raw() <= mul(down, minimum_error, FloatDP(RELATIVE_SMALLNESS, dp))) {
            // Further parameter subdivision cannot remove intrinsic output
            // uncertainty. Discretise its enclosing box at the requested grid resolution.
            if (diagnostics != nullptr)
                ++diagnostics->adjoin_calls;
            profile_paver_operation(
                diagnostics, &PaverDiagnostics::adjoin_ns,
                [&] { paving.adjoin_outer_approximation(cast_exact_box(range), fineness); });
            return;
        }
        if (diagnostics != nullptr)
            ++diagnostics->parameter_splits;
        Pair<ValidatedConstrainedImageSet, ValidatedConstrainedImageSet> subsets = set.split(sensitivity.first);
        const Nat child_frontier_depth =
            frontier == nullptr ? 0u : remaining_frontier_depth - 1u;
        this->_adjoin_outer_approximation_recursion(
            paving, subsets.first, fineness, max_errors, diagnostics,
            frontier, child_frontier_depth);
        this->_adjoin_outer_approximation_recursion(
            paving, subsets.second, fineness, max_errors, diagnostics,
            frontier, child_frontier_depth);
    }
}

Void AffinePaver::adjoin_outer_approximation(PavingInterface& paving,
                                             const ValidatedConstrainedImageSet& set,
                                             Nat fineness) const {
    this->_adjoin_outer_approximation(paving, set, fineness, nullptr);
}

Void AffinePaver::adjoin_outer_approximation(PavingInterface& paving,
                                             const ValidatedConstrainedImageSet& set,
                                             Nat fineness,
                                             AffinePaverDiagnostics& diagnostics) const {
    this->_adjoin_outer_approximation(paving, set, fineness, &diagnostics);
}

Void AffinePaver::_adjoin_outer_approximation(PavingInterface& paving,
                                              const ValidatedConstrainedImageSet& set,
                                              Nat fineness,
                                              AffinePaverDiagnostics* diagnostics) const {
    if (diagnostics != nullptr)
        *diagnostics = {};
    ARIADNE_PRECONDITION(paving.dimension() == set.dimension());
    const ExactBoxType& subdomain = set.reduced_domain();
    if (definitely(subdomain.is_empty())) {
        return;
    }
    const ValidatedVectorMultivariateFunction& function = set.function();
    const List<ValidatedConstraint>& constraints = set.constraints();

    // Bound the maximum number of splittings allowed to draw a particular set.
    // Note that this gives rise to possibly 2^MAX_DEPTH split sets!!
    static const Nat MAXIMUM_DEPTH = 16;
    if (diagnostics != nullptr)
        diagnostics->depth_limit = MAXIMUM_DEPTH;

    // The basic approximation error when plotting with accuracy=0
    static const ExactDouble BASIC_ERROR = 0.0625_pr;

    const ExactDouble max_error = BASIC_ERROR / TwoExp(static_cast<Int>(fineness));

    // Constraint functions are constructed on the mapping function's domain.
    ValidatedVectorMultivariateFunction fg = join(function, set.constraint_function());

    List<ExactBoxType> subdomains;
    List<ExactBoxType> unsplitdomains;
    List<ExactBoxType> splitdomains;
    unsplitdomains.append(subdomain);
    ExactBoxType splitdomain1, splitdomain2;
    for (Nat i = 0; i != MAXIMUM_DEPTH; ++i) {
        if (diagnostics != nullptr)
            ++diagnostics->completed_levels;
        // std::cerr<<"i="<<i<<"\nsubdomains="<<subdomains<<"\nunsplitdomains="<<unsplitdomains<<"\n\n";
        for (SizeType n = 0; n != unsplitdomains.size(); ++n) {
            SizeType k;
            FloatDPError err(dp);
            make_lpair(k, err) = nonlinearity_index_and_error(fg, unsplitdomains[n]);
            // std::cerr<<"  domain="<<unsplitdomains[n]<<" k="<<k<<" err="<<err<<" max_err="<<max_error<<"\n";
            if (k == subdomain.size() || definitely(err < max_error)) {
                subdomains.append(unsplitdomains[n]);
            } else {
                make_lpair(splitdomain1, splitdomain2) = unsplitdomains[n].split(k);
                if (diagnostics != nullptr)
                    ++diagnostics->split_domains;
                splitdomains.append(splitdomain1);
                splitdomains.append(splitdomain2);
            }
        }
        unsplitdomains.swap(splitdomains);
        splitdomains.clear();
        if (unsplitdomains.empty()) {
            break;
        }
    }
    if (diagnostics != nullptr)
        diagnostics->unresolved_domains = unsplitdomains.size();
    subdomains.concatenate(unsplitdomains);
    if (!unsplitdomains.empty()) {
        ARIADNE_WARN("Cannot obtain desired accuracy in outer approximation without excessive splitting.");
    }

    for (SizeType n = 0; n != subdomains.size(); ++n) {
        ValidatedConstrainedImageSet subset(subdomains[n], function, constraints);
        adjoin_outer_approximation_to(paving, subset.affine_over_approximation(), fineness);
    }
}

namespace {

template<class Y, class PR>
decltype(auto) make_raw_float(const Y& y, PR pr) { return RawFloat<PR>(y, pr); }

inline Bool strictly_smaller_by_factor(PositiveFloatDPUpperBound x1, PositiveFloatDPUpperBound x2, ExactDouble sf) {
    return x1.raw() < mul(down, x2.raw(), make_raw_float(sf, dp));
}
inline Bool strictly_smaller(PositiveFloatDPUpperBound x1, PositiveFloatDPUpperBound x2) {
    return x1.raw() < x2.raw();
}

FloatDP cell_splitting_radius(const GridCell& cell) {
    // The zero-dimensional cell is a singleton, with no coordinate widths.
    return cell.dimension() == 0u ? FloatDP(0, dp) : cell.box().radius().upper().raw();
}

// Adjoin an over-approximation to the solution of $f(dom)$ such that $g(D) in C$ to the paving p, looking only at solutions in b.
Void procedure_constraint_adjoin_outer_approximation_recursion(
    PavingInterface& paving, const ExactBoxType& domain, const ValidatedVectorMultivariateFunction& f,
    const ValidatedVectorMultivariateFunction& g, const ExactBoxType& codomain, const GridCell& cell, Int max_dpth, SizeType splt, const List<ValidatedProcedure>& procedures, PaverDiagnostics* diagnostics) {
    LOGGING_SCOPE_CREATE;
    if (diagnostics != nullptr)
        ++diagnostics->visited_nodes;

    const SizeType m = domain.size();
    const SizeType nf = f.result_size();
    const SizeType ng = g.result_size();

    const ExactBoxType& cell_box = cell.box();
    const Vector<PositiveFloatDP> scalings = Vector<PositiveFloatDP>(paving.grid().lengths());

    if (diagnostics != nullptr)
        ++diagnostics->map_evaluations;
    UpperBoxType bbox = profile_paver_operation(
        diagnostics, &PaverDiagnostics::map_range_ns,
        [&] { return apply(f, domain); });

    FloatDP domwdth = average_width(domain).raw();
    FloatDP bbxwdth = average_scaled_width(bbox, paving.grid().lengths());
    FloatDP clwdth = average_scaled_width(cell_box, paving.grid().lengths());

    LOGGING_PRINTLN_AT(1, "splt=" << splt << " dpth=" << cell.depth() << " max_dpth=" << max_dpth);
    LOGGING_PRINTLN_AT(1, "domwdth=" << domwdth << " bbxwdth=" << bbxwdth << " clwdth=" << clwdth << " dom=" << domain << " bbox=" << bbox << " cell=" << cell.box());

    ConstraintSolver constraint_solver;

    if (paving.superset(cell)) {
        if (diagnostics != nullptr)
            ++diagnostics->already_covered;
        LOGGING_PRINTLN("Cell is already a subset of paving");
        return;
    }

    // Try to prove disjointness
    const ExactBoxType& old_domain = domain;
    UpperBoxType new_domain(old_domain);
    PositiveFloatDPUpperBound olddomwdth = average_width(domain);
    PositiveFloatDPUpperBound newdomwdth = olddomwdth;

    static const ExactDouble ACCEPTABLE_REDUCTION_FACTOR = 0.75_x;

    // ExactBoxType reduction steps
    for (SizeType i = 0; i != nf; ++i) {
        for (SizeType j = 0; j != m; ++j) {
            if (diagnostics != nullptr) {
                ++diagnostics->reduction_calls;
                ++diagnostics->box_reduction_calls;
            }
            profile_paver_operation(
                diagnostics, &PaverDiagnostics::box_reduction_ns,
                [&] { constraint_solver.box_reduce(new_domain, f[i], cell_box[i], j); });
            if (definitely(new_domain.is_empty())) {
                if (diagnostics != nullptr)
                    ++diagnostics->disjoint_rejections;
                LOGGING_PRINTLN("Proved disjointness using box reduce");
                return;
            }
        }
    }
    for (SizeType i = 0; i != ng; ++i) {
        for (SizeType j = 0; j != m; ++j) {
            if (diagnostics != nullptr) {
                ++diagnostics->reduction_calls;
                ++diagnostics->box_reduction_calls;
                ++diagnostics->constraint_reduction_calls;
            }
            profile_paver_operation(
                diagnostics, &PaverDiagnostics::box_reduction_ns,
                [&] { constraint_solver.box_reduce(new_domain, g[i], codomain[i], j); });
            if (definitely(new_domain.is_empty())) {
                if (diagnostics != nullptr)
                    ++diagnostics->disjoint_rejections;
                LOGGING_PRINTLN("Proved disjointness using box reduce");
                return;
            }
        }
    }
    newdomwdth = average_width(new_domain);
    LOGGING_PRINTLN_AT(1, "domwdth=" << newdomwdth << " olddomwdth=" << olddomwdth << " dom=" << new_domain << " box reduce");

    // Hull reduction steps
    do {
        olddomwdth = newdomwdth;
        for (SizeType i = 0; i != nf; ++i) {
            if (diagnostics != nullptr) {
                ++diagnostics->reduction_calls;
                ++diagnostics->hull_reduction_calls;
            }
            profile_paver_operation(
                diagnostics, &PaverDiagnostics::hull_reduction_ns,
                [&] { constraint_solver.hull_reduce(new_domain, procedures[i], cell_box[i]); });
            if (definitely(new_domain.is_empty())) {
                if (diagnostics != nullptr)
                    ++diagnostics->disjoint_rejections;
                LOGGING_PRINTLN("Proved disjointness using hull reduce");
                return;
            }
            // constraint_solver.hull_reduce(new_domain,f[i],cell_box[i]);
        }
        for (SizeType i = 0; i != ng; ++i) {
            if (diagnostics != nullptr) {
                ++diagnostics->reduction_calls;
                ++diagnostics->hull_reduction_calls;
                ++diagnostics->constraint_reduction_calls;
            }
            profile_paver_operation(
                diagnostics, &PaverDiagnostics::hull_reduction_ns,
                [&] { constraint_solver.hull_reduce(new_domain, procedures[nf + i], codomain[i]); });
            if (definitely(new_domain.is_empty())) {
                if (diagnostics != nullptr)
                    ++diagnostics->disjoint_rejections;
                LOGGING_PRINTLN("Proved disjointness using hull reduce");
                return;
            }
            // constraint_solver.hull_reduce(new_domain,g[i],codomain[i]);
        }
        newdomwdth = average_width(new_domain);
        LOGGING_PRINTLN_AT(2, "domwdth=" << newdomwdth << " dom=" << new_domain);
        // Each reduction above returns immediately if the domain becomes empty.
    } while (strictly_smaller_by_factor(newdomwdth, olddomwdth, ACCEPTABLE_REDUCTION_FACTOR));

    LOGGING_PRINTLN_AT(1, "new_domain=" << new_domain);

    domwdth = average_scaled_width(new_domain, RawFloatDPVector(new_domain.size(), FloatDP(1.0_x, dp)));
    if (diagnostics != nullptr)
        ++diagnostics->map_evaluations;
    bbox = profile_paver_operation(
        diagnostics, &PaverDiagnostics::map_range_ns,
        [&] { return apply(f, new_domain); });
    bbxwdth = average_scaled_width(bbox, paving.grid().lengths());
    // Retain short-circuiting: no constraint interval evaluation is
    // performed or counted if the image has already been proved disjoint.
    if (definitely(bbox.disjoint(cell_box)) ||
        definitely(codomain.disjoint(profile_constraint_range(g, new_domain, diagnostics)))) {
        if (diagnostics != nullptr)
            ++diagnostics->disjoint_rejections;
        LOGGING_PRINTLN("Proved disjointness using image of new domain");
        return;
    }

    LOGGING_PRINTLN_AT(1, "domwdth=" << domwdth << " bbxwdth=" << bbxwdth << " clwdth=" << clwdth << " dom=" << new_domain << " bbox=" << bbox << " cell=" << cell.box());

    // Decide whether to split cell or split domain by comparing size of
    // bounding box with the cell and splitting the larger.
    // It seems that a more efficient algorithm results if the domain
    // is only split if the bounding box is much larger, so we preferentiably
    // split the cell unless the bounding box is 4 times as large
    PositiveFloatDPUpperBound bbxmaxwdth = maximum_scaled_width(bbox, scalings);
    PositiveFloatDPUpperBound clmaxwdth = maximum_scaled_width(cell_box, scalings);
    ExactDouble RELATIVE_SPLITTING_SIZE = 4.0_x;

    if (!strictly_smaller_by_factor(bbxmaxwdth, clmaxwdth, RELATIVE_SPLITTING_SIZE) || (cell.depth() >= max_dpth && strictly_smaller(clmaxwdth, bbxmaxwdth))) {
        if (diagnostics != nullptr)
            ++diagnostics->sensitivity_evaluations;
        const auto sensitivity = profile_paver_operation(
            diagnostics, &PaverDiagnostics::sensitivity_ns,
            [&] { return lipschitz_index_and_error(f, new_domain); });
        if (sensitivity.first < m && sensitivity.second.raw() > cell_splitting_radius(cell)) {
            if (diagnostics != nullptr)
                ++diagnostics->parameter_splits;
            LOGGING_PRINTLN("Splitting domain on coordinate " << sensitivity.first);
            const auto domains = cast_exact_box(new_domain).split(sensitivity.first);
            procedure_constraint_adjoin_outer_approximation_recursion(paving, domains.first, f, g, codomain, cell, max_dpth, splt + 1, procedures, diagnostics);
            procedure_constraint_adjoin_outer_approximation_recursion(paving, domains.second, f, g, codomain, cell, max_dpth, splt + 1, procedures, diagnostics);
            return;
        }
    }
    if (cell.depth() >= max_dpth) {
        if (diagnostics != nullptr)
            ++diagnostics->adjoin_calls;
        LOGGING_PRINTLN("Adjoining cell " << cell_box);
        profile_paver_operation(
            diagnostics, &PaverDiagnostics::adjoin_ns,
            [&] { paving.adjoin(cell); });
    } else {
        if (diagnostics != nullptr)
            ++diagnostics->cell_splits;
        LOGGING_PRINTLN("Splitting cell " << cell_box);
        Pair<GridCell, GridCell> sb = cell.split();
        procedure_constraint_adjoin_outer_approximation_recursion(paving, cast_exact_box(new_domain), f, g, codomain, sb.first, max_dpth, splt, procedures, diagnostics);
        procedure_constraint_adjoin_outer_approximation_recursion(paving, cast_exact_box(new_domain), f, g, codomain, sb.second, max_dpth, splt, procedures, diagnostics);
    }
}

// The former hot-start solver did not update its feasibility variable:
// it remained zero. Use validated interval subdivision with a sensitivity check, so intrinsic
// uncertainty cannot cause endless parameter splits.
Void interval_constraint_adjoin_outer_approximation_recursion(
    PavingInterface& paving, const ExactBoxType& domain,
    const ValidatedVectorMultivariateFunction& function,
    const ValidatedVectorMultivariateFunction& constraints,
    const ValidatedVectorMultivariateFunction& combined,
    const ExactBoxType& bounds, const GridCell& cell, Nat fineness,
    PaverDiagnostics* diagnostics) {
    if (diagnostics != nullptr)
        ++diagnostics->visited_nodes;
    if (paving.superset(cell)) {
        if (diagnostics != nullptr)
            ++diagnostics->already_covered;
        return;
    }
    if (diagnostics != nullptr) {
        ++diagnostics->map_evaluations;
        if (constraints.result_size() != 0u)
            ++diagnostics->constraint_evaluations;
    }
    const auto combined_range = profile_paver_operation(
        diagnostics,
        constraints.result_size() != 0u ? &PaverDiagnostics::joint_range_ns
                                        : &PaverDiagnostics::map_range_ns,
        [&] { return apply(combined, domain); });
    if (definitely(disjoint(combined_range, product(cell.box(), bounds)))) {
        if (diagnostics != nullptr)
            ++diagnostics->disjoint_rejections;
        return;
    }

    if (diagnostics != nullptr)
        ++diagnostics->sensitivity_evaluations;
    const auto sensitivity = profile_paver_operation(
        diagnostics, &PaverDiagnostics::sensitivity_ns,
        [&] { return lipschitz_index_and_error(function, domain); });
    if (sensitivity.first < domain.dimension() && sensitivity.second.raw() > cell_splitting_radius(cell)) {
        if (diagnostics != nullptr)
            ++diagnostics->parameter_splits;
        const auto domains = domain.split(sensitivity.first);
        interval_constraint_adjoin_outer_approximation_recursion(paving, domains.first, function, constraints, combined, bounds, cell, fineness, diagnostics);
        interval_constraint_adjoin_outer_approximation_recursion(paving, domains.second, function, constraints, combined, bounds, cell, fineness, diagnostics);
    } else if (cell.depth() >= Int(fineness * cell.dimension())) {
        if (diagnostics != nullptr)
            ++diagnostics->adjoin_calls;
        profile_paver_operation(
            diagnostics, &PaverDiagnostics::adjoin_ns,
            [&] { paving.adjoin(cell); });
    } else {
        if (diagnostics != nullptr)
            ++diagnostics->cell_splits;
        const auto cells = cell.split();
        interval_constraint_adjoin_outer_approximation_recursion(paving, domain, function, constraints, combined, bounds, cells.first, fineness, diagnostics);
        interval_constraint_adjoin_outer_approximation_recursion(paving, domain, function, constraints, combined, bounds, cells.second, fineness, diagnostics);
    }
}

Void constraint_adjoin_outer_approximation(
    PavingInterface& paving, const ExactBoxType& domain,
    const ValidatedVectorMultivariateFunction& function,
    const ValidatedVectorMultivariateFunction& constraints, const ExactBoxType& bounds, Nat fineness,
    PaverDiagnostics* diagnostics) {
    if (diagnostics != nullptr)
        ++diagnostics->map_evaluations;
    const auto initial_range = profile_paver_operation(
        diagnostics, &PaverDiagnostics::map_range_ns,
        [&] { return apply(function, domain); });
    const GridCell cell = GridCell::smallest_enclosing_primary_cell(
        cast_exact_box(initial_range), paving.grid());
    const auto initial_constraints =
        profile_constraint_range(constraints, domain, diagnostics);
    const ExactBoxType constraint_bounds = intersection(
        cast_exact_box(widen(initial_constraints, 1)), bounds);
    if (diagnostics != nullptr)
        ++diagnostics->join_constructions;
    const auto combined = profile_paver_operation(
        diagnostics, &PaverDiagnostics::join_ns,
        [&] { return join(function, constraints); });
    interval_constraint_adjoin_outer_approximation_recursion(
        paving, domain, function, constraints, combined, constraint_bounds,
        cell, fineness, diagnostics);
}

Void procedure_constraint_adjoin_outer_approximation(PavingInterface& p, const ExactBoxType& d, const ValidatedVectorMultivariateFunction& f,
                                                     const ValidatedVectorMultivariateFunction& g, const ExactBoxType& c, Nat e,
                                                     PaverDiagnostics* diagnostics) {
    if (diagnostics != nullptr)
        ++diagnostics->map_evaluations;
    const auto initial_range = profile_paver_operation(
        diagnostics, &PaverDiagnostics::map_range_ns,
        [&] { return apply(f, d); });
    GridCell b = p.smallest_enclosing_primary_cell(cast_exact_box(initial_range));

    List<ValidatedProcedure> procedures;
    procedures.reserve(f.result_size() + g.result_size());
    for (SizeType i = 0; i != f.result_size(); ++i) {
        procedures.append(make_procedure(f[i]));
    }
    for (SizeType i = 0; i != g.result_size(); ++i) {
        procedures.append(make_procedure(g[i]));
    }

    Ariadne::procedure_constraint_adjoin_outer_approximation_recursion(p, d, f, g, c, b, static_cast<Int>(e * p.dimension()), 0, procedures, diagnostics);
    // std::cerr<<"Computing outer approximation considered a total of "<<COUNT_TESTS<<" domains/cells\n";
    // std::cerr<<"Measure of paving is "<<p.measure()<<"\n";

    p.recombine();
}

} // namespace

Void ReducePaver::adjoin_outer_approximation(
    PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness) const {
    this->_adjoin_outer_approximation(paving, set, fineness, nullptr);
}

Void ReducePaver::adjoin_outer_approximation(
    PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness,
    PaverDiagnostics& diagnostics) const {
    diagnostics = {};
    this->_adjoin_outer_approximation(paving, set, fineness, &diagnostics);
    diagnostics.contractor_ns =
        diagnostics.box_reduction_ns + diagnostics.hull_reduction_ns;
}

Void ReducePaver::_adjoin_outer_approximation(
    PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness,
    PaverDiagnostics* diagnostics) const {
    ARIADNE_PRECONDITION(paving.dimension() == set.dimension());
    if (definitely(set.reduced_domain().is_empty()))
        return;
    // With no parameters there is no domain to reduce. Use interval enclosure.
    if (set.reduced_domain().dimension() == 0u) {
        return constraint_adjoin_outer_approximation(
            paving, set.reduced_domain(), set.function(),
            set.constraint_function(), set.constraint_bounds(), fineness, diagnostics);
    }
    return procedure_constraint_adjoin_outer_approximation(
        paving, set.reduced_domain(), set.function(),
        set.constraint_function(), set.constraint_bounds(), fineness, diagnostics);
}

Void ConstraintPaver::adjoin_outer_approximation(
    PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness) const {
    this->_adjoin_outer_approximation(paving, set, fineness, nullptr);
}

Void ConstraintPaver::adjoin_outer_approximation(
    PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness,
    PaverDiagnostics& diagnostics) const {
    diagnostics = {};
    this->_adjoin_outer_approximation(paving, set, fineness, &diagnostics);
}

Void ConstraintPaver::_adjoin_outer_approximation(
    PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness,
    PaverDiagnostics* diagnostics) const {
    ARIADNE_PRECONDITION(paving.dimension() == set.dimension());
    if (definitely(set.reduced_domain().is_empty()))
        return;
    return constraint_adjoin_outer_approximation(
        paving, set.reduced_domain(), set.function(), set.constraint_function(),
        set.constraint_bounds(), fineness, diagnostics);
}

} // namespace Ariadne
