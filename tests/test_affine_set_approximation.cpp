/***************************************************************************
 *            test_affine_sets.cpp
 *
 *  Copyright  2009-20  Pieter Collins
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne.
 *
 *  Ariadne is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <chrono>
#include "function/function.hpp"
#include "geometry/box.hpp"
#include "set/affine_set.hpp"
#include "set/function_set.hpp"
#include "paving/grid_paving.hpp"
#include "paving/set_approximation.hpp"

#include "macro/testing.hpp"

using namespace Ariadne;
using namespace std;

namespace Ariadne {

template<class X> AffineConstraint<X> operator<=(const Dyadic& l, const Affine<X>& a) {
    return AffineConstraint<X>({l,a.value().precision()},a,{+infty,a.value().precision()}); }
template<class X> AffineConstraint<X> operator<=(const Affine<X>& a, const Dyadic& u) {
    return AffineConstraint<X>(-infty,a,u); }
template<class X> AffineConstraint<X> operator<=(const AffineConstraint<X>& ac, const Dyadic& u) {
    ARIADNE_ASSERT(decide(ac.upper_bound()==infty));
    return AffineConstraint<X>(ac.lower_bound(),ac.function(),{u,ac.lower_bound().precision()}); }
template<class X> AffineConstraint<X> operator==(const Affine<X>& a, const Dyadic& u) {
    return AffineConstraint<X>(u,a,u); }
}


class TestAffineSetPaving
{
  private:
    ExactBoxType D;
    Vector< Affine<FloatDPBounds> > x;
    ValidatedAffineConstrainedImageSet set;
  public:
    TestAffineSetPaving() :
        set(ExactBoxType::unit_box(2),Matrix<FloatDP>(2,2,dp),Vector<FloatDP>(2,dp)) { }

    Void test_robust_outer_approximation() {
        D=ExactBoxType::unit_box(2);
        x=Affine<FloatDPBounds>::variables(2,dp);
        set=ValidatedAffineConstrainedImageSet(D,{x[0],x[1]});
        set.new_parameter_constraint(x[0]+x[1]<=0.5_x);

        GridTreePaving paving(Grid(2));
        robust_adjoin_outer_approximation_to(paving,set,0);
        ARIADNE_TEST_ASSERT(paving.size()>0);

        // Repeating the ordinary approximation must recognise cells already
        // contained in the paving and leave the result unchanged.
        SizeType size=paving.size();
        adjoin_outer_approximation_to(paving,set,0);
        ARIADNE_TEST_EQUALS(paving.size(),size);

        ValidatedAffineConstrainedImageSet empty_set(D,{x[0],x[1]});
        empty_set.new_parameter_constraint(x[0]<=-2.0_x);
        GridTreePaving empty_paving(Grid(2));
        robust_adjoin_outer_approximation_to(empty_paving,empty_set,0);
        ARIADNE_TEST_EQUALS(empty_paving.size(),0u);

        ValidatedAffineConstrainedImageSet infeasible_set(D,{x[0],x[1]});
        infeasible_set.new_parameter_constraint(x[0]<=-2.0_x);
        infeasible_set.new_parameter_constraint(-x[0]<=-2.0_x);
        GridTreePaving infeasible_paving(Grid(2));
        robust_adjoin_outer_approximation_to(infeasible_paving,infeasible_set,0);
        ARIADNE_TEST_EQUALS(infeasible_paving.size(),0u);

        ExactBoxType small_box({{0.25_x,0.75_x},{0.25_x,0.75_x}});
        ValidatedAffineConstrainedImageSet small_set(small_box);
        GridTreePaving covered_paving(Grid(2));
        robust_adjoin_outer_approximation_to(covered_paving,small_set,0);
        ARIADNE_TEST_ASSERT(covered_paving.size()>0);
        SizeType covered_size=covered_paving.size();
        robust_adjoin_outer_approximation_to(covered_paving,small_set,0);
        ARIADNE_TEST_EQUALS(covered_paving.size(),covered_size);

    }

    Void test_outer_approximation() {

        {
            D=ExactBoxType::unit_box(2);
            x=Affine<FloatDPBounds>::variables(2,dp);
            set=ValidatedAffineConstrainedImageSet(D,{x[0],x[1]});
            set.new_parameter_constraint(x[0]+x[1]<=-0.5_x);
            ARIADNE_TEST_PRINT(set);

            const auto start1=std::chrono::steady_clock::now();
        GridTreePaving paving = outer_approximation(set,Grid(2),4);
        const auto elapsed_ms1=std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-start1).count();
        std::cerr<<"test_outer_approximation case 1: "<<elapsed_ms1<<" ms\n";
            paving.recombine();
            std::cout<<std::setprecision(17);
            ARIADNE_TEST_PRINT(paving);

            ARIADNE_TEST_ASSERT(paving.size()>0);
        }

        {
            // The following set has difficulties
            set=ValidatedAffineConstrainedImageSet(D,{x[0],x[1]});
            set.new_parameter_constraint(2.0_x*x[0]+1.0_x*x[1]<=0.5_x);
            set.new_parameter_constraint(-0.5_x*x[0]+1.0_x*x[1]<=0.75_x);
            set.new_parameter_constraint(-0.5_x*x[0]-1.0_x*x[1]<=0.875_x);

            const auto start2=std::chrono::steady_clock::now();
        GridTreePaving paving=outer_approximation(set,Grid(2),3);
        const auto elapsed_ms2=std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-start2).count();
        std::cerr<<"test_outer_approximation case 2: "<<elapsed_ms2<<" ms\n";
            ARIADNE_TEST_ASSERT(paving.size()>0);
        }

        {
            D=ExactBoxType::unit_box(3);
            x=Affine<FloatDPBounds>::variables(3,dp);

            set=ValidatedAffineConstrainedImageSet( D, {2.0_pr*x[0]+3.0_pr*x[1]+1.0_pr*x[2]+0.05_pr, x[0]+x[1]+2.051_pr} );

            set.new_parameter_constraint(+1.01_pr*x[0]+0.51_pr*x[1]+0.251_pr*x[2]<=0.751_pr);
            set.new_parameter_constraint(-0.51_pr*x[0]+1.51_pr*x[1]+0.01_pr*x[2]<=0.51_pr);
            set.new_parameter_constraint(-2.01_pr*x[0]-3.01_pr*x[1]-1.02_pr*x[2]<=3.01_pr);

            Grid grid(2);
            GridTreePaving paving(grid);
            const auto start3=std::chrono::steady_clock::now();
        adjoin_outer_approximation_to(paving,set,3);
        const auto elapsed_ms3=std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-start3).count();
        std::cerr<<"test_outer_approximation case 3: "<<elapsed_ms3<<" ms\n";
            paving.recombine();

            ARIADNE_TEST_ASSERT(paving.size()>0);
        }

        {
            set=ValidatedAffineConstrainedImageSet(ExactBoxType::unit_box(3),{-0.9375_x+0.0625_x*x[0]+0.5_x*x[1],0.87890625_x-0.1171875_x*x[0]+x[1]+0.00390625_x*x[2]});
            set.new_parameter_constraint(0.0625_x*x[0]+x[1]<=1.1875_x);
            ARIADNE_TEST_PRINT(set);

            const auto start4=std::chrono::steady_clock::now();
        GridTreePaving paving = outer_approximation(set,Grid(2),4);
        const auto elapsed_ms4=std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-start4).count();
        std::cerr<<"test_outer_approximation case 4: "<<elapsed_ms4<<" ms\n";
            paving.recombine();
            std::cout<<std::setprecision(17);
            ARIADNE_TEST_PRINT(paving);

            ARIADNE_TEST_ASSERT(paving.size()>0);
        }


    }

    Void test_uniform_error() {
        FloatDP e(0.25_x,dp);
        FloatDPBounds E = FloatDPBounds(-e,+e);
        D = ExactBoxType::unit_box(2);
        x = Affine<FloatDPBounds>::variables(2,dp);
        ValidatedAffineConstrainedImageSet set1=ValidatedAffineConstrainedImageSet(D, {0.5_x*x[0]+0.25_x*x[1]+E/2, 0.25_x*x[0]-0.25_x*x[1]+E} );
        ARIADNE_TEST_PRINT(set1);
        D = ExactBoxType::unit_box(4);
        x = Affine<FloatDPBounds>::variables(4,dp);
        ValidatedAffineConstrainedImageSet set2=ValidatedAffineConstrainedImageSet(D, {0.5_x*x[0]+0.25_x*x[1]+e*x[2]/2, 0.25_x*x[0]-0.25_x*x[1]+e*x[3]} );
        ARIADNE_TEST_PRINT(set2);

        Grid g(2);
        GridTreePaving paving1=outer_approximation(set1,g,4);
        GridTreePaving paving2=outer_approximation(set2,g,4);

        ARIADNE_TEST_ASSERT(paving1==paving2);

    }

    Void test_logging_branches() {
        unsigned int const old_verbosity=Logger::instance().configuration().verbosity();
        Logger::instance().configuration().set_verbosity(10u);

        D=ExactBoxType::unit_box(2);
        x=Affine<FloatDPBounds>::variables(2,dp);
        set=ValidatedAffineConstrainedImageSet(D,{x[0],x[1]});

        ARIADNE_TEST_ASSERT(!definitely(set.separated(ExactBoxType{{-0.5_x,+0.5_x},{-0.5_x,+0.5_x}})));

        GridTreePaving paving(Grid(2));
        robust_adjoin_outer_approximation_to(paving,set,0);
        ARIADNE_TEST_ASSERT(paving.size()>0);

        List<Pair<double,double>> vertices=set.boundary(0,1);
        ARIADNE_TEST_ASSERT(!vertices.empty());

        Logger::instance().configuration().set_verbosity(old_verbosity);
    }

    Void test_invalid_paving() {
        D=ExactBoxType::unit_box(2);
        x=Affine<FloatDPBounds>::variables(2,dp);
        set=ValidatedAffineConstrainedImageSet(D,{x[0],x[1]});
        GridTreePaving bad_paving(Grid(1));
        ARIADNE_TEST_FAIL(adjoin_outer_approximation_to(bad_paving,set,0));
        ARIADNE_TEST_FAIL(robust_adjoin_outer_approximation_to(bad_paving,set,0));
    }

    Void test() {
        ARIADNE_TEST_CALL(test_outer_approximation());
        ARIADNE_TEST_CALL(test_robust_outer_approximation());
        ARIADNE_TEST_CALL(test_uniform_error());
        ARIADNE_TEST_CALL(test_logging_branches());
        ARIADNE_TEST_CALL(test_invalid_paving());
    }
};

Int main() {
    TestAffineSetPaving().test();
    return ARIADNE_TEST_FAILURES;
}
