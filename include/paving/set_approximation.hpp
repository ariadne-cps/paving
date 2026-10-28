/***************************************************************************
 *            set_approximation.hpp
 *
 *  Copyright  2026  Luca Geretti
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne.
 *
 *  Ariadne is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef ARIADNE_PAVING_SET_APPROXIMATION_HPP
#define ARIADNE_PAVING_SET_APPROXIMATION_HPP

#include "geometry/declarations.hpp"

namespace Ariadne {

class Grid;
class PavingInterface;
class GridTreePaving;
class ConstrainedImageSet;
class ValidatedConstrainedImageSet;
class ValidatedAffineConstrainedImageSet;

// Paving operations for set-owned representations. The set module does not depend on paving.
Void adjoin_outer_approximation_to(PavingInterface& paving, const ConstrainedImageSet& set, Nat fineness);
Void adjoin_outer_approximation_to(PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness);
GridTreePaving outer_approximation(const ValidatedConstrainedImageSet& set, const Grid& grid, Nat fineness);
Void adjoin_outer_approximation_to(PavingInterface& paving, const ValidatedAffineConstrainedImageSet& set, Nat fineness);
Void robust_adjoin_outer_approximation_to(PavingInterface& paving, const ValidatedAffineConstrainedImageSet& set, Nat fineness);
GridTreePaving outer_approximation(const ValidatedAffineConstrainedImageSet& set, const Grid& grid, Nat fineness);

} // namespace Ariadne

#endif
