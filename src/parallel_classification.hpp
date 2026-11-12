/***************************************************************************
 *            src/parallel_classification.hpp
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

/** @file src/parallel_classification.hpp
 *  @brief Parallel classification of cells during paving operations.
 */

#pragma once

#include <functional>
#include "utility/typedefs.hpp"

namespace Ariadne {
/** @brief ThreadManager undefined used by paving algorithms. */
class ThreadManager;

namespace PavingDetail {

/** @brief Perform the requested paving operation. */
int require_rounding_mode(int mode);

// Run disjoint classification jobs and join every worker before returning
// or propagating an error. A caller-thread failure takes precedence.
Void run_parallel_classification(SizeType count,
    std::function<Void(SizeType)> const& classify,ThreadManager& threading);

}
}
