/***************************************************************************
 *            src/prepared_separation.hpp
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

/** @file src/prepared_separation.hpp
 *  @brief Precomputed geometric separation for paving algorithms.
 */

#ifndef ARIADNE_PAVING_PREPARED_SEPARATION_HPP
#define ARIADNE_PAVING_PREPARED_SEPARATION_HPP

#include "set/function_set.hpp"
#include <typeinfo>

namespace Ariadne::PavingDetail {

// Preparation snapshots the base implementation. Derived virtual predicates
// must retain their dispatch, and the solver requires positive dimension.
inline ConstrainedImageSet const* prepared_separation_source(
        EffectiveEuclideanCompactSetInterface const& set) {
    auto const* constrained=dynamic_cast<ConstrainedImageSet const*>(&set);
    if(constrained and typeid(*constrained)==typeid(ConstrainedImageSet)
       and constrained->dimension()!=0u) return constrained;
    return nullptr;
}

}
#endif
