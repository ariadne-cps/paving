// Regression tests for the paving-layer augmented state representation.
// No drawing, labelled, system or evolution includes are required here.
#include <memory>
#include <sstream>
#include "paving/augmented_paving.hpp"
#include "set/function_set.hpp"
#include "geometry/set_wrapper.hpp"
#include "ariadne-testing.hpp"

using namespace Ariadne;

class TestAugmentedPaving : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;
    Void test_mapping_and_compatibility() {
        Grid grid(2);
        EffectiveVectorMultivariateFunction f=EffectiveVectorMultivariateFunction::identity(2);
        AugmentedPaving paving(grid,f);
        ARIADNE_TESTING_PROVE(paving.is_empty());
        ARIADNE_TESTING_EXPECT_EQUAL(paving.dimension(),2u);
        ARIADNE_TESTING_PROVE(&paving.state_paving()==&paving.state_set());
        ARIADNE_TESTING_EXPECT_EQUAL(paving.auxiliary_mapping().result_size(),2u);

        ExactBoxType box({{-1,1},{-1,1}});
        paving.adjoin_outer_approximation(box,2u);
        ARIADNE_TESTING_PROVE(!paving.is_empty());
        ARIADNE_TESTING_PROVE(paving.size()>0u);
        BinaryTreePaving graph=paving.state_auxiliary_paving();
        ARIADNE_TESTING_EXPECT_EQUAL(graph.dimension(),4u);
        ARIADNE_TESTING_PROVE(!graph.is_empty());

        // Legacy names must remain mathematically equivalent while clients migrate.
        BinaryTreePaving legacy=paving.state_auxiliary_set();
        ARIADNE_TESTING_PROVE(subset(graph,legacy));
        ARIADNE_TESTING_PROVE(subset(legacy,graph));
        std::unique_ptr<AugmentedPaving> copy(paving.clone());
        ARIADNE_TESTING_EXPECT_EQUAL(copy->size(),paving.size());
        copy->clear();
        ARIADNE_TESTING_PROVE(copy->is_empty());
        ARIADNE_TESTING_PROVE(!paving.is_empty());
    }


    // Every approximation overload must preserve the state geometry of the
    // corresponding BinaryTreePaving operation. These are nontrivial sets,
    // rather than calls made only to mark forwarding functions as executed.
    Void test_approximation_forwarding() {
        Grid grid(1u);
        Mapping mapping=Mapping::identity(1u);
        ExactBoxType box{{0.125_x,0.875_x}};
        ExactBoxSetType regular(box);
        OpenSet<EffectiveTag,RealVector> open(box);
        OvertSet<EffectiveTag,RealVector> overt(box);
        CompactSet<EffectiveTag,RealVector> compact(box);
        ValidatedConstrainedImageSet validated(box);

        AugmentedPaving inner(grid,mapping);
        BinaryTreePaving expected_inner(grid);
        inner.adjoin_inner_approximation(regular,3u);
        expected_inner.adjoin_inner_approximation(regular,3u);
        ARIADNE_TESTING_PROVE(!inner.is_empty());
        ARIADNE_TESTING_PROVE(subset(inner.state_paving(),expected_inner));
        ARIADNE_TESTING_PROVE(subset(expected_inner,inner.state_paving()));

        AugmentedPaving lower(grid,mapping);
        BinaryTreePaving expected_lower(grid);
        lower.adjoin_lower_approximation(overt.reference(),0u,3u);
        expected_lower.adjoin_lower_approximation(overt.reference(),0u,3u);
        ARIADNE_TESTING_PROVE(!lower.is_empty());
        ARIADNE_TESTING_PROVE(subset(lower.state_paving(),expected_lower));
        ARIADNE_TESTING_PROVE(subset(expected_lower,lower.state_paving()));

        AugmentedPaving effective_outer(grid,mapping);
        BinaryTreePaving expected_effective_outer(grid);
        effective_outer.adjoin_outer_approximation(compact.reference(),2u);
        expected_effective_outer.adjoin_outer_approximation(compact.reference(),2u);
        ARIADNE_TESTING_PROVE(effective_outer.state_paving().superset(box));
        ARIADNE_TESTING_PROVE(subset(effective_outer.state_paving(),expected_effective_outer));
        ARIADNE_TESTING_PROVE(subset(expected_effective_outer,effective_outer.state_paving()));

        AugmentedPaving validated_outer(grid,mapping);
        BinaryTreePaving expected_validated_outer(grid);
        validated_outer.adjoin_outer_approximation(validated,2u);
        expected_validated_outer.adjoin_outer_approximation(validated,2u);
        ARIADNE_TESTING_PROVE(validated_outer.state_paving().superset(box));
        ARIADNE_TESTING_PROVE(subset(validated_outer.state_paving(),expected_validated_outer));
        ARIADNE_TESTING_PROVE(subset(expected_validated_outer,validated_outer.state_paving()));
    }

    // Exercise the public forwarding API with a fully enabled paving,
    // and check the resulting state rather than just the forwarding calls.
    Void test_state_operations() {
        Grid grid(1u);
        Mapping mapping=Mapping::identity(1u);
        BinaryTreePaving full_state(grid,true);
        AugmentedPaving full(full_state,mapping);
        AugmentedPaving other(grid,mapping);
        AugmentedPaving const& constant=full;

        ARIADNE_TESTING_PROVE(&constant.state_paving()==&constant.state_set());
        ARIADNE_TESTING_PROVE(&constant.auxiliary_mapping()==&constant.auxiliary_function());
        ARIADNE_TESTING_PROVE(&constant.auxiliary_mapping()==&constant.auxiliary_data());
        ARIADNE_TESTING_EXPECT_EQUAL(constant.grid(),grid);
        ARIADNE_TESTING_PROVE(constant.begin()!=constant.end());
        ARIADNE_TESTING_PROVE(other.begin()==other.end());

        std::ostringstream repr;
        repr << constant;
        ARIADNE_TESTING_PROVE(repr.str().find("AugmentedPaving(")==0u);

        ARIADNE_TESTING_PROVE(!subset(full,other));
        ARIADNE_TESTING_PROVE(!intersect(full,other));
        other.adjoin(full);
        ARIADNE_TESTING_PROVE(subset(full,other));
        ARIADNE_TESTING_PROVE(intersect(full,other));
        other.restrict(full);
        ARIADNE_TESTING_PROVE(subset(other,full));
        other.remove(full);
        ARIADNE_TESTING_PROVE(other.is_empty());

        full.mince(1u);
        ARIADNE_TESTING_PROVE(!full.is_empty());
        full.recombine();
        full.restrict_to_extent(0u);
        ARIADNE_TESTING_PROVE(!full.is_empty());
    }
};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_augmented_paving");
    ARIADNE_TESTING_ADD_TEST(TestAugmentedPaving,test_mapping_and_compatibility);
    ARIADNE_TESTING_ADD_TEST(TestAugmentedPaving,test_state_operations);
    ARIADNE_TESTING_ADD_TEST(TestAugmentedPaving,test_approximation_forwarding);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc,argv);
}
