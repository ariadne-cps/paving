/***************************************************************************
 *            src/parallel_classification.cpp
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

/** @file src/parallel_classification.cpp
 *  @brief Implement parallel classification operations.
 */

#include "parallel_classification.hpp"
#include "threading/task_group.hpp"
#include "threading/thread_manager.hpp"
#include <exception>
#include <stdexcept>

namespace Ariadne::PavingDetail {

int require_rounding_mode(int mode) {
    if(mode<0)
        throw std::runtime_error("Unable to read floating-point rounding mode");
    return mode;
}

Void run_parallel_classification(SizeType count,
        std::function<Void(SizeType)> const& classify,ThreadManager& threading) {
    if(count==0u) return;
    TaskGroup<ThreadManager> tasks(threading);
    for(SizeType index=1u;index<count;++index)
        tasks.fork([&,index] { classify(index); });
    std::exception_ptr failure;
    try { classify(0u); }
    catch(...) { failure=std::current_exception(); }
    try { tasks.join(); }
    catch(...) {
        if(not failure) failure=std::current_exception();
    }
    if(failure) std::rethrow_exception(failure);
}

}
