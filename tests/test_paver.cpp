/***************************************************************************
 *            test_paver.cpp
 *
 *  This file is part of Ariadne, distributed under the GNU GPL version 3
 *  or (at your option) any later version.
 ****************************************************************************/
#include "paving/paver.hpp"
#include "set/function_set.hpp"
#include "function/taylor_function.hpp"
#include "paving/grid_paving.hpp"
#include "macro/testing.hpp"
#include "logging/logging.hpp"
#include <sstream>
using namespace Ariadne;

Void check_paver(const Paver& paver) {
    ARIADNE_TEST_PRINT(paver);
    auto x=EffectiveScalarMultivariateFunction::coordinate(1u,0u);
    ValidatedVectorMultivariateFunction identity({x});
    ExactBoxType domain{{0.125_x,0.375_x}};
    ValidatedConstrainedImageSet set(domain,identity);
    Grid grid(1u);
    GridTreePaving approximation(grid);
    paver.adjoin_outer_approximation(approximation,set,2u);
    ARIADNE_TEST_ASSERT(approximation.superset(GridCell(grid,0u,BinaryWord("00"))));
    ARIADNE_TEST_ASSERT(approximation.superset(GridCell(grid,0u,BinaryWord("01"))));
    ARIADNE_TEST_ASSERT(definitely(subset(cast_exact_box(approximation.bounding_box()),ExactBoxType{{-0.5_x,0.75_x}})));
    GridTreePaving full(grid);
    full.adjoin(GridCell(grid,0u,BinaryWord()));
    paver.adjoin_outer_approximation(full,set,2u);
    ARIADNE_TEST_ASSERT(full.superset(GridCell(grid,0u,BinaryWord())));
    ValidatedConstrainedImageSet constrained(domain,identity,List<ValidatedConstraint>{0<=x<=1});
    GridTreePaving constrained_result(grid);
    paver.adjoin_outer_approximation(constrained_result,constrained,2u);
    ARIADNE_TEST_ASSERT(constrained_result.superset(GridCell(grid,0u,BinaryWord("00"))));
    ValidatedConstrainedImageSet below(domain,identity,List<ValidatedConstraint>{x<=-1});
    ValidatedConstrainedImageSet above(domain,identity,List<ValidatedConstraint>{x>=1});
    GridTreePaving excluded(grid);
    paver.adjoin_outer_approximation(excluded,below,2u);
    paver.adjoin_outer_approximation(excluded,above,2u);
    ARIADNE_TEST_ASSERT(excluded.is_empty());
    ValidatedConstrainedImageSet empty(ExactBoxType(1u),identity);
    paver.adjoin_outer_approximation(excluded,empty,2u);
    ARIADNE_TEST_ASSERT(excluded.is_empty());
    GridTreePaving wrong_dimension(Grid(2u));
    ARIADNE_TEST_FAIL(paver.adjoin_outer_approximation(wrong_dimension,set,2u));
    std::ostringstream repr;
    repr<<paver;
    ARIADNE_TEST_ASSERT(repr.str().find("Paver()")!=String::npos);
}

Void test_paver_strategies() {
    ARIADNE_TEST_CALL(check_paver(Paver(AffinePaver())));
    ARIADNE_TEST_CALL(check_paver(Paver(SubdivisionPaver())));
    ARIADNE_TEST_CALL(check_paver(Paver(ReducePaver())));
    ARIADNE_TEST_CALL(check_paver(Paver(ConstraintPaver())));
    ARIADNE_TEST_CALL(check_paver(Paver(OptimalConstraintPaver())));
}

Void test_nonlinear_pavers() {
    auto x=EffectiveScalarMultivariateFunction::coordinate(1u,0u);
    ExactBoxType square{{-1.0_x,1.0_x},{-1.0_x,1.0_x}};
    auto u=EffectiveScalarMultivariateFunction::coordinate(2u,0u);
    auto v=EffectiveScalarMultivariateFunction::coordinate(2u,1u);
    ValidatedVectorMultivariateFunction nonlinear({u*u,v*v});
    ValidatedVectorMultivariateTaylorFunctionModelDP model(square,nonlinear,TrivialSweeper<FloatDP>(dp));
    GridTreePaving model_paving(Grid(2u));
    AffinePaver().adjoin_outer_approximation(model_paving,ValidatedConstrainedImageSet(square,ValidatedVectorMultivariateFunctionModelDP(model)),0u);
    ARIADNE_TEST_ASSERT(!model_paving.is_empty());
    ValidatedVectorMultivariateFunction quadratic({x*x});
    ValidatedConstrainedImageSet set(ExactBoxType{{-1.0_x,1.0_x}},quadratic);
    Grid grid(1u);
    const Paver strategies[]={Paver(AffinePaver()),Paver(SubdivisionPaver()),Paver(ReducePaver()),Paver(ConstraintPaver()),Paver(OptimalConstraintPaver())};
    for(const auto& paver:strategies) {
        ARIADNE_TEST_PRINT(paver);
        GridTreePaving approximation(grid);
        paver.adjoin_outer_approximation(approximation,set,1u);
        const ExactBoxType bounds=cast_exact_box(approximation.bounding_box());
        ARIADNE_TEST_ASSERT(bounds[0].lower_bound()<=FloatDP(0,dp));
        ARIADNE_TEST_ASSERT(bounds[0].upper_bound()>=FloatDP(1,dp));
        ARIADNE_TEST_ASSERT(definitely(subset(bounds,ExactBoxType{{-2.0_x,2.0_x}})));
    }
}

