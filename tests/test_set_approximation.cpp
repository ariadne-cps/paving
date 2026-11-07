#include "set/function_set.hpp"
#include "set/affine_set.hpp"
#include "paving/set_approximation.hpp"
#include "paving/binary_tree_paving.hpp"
#include "ariadne-testing.hpp"
#include <type_traits>
#include <utility>

using namespace Ariadne;

using ConstrainedPavingResult = decltype(outer_approximation(
    std::declval<const ValidatedConstrainedImageSet&>(),
    std::declval<const Grid&>(),std::declval<Nat>()));
using AffinePavingResult = decltype(outer_approximation(
    std::declval<const ValidatedAffineConstrainedImageSet&>(),
    std::declval<const Grid&>(),std::declval<Nat>()));
static_assert(std::is_same_v<ConstrainedPavingResult,BinaryTreePaving>);
static_assert(std::is_same_v<AffinePavingResult,BinaryTreePaving>);

class TestSetApproximation : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

    Void test_effective_set_approximation() {
        List<EffectiveScalarMultivariateFunction> s=EffectiveScalarMultivariateFunction::coordinates(3);
        List<EffectiveScalarMultivariateFunction> x=EffectiveScalarMultivariateFunction::coordinates(2);

        RealBox d(3,RealInterval(-1,+2));
        EffectiveConstrainedImageSet set(d,{s[0],s[0]*s[0]/4+s[1]+s[2]/2});
        set.new_parameter_constraint(0<=s[0]+s[1]<=1);
        set.new_space_constraint(x[0]+x[1]<=2.0_x);
        ARIADNE_TESTING_NOTE("set = " << (set));
        BinaryTreePaving paving(2);
        Nat depth=2;
        paving.adjoin_outer_approximation(set,depth);
        adjoin_outer_approximation_to(paving,set,depth);
    }
Void test_validated_set_approximation() {
    ExactBoxType domain{{-1.0_x,1.0_x},{-2.0_x,2.0_x}};
    ValidatedConstrainedImageSet box_set(domain);
    Grid grid(2u);
    auto outer=outer_approximation(box_set,grid,0u);
    ARIADNE_TESTING_PROVE(!outer.is_empty());
    BinaryTreePaving another_outer(grid);
    adjoin_outer_approximation_to(another_outer,box_set,0u);
    ARIADNE_TESTING_PROVE(!another_outer.is_empty());
}

};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_set_approximation");
    ARIADNE_TESTING_ADD_TEST(TestSetApproximation, test_effective_set_approximation);
    ARIADNE_TESTING_ADD_TEST(TestSetApproximation, test_validated_set_approximation);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}
