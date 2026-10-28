/***************************************************************************
 *            test_grid.cpp
 *
 *  This file is part of Ariadne, distributed under the GNU GPL version 3
 *  or (at your option) any later version.
 ****************************************************************************/
#include "paving/grid.hpp"
#include "utility/exceptions.hpp"
#include "macro/testing.hpp"
#include <sstream>
using namespace Ariadne;

Void test_grid_coordinates() {
    Grid unspecified;
    ARIADNE_TEST_EQUALS(unspecified.dimension(),0u);
    Grid unit_grid(2u);
    Grid half_grid(2u,0.5);
    Grid raw_half(2u,FloatDP(0.5_x,dp));
    Grid lengths(Vector<ApproximateDouble>{0.5,2.0});
    Grid raw_lengths(Vector<FloatDP>({0.5_x,2.0_x},dp));
    ARIADNE_TEST_ASSERT(half_grid==raw_half);
    ARIADNE_TEST_ASSERT(lengths==raw_lengths);
    ARIADNE_TEST_ASSERT(unit_grid!=half_grid);
    Grid shifted(Vector<ApproximateDouble>{1.0,-2.0},Vector<ApproximateDouble>{0.5,2.0});
    Grid different_origin(Vector<FloatDP>({2.0_x,-2.0_x},dp),Vector<FloatDP>({0.5_x,2.0_x},dp));
    ARIADNE_TEST_ASSERT(shifted!=different_origin);
    ARIADNE_TEST_THROWS(Grid(Vector<FloatDP>({1.0_x},dp),Vector<FloatDP>({1.0_x,1.0_x},dp)),IncompatibleSizes);
    ARIADNE_TEST_EQUALS(shifted.coordinate(0u,2.5),FloatDP(2.25_x,dp));
    ARIADNE_TEST_EQUALS(shifted.subdivision_coordinate(0u,2.5),FloatDP(2.25_x,dp));
    ARIADNE_TEST_EQUALS(shifted.subdivision_coordinate(1u,Int(-2)),FloatDP(-6,dp));
    ARIADNE_TEST_EQUALS(shifted.subdivision_index(0u,FloatDP(2,dp)),2);
    ARIADNE_TEST_THROWS(shifted.subdivision_index(0u,FloatDP(2.25_x,dp)),InvalidGridPosition);
    ARIADNE_TEST_EQUALS(shifted.subdivision_lower_index(0u,FloatDPLowerBound(2.25_x,dp)),2);
    ARIADNE_TEST_EQUALS(shifted.subdivision_upper_index(0u,FloatDPUpperBound(2.25_x,dp)),3);
    // The spacing exceeds the ULP at the origin. Individual coordinates
    // still round down (index 3) or up (index 1), requiring index correction.
    Grid rounded_grid(Vector<FloatDP>({10000000000000000.0_x},dp),Vector<FloatDP>({3.0_x},dp));
    auto down_coordinate=rounded_grid.subdivision_coordinate(0u,Int(3));
    auto up_coordinate=rounded_grid.subdivision_coordinate(0u,Int(1));
    ARIADNE_TEST_EQUALS(rounded_grid.subdivision_lower_index(0u,FloatDPLowerBound(down_coordinate)),3);
    ARIADNE_TEST_EQUALS(rounded_grid.subdivision_upper_index(0u,FloatDPUpperBound(up_coordinate)),1);
    auto pt=shifted.point(Array<Int>{2,-1});
    ARIADNE_TEST_EQUALS(pt[0],FloatDP(2,dp));
    ARIADNE_TEST_EQUALS(pt[1],FloatDP(-4,dp));
    auto indices=shifted.index(pt);
    ARIADNE_TEST_EQUALS(indices[0],2.0);
    ARIADNE_TEST_EQUALS(indices[1],-1.0);
    auto fractional=shifted.point(Array<double>{0.5,1.5});
    ARIADNE_TEST_EQUALS(fractional[0],FloatDP(1.25_x,dp));
    ARIADNE_TEST_EQUALS(fractional[1],FloatDP(1,dp));
    auto box=shifted.box(Array<double>{0.0,-1.0},Array<double>{2.0,2.0});
    ARIADNE_TEST_ASSERT(same(box,ExactBoxType{{1.0_x,2.0_x},{-4.0_x,2.0_x}}));
    ARIADNE_TEST_EQUALS(shifted.lower_index(box)[1],-1.0);
    ARIADNE_TEST_EQUALS(shifted.upper_index(box)[0],2.0);
    ARIADNE_TEST_EQUALS(join(unit_grid,shifted).dimension(),4u);
    Grid zero(0u);
    ARIADNE_TEST_EQUALS(zero.index(ExactPointType()).size(),0u);
    ARIADNE_TEST_EQUALS(zero.point(Array<Int>{}).size(),0u);
    ARIADNE_TEST_EQUALS(zero.point(Array<double>{}).size(),0u);
    ARIADNE_TEST_EQUALS(zero.lower_index(ExactBoxType()).size(),0u);
    ARIADNE_TEST_EQUALS(zero.upper_index(ExactBoxType()).size(),0u);
    ARIADNE_TEST_EQUALS(zero.box(Array<double>{},Array<double>{}).size(),0u);
    std::ostringstream repr;
    repr<<shifted;
    ARIADNE_TEST_ASSERT(repr.str().find("origin=")!=String::npos);
}
Void test_grid_dimension_comparisons() {
    Grid zero_dim(0u),one_dim(1u),two_dim(2u);
    ARIADNE_TEST_ASSERT(zero_dim!=one_dim);
    ARIADNE_TEST_ASSERT(one_dim!=zero_dim);
    ARIADNE_TEST_ASSERT(one_dim!=two_dim);
    ARIADNE_TEST_ASSERT(two_dim!=one_dim);
    ARIADNE_TEST_ASSERT(zero_dim!=two_dim);
    ARIADNE_TEST_ASSERT(two_dim!=zero_dim);
    ARIADNE_TEST_ASSERT(zero_dim==Grid(0u));
    ARIADNE_TEST_ASSERT(one_dim==Grid(1u));
    ARIADNE_TEST_ASSERT(two_dim==Grid(2u));
    Grid shared=two_dim;
    ARIADNE_TEST_ASSERT(two_dim==shared);
}
Int main() {
    ARIADNE_TEST_CALL(test_grid_coordinates());
    ARIADNE_TEST_CALL(test_grid_dimension_comparisons());
    return ARIADNE_TEST_FAILURES;
}
