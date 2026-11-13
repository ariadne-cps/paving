/***************************************************************************
 *            tests/test_paver.cpp
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

/** @file tests/test_paver.cpp
 *  @brief Test paving geometry and backend behavior.
 */

#include <sstream>
#include <vector>

#include "function/taylor_function.hpp"
#include "logging/logging.hpp"
#include "paving/binary_tree_paving.hpp"
#include "paving/paver.hpp"
#include "set/function_set.hpp"

#include "ariadne-testing.hpp"
using namespace Ariadne;

// Keep the generic geometric-set dispatch reachable as an independent
// reference for checking the specialized ExactBoxSetType fast path.
class ReferenceBoxSet : public virtual EffectiveEuclideanRegularLocatedSetInterface {
    ExactBoxType _box;

  public:
    explicit ReferenceBoxSet(const ExactBoxType& box) : _box(box) {}
    ReferenceBoxSet* clone() const override { return new ReferenceBoxSet(*this); }
    DimensionType dimension() const override { return _box.dimension(); }
    LowerKleenean separated(const ExactBoxType& other) const override {
        return _box.separated(other);
    }
    LowerKleenean overlaps(const ExactBoxType& other) const override {
        return _box.overlaps(other);
    }
    LowerKleenean covers(const ExactBoxType& other) const override {
        return _box.covers(other);
    }
    LowerKleenean inside(const ExactBoxType& other) const override {
        return _box.inside(other);
    }
    UpperBoxType bounding_box() const override { return _box.bounding_box(); }
    OutputStream& _write(OutputStream& os) const override { return os << _box; }
};

