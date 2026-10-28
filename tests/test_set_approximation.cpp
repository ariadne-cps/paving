#include "set/function_set.hpp"
#include "set/affine_set.hpp"
#include "paving/set_approximation.hpp"
#include "paving/grid_paving.hpp"
#include "macro/testing.hpp"
#include <type_traits>
#include <utility>

using namespace Ariadne;

using ConstrainedPavingResult = decltype(outer_approximation(
    std::declval<const ValidatedConstrainedImageSet&>(),
    std::declval<const Grid&>(),std::declval<Nat>()));
using AffinePavingResult = decltype(outer_approximation(
    std::declval<const ValidatedAffineConstrainedImageSet&>(),
    std::declval<const Grid&>(),std::declval<Nat>()));
static_assert(std::is_same_v<ConstrainedPavingResult,GridTreePaving>);
static_assert(std::is_same_v<AffinePavingResult,GridTreePaving>);

Void test_effective_set_approximation() {
        List<EffectiveScalarMultivariateFunction> s=EffectiveScalarMultivariateFunction::coordinates(3);
        List<EffectiveScalarMultivariateFunction> x=EffectiveScalarMultivariateFunction::coordinates(2);

        RealBox d(3,RealInterval(-1,+2));
        EffectiveConstrainedImageSet set(d,{s[0],s[0]*s[0]/4+s[1]+s[2]/2});
        set.new_parameter_constraint(0<=s[0]+s[1]<=1);
        set.new_space_constraint(x[0]+x[1]<=2.0_x);
        ARIADNE_TEST_PRINT(set);
        GridTreePaving paving(2);
        Nat depth=2;
        paving.adjoin_outer_approximation(set,depth);
        adjoin_outer_approximation_to(paving,set,depth);
    }
Void test_validated_set_approximation() {
    ExactBoxType domain{{-1.0_x,1.0_x},{-2.0_x,2.0_x}};
    ValidatedConstrainedImageSet box_set(domain);
    Grid grid(2u);
    auto outer=outer_approximation(box_set,grid,0u);
    ARIADNE_TEST_ASSERT(!outer.is_empty());
    GridTreePaving another_outer(grid);
    adjoin_outer_approximation_to(another_outer,box_set,0u);
    ARIADNE_TEST_ASSERT(!another_outer.is_empty());
}

Int main() {
    ARIADNE_TEST_CALL(test_effective_set_approximation());
    ARIADNE_TEST_CALL(test_validated_set_approximation());
    return ARIADNE_TEST_FAILURES;
}
