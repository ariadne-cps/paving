#include "../src/parallel_classification.hpp"
#include "threading/thread_manager.hpp"
#include "ariadne-testing.hpp"
#include <array>
#include <cfenv>
#include <stdexcept>
#include <string>

using namespace Ariadne;

class TestParallelClassification : public Testing::ContextFixture {
public:
    using ContextFixture::ContextFixture;

    Void test_rounding_mode_read_results() {
        for(int mode:{FE_TONEAREST,FE_DOWNWARD,FE_UPWARD,FE_TOWARDZERO})
            ARIADNE_TESTING_EXPECT_EQUAL(PavingDetail::require_rounding_mode(mode),mode);
        ARIADNE_TESTING_EXPECT_THROWS(std::runtime_error,PavingDetail::require_rounding_mode(-1));
        ARIADNE_TESTING_EXPECT_THROWS(std::runtime_error,PavingDetail::require_rounding_mode(-2));
    }

    Void test_all_jobs_join_and_caller_error_precedence() {
        auto& threading=ThreadManager::instance();
        for(unsigned failures:{0u,1u,2u,3u}) {
            std::array<unsigned,3> completed{};
            std::string error;
            try {
                PavingDetail::run_parallel_classification(3u,[&](SizeType index) {
                    ++completed[index];
                    if(index==0u && (failures&1u)!=0u) throw std::runtime_error("caller");
                    if(index==1u && (failures&2u)!=0u) throw std::runtime_error("worker");
                },threading);
            } catch(std::runtime_error const& exception) { error=exception.what(); }
            for(auto count:completed) ARIADNE_TESTING_EXPECT_EQUAL(count,1u);
            std::string expected=(failures&1u)!=0u ? "caller" : ((failures&2u)!=0u ? "worker" : "");
            ARIADNE_TESTING_EXPECT_EQUAL(error,expected);
        }
        unsigned calls=0u;
        PavingDetail::run_parallel_classification(0u,[&](SizeType) { ++calls; },threading);
        ARIADNE_TESTING_EXPECT_EQUAL(calls,0u);
        PavingDetail::run_parallel_classification(1u,[&](SizeType) { ++calls; },threading);
        ARIADNE_TESTING_EXPECT_EQUAL(calls,1u);
    }
};

int main(int argc,char** argv) {
    ARIADNE_TESTING_SUITE("test_parallel_classification");
    ARIADNE_TESTING_ADD_TEST(TestParallelClassification,test_rounding_mode_read_results);
    ARIADNE_TESTING_ADD_TEST(TestParallelClassification,test_all_jobs_join_and_caller_error_precedence);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc,argv);
}
