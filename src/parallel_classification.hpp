#pragma once

#include <functional>
#include "utility/typedefs.hpp"

namespace Ariadne {
class ThreadManager;

namespace PavingDetail {

int require_rounding_mode(int mode);

// Run disjoint classification jobs and join every worker before returning
// or propagating an error. A caller-thread failure takes precedence.
Void run_parallel_classification(SizeType count,
    std::function<Void(SizeType)> const& classify,ThreadManager& threading);

}
}
