// Boundary tests for logical paving metadata, without allocating the
// exponentially large geometric sets described by those metadata.
#include <limits>
#include "paving/paving_arithmetic.hpp"
#include "ariadne-testing.hpp"
using namespace Ariadne;

class TestPavingArithmetic : public Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

    Void test_size_and_index_boundaries() {
        constexpr SizeType limit=std::numeric_limits<SizeType>::max();
        ARIADNE_TESTING_EXPECT_EQUAL(PavingDetail::checked_add(0u,0u),0u);
        ARIADNE_TESTING_EXPECT_EQUAL(PavingDetail::checked_add(limit-1u,1u),limit);
        ARIADNE_TESTING_EXPECT_EXCEPTION(PavingDetail::checked_add(limit,1u));
        ARIADNE_TESTING_EXPECT_EQUAL(PavingDetail::checked_add(3u,4u,7u),7u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(PavingDetail::checked_add(8u,0u,7u));
        ARIADNE_TESTING_EXPECT_EXCEPTION(PavingDetail::checked_add(3u,5u,7u));
        ARIADNE_TESTING_EXPECT_EQUAL(PavingDetail::checked_product(limit,0u),0u);
        ARIADNE_TESTING_EXPECT_EQUAL(PavingDetail::checked_product(0u,limit),0u);
        ARIADNE_TESTING_EXPECT_EQUAL(PavingDetail::checked_product(limit,1u),limit);
        ARIADNE_TESTING_EXPECT_EQUAL(PavingDetail::checked_product(limit/2u,2u),limit-1u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(PavingDetail::checked_product(limit/2u+1u,2u));
        ARIADNE_TESTING_EXPECT_EQUAL(PavingDetail::checked_product(3u,2u,7u),6u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(PavingDetail::checked_product(4u,2u,7u));

        ARIADNE_TESTING_EXPECT_EQUAL(PavingDetail::checked_index<SizeType>(limit),limit);

        constexpr SizeType nat_limit=std::numeric_limits<Nat>::max();
        constexpr SizeType offset_limit=std::numeric_limits<std::ptrdiff_t>::max();
        ARIADNE_TESTING_EXPECT_EQUAL(PavingDetail::checked_index<Nat>(nat_limit),nat_limit);
        ARIADNE_TESTING_EXPECT_EQUAL(PavingDetail::checked_index<std::ptrdiff_t>(offset_limit),
            std::numeric_limits<std::ptrdiff_t>::max());
        if constexpr (nat_limit<limit) {
            ARIADNE_TESTING_EXPECT_EXCEPTION(PavingDetail::checked_index<Nat>(nat_limit+1u));
        }
        ARIADNE_TESTING_EXPECT_EXCEPTION(PavingDetail::checked_index<std::ptrdiff_t>(offset_limit+1u));
    }

    Void test_grid_word_depth_boundaries() {
        constexpr SizeType limit=std::numeric_limits<Int>::max();
        ARIADNE_TESTING_EXPECT_EQUAL(PavingDetail::checked_cell_depth(2u,3u,2u),10u);
        ARIADNE_TESTING_EXPECT_EQUAL(PavingDetail::checked_cell_depth(limit-2u,1u,1u),limit-1u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(PavingDetail::checked_cell_depth(limit-1u,1u,1u));
        ARIADNE_TESTING_EXPECT_EXCEPTION(PavingDetail::checked_cell_depth(0u,0u,limit+1u));
        ARIADNE_TESTING_EXPECT_EQUAL(PavingDetail::checked_cell_depth(0u,0u,limit),0u);
        // A zero-dimensional cell needs no word bits, regardless of level.
        ARIADNE_TESTING_EXPECT_EQUAL(PavingDetail::checked_cell_depth(limit,limit,0u),0u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(PavingDetail::checked_cell_depth(
            std::numeric_limits<SizeType>::max(),1u,0u));
    }
};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_paving_arithmetic");
    ARIADNE_TESTING_ADD_TEST(TestPavingArithmetic,test_size_and_index_boundaries);
    ARIADNE_TESTING_ADD_TEST(TestPavingArithmetic,test_grid_word_depth_boundaries);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc,argv);
}
