/***************************************************************************
 *            tests/test_affine_set_approximation.cpp
 *
 *  Copyright  2009-20  Pieter Collins
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

/** @file tests/test_affine_set_approximation.cpp
 *  @brief Test paving functionality and backend invariants.
 */

#include <chrono>
#include <iomanip>
#include <iostream>

#include "function/function.hpp"
#include "geometry/box.hpp"
#include "paving/binary_tree_paving.hpp"
#include "paving/set_approximation.hpp"
#include "set/affine_set.hpp"
#include "set/function_set.hpp"

#include "ariadne-testing.hpp"

using namespace Ariadne;
using namespace std;

namespace Ariadne {

template<class X>
AffineConstraint<X> operator<=(const Dyadic& l, const Affine<X>& a) {
    return AffineConstraint<X>({l, a.value().precision()}, a, {+infty, a.value().precision()});
}
template<class X>
AffineConstraint<X> operator<=(const Affine<X>& a, const Dyadic& u) {
    return AffineConstraint<X>(-infty, a, u);
}
template<class X>
AffineConstraint<X> operator<=(const AffineConstraint<X>& ac, const Dyadic& u) {
    ARIADNE_ASSERT(decide(ac.upper_bound() == infty));
    return AffineConstraint<X>(ac.lower_bound(), ac.function(), {u, ac.lower_bound().precision()});
}
template<class X>
AffineConstraint<X> operator==(const Affine<X>& a, const Dyadic& u) {
    return AffineConstraint<X>(u, a, u);
}
} // namespace Ariadne

class TestAffineSetPaving : public Ariadne::Testing::ContextFixture {
  private:
    ExactBoxType D;
    Vector<Affine<FloatDPBounds>> x;
    ValidatedAffineConstrainedImageSet set;

  public:
    explicit TestAffineSetPaving(Ariadne::Testing::Context& context) : ContextFixture(context),
                                                                       set(ExactBoxType::unit_box(2), Matrix<FloatDP>(2, 2, dp), Vector<FloatDP>(2, dp)) {}

    Void test_robust_outer_approximation() {
        D = ExactBoxType::unit_box(2);
        x = Affine<FloatDPBounds>::variables(2, dp);
        set = ValidatedAffineConstrainedImageSet(D, {x[0], x[1]});
        set.new_parameter_constraint(x[0] + x[1] <= 0.5_x);

        BinaryTreePaving paving(Grid(2));
        robust_adjoin_outer_approximation_to(paving, set, 0);
        ARIADNE_TESTING_PROVE(paving.size() > 0);

        // Repeating the ordinary approximation must recognise cells already
        // contained in the paving and leave the result unchanged.
        SizeType size = paving.size();
        adjoin_outer_approximation_to(paving, set, 0);
        ARIADNE_TESTING_EXPECT_EQUAL(paving.size(), size);

        ValidatedAffineConstrainedImageSet empty_set(D, {x[0], x[1]});
        empty_set.new_parameter_constraint(x[0] <= -2.0_x);
        BinaryTreePaving empty_paving(Grid(2));
        robust_adjoin_outer_approximation_to(empty_paving, empty_set, 0);
        ARIADNE_TESTING_EXPECT_EQUAL(empty_paving.size(), 0u);

        ValidatedAffineConstrainedImageSet infeasible_set(D, {x[0], x[1]});
        infeasible_set.new_parameter_constraint(x[0] <= -2.0_x);
        infeasible_set.new_parameter_constraint(-x[0] <= -2.0_x);
        BinaryTreePaving infeasible_paving(Grid(2));
        robust_adjoin_outer_approximation_to(infeasible_paving, infeasible_set, 0);
        ARIADNE_TESTING_EXPECT_EQUAL(infeasible_paving.size(), 0u);

        ExactBoxType small_box({
            {0.25_x, 0.75_x},
            {0.25_x, 0.75_x}
        });
        ValidatedAffineConstrainedImageSet small_set(small_box);
        BinaryTreePaving covered_paving(Grid(2));
        robust_adjoin_outer_approximation_to(covered_paving, small_set, 0);
        ARIADNE_TESTING_PROVE(covered_paving.size() > 0);
        SizeType covered_size = covered_paving.size();
        robust_adjoin_outer_approximation_to(covered_paving, small_set, 0);
        ARIADNE_TESTING_EXPECT_EQUAL(covered_paving.size(), covered_size);
    }

