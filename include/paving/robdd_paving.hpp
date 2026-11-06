/***************************************************************************
 *            robdd_paving.hpp
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

#ifndef ARIADNE_PAVING_ROBDD_PAVING_HPP
#define ARIADNE_PAVING_ROBDD_PAVING_HPP

#include <functional>
#include <memory>
#include <utility>

#include "bdd/robdd.hpp"
#include "paving/binary_tree.hpp"
#include "paving/grid_cell.hpp"
#include "paving/paving_interface.hpp"

namespace Ariadne {

class GridTreeSubpaving;

//! \brief Experimental ROBDD-backed paving with value-like root updates.
//! \details The GridCell stores the geometric root, while the Robdd stores
//! membership as a Boolean function of successive binary subdivision bits.
//! Copies share immutable canonical ROBDD nodes, but changing one paving's
//! root leaves other copies unchanged. GridTreePaving/GridTreeCursor semantics
//! are unaffected. A shared RobddManager must outlive pavings and iterators.
class RobddPaving {
  public:
    //! \brief Encode an existing explicit tree without retaining its node pointers.
    //! \details The root cell and maximal tree depth are copied.
    RobddPaving(RobddManager& manager, GridTreeSubpaving const& source);

    //! \brief Construct a snapshot from a Boolean root and grid metadata.
    //! \pre root belongs to manager, and its support lies in [0, depth).
    RobddPaving(
        RobddManager& manager, GridCell const& root_cell,
        Robdd const& root, SizeType depth);

    //! \brief Geometric root of the represented subpaving.
    GridCell const& root_cell() const { return _root_cell; }

    //! \brief Underlying grid shared by all cells in the paving.
    Grid const& grid() const { return _root_cell.grid(); }

    //! \brief Dimension of the underlying grid.
    DimensionType dimension() const { return grid().dimension(); }

    //! \brief Immutable Boolean root shared through the manager.
    Robdd const& robdd() const { return _root; }

    //! \brief Logical number of subdivision decisions below root_cell().
    SizeType depth() const { return _depth; }

    //! \brief True when no subdivision path is enabled.
    Bool is_empty() const { return _root.is_false(); }

    //! \brief Test an exact-depth path relative to the root cell.
    //! \pre path.size() equals depth().
    Bool is_enabled(BinaryWord const& path) const;

    //! \brief Count enabled cells at the declared logical depth.
    //! \details Every satisfying full-length path is a distinct cell, including
    //! all choices of absent (don't-care) variables. This differs from the
    //! number of enabled leaves in a recombined binary tree.
    SizeType enabled_cell_count() const;

    //! \brief Stream enabled cells at the declared depth, left before right.
    //! \details Emits full-length paths; true terminals expand over every
    //! remaining subdivision level. The callback's GridCell reference is
    //! valid only during the call. Enumeration may be exponential.
    Void for_each_enabled_cell(
        std::function<void(GridCell const&)> const& visitor) const;

    //! \brief Count enabled leaves of the canonically expanded, recombined tree.
    //! \details True terminals end an enabled leaf immediately; false terminals
    //! emit nothing. Skipped BDD variables are expanded in both directions, as
    //! they would be in an ordered binary tree. Unlike enabled_cell_count(),
    //! this count does not expand a true terminal to the declared depth.
    //! It matches the size of to_binary_tree() after recombine(), but an
    //! unrecombined explicit tree can contain additional redundant leaves.
    SizeType enabled_leaf_count() const;

    //! \brief Number of enabled leaves in the canonical tree interpretation.
    //! \details Unlike enabled_cell_count(), this does not depend on the
    //! requested subdivision depth after a true terminal. This matches the
    //! size of a recombined explicit binary-tree paving.
    SizeType size() const { return enabled_leaf_count(); }

    //! \brief Common forward-iterator interface used by other paving types.
    //! \details Iterates compact enabled leaves, not fixed-depth cells.
    //! The iterator retains its own traversal state and Boolean root snapshot;
    //! the RobddManager must outlive the iterator.
    using ConstIterator = ForwardConstantIteratorHandle<GridCell>;

    //! \brief Return an iterator to the first enabled compact leaf.
    ConstIterator begin() const;

    //! \brief Return a past-the-end iterator for compact leaves.
    ConstIterator end() const;

    //! \brief Visit enabled leaves of the compact ordered binary tree.
    //! \details A true terminal emits the shortest path at its current level.
    //! Skipped variables still yield separate left/right subtrees, and
    //! iteration is in left-before-right depth-first order, matching a
    //! recombined BinaryTreeNode representation. The callback's GridCell
    //! reference is temporary and must not be retained.
    Void for_each_enabled_leaf(
        std::function<void(GridCell const&)> const& visitor) const;

    //! \brief Increase logical depth without changing any ROBDD node.
    Void mince_to_depth(SizeType new_depth);

    //! \brief Drop unnecessary trailing subdivision levels.
    //! \details Canonical ROBDD reduction already removes equal branches.
    Void recombine();

    //! \brief Reconstruct a mutable explicit tree for migration comparisons.
    //! \details Expands all enabled paths to depth() and may be exponential.
    std::unique_ptr<BinaryTreeNode> to_binary_tree() const;

    //! \brief Enable or disable the entire root cell.
    //! \details Replaces the ROBDD root with a terminal and resets depth to 0.
    Void set_root_cell(Bool enabled);

    //! \brief Disable all cells without changing the geometric root.
    Void clear();

    //! \brief In-place union with a paving on the same grid and manager.
    Void adjoin(RobddPaving const& other);

    //! \brief In-place intersection with a paving on the same grid and manager.
    Void restrict(RobddPaving const& other);

    //! \brief In-place difference with a paving on the same grid and manager.
    Void remove(RobddPaving const& other);

    //! \brief Adjoin a cell from the same grid.
    Void adjoin(GridCell const& cell);

    //! \brief Remove a cell from the same grid.
    Void remove(GridCell const& cell);

    //! \brief Return an immutable union with a paving on the same grid and manager.
    RobddPaving join(RobddPaving const& other) const;

    //! \brief Return an immutable intersection with a paving on the same grid and manager.
    RobddPaving intersection(RobddPaving const& other) const;

    //! \brief Return an immutable set difference with a paving on the same grid and manager.
    RobddPaving difference(RobddPaving const& other) const;

    //! \brief Test subset inclusion for compatible roots.
    Bool subset(RobddPaving const& other) const;

    //! \brief Test whether two compatible pavings intersect.
    Bool intersects(RobddPaving const& other) const;

  private:
    Void _check_compatible(RobddPaving const& other) const;
    GridCell _common_root(RobddPaving const& other) const;
    std::pair<Robdd,SizeType> _lift_to(GridCell const& ancestor) const;

    RobddManager* _manager;
    GridCell _root_cell;
    Robdd _root;
    SizeType _depth;
};

} // namespace Ariadne

#endif // ARIADNE_PAVING_ROBDD_PAVING_HPP