class TestPaver : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

    Void check_paver(const Paver& paver) {
        ARIADNE_TESTING_NOTE("paver = " << (paver));
        auto x = EffectiveScalarMultivariateFunction::coordinate(1u, 0u);
        ValidatedVectorMultivariateFunction identity({x});
        ExactBoxType domain{
            {0.125_x, 0.375_x}
        };
        ValidatedConstrainedImageSet set(domain, identity);
        Grid grid(1u);
        BinaryTreePaving approximation(grid);
        paver.adjoin_outer_approximation(approximation, set, 2u);
        ARIADNE_TESTING_PROVE(approximation.superset(GridCell(grid, 0u, BinaryWord("00"))));
        ARIADNE_TESTING_PROVE(approximation.superset(GridCell(grid, 0u, BinaryWord("01"))));
        ARIADNE_TESTING_PROVE(definitely(subset(cast_exact_box(approximation.bounding_box()), ExactBoxType{
                                                                                                  {-0.5_x, 0.75_x}
        })));
        BinaryTreePaving full(grid);
        full.adjoin(GridCell(grid, 0u, BinaryWord()));
        paver.adjoin_outer_approximation(full, set, 2u);
        ARIADNE_TESTING_PROVE(full.superset(GridCell(grid, 0u, BinaryWord())));
        ValidatedConstrainedImageSet constrained(domain, identity, List<ValidatedConstraint>{0 <= x <= 1});
        BinaryTreePaving constrained_result(grid);
        paver.adjoin_outer_approximation(constrained_result, constrained, 2u);
        ARIADNE_TESTING_PROVE(constrained_result.superset(GridCell(grid, 0u, BinaryWord("00"))));
        ValidatedConstrainedImageSet below(domain, identity, List<ValidatedConstraint>{x <= -1});
        ValidatedConstrainedImageSet above(domain, identity, List<ValidatedConstraint>{x >= 1});
        BinaryTreePaving excluded(grid);
        paver.adjoin_outer_approximation(excluded, below, 2u);
        paver.adjoin_outer_approximation(excluded, above, 2u);
        ARIADNE_TESTING_PROVE(excluded.is_empty());
        ValidatedConstrainedImageSet empty(ExactBoxType(1u), identity);
        paver.adjoin_outer_approximation(excluded, empty, 2u);
        ARIADNE_TESTING_PROVE(excluded.is_empty());
        BinaryTreePaving wrong_dimension(Grid(2u));
        ARIADNE_TESTING_EXPECT_EXCEPTION(paver.adjoin_outer_approximation(wrong_dimension, set, 2u));
        std::ostringstream repr;
        repr << paver;
        ARIADNE_TESTING_PROVE(repr.str().find("Paver()") != String::npos);
    }

    Void test_paver_strategies() {
        check_paver(Paver(AffinePaver()));
        check_paver(Paver(SubdivisionPaver()));
        check_paver(Paver(ReducePaver()));
        check_paver(Paver(ConstraintPaver()));
    }

    Void test_exact_box_fast_path() {
        // These boxes exercise interiors, grid boundaries, empty-interior
        // intervals, out-of-unit extents, repeated unions, and nesting.
        const ExactIntervalType intervals[] = {
            ExactIntervalType(0.25_x, 0.75_x),
            ExactIntervalType(0.5_x, 0.5_x),
            ExactIntervalType(-0.25_x, 1.25_x),
            ExactIntervalType(0.125_x, 0.375_x)};
        for (unsigned dimension = 1u; dimension <= 3u; ++dimension) {
            const Grid grid(dimension);
            BinaryTreePaving fast(grid), reference(grid);
            for (const ExactIntervalType& interval: intervals) {
                const ExactBoxType box(dimension, interval);
                const ExactBoxSetType exact(box);
                const ReferenceBoxSet proxy(box);
                for (unsigned fineness: {0u, 1u, 2u}) {
                    fast.adjoin_outer_approximation(
                        static_cast<const EffectiveEuclideanCompactSetInterface&>(exact),
                        fineness);
                    reference.adjoin_outer_approximation(
                        static_cast<const EffectiveEuclideanCompactSetInterface&>(proxy),
                        fineness);
                    for (auto it = fast.begin(), end = fast.end(); it != end; ++it)
                        ARIADNE_TESTING_PROVE(reference.superset(*it));
                    for (auto it = reference.begin(), end = reference.end();
                         it != end; ++it)
                        ARIADNE_TESTING_PROVE(fast.superset(*it));
                }
            }
        }
    }

    Void test_incremental_box_traversal_scaled_grid() {
        // The incremental lattice-box split must produce exactly the same
        // physical cells as re-evaluating a GridCell's full binary word.
        const Grid grid(
            Vector<ApproximateDouble>{1.0, -2.0},
            Vector<ApproximateDouble>{0.5, 2.0});
        const ExactBoxType boxes[] = {
            ExactBoxType{{0.875_x, 1.125_x},  {-3.0_x, 1.5_x}},
            ExactBoxType{    {1.0_x, 1.0_x}, {-2.0_x, -2.0_x}},
            ExactBoxType{ {-0.25_x, 2.25_x}, {-5.0_x, -1.0_x}},
            ExactBoxType{{1.125_x, 1.375_x},  {-1.5_x, 0.5_x}}
        };
        for (unsigned fineness: {0u, 1u, 2u, 3u}) {
            BinaryTreePaving fast(grid), reference(grid);
            for (const ExactBoxType& box: boxes) {
                const ExactBoxSetType exact(box);
                const ReferenceBoxSet proxy(box);
                fast.adjoin_outer_approximation(
                    static_cast<const EffectiveEuclideanCompactSetInterface&>(exact),
                    fineness);
                reference.adjoin_outer_approximation(
                    static_cast<const EffectiveEuclideanCompactSetInterface&>(proxy),
                    fineness);
                for (auto it = fast.begin(), end = fast.end(); it != end; ++it)
                    ARIADNE_TESTING_PROVE(reference.superset(*it));
                for (auto it = reference.begin(), end = reference.end();
                     it != end; ++it)
                    ARIADNE_TESTING_PROVE(fast.superset(*it));
            }
            // Already-enabled branches must still short-circuit.
            const GridCell occupied(grid, 0u, BinaryWord("0"));
            fast.adjoin(occupied);
            reference.adjoin(occupied);
            const ExactBoxSetType exact(boxes[0]);
            const ReferenceBoxSet proxy(boxes[0]);
            fast.adjoin_outer_approximation(
                static_cast<const EffectiveEuclideanCompactSetInterface&>(exact),
                fineness);
            reference.adjoin_outer_approximation(
                static_cast<const EffectiveEuclideanCompactSetInterface&>(proxy),
                fineness);
            for (auto it = fast.begin(), end = fast.end(); it != end; ++it)
                ARIADNE_TESTING_PROVE(reference.superset(*it));
            for (auto it = reference.begin(), end = reference.end();
                 it != end; ++it)
                ARIADNE_TESTING_PROVE(fast.superset(*it));
        }
    }

    Void test_incremental_physical_box_geometry() {
        // Compare every propagated node box against GridCell's independent
        // full-path reconstruction, including depth-dependent split axes,
        // shifted origins, and nonuniform grid scales.
        const std::vector<Grid> grids{
            Grid(1u), Grid(2u), Grid(3u),
            Grid(Vector<ApproximateDouble>{1.0, -2.0},
                 Vector<ApproximateDouble>{0.5, 2.0}),
            Grid(Vector<ApproximateDouble>{0.125, -1.5, 4.0},
                 Vector<ApproximateDouble>{0.25, 0.75, 2.0})};
        struct CellGeometry {
            BinaryWord word;
            LatticeBoxType lattice;
            ExactBoxType physical;
        };
        for (const Grid& grid: grids) {
            const Vector<FloatDP> origin(grid.origin());
            const Vector<FloatDP> lengths(grid.lengths());
            for (Nat extent: {0u, 1u, 2u}) {
                const auto root_lattice = GridAbstractCell::primary_cell_lattice_box(
                    extent, static_cast<dimension_type>(grid.dimension()));
                const auto root_physical =
                    GridAbstractCell::lattice_box_to_space(root_lattice, grid);
                std::vector<CellGeometry> frontier{
                    {BinaryWord{}, root_lattice, root_physical}
                };
                for (Nat depth = 0u; depth <= 9u; ++depth) {
                    std::vector<CellGeometry> next;
                    if (depth != 9u)
                        next.reserve(frontier.size() * 2u);
                    for (const CellGeometry& candidate: frontier) {
                        const ExactBoxType expected =
                            GridCell(grid, extent, candidate.word).box();
                        ARIADNE_TESTING_PROVE(same(candidate.physical, expected));
                        if (depth == 9u)
                            continue;
                        const SizeType axis = depth % grid.dimension();
                        const auto lattice_midpoint =
                            cast_exact(candidate.lattice[axis].midpoint());
                        const auto physical_midpoint = cast_exact(
                            add(approx, origin[axis],
                                mul(approx, lengths[axis], lattice_midpoint)));

                        CellGeometry left(candidate);
                        left.word.push_back(false);
                        left.lattice[axis].set_upper_bound(lattice_midpoint);
                        left.physical[axis].set_upper_bound(physical_midpoint);
                        next.push_back(std::move(left));

                        CellGeometry right(candidate);
                        right.word.push_back(true);
                        right.lattice[axis].set_lower_bound(lattice_midpoint);
                        right.physical[axis].set_lower_bound(physical_midpoint);
                        next.push_back(std::move(right));
                    }
                    frontier = std::move(next);
                }
            }
        }
    }

    Void test_inplace_box_geometry_restoration() {
        // Check every in-place update against the full GridCell path, then
        // check that both axes' intervals are restored after both children.
        const std::vector<Grid> grids{
            Grid(1u), Grid(2u), Grid(3u),
            Grid(Vector<ApproximateDouble>{1.0, -2.0},
                 Vector<ApproximateDouble>{0.5, 2.0}),
            Grid(Vector<ApproximateDouble>{0.125, -1.5, 4.0},
                 Vector<ApproximateDouble>{0.25, 0.75, 2.0})};
        for (const Grid& grid: grids) {
            const Vector<FloatDP> origin(grid.origin());
            const Vector<FloatDP> lengths(grid.lengths());
            for (Nat extent: {0u, 1u, 2u}) {
                LatticeBoxType lattice =
                    GridAbstractCell::primary_cell_lattice_box(
                        extent, static_cast<dimension_type>(grid.dimension()));
                ExactBoxType physical =
                    GridAbstractCell::lattice_box_to_space(lattice, grid);
                BinaryWord path;
                auto visit = [&](auto&& self, Nat depth) -> Void {
                    const auto full_lattice = GridCell::compute_lattice_box(
                        grid.dimension(), extent, path);
                    const auto full_physical = GridCell(grid, extent, path).box();
                    ARIADNE_TESTING_PROVE(same(lattice, full_lattice));
                    ARIADNE_TESTING_PROVE(same(physical, full_physical));
                    if (depth == 8u)
                        return;

                    const SizeType axis = depth % grid.dimension();
                    const ExactIntervalType saved_lattice(lattice[axis]);
                    const ExactIntervalType saved_physical(physical[axis]);
                    const auto split = cast_exact(lattice[axis].midpoint());
                    const auto physical_split = cast_exact(
                        add(approx, origin[axis],
                            mul(approx, lengths[axis], split)));

                    lattice[axis].set_upper_bound(split);
                    physical[axis].set_upper_bound(physical_split);
                    path.push_back(false);
                    self(self, depth + 1u);
                    path.pop_back();

                    lattice[axis] = saved_lattice;
                    physical[axis] = saved_physical;
                    lattice[axis].set_lower_bound(split);
                    physical[axis].set_lower_bound(physical_split);
                    path.push_back(true);
                    self(self, depth + 1u);
                    path.pop_back();

                    lattice[axis] = saved_lattice;
                    physical[axis] = saved_physical;
                    ARIADNE_TESTING_PROVE(same(lattice, full_lattice));
                    ARIADNE_TESTING_PROVE(same(physical, full_physical));
                };
                visit(visit, 0u);
            }
        }
    }

    Void test_incremental_exact_box_classification() {
        // A non-separated parent can only become separated along its
        // split axis. Compare this shortcut and the incrementally updated
        // strict-coverage count with the complete box predicates.
        const std::vector<Grid> grids{
            Grid(1u), Grid(2u), Grid(3u),
            Grid(Vector<ApproximateDouble>{1.0, -2.0},
                 Vector<ApproximateDouble>{0.5, 2.0})};
        const ExactIntervalType intervals[] = {
            ExactIntervalType(0.25_x, 0.75_x),
            ExactIntervalType(0.5_x, 0.5_x),
            ExactIntervalType(-0.25_x, 1.25_x),
            ExactIntervalType(0.125_x, 0.375_x)};
        for (const Grid& grid: grids) {
            const Vector<FloatDP> origin(grid.origin());
            const Vector<FloatDP> lengths(grid.lengths());
            std::vector<ExactBoxType> boxes;
            for (const ExactIntervalType& interval: intervals)
                boxes.emplace_back(grid.dimension(), interval);
            ExactBoxType mixed(grid.dimension(), intervals[0]);
            for (SizeType i = 0u; i != grid.dimension(); ++i)
                mixed[i] = intervals[i % 4u];
            boxes.push_back(mixed);

            for (const ExactBoxType& box: boxes) {
                for (Nat extent: {0u, 1u, 2u}) {
                    LatticeBoxType lattice =
                        GridAbstractCell::primary_cell_lattice_box(
                            extent, static_cast<dimension_type>(grid.dimension()));
                    ExactBoxType physical =
                        GridAbstractCell::lattice_box_to_space(lattice, grid);
                    Nat uncovered_axes = 0u;
                    for (SizeType i = 0u; i != grid.dimension(); ++i)
                        uncovered_axes += static_cast<Nat>(
                            !definitely(Ariadne::covers(box[i], physical[i])));

                    auto visit = [&](auto&& self, Nat depth, Bool is_separated,
                                     Nat uncovered_count) -> Void {
                        ARIADNE_TESTING_PROVE(
                            is_separated == definitely(box.separated(physical)));
                        ARIADNE_TESTING_PROVE(
                            (uncovered_count == 0u) == definitely(box.covers(physical)));
                        // The actual algorithm never visits descendants of
                        // a node already separated from the requested box.
                        if (depth == 7u || is_separated)
                            return;

                        const SizeType axis = depth % grid.dimension();
                        const Bool was_uncovered =
                            !definitely(Ariadne::covers(box[axis], physical[axis]));
                        const Nat other_uncovered =
                            uncovered_count - static_cast<Nat>(was_uncovered);
                        const ExactIntervalType saved_lattice(lattice[axis]);
                        const ExactIntervalType saved_physical(physical[axis]);
                        const auto split_value = cast_exact(lattice[axis].midpoint());
                        const auto physical_split = cast_exact(
                            add(approx, origin[axis],
                                mul(approx, lengths[axis], split_value)));
                        const Bool expected_left_separated = definitely(
                            box[axis].lower_bound() > physical_split);
                        const Bool expected_right_separated = definitely(
                            box[axis].upper_bound() < physical_split);

                        lattice[axis].set_upper_bound(split_value);
                        physical[axis].set_upper_bound(physical_split);
                        ARIADNE_TESTING_PROVE(
                            expected_left_separated ==
                            definitely(Ariadne::separated(box[axis], physical[axis])));
                        self(self, depth + 1u,
                             expected_left_separated,
                             other_uncovered + static_cast<Nat>(
                                                   !definitely(Ariadne::covers(box[axis], physical[axis]))));

                        lattice[axis] = saved_lattice;
                        physical[axis] = saved_physical;
                        lattice[axis].set_lower_bound(split_value);
                        physical[axis].set_lower_bound(physical_split);
                        ARIADNE_TESTING_PROVE(
                            expected_right_separated ==
                            definitely(Ariadne::separated(box[axis], physical[axis])));
                        self(self, depth + 1u,
                             expected_right_separated,
                             other_uncovered + static_cast<Nat>(
                                                   !definitely(Ariadne::covers(box[axis], physical[axis]))));

                        lattice[axis] = saved_lattice;
                        physical[axis] = saved_physical;
                    };
                    visit(visit, 0u,
                          definitely(box.separated(physical)), uncovered_axes);
                }
            }
        }
    }

    Void test_pruned_exact_box_subtrees() {
        // The fast path must match generic insertion even with pre-enabled
        // sibling branches and boxes touching subdivision boundaries.
        const std::vector<Grid> grids{
            Grid(2u),
            Grid(4u),
            Grid(Vector<ApproximateDouble>{1.0, -2.0},
                 Vector<ApproximateDouble>{0.5, 2.0})};
        const ExactIntervalType intervals[] = {
            ExactIntervalType(0.25_x, 0.75_x),
            ExactIntervalType(0.5_x, 0.5_x),
            ExactIntervalType(-0.25_x, 1.25_x),
            ExactIntervalType(0.0_x, 0.0_x)};
        for (const Grid& grid: grids) {
            for (Nat fineness: {0u, 1u, 2u}) {
                BinaryTreePaving fast(grid), reference(grid);
                // Enable both an entire sibling branch and a nested one.
                const GridCell occupied_a(grid, 0u, BinaryWord("0"));
                const GridCell occupied_b(grid, 0u, BinaryWord("101"));
                fast.adjoin(occupied_a);
                fast.adjoin(occupied_b);
                reference.adjoin(occupied_a);
                reference.adjoin(occupied_b);
                for (const ExactIntervalType& interval: intervals) {
                    const ExactBoxType box(grid.dimension(), interval);
                    const ExactBoxSetType exact(box);
                    const ReferenceBoxSet proxy(box);
                    for (unsigned repetition = 0u; repetition != 2u; ++repetition) {
                        fast.adjoin_outer_approximation(
                            static_cast<const EffectiveEuclideanCompactSetInterface&>(
                                exact),
                            fineness);
                        reference.adjoin_outer_approximation(
                            static_cast<const EffectiveEuclideanCompactSetInterface&>(
                                proxy),
                            fineness);
                        for (auto it = fast.begin(), end = fast.end(); it != end; ++it)
                            ARIADNE_TESTING_PROVE(reference.superset(*it));
                        for (auto it = reference.begin(), end = reference.end();
                             it != end; ++it)
                            ARIADNE_TESTING_PROVE(fast.superset(*it));
                    }
                }
            }
        }
    }

    Void test_instrumented_pavers() {
        auto x = EffectiveScalarMultivariateFunction::coordinate(1u, 0u);
        ValidatedVectorMultivariateFunction identity({x});
        ValidatedConstrainedImageSet set(
            ExactBoxType{
                {0.125_x, 0.375_x}
        },
            identity);
        Grid grid(1u);

        auto check_geometry = [&](const BinaryTreePaving& expected,
                                  const BinaryTreePaving& actual) {
            for (auto it = expected.begin(), end = expected.end(); it != end; ++it)
                ARIADNE_TESTING_PROVE(actual.superset(*it));
            for (auto it = actual.begin(), end = actual.end(); it != end; ++it)
                ARIADNE_TESTING_PROVE(expected.superset(*it));
        };
        auto check_counters = [&](const PaverDiagnostics& counters) {
            ARIADNE_TESTING_PROVE(counters.visited_nodes > 0u);
            ARIADNE_TESTING_PROVE(counters.map_evaluations > 0u);
            ARIADNE_TESTING_PROVE(counters.adjoin_calls > 0u);
        };

        {
            BinaryTreePaving plain(grid), instrumented(grid);
            SubdivisionPaver strategy;
            strategy.adjoin_outer_approximation(plain, set, 2u);
            PaverDiagnostics counters;
            strategy.adjoin_outer_approximation(instrumented, set, 2u, counters);
            check_geometry(plain, instrumented);
            check_counters(counters);
            ARIADNE_TESTING_EXPECT_EQUAL(counters.constraint_evaluations, 0u);
            ARIADNE_TESTING_EXPECT_EQUAL(counters.constraint_reduction_calls, 0u);
            ARIADNE_TESTING_EXPECT_EQUAL(
                counters.visited_nodes, 2u * counters.parameter_splits + 1u);
        }
        {
            BinaryTreePaving plain(grid), instrumented(grid);
            ReducePaver strategy;
            strategy.adjoin_outer_approximation(plain, set, 2u);
            PaverDiagnostics counters;
            strategy.adjoin_outer_approximation(instrumented, set, 2u, counters);
            check_geometry(plain, instrumented);
            check_counters(counters);
            ARIADNE_TESTING_PROVE(counters.reduction_calls > 0u);
            ARIADNE_TESTING_EXPECT_EQUAL(
                counters.reduction_calls,
                counters.box_reduction_calls + counters.hull_reduction_calls);
            ARIADNE_TESTING_EXPECT_EQUAL(
                counters.contractor_ns,
                counters.box_reduction_ns + counters.hull_reduction_ns);
            ARIADNE_TESTING_EXPECT_EQUAL(counters.constraint_evaluations, 0u);
            ARIADNE_TESTING_EXPECT_EQUAL(counters.constraint_reduction_calls, 0u);
        }
        {
            BinaryTreePaving plain(grid), instrumented(grid);
            ConstraintPaver strategy;
            strategy.adjoin_outer_approximation(plain, set, 2u);
            PaverDiagnostics counters;
            strategy.adjoin_outer_approximation(instrumented, set, 2u, counters);
            check_geometry(plain, instrumented);
            check_counters(counters);
            ARIADNE_TESTING_EXPECT_EQUAL(counters.reduction_calls, 0u);
            ARIADNE_TESTING_EXPECT_EQUAL(counters.join_constructions, 1u);
            ARIADNE_TESTING_EXPECT_EQUAL(counters.constraint_evaluations, 0u);
            ARIADNE_TESTING_EXPECT_EQUAL(counters.constraint_reduction_calls, 0u);
        }
        {
            // Real constraints are counted separately from calls to the
            // box/hull contractors that process those constraints.
            const ValidatedConstrainedImageSet constrained(
                ExactBoxType{
                    {0.0_x, 1.0_x}
            },
                identity,
                List<ValidatedConstraint>{x >= 0.125_x, x <= 0.375_x});
            BinaryTreePaving reduction(grid), interval(grid);
            PaverDiagnostics reduced, interval_only;
            ReducePaver().adjoin_outer_approximation(
                reduction, constrained, 2u, reduced);
            ConstraintPaver().adjoin_outer_approximation(
                interval, constrained, 2u, interval_only);
            ARIADNE_TESTING_PROVE(reduced.constraint_reduction_calls > 0u);
            ARIADNE_TESTING_PROVE(reduced.constraint_evaluations > 0u);
            ARIADNE_TESTING_PROVE(interval_only.constraint_evaluations > 0u);
            ARIADNE_TESTING_EXPECT_EQUAL(interval_only.constraint_reduction_calls, 0u);
            ARIADNE_TESTING_EXPECT_EQUAL(interval_only.join_constructions, 1u);
            ARIADNE_TESTING_PROVE(
                reduced.constraint_reduction_calls <= reduced.reduction_calls);
        }
        {
            PaverDiagnostics counters;
            counters.visited_nodes = 42u;
            BinaryTreePaving empty(grid);
            SubdivisionPaver().adjoin_outer_approximation(
                empty, ValidatedConstrainedImageSet(ExactBoxType(1u), identity),
                2u, counters);
            ARIADNE_TESTING_PROVE(empty.is_empty());
            ARIADNE_TESTING_EXPECT_EQUAL(counters.visited_nodes, 1u);
            ARIADNE_TESTING_EXPECT_EQUAL(counters.adjoin_calls, 0u);
        }
    }

    Void test_nonlinear_pavers() {
        auto x = EffectiveScalarMultivariateFunction::coordinate(1u, 0u);
        ExactBoxType square{
            {-1.0_x, 1.0_x},
            {-1.0_x, 1.0_x}
        };
        auto u = EffectiveScalarMultivariateFunction::coordinate(2u, 0u);
        auto v = EffectiveScalarMultivariateFunction::coordinate(2u, 1u);
        ValidatedVectorMultivariateFunction nonlinear({u * u, v * v});
        ValidatedVectorMultivariateTaylorFunctionModelDP model(square, nonlinear, TrivialSweeper<FloatDP>(dp));
        BinaryTreePaving model_paving(Grid(2u));
        AffinePaver().adjoin_outer_approximation(model_paving, ValidatedConstrainedImageSet(square, ValidatedVectorMultivariateFunctionModelDP(model)), 0u);
        ARIADNE_TESTING_PROVE(!model_paving.is_empty());
        ValidatedVectorMultivariateFunction quadratic({x * x});
        ValidatedConstrainedImageSet set(ExactBoxType{
                                             {-1.0_x, 1.0_x}
        },
                                         quadratic);
        Grid grid(1u);
        const Paver strategies[] = {Paver(AffinePaver()), Paver(SubdivisionPaver()), Paver(ReducePaver()), Paver(ConstraintPaver())};
        for (const auto& paver: strategies) {
            ARIADNE_TESTING_NOTE("paver = " << (paver));
            BinaryTreePaving approximation(grid);
            paver.adjoin_outer_approximation(approximation, set, 1u);
            const ExactBoxType bounds = cast_exact_box(approximation.bounding_box());
            ARIADNE_TESTING_PROVE(bounds[0].lower_bound() <= FloatDP(0, dp));
            ARIADNE_TESTING_PROVE(bounds[0].upper_bound() >= FloatDP(1, dp));
            ARIADNE_TESTING_PROVE(definitely(subset(bounds, ExactBoxType{
                                                                {-2.0_x, 2.0_x}
            })));
        }
    }

    Void test_intrinsic_uncertainty() {
        const Paver strategies[] = {Paver(AffinePaver()), Paver(SubdivisionPaver()), Paver(ReducePaver()), Paver(ConstraintPaver())};
        for (SizeType arity = 0u; arity != 2u; ++arity) {
            auto constant = ValidatedScalarMultivariateFunction::constant(arity, ValidatedNumber(FloatDPBounds(0.25_x, 0.75_x, dp)));
            ValidatedVectorMultivariateFunction mapping({constant});
            ValidatedConstrainedImageSet set(ExactBoxType(arity, ExactIntervalType(-1, 1)), mapping);
            for (const auto& paver: strategies) {
                ARIADNE_TESTING_NOTE("arity = " << (arity));
                ARIADNE_TESTING_NOTE("paver = " << (paver));
                BinaryTreePaving approximation(Grid(1u));
                paver.adjoin_outer_approximation(approximation, set, 3u);
                const auto bounds = cast_exact_box(approximation.bounding_box());
                ARIADNE_TESTING_PROVE(bounds[0].lower_bound() <= FloatDP(0.25_x, dp));
                ARIADNE_TESTING_PROVE(bounds[0].upper_bound() >= FloatDP(0.75_x, dp));
                ARIADNE_TESTING_PROVE(definitely(subset(bounds, ExactBoxType{
                                                                    {0.0_x, 1.0_x}
                })));
            }
        }
        auto identity = ValidatedVectorMultivariateFunction::identity(1u);
        ValidatedConstrainedImageSet set(ExactBoxType{
                                             {0.125_x, 0.375_x}
        },
                                         identity);
        BinaryTreePaving paving(Grid(1u));
        SubdivisionPaver subdivision;
        ARIADNE_TESTING_EXPECT_EXCEPTION(subdivision.adjoin_outer_approximation_recursion(paving, set, 2u, Vector<FloatDP>(0u, dp)));
        ARIADNE_TESTING_EXPECT_EXCEPTION(subdivision.adjoin_outer_approximation_recursion(paving, set, 2u, Vector<FloatDP>(1u, FloatDP(0, dp))));
    }

    Void test_affine_depth_limit() {
        auto x = EffectiveScalarMultivariateFunction::coordinate(1u, 0u);
        ValidatedVectorMultivariateFunction mapping({10000000000.0_x * x * x});
        ValidatedConstrainedImageSet set(ExactBoxType{
                                             {-1.0_x, 1.0_x}
        },
                                         mapping);
        BinaryTreePaving approximation(Grid(1u, FloatDP(1000000000000.0_x, dp)));
        AffinePaverDiagnostics diagnostics;
        AffinePaver().adjoin_outer_approximation(approximation, set, 0u, diagnostics);
        ARIADNE_TESTING_PROVE(diagnostics.depth_limit_reached());
        ARIADNE_TESTING_PROVE(diagnostics.unresolved_domains > 0u);
        ARIADNE_TESTING_PROVE(diagnostics.split_domains > 0u);
        ARIADNE_TESTING_EXPECT_EQUAL(diagnostics.completed_levels, diagnostics.depth_limit);
        auto bounds = cast_exact_box(approximation.bounding_box());
        ARIADNE_TESTING_PROVE(bounds[0].lower_bound() <= FloatDP(0, dp));
        ARIADNE_TESTING_PROVE(bounds[0].upper_bound() >= FloatDP(10000000000.0_x, dp));
        ARIADNE_TESTING_PROVE(definitely(subset(bounds, ExactBoxType{
                                                            {-2000000000000.0_x, 2000000000000.0_x}
        })));
    }

    Void test_correlated_constraints();

    Void test_reduce_contractors() {
        auto& configuration = Logger::instance().configuration();
        const auto old_verbosity = configuration.verbosity();
        struct RestoreVerbosity {
            unsigned int value;
            ~RestoreVerbosity() { Logger::instance().configuration().set_verbosity(value); }
        } restore{old_verbosity};
        configuration.set_verbosity(32u);
        auto x = EffectiveScalarMultivariateFunction::coordinate(1u, 0u);
        ExactBoxType interval{
            {0.0_x, 1.0_x}
        };
        ValidatedVectorMultivariateFunction identity({x});
        ValidatedVectorMultivariateFunction quadratic({x * (1 - x)});
        BinaryTreePaving empty(Grid(1u));
        ReducePaver().adjoin_outer_approximation(empty, ValidatedConstrainedImageSet(interval, identity, List<ValidatedConstraint>{x * (1 - x) >= 0.5_x}), 2u);
        ARIADNE_TESTING_PROVE(empty.is_empty());
        BinaryTreePaving quadratic_paving(Grid(1u));
        ReducePaver().adjoin_outer_approximation(quadratic_paving, ValidatedConstrainedImageSet(interval, quadratic), 2u);
        const auto quadratic_bounds = cast_exact_box(quadratic_paving.bounding_box());
        ARIADNE_TESTING_PROVE(quadratic_bounds[0].lower_bound() <= FloatDP(0, dp));
        ARIADNE_TESTING_PROVE(quadratic_bounds[0].upper_bound() >= FloatDP(0.25_x, dp));
        ARIADNE_TESTING_PROVE(definitely(subset(quadratic_bounds, ExactBoxType{
                                                                      {-0.5_x, 1.0_x}
        })));
        auto u = EffectiveScalarMultivariateFunction::coordinate(2u, 0u);
        auto v = EffectiveScalarMultivariateFunction::coordinate(2u, 1u);
        ValidatedVectorMultivariateFunction circle({u * u + v * v});
        BinaryTreePaving circle_paving(Grid(1u));
        ReducePaver().adjoin_outer_approximation(circle_paving, ValidatedConstrainedImageSet(ExactBoxType{
                                                                                                 {-1.0_x, 1.0_x},
                                                                                                 {-1.0_x, 1.0_x}
        },
                                                                                             circle),
                                                 2u);
        auto circle_bounds = cast_exact_box(circle_paving.bounding_box());
        ARIADNE_TESTING_PROVE(circle_bounds[0].lower_bound() <= FloatDP(0, dp));
        ARIADNE_TESTING_PROVE(circle_bounds[0].upper_bound() >= FloatDP(2, dp));
        ValidatedVectorMultivariateFunction bilinear({u * v, u + v});
        ValidatedConstrainedImageSet triangle(ExactBoxType{
                                                  {0.0_x, 1.0_x},
                                                  {0.0_x, 1.0_x}
        },
                                              bilinear, List<ValidatedConstraint>{u - v >= 0.75_x});
        BinaryTreePaving triangle_paving(Grid(2u));
        ReducePaver().adjoin_outer_approximation(triangle_paving, triangle, 2u);
        ARIADNE_TESTING_PROVE(definitely(subset(ExactBoxType{
                                                    {0.0_x, 0.0_x},
                                                    {1.0_x, 1.0_x}
        },
                                                cast_exact_box(triangle_paving.bounding_box()))));
        BinaryTreePaving zero_output(Grid(0u));
        ReducePaver().adjoin_outer_approximation(zero_output, ValidatedConstrainedImageSet(interval, ValidatedVectorMultivariateFunction::zeros(0u, 1u)), 0u);
        ARIADNE_TESTING_PROVE(!zero_output.is_empty());
        ARIADNE_TESTING_EXPECT_EQUAL(zero_output.dimension(), 0u);
        check_paver(Paver(ReducePaver()));
        test_correlated_constraints();
    }
};