    Void test_outer_approximation() {
        {
            D = ExactBoxType::unit_box(2);
            x = Affine<FloatDPBounds>::variables(2, dp);
            set = ValidatedAffineConstrainedImageSet(D, {x[0], x[1]});
            set.new_parameter_constraint(x[0] + x[1] <= -0.5_x);
            ARIADNE_TESTING_NOTE("set = " << (set));

            const auto start1 = std::chrono::steady_clock::now();
            BinaryTreePaving paving = outer_approximation(set, Grid(2), 4);
            const auto elapsed_ms1 = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start1).count();
            ARIADNE_TESTING_NOTE("test_outer_approximation case 1: " << elapsed_ms1 << " ms");
            paving.recombine();
            // Apply the original precision to each testing diagnostic stream.
            ARIADNE_TESTING_NOTE(std::setprecision(17) << "paving = " << (paving));

            ARIADNE_TESTING_PROVE(paving.size() > 0);
        }

        {
            // The following set has difficulties
            set = ValidatedAffineConstrainedImageSet(D, {x[0], x[1]});
            set.new_parameter_constraint(2.0_x * x[0] + 1.0_x * x[1] <= 0.5_x);
            set.new_parameter_constraint(-0.5_x * x[0] + 1.0_x * x[1] <= 0.75_x);
            set.new_parameter_constraint(-0.5_x * x[0] - 1.0_x * x[1] <= 0.875_x);

            const auto start2 = std::chrono::steady_clock::now();
            BinaryTreePaving paving = outer_approximation(set, Grid(2), 3);
            const auto elapsed_ms2 = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start2).count();
            ARIADNE_TESTING_NOTE("test_outer_approximation case 2: " << elapsed_ms2 << " ms");
            ARIADNE_TESTING_PROVE(paving.size() > 0);
        }

        {
            D = ExactBoxType::unit_box(3);
            x = Affine<FloatDPBounds>::variables(3, dp);

            set = ValidatedAffineConstrainedImageSet(D, {2.0_pr * x[0] + 3.0_pr * x[1] + 1.0_pr * x[2] + 0.05_pr, x[0] + x[1] + 2.051_pr});

            set.new_parameter_constraint(+1.01_pr * x[0] + 0.51_pr * x[1] + 0.251_pr * x[2] <= 0.751_pr);
            set.new_parameter_constraint(-0.51_pr * x[0] + 1.51_pr * x[1] + 0.01_pr * x[2] <= 0.51_pr);
            set.new_parameter_constraint(-2.01_pr * x[0] - 3.01_pr * x[1] - 1.02_pr * x[2] <= 3.01_pr);

            Grid grid(2);
            BinaryTreePaving paving(grid);
            const auto start3 = std::chrono::steady_clock::now();
            adjoin_outer_approximation_to(paving, set, 3);
            const auto elapsed_ms3 = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start3).count();
            ARIADNE_TESTING_NOTE("test_outer_approximation case 3: " << elapsed_ms3 << " ms");
            paving.recombine();

            ARIADNE_TESTING_PROVE(paving.size() > 0);
        }

        {
            set = ValidatedAffineConstrainedImageSet(ExactBoxType::unit_box(3), {-0.9375_x + 0.0625_x * x[0] + 0.5_x * x[1], 0.87890625_x - 0.1171875_x * x[0] + x[1] + 0.00390625_x * x[2]});
            set.new_parameter_constraint(0.0625_x * x[0] + x[1] <= 1.1875_x);
            ARIADNE_TESTING_NOTE("set = " << (set));

            const auto start4 = std::chrono::steady_clock::now();
            BinaryTreePaving paving = outer_approximation(set, Grid(2), 4);
            const auto elapsed_ms4 = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start4).count();
            ARIADNE_TESTING_NOTE("test_outer_approximation case 4: " << elapsed_ms4 << " ms");
            paving.recombine();
            // Apply the original precision to each testing diagnostic stream.
            ARIADNE_TESTING_NOTE(std::setprecision(17) << "paving = " << (paving));

            ARIADNE_TESTING_PROVE(paving.size() > 0);
        }
    }

    Void test_uniform_error() {
        FloatDP e(0.25_x, dp);
        FloatDPBounds E = FloatDPBounds(-e, +e);
        D = ExactBoxType::unit_box(2);
        x = Affine<FloatDPBounds>::variables(2, dp);
        ValidatedAffineConstrainedImageSet set1 = ValidatedAffineConstrainedImageSet(D, {0.5_x * x[0] + 0.25_x * x[1] + E / 2, 0.25_x * x[0] - 0.25_x * x[1] + E});
        ARIADNE_TESTING_NOTE("set1 = " << (set1));
        D = ExactBoxType::unit_box(4);
        x = Affine<FloatDPBounds>::variables(4, dp);
        ValidatedAffineConstrainedImageSet set2 = ValidatedAffineConstrainedImageSet(D, {0.5_x * x[0] + 0.25_x * x[1] + e * x[2] / 2, 0.25_x * x[0] - 0.25_x * x[1] + e * x[3]});
        ARIADNE_TESTING_NOTE("set2 = " << (set2));

        Grid g(2);
        BinaryTreePaving paving1 = outer_approximation(set1, g, 4);
        BinaryTreePaving paving2 = outer_approximation(set2, g, 4);

        ARIADNE_TESTING_PROVE(paving1 == paving2);
    }

    Void test_logging_branches() {
        const unsigned int old_verbosity = Logger::instance().configuration().verbosity();
        Logger::instance().configuration().set_verbosity(10u);

        D = ExactBoxType::unit_box(2);
        x = Affine<FloatDPBounds>::variables(2, dp);
        set = ValidatedAffineConstrainedImageSet(D, {x[0], x[1]});

        ARIADNE_TESTING_PROVE(!definitely(set.separated(ExactBoxType{
            {-0.5_x, +0.5_x},
            {-0.5_x, +0.5_x}
        })));

        BinaryTreePaving paving(Grid(2));
        robust_adjoin_outer_approximation_to(paving, set, 0);
        ARIADNE_TESTING_PROVE(paving.size() > 0);

        List<Pair<double, double>> vertices = set.boundary(0, 1);
        ARIADNE_TESTING_PROVE(!vertices.empty());

        Logger::instance().configuration().set_verbosity(old_verbosity);
    }

    Void test_invalid_paving() {
        D = ExactBoxType::unit_box(2);
        x = Affine<FloatDPBounds>::variables(2, dp);
        set = ValidatedAffineConstrainedImageSet(D, {x[0], x[1]});
        BinaryTreePaving bad_paving(Grid(1));
        ARIADNE_TESTING_EXPECT_EXCEPTION(adjoin_outer_approximation_to(bad_paving, set, 0));
        ARIADNE_TESTING_EXPECT_EXCEPTION(robust_adjoin_outer_approximation_to(bad_paving, set, 0));
    }
};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_affine_set_approximation");
    ARIADNE_TESTING_ADD_TEST(TestAffineSetPaving, test_outer_approximation);
    ARIADNE_TESTING_ADD_TEST(TestAffineSetPaving, test_robust_outer_approximation);
    ARIADNE_TESTING_ADD_TEST(TestAffineSetPaving, test_uniform_error);
    ARIADNE_TESTING_ADD_TEST(TestAffineSetPaving, test_logging_branches);
    ARIADNE_TESTING_ADD_TEST(TestAffineSetPaving, test_invalid_paving);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}
