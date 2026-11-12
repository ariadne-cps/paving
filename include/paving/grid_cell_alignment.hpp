/***************************************************************************
 *            paving/grid_cell_alignment.hpp
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

/** @file paving/grid_cell_alignment.hpp
 *  @brief Alignment and common ancestors of dyadic grid cells.
 */

#ifndef ARIADNE_PAVING_GRID_CELL_ALIGNMENT_HPP
#define ARIADNE_PAVING_GRID_CELL_ALIGNMENT_HPP

#include "paving/grid_cell.hpp"

namespace Ariadne {

//! \brief Express the path of a cell relative to a primary-cell extent.
//! \details Prepends the primary-cell subdivision path to the cell word.
//! This operation is independent of the paving representation.
//! \pre extent >= cell.root_extent().
BinaryWord cell_path_at_extent(GridCell const& cell, Nat extent);

//! \brief Return the smallest common binary-prefix ancestor of two cells.
//! \details Aligns both cells at their greater primary-cell extent, then
//! returns a cell corresponding to their longest common path prefix.
//! \pre The cells must belong to the same grid.
GridCell common_root_cell(GridCell const& first, GridCell const& second);

//! \brief Return the subdivision path from an ancestor to a descendant.
//! \details Returns an empty path for geometrically identical cells, even
//! if expressed with different primary-cell extents.
//! \pre Both cells must belong to the same grid, the ancestor's primary
//! extent must be no smaller, and the ancestor must contain the descendant.
BinaryWord relative_cell_path(
    GridCell const& ancestor, GridCell const& descendant);

} // namespace Ariadne

#endif // ARIADNE_PAVING_GRID_CELL_ALIGNMENT_HPP
