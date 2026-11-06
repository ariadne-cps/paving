/***************************************************************************
 *            paving_cell_operations.hpp
 *
 *  Copyright  2026  Luca Geretti
 ****************************************************************************/

/*
 *  This file is part of Ariadne Paving.
 *
 *  Ariadne Paving is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 */

#ifndef ARIADNE_PAVING_CELL_OPERATIONS_HPP
#define ARIADNE_PAVING_CELL_OPERATIONS_HPP

#include "utility/macros.hpp"
#include "paving/grid_cell.hpp"

namespace Ariadne {

//! \brief Enable or disable a cell contained in the root of a paving.
//! \details This operation requires only the common geometric and cell
//! mutation APIs and works with both concrete paving backends as well as
//! PavingInterface. It does not introduce a new backend-specific view.
//! \pre cell must belong to the same grid and lie inside the root cell.
//! \note This is an in-place operation on the owning paving, not a
//! detached subpaving snapshot.
template<class Paving>
Void set_paving_cell(Paving& paving, GridCell const& cell, Bool enabled) {
    ARIADNE_PRECONDITION_MSG(paving.grid() == cell.grid(),
        "Cannot modify a cell on a different grid");
    ARIADNE_PRECONDITION_MSG(
        cell.box().subset(paving.root_cell().box()),
        "The modified cell must be contained in the paving root");
    if (enabled) {
        paving.adjoin(cell);
    } else {
        paving.remove(cell);
    }
}

} // namespace Ariadne

#endif // ARIADNE_PAVING_CELL_OPERATIONS_HPP