Void test_intrinsic_uncertainty() {
    const Paver strategies[]={Paver(AffinePaver()),Paver(SubdivisionPaver()),Paver(ReducePaver()),Paver(ConstraintPaver()),Paver(OptimalConstraintPaver())};
    for(SizeType arity=0u; arity!=2u; ++arity) {
        auto constant=ValidatedScalarMultivariateFunction::constant(arity,ValidatedNumber(FloatDPBounds(0.25_x,0.75_x,dp)));
        ValidatedVectorMultivariateFunction mapping({constant});
        ValidatedConstrainedImageSet set(ExactBoxType(arity,ExactIntervalType(-1,1)),mapping);
        for(const auto& paver:strategies) {
            ARIADNE_TEST_PRINT(arity);
            ARIADNE_TEST_PRINT(paver);
            GridTreePaving approximation(Grid(1u));
            paver.adjoin_outer_approximation(approximation,set,3u);
            const auto bounds=cast_exact_box(approximation.bounding_box());
            ARIADNE_TEST_ASSERT(bounds[0].lower_bound()<=FloatDP(0.25_x,dp));
            ARIADNE_TEST_ASSERT(bounds[0].upper_bound()>=FloatDP(0.75_x,dp));
            ARIADNE_TEST_ASSERT(definitely(subset(bounds,ExactBoxType{{0.0_x,1.0_x}})));
        }
    }
    auto identity=ValidatedVectorMultivariateFunction::identity(1u);
    ValidatedConstrainedImageSet set(ExactBoxType{{0.125_x,0.375_x}},identity);
    GridTreePaving paving(Grid(1u));
    SubdivisionPaver subdivision;
    ARIADNE_TEST_FAIL(subdivision.adjoin_outer_approximation_recursion(paving,set,2u,Vector<FloatDP>(0u,dp)));
    ARIADNE_TEST_FAIL(subdivision.adjoin_outer_approximation_recursion(paving,set,2u,Vector<FloatDP>(1u,FloatDP(0,dp))));
}

Void test_affine_depth_limit() {
    auto x=EffectiveScalarMultivariateFunction::coordinate(1u,0u);
    ValidatedVectorMultivariateFunction mapping({10000000000.0_x*x*x});
    ValidatedConstrainedImageSet set(ExactBoxType{{-1.0_x,1.0_x}},mapping);
    GridTreePaving approximation(Grid(1u,FloatDP(1000000000000.0_x,dp)));
    AffinePaver().adjoin_outer_approximation(approximation,set,0u);
    auto bounds=cast_exact_box(approximation.bounding_box());
    ARIADNE_TEST_ASSERT(bounds[0].lower_bound()<=FloatDP(0,dp));
    ARIADNE_TEST_ASSERT(bounds[0].upper_bound()>=FloatDP(10000000000.0_x,dp));
    ARIADNE_TEST_ASSERT(definitely(subset(bounds,ExactBoxType{{-2000000000000.0_x,2000000000000.0_x}})));
}

Void test_correlated_constraints();

