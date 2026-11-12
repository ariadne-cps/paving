/***************************************************************************
 *            tests/test_grid.cpp
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

/** @file tests/test_grid.cpp
 *  @brief Test paving functionality and backend invariants.
 */

#include "paving/grid.hpp"
#include "utility/exceptions.hpp"
#include "ariadne-testing.hpp"
#include <sstream>
using namespace Ariadne;

class TestGrid : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

    Void test_grid_coordinates() {
    Grid unspecified;
    ARIADNE_TESTING_EXPECT_EQUAL(unspecified.dimension(),0u);
    Grid unit_grid(2u);
    Grid half_grid(2u,0.5);
    Grid raw_half(2u,FloatDP(0.5_x,dp));
    Grid lengths(Vector<ApproximateDouble>{0.5,2.0});
    Grid raw_lengths(Vector<FloatDP>({0.5_x,2.0_x},dp));
    ARIADNE_TESTING_PROVE(half_grid==raw_half);
    ARIADNE_TESTING_PROVE(lengths==raw_lengths);
    ARIADNE_TESTING_PROVE(unit_grid!=half_grid);
    Grid shifted(Vector<ApproximateDouble>{1.0,-2.0},Vector<ApproximateDouble>{0.5,2.0});
    Grid different_origin(Vector<FloatDP>({2.0_x,-2.0_x},dp),Vector<FloatDP>({0.5_x,2.0_x},dp));
    ARIADNE_TESTING_PROVE(shifted!=different_origin);
    ARIADNE_TESTING_EXPECT_THROWS(IncompatibleSizes, Grid(Vector<FloatDP>({1.0_x},dp),Vector<FloatDP>({1.0_x,1.0_x},dp)));
    ARIADNE_TESTING_EXPECT_EQUAL(shifted.coordinate(0u,2.5),FloatDP(2.25_x,dp));
    ARIADNE_TESTING_EXPECT_EQUAL(shifted.subdivision_coordinate(0u,2.5),FloatDP(2.25_x,dp));
    ARIADNE_TESTING_EXPECT_EQUAL(shifted.subdivision_coordinate(1u,Int(-2)),FloatDP(-6,dp));
    ARIADNE_TESTING_EXPECT_EQUAL(shifted.subdivision_index(0u,FloatDP(2,dp)),2);
    ARIADNE_TESTING_EXPECT_THROWS(InvalidGridPosition, shifted.subdivision_index(0u,FloatDP(2.25_x,dp)));
    ARIADNE_TESTING_EXPECT_EQUAL(shifted.subdivision_lower_index(0u,FloatDPLowerBound(2.25_x,dp)),2);
    ARIADNE_TESTING_EXPECT_EQUAL(shifted.subdivision_upper_index(0u,FloatDPUpperBound(2.25_x,dp)),3);
    // The spacing exceeds the ULP at the origin. Individual coordinates
    // still round down (index 3) or up (index 1), requiring index correction.
    Grid rounded_grid(Vector<FloatDP>({10000000000000000.0_x},dp),Vector<FloatDP>({3.0_x},dp));
    auto down_coordinate=rounded_grid.subdivision_coordinate(0u,Int(3));
    auto up_coordinate=rounded_grid.subdivision_coordinate(0u,Int(1));
    ARIADNE_TESTING_EXPECT_EQUAL(rounded_grid.subdivision_lower_index(0u,FloatDPLowerBound(down_coordinate)),3);
    ARIADNE_TESTING_EXPECT_EQUAL(rounded_grid.subdivision_upper_index(0u,FloatDPUpperBound(up_coordinate)),1);
    auto pt=shifted.point(Array<Int>{2,-1});
    ARIADNE_TESTING_EXPECT_EQUAL(pt[0],FloatDP(2,dp));
    ARIADNE_TESTING_EXPECT_EQUAL(pt[1],FloatDP(-4,dp));
    auto indices=shifted.index(pt);
    ARIADNE_TESTING_EXPECT_EQUAL(indices[0],2.0);
    ARIADNE_TESTING_EXPECT_EQUAL(indices[1],-1.0);
    auto fractional=shifted.point(Array<double>{0.5,1.5});
    ARIADNE_TESTING_EXPECT_EQUAL(fractional[0],FloatDP(1.25_x,dp));
    ARIADNE_TESTING_EXPECT_EQUAL(fractional[1],FloatDP(1,dp));
    auto box=shifted.box(Array<double>{0.0,-1.0},Array<double>{2.0,2.0});
    ARIADNE_TESTING_PROVE(same(box,ExactBoxType{{1.0_x,2.0_x},{-4.0_x,2.0_x}}));
    ARIADNE_TESTING_EXPECT_EQUAL(shifted.lower_index(box)[1],-1.0);
    ARIADNE_TESTING_EXPECT_EQUAL(shifted.upper_index(box)[0],2.0);
    ARIADNE_TESTING_EXPECT_EQUAL(join(unit_grid,shifted).dimension(),4u);
    Grid zero(0u);
    ARIADNE_TESTING_EXPECT_EQUAL(zero.index(ExactPointType()).size(),0u);
    ARIADNE_TESTING_EXPECT_EQUAL(zero.point(Array<Int>{}).size(),0u);
    ARIADNE_TESTING_EXPECT_EQUAL(zero.point(Array<double>{}).size(),0u);
    ARIADNE_TESTING_EXPECT_EQUAL(zero.lower_index(ExactBoxType()).size(),0u);
    ARIADNE_TESTING_EXPECT_EQUAL(zero.upper_index(ExactBoxType()).size(),0u);
    ARIADNE_TESTING_EXPECT_EQUAL(zero.box(Array<double>{},Array<double>{}).size(),0u);
    std::ostringstream repr;
    repr<<shifted;
    ARIADNE_TESTING_PROVE(repr.str().find("origin=")!=String::npos);
}
Void test_grid_dimension_comparisons() {
    Grid zero_dim(0u),one_dim(1u),two_dim(2u);
    ARIADNE_TESTING_PROVE(zero_dim!=one_dim);
    ARIADNE_TESTING_PROVE(one_dim!=zero_dim);
    ARIADNE_TESTING_PROVE(one_dim!=two_dim);
    ARIADNE_TESTING_PROVE(two_dim!=one_dim);
    ARIADNE_TESTING_PROVE(zero_dim!=two_dim);
    ARIADNE_TESTING_PROVE(two_dim!=zero_dim);
    ARIADNE_TESTING_PROVE(zero_dim==Grid(0u));
    ARIADNE_TESTING_PROVE(one_dim==Grid(1u));
    ARIADNE_TESTING_PROVE(two_dim==Grid(2u));
    Grid shared=two_dim;
    ARIADNE_TESTING_PROVE(two_dim==shared);
}
};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_grid");
    ARIADNE_TESTING_ADD_TEST(TestGrid, test_grid_coordinates);
    ARIADNE_TESTING_ADD_TEST(TestGrid, test_grid_dimension_comparisons);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}