Void TestPaver::test_correlated_constraints() {
    auto u = EffectiveScalarMultivariateFunction::coordinate(2u, 0u);
    auto v = EffectiveScalarMultivariateFunction::coordinate(2u, 1u);
    const EffectiveScalarMultivariateFunction mappings[] = {u * v, sqr(u) + sqr(v), u * v};
    const EffectiveConstraint conditions[] = {sqr(u - v) >= 0.75_x, u * u + v * v <= 0.5_x, u * v <= 0.0625_x};
    const ExactDouble attained_values[] = {0.125_x, 0.5_x, 0.0625_x};
    for (SizeType i = 0; i != 3u; ++i) {
        ARIADNE_TESTING_NOTE("i = " << (i));
        ValidatedVectorMultivariateFunction mapping({mappings[i]});
        ValidatedConstrainedImageSet set(ExactBoxType{
                                             {0.0_x, 1.0_x},
                                             {0.0_x, 1.0_x}
        },
                                         mapping, List<ValidatedConstraint>{conditions[i]});
        BinaryTreePaving paving(Grid(1u));
        ReducePaver().adjoin_outer_approximation(paving, set, 2u);
        const auto bounds = cast_exact_box(paving.bounding_box());
        ARIADNE_TESTING_PROVE(bounds[0].lower_bound() <= FloatDP(0, dp));
        ARIADNE_TESTING_PROVE(bounds[0].upper_bound() >= FloatDP(attained_values[i], dp));
        ARIADNE_TESTING_PROVE(definitely(subset(bounds, ExactBoxType{
                                                            {-0.5_x, 1.0_x}
        })));
    }
}

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_paver");
    ARIADNE_TESTING_ADD_TEST(TestPaver, test_correlated_constraints);
    ARIADNE_TESTING_ADD_TEST(TestPaver, test_affine_depth_limit);
    ARIADNE_TESTING_ADD_TEST(TestPaver, test_reduce_contractors);
    ARIADNE_TESTING_ADD_TEST(TestPaver, test_intrinsic_uncertainty);
    ARIADNE_TESTING_ADD_TEST(TestPaver, test_paver_strategies);
    ARIADNE_TESTING_ADD_TEST(TestPaver, test_instrumented_pavers);
    ARIADNE_TESTING_ADD_TEST(TestPaver, test_exact_box_fast_path);
    ARIADNE_TESTING_ADD_TEST(TestPaver, test_incremental_box_traversal_scaled_grid);
    ARIADNE_TESTING_ADD_TEST(TestPaver, test_incremental_physical_box_geometry);
    ARIADNE_TESTING_ADD_TEST(TestPaver, test_inplace_box_geometry_restoration);
    ARIADNE_TESTING_ADD_TEST(TestPaver, test_incremental_exact_box_classification);
    ARIADNE_TESTING_ADD_TEST(TestPaver, test_pruned_exact_box_subtrees);
    ARIADNE_TESTING_ADD_TEST(TestPaver, test_nonlinear_pavers);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}
