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

#include "function/taylor_function.hpp"
#include "logging/logging.hpp"
#include "paving/binary_tree_paving.hpp"
#include "paving/paver.hpp"
#include "set/function_set.hpp"

#include "ariadne-testing.hpp"
using namespace Ariadne;

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
    ARIADNE_TESTING_ADD_TEST(TestPaver, test_nonlinear_pavers);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}
