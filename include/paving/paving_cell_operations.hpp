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

#include <limits>

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

//! \brief A non-owning, backend-neutral mutable view rooted at a grid cell.
//! \details Supports cell membership, root-cell mutation and nested branch
//! navigation through the common paving APIs. Copies are views of the same
//! owner; they are NOT independent clones. The owning paving must outlive
//! all views. This intentionally does not implement SubPavingInterface:
//! full iteration, mincing and detached clone semantics need separate design.
//! \tparam Paving A mutable paving providing grid(), root_cell(),
//! superset(GridCell), adjoin(GridCell) and remove(GridCell).
template<class Paving>
class PavingCellView {
  public:
    PavingCellView(Paving& paving, GridCell const& root)
        : _paving(&paving), _root(root) {
        ARIADNE_PRECONDITION_MSG(paving.grid() == root.grid(),
            "Cannot create a cell view on a different grid");
        ARIADNE_PRECONDITION_MSG(
            root.box().subset(paving.root_cell().box()),
            "The cell view must lie within the owning paving root");
    }

    GridCell const& root_cell() const { return _root; }
    Grid const& grid() const { return _paving->grid(); }
    DimensionType dimension() const { return _root.dimension(); }

    //! \brief Test coverage only inside this view's geometric region.
    //! \details An out-of-view cell is not covered, even when the
    //! containing paving has that cell enabled.
    Bool superset(GridCell const& cell) const {
        ARIADNE_PRECONDITION_MSG(grid() == cell.grid(),
            "Cannot compare a cell view to a different grid");
        return cell.box().subset(_root.box()) and _paving->superset(cell);
    }

    //! \brief Visit the owner's enabled leaves clipped to this view.
    //! \details In a binary grid, two cells with overlapping interiors
    //! are either nested or equal. An owning leaf containing the view's
    //! root contributes precisely that root; otherwise only enabled
    //! leaves contained in the view contribute. This avoids materializing
    //! a new paving and works with either backend's constant iterators.
    //! \note Do not mutate the owner while this visit is in progress.
    template<class Visitor>
    Void for_each_enabled_leaf(Visitor&& visitor) const {
        for (auto it=_paving->begin(), end=_paving->end(); it!=end; ++it) {
            if (_root.box().subset(it->box())) {
                visitor(_root);
            } else if (it->box().subset(_root.box())) {
                visitor(*it);
            }
        }
    }

    //! \brief Number of enabled leaves in the restricted region.
    //! \details Reflects the owner's current stored leaf decomposition.
    //! An explicitly minced tree may have more leaves than a canonical
    //! ROBDD representing the same set until the tree is recombined.
    SizeType size() const {
        SizeType count=0u;
        for_each_enabled_leaf([&](GridCell const&) {
            ARIADNE_PRECONDITION_MSG(
                count < std::numeric_limits<SizeType>::max(),
                "Cell-view enabled-leaf count exceeds SizeType capacity");
            ++count;
        });
        return count;
    }

    //! \brief Update the owner at this view's root, not at its entire root.
    Void set_root_cell(Bool enabled) {
        set_paving_cell(*_paving,_root,enabled);
    }

    //! \brief Return a nested view of the same owning paving.
    //! \details Changes through either view affect their common owner.
    PavingCellView branch(Bool right) const {
        return PavingCellView(*_paving,_root.split(right));
    }

  private:
    Paving* _paving;
    GridCell _root;
};

//! \brief Deduce the backend when constructing a mutable cell view.
template<class Paving>
PavingCellView<Paving> paving_cell_view(
        Paving& paving, GridCell const& root) {
    return PavingCellView<Paving>(paving,root);
}

} // namespace Ariadne

#endif // ARIADNE_PAVING_CELL_OPERATIONS_HPP
