/***************************************************************************
 *            robdd_grid_paving.hpp
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

#ifndef ARIADNE_PAVING_ROBDD_GRID_PAVING_HPP
#define ARIADNE_PAVING_ROBDD_GRID_PAVING_HPP

#include <functional>
#include <memory>
#include <utility>

#include "bdd/robdd.hpp"
#include "paving/binary_tree.hpp"
#include "paving/grid_cell.hpp"

namespace Ariadne {

class GridTreeSubpaving;

//! \brief Experimental immutable ROBDD-backed snapshot of a grid subpaving.
//! \details The GridCell stores the geometric root, while the Robdd stores
//! membership as a Boolean function of successive binary subdivision bits.
//! The fixed natural variable order is the subdivision order. This type is
//! additive: existing GridTreePaving/GridTreeCursor semantics are unchanged.
//! A shared RobddManager must outlive every snapshot and returned Robdd.
class RobddGridPaving {
  public:
    //! \brief Encode an existing explicit tree without retaining its node pointers.
    //! \details The root cell and maximal tree depth are copied.
    RobddGridPaving(RobddManager& manager, GridTreeSubpaving const& source);

    //! \brief Construct a snapshot from a Boolean root and grid metadata.
    //! \pre root belongs to manager, and its support lies in [0, depth).
    RobddGridPaving(
        RobddManager& manager, GridCell const& root_cell,
        Robdd const& root, SizeType depth);

    //! \brief Geometric root of the represented subpaving.
    GridCell const& root_cell() const { return _root_cell; }

    //! \brief Immutable Boolean root shared through the manager.
    Robdd const& robdd() const { return _root; }

    //! \brief Logical number of subdivision decisions below root_cell().
    SizeType depth() const { return _depth; }

    //! \brief True when no subdivision path is enabled.
    Bool is_empty() const { return _root.is_false(); }

    //! \brief Test an exact-depth path relative to the root cell.
    //! \pre path.size() equals depth().
    Bool is_enabled(BinaryWord const& path) const;

    //! \brief Number of enabled cells at the current logical depth.
    //! \details Counts full-length paths, not the number of compact leaves.
    SizeType enabled_cell_count() const;

    //! \brief Stream enabled cells in left-before-right binary path order.
    //! \details Each GridCell is passed by temporary reference; do not retain
    //! the reference after the callback. A longer logical depth may enumerate
    //! exponentially many cells even when the ROBDD is small.
    Void for_each_enabled_cell(
        std::function<void(GridCell const&)> const& visitor) const;

    //! \brief Increase logical depth without changing any ROBDD node.
    Void mince_to_depth(SizeType new_depth);

    //! \brief Drop unnecessary trailing subdivision levels.
    //! \details Canonical ROBDD reduction already removes equal branches.
    Void recombine();

    //! \brief Reconstruct a mutable explicit tree for migration comparisons.
    //! \details Expands all enabled paths to depth() and may be exponential.
    std::unique_ptr<BinaryTreeNode> to_binary_tree() const;

    //! \brief Return an immutable union with a paving on the same grid and manager.
    RobddGridPaving join(RobddGridPaving const& other) const;

    //! \brief Return an immutable intersection with a paving on the same grid and manager.
    RobddGridPaving intersection(RobddGridPaving const& other) const;

    //! \brief Return an immutable set difference with a paving on the same grid and manager.
    RobddGridPaving difference(RobddGridPaving const& other) const;

    //! \brief Test subset inclusion for compatible roots.
    Bool subset(RobddGridPaving const& other) const;

    //! \brief Test whether two compatible pavings intersect.
    Bool intersects(RobddGridPaving const& other) const;

  private:
    Void _check_compatible(RobddGridPaving const& other) const;
    GridCell _common_root(RobddGridPaving const& other) const;
    std::pair<Robdd,SizeType> _lift_to(GridCell const& ancestor) const;

    RobddManager* _manager;
    GridCell _root_cell;
    Robdd _root;
    SizeType _depth;
};

} // namespace Ariadne

#endif // ARIADNE_PAVING_ROBDD_GRID_PAVING_HPP
