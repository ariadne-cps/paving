/***************************************************************************
 *            paving/set_approximation.hpp
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

/** @file paving/set_approximation.hpp
 *  @brief Approximation of constrained image sets using pavings.
 */

#ifndef ARIADNE_PAVING_SET_APPROXIMATION_HPP
#define ARIADNE_PAVING_SET_APPROXIMATION_HPP

#include "geometry/declarations.hpp"

namespace Ariadne {

/** @brief Grid undefined used by paving algorithms. */
class Grid;
/** @brief PavingInterface undefined used by paving algorithms. */
class PavingInterface;
/** @brief BinaryTreePaving undefined used by paving algorithms. */
class BinaryTreePaving;
/** @brief ConstrainedImageSet undefined used by paving algorithms. */
class ConstrainedImageSet;
/** @brief ValidatedConstrainedImageSet undefined used by paving algorithms. */
class ValidatedConstrainedImageSet;
/** @brief ValidatedAffineConstrainedImageSet undefined used by paving algorithms. */
class ValidatedAffineConstrainedImageSet;

// Paving operations for set-owned representations. The set module does not depend on paving.
/** @brief Perform the requested paving operation. */
Void adjoin_outer_approximation_to(PavingInterface& paving, const ConstrainedImageSet& set, Nat fineness);
/** @brief Perform the requested paving operation. */
Void adjoin_outer_approximation_to(PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness);
/** @brief Perform the requested paving operation. */
BinaryTreePaving outer_approximation(const ValidatedConstrainedImageSet& set, const Grid& grid, Nat fineness);
/** @brief Perform the requested paving operation. */
Void adjoin_outer_approximation_to(PavingInterface& paving, const ValidatedAffineConstrainedImageSet& set, Nat fineness);
/** @brief Perform the requested paving operation. */
Void robust_adjoin_outer_approximation_to(PavingInterface& paving, const ValidatedAffineConstrainedImageSet& set, Nat fineness);
/** @brief Perform the requested paving operation. */
BinaryTreePaving outer_approximation(const ValidatedAffineConstrainedImageSet& set, const Grid& grid, Nat fineness);

} // namespace Ariadne

#endif
