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
