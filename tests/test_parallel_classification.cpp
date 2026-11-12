/***************************************************************************
 *            tests/test_parallel_classification.cpp
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

/** @file tests/test_parallel_classification.cpp
 *  @brief Test paving geometry and backend behavior.
 */

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