Void test_reduce_contractors() {
    auto& configuration=Logger::instance().configuration();
    const auto old_verbosity=configuration.verbosity();
    struct RestoreVerbosity {
        unsigned int value;
        ~RestoreVerbosity() { Logger::instance().configuration().set_verbosity(value); }
    } restore{old_verbosity};
    configuration.set_verbosity(32u);
    auto x=EffectiveScalarMultivariateFunction::coordinate(1u,0u);
    ExactBoxType interval{{0.0_x,1.0_x}};
    ValidatedVectorMultivariateFunction identity({x});
    ValidatedVectorMultivariateFunction quadratic({x*(1-x)});
    GridTreePaving empty(Grid(1u));
    ReducePaver().adjoin_outer_approximation(empty,ValidatedConstrainedImageSet(interval,identity,List<ValidatedConstraint>{x*(1-x)>=0.5_x}),2u);
    ARIADNE_TEST_ASSERT(empty.is_empty());
    GridTreePaving quadratic_paving(Grid(1u));
    ReducePaver().adjoin_outer_approximation(quadratic_paving,ValidatedConstrainedImageSet(interval,quadratic),2u);
    const auto quadratic_bounds=cast_exact_box(quadratic_paving.bounding_box());
    ARIADNE_TEST_ASSERT(quadratic_bounds[0].lower_bound()<=FloatDP(0,dp));
    ARIADNE_TEST_ASSERT(quadratic_bounds[0].upper_bound()>=FloatDP(0.25_x,dp));
    ARIADNE_TEST_ASSERT(definitely(subset(quadratic_bounds,ExactBoxType{{-0.5_x,1.0_x}})));
    auto u=EffectiveScalarMultivariateFunction::coordinate(2u,0u);
    auto v=EffectiveScalarMultivariateFunction::coordinate(2u,1u);
    ValidatedVectorMultivariateFunction circle({u*u+v*v});
    GridTreePaving circle_paving(Grid(1u));
    ReducePaver().adjoin_outer_approximation(circle_paving,ValidatedConstrainedImageSet(ExactBoxType{{-1.0_x,1.0_x},{-1.0_x,1.0_x}},circle),2u);
    auto circle_bounds=cast_exact_box(circle_paving.bounding_box());
    ARIADNE_TEST_ASSERT(circle_bounds[0].lower_bound()<=FloatDP(0,dp));
    ARIADNE_TEST_ASSERT(circle_bounds[0].upper_bound()>=FloatDP(2,dp));
    ValidatedVectorMultivariateFunction bilinear({u*v,u+v});
    ValidatedConstrainedImageSet triangle(ExactBoxType{{0.0_x,1.0_x},{0.0_x,1.0_x}},bilinear,List<ValidatedConstraint>{u-v>=0.75_x});
    GridTreePaving triangle_paving(Grid(2u));
    ReducePaver().adjoin_outer_approximation(triangle_paving,triangle,2u);
    ARIADNE_TEST_ASSERT(definitely(subset(ExactBoxType{{0.0_x,0.0_x},{1.0_x,1.0_x}},cast_exact_box(triangle_paving.bounding_box()))));
    GridTreePaving zero_output(Grid(0u));
    ReducePaver().adjoin_outer_approximation(zero_output,ValidatedConstrainedImageSet(interval,ValidatedVectorMultivariateFunction::zeros(0u,1u)),0u);
    ARIADNE_TEST_ASSERT(!zero_output.is_empty());
    ARIADNE_TEST_EQUALS(zero_output.dimension(),0u);
    ARIADNE_TEST_CALL(check_paver(Paver(ReducePaver())));
    ARIADNE_TEST_CALL(test_correlated_constraints());
}

Void test_correlated_constraints() {
    auto u=EffectiveScalarMultivariateFunction::coordinate(2u,0u);
    auto v=EffectiveScalarMultivariateFunction::coordinate(2u,1u);
    const EffectiveScalarMultivariateFunction mappings[]={u*v,sqr(u)+sqr(v),u*v};
    const EffectiveConstraint conditions[]={sqr(u-v)>=0.75_x,u*u+v*v<=0.5_x,u*v<=0.0625_x};
    const ExactDouble attained_values[]={0.125_x,0.5_x,0.0625_x};
    for(SizeType i=0;i!=3u;++i) {
        ARIADNE_TEST_PRINT(i);
        ValidatedVectorMultivariateFunction mapping({mappings[i]});
        ValidatedConstrainedImageSet set(ExactBoxType{{0.0_x,1.0_x},{0.0_x,1.0_x}},mapping,List<ValidatedConstraint>{conditions[i]});
        GridTreePaving paving(Grid(1u));
        ReducePaver().adjoin_outer_approximation(paving,set,2u);
        const auto bounds=cast_exact_box(paving.bounding_box());
        ARIADNE_TEST_ASSERT(bounds[0].lower_bound()<=FloatDP(0,dp));
        ARIADNE_TEST_ASSERT(bounds[0].upper_bound()>=FloatDP(attained_values[i],dp));
        ARIADNE_TEST_ASSERT(definitely(subset(bounds,ExactBoxType{{-0.5_x,1.0_x}})));
    }
}

Int main() {
    ARIADNE_TEST_CALL(test_correlated_constraints());
    ARIADNE_TEST_CALL(test_affine_depth_limit());
    ARIADNE_TEST_CALL(test_reduce_contractors());
    ARIADNE_TEST_CALL(test_intrinsic_uncertainty());
    ARIADNE_TEST_CALL(test_paver_strategies());
    ARIADNE_TEST_CALL(test_nonlinear_pavers());
    return ARIADNE_TEST_FAILURES;
}
