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
#include <mutex>
#include <utility>
#include <vector>

#include "bdd/robdd.hpp"
#include "tree/binary_tree.hpp"
#include "paving/grid_cell.hpp"
#include "paving/paving_interface.hpp"

namespace Ariadne {

class GridTreeSubpaving;
class SubPavingInterface;

//! \brief Experimental ROBDD-backed paving with value-like root updates.
//! \details The GridCell stores the geometric root, while the Robdd stores
//! membership as a Boolean function of successive binary subdivision bits.
//! Copies share immutable canonical ROBDD nodes, but changing one paving's
//! root leaves other copies unchanged. GridTreePaving/GridTreeCursor semantics
//! are unaffected. A shared RobddManager must outlive pavings and iterators.
class RobddPaving : public virtual PavingInterface {
  public:
    //! Snapshot copies and assignments synchronize with concurrent adjoin.
    //! Assignment requires the same grid; the manager and root may change.
    RobddPaving(RobddPaving const& other);
    RobddPaving& operator=(RobddPaving const& other);
    //! \brief Create an independent paving value for polymorphic handles.
    //! \details The copy shares only immutable manager-owned BDD nodes.
    RobddPaving* clone() const override;

    //! \brief Encode an existing explicit tree without retaining its node pointers.
    //! \details The root cell and maximal tree depth are copied.
    RobddPaving(RobddManager& manager, GridTreeSubpaving const& source);

    //! \brief Import any paving through the representation-neutral interface.
    //! \details Iterates its enabled cells into this manager; no explicit-tree
    //! conversion is required, but import cost depends on the cell count.
    RobddPaving(RobddManager& manager, SubPavingInterface const& source);

    //! \brief Construct a snapshot from a Boolean root and grid metadata.
    //! \pre root belongs to manager, and its support lies in [0, depth).
    RobddPaving(
        RobddManager& manager, GridCell const& root_cell,
        Robdd const& root, SizeType depth);

    //! \brief Build the exact union of arbitrary cells in one bottom-up pass.
    //! \details Builds a prefix trie and reduces it through the shared BDD
    //! manager. Duplicate, overlapping and ancestor/descendant cells are
    //! supported, including cells with different primary-cell extents.
    //! No per-cell BDD union or explicit paving tree is materialized.
    //! The manager must outlive the returned paving.
    //! \pre All cells belong to root_cell.grid() and lie inside root_cell.
    static RobddPaving from_cells(
        RobddManager& manager, GridCell const& root_cell,
        std::vector<GridCell> const& cells);

    //! \brief Geometric root of the represented subpaving.
    GridCell root_cell() const override;

    //! \brief Find the smallest primary grid cell enclosing a given box.
    //! \details Uses the same grid geometry and dimension check as the
    //! explicit-tree backend, regardless of the represented Boolean set.
    GridCell smallest_enclosing_primary_cell(UpperBoxType const& box) const override;

    //! \brief Axis-aligned bounding box of all enabled cells.
    //! \details The empty paving returns an empty box of the grid dimension.
    UpperBoxType bounding_box() const override;

    //! \brief Certify that the paving lies in the interior of a box.
    //! \details Uses the same conservative narrowing convention as the
    //! explicit binary-tree implementation.
    ValidatedLowerKleenean inside(ExactBoxType const& box) const override;

    //! \brief Certify separation from a box, including its boundary.
    //! \details Uses conservative widening of the query box.
    ValidatedLowerKleenean separated(ExactBoxType const& box) const override;

    //! \brief Certify nonempty intersection of interiors with a box.
    //! \details Uses conservative narrowing of the query box.
    ValidatedLowerKleenean overlaps(ExactBoxType const& box) const override;

    //! \brief Underlying grid shared by all cells in the paving.
    //! \warning This reference does not preserve its lifetime across
    //! concurrent assignment. Use root_cell() for a detached geometry value.
    Grid const& grid() const override { return _root_cell.grid(); }

    //! \brief Dimension of the underlying grid, synchronized with assignment.
    DimensionType dimension() const override;

    //! \brief Return a canonical root handle by value, not by unsafe reference.
    //! \details This accessor is coherent with concurrent adjoin.
    //! \brief Immutable Boolean root shared through the manager.
    Robdd robdd() const;

    //! \brief Logical number of subdivision decisions below root_cell().
    SizeType depth() const;

    //! \brief True when no subdivision path is enabled.
    Bool is_empty() const;

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
    SizeType size() const override { return enabled_leaf_count(); }

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

    //! \brief Subdivide logically to the grid-relative fineness.
    //! \details Matches the geometric depth chosen by the tree backend's
    //! mince(fineness): max(0,(root_extent+fineness)*dimension-
    //! root_cell.word().size()). Canonical true leaves remain compact,
    //! therefore size() does not change just because of logical mincing.
    Void mince(Nat fineness) override;

    //! \brief Increase logical depth without changing any ROBDD node.
    Void mince_to_depth(SizeType new_depth);

    //! \brief Drop unnecessary trailing subdivision levels.
    //! \details Canonical ROBDD reduction already removes equal branches.
    Void recombine() override;

    //! \brief Reconstruct a mutable explicit tree for migration comparisons.
    //! \details Expands all enabled paths to depth() and may be exponential.
    std::unique_ptr<BinaryTreeNode> to_binary_tree() const;

    //! \brief Enable or disable the entire root cell.
    //! \details Replaces the ROBDD root with a terminal and resets depth to 0.
    Void set_root_cell(Bool enabled) override;

    //! \brief Disable all cells without changing the geometric root.
    Void clear();

    //! \brief Thread-safe atomic union with a same-manager paving.
    //! \details Concurrent calls to adjoin on the same destination are
    //! serialized. Source pavings are copied as consistent snapshots.
    Void adjoin(RobddPaving const& other);

    //! \brief Atomically adjoin many cells under one paving lock.
    //! \details Empty input is a no-op; the current paving need not be empty.
    //! The geometric root is enlarged as required by the input cells.
    //! Input validation precedes changes to this paving's root value.
    Void adjoin(std::vector<GridCell> const& cells) override;

    //! \brief In-place union with a ROBDD-backed paving or view.
    //! \details Other backends require explicit construction from their
    //! SubPavingInterface; implicit backend migration is not permitted.
    Void adjoin(SubPavingInterface const& other) override;

    //! \brief In-place intersection with a paving on the same grid and manager.
    Void restrict(RobddPaving const& other);

    //! \brief In-place intersection with a representation-neutral subpaving.
    Void restrict(SubPavingInterface const& other) override;

    //! \brief In-place difference with a paving on the same grid and manager.
    Void remove(RobddPaving const& other);

    //! \brief In-place removal of any subpaving on the same grid.
    Void remove(SubPavingInterface const& other) override;

    //! \brief Adjoin the outer approximation of a compact set.
    //! \details Uses certified separation, optional open-set coverage,
    //! and the grid-relative target depth of GridTreePaving. Does not
    //! materialize an explicit binary tree.
    Void adjoin_outer_approximation(
        EffectiveEuclideanCompactSetInterface const& set, Nat fineness) override;

    //! \brief Adjoin a grid-aligned outer approximation of a box.
    //! \details Delegates through the compact-set overload, matching
    //! GridTreePaving's geometric and fineness conventions.
    Void adjoin_outer_approximation(UpperBoxType const& box, Nat fineness) override;

    //! \brief Adjoin the certified inner approximation of an open set.
    //! \details The primary cell extent bounds the search; only cells
    //! with a verified coverage proof are inserted into the ROBDD.
    Void adjoin_inner_approximation(
        EffectiveEuclideanOpenSetInterface const& set,
        Nat extent, Nat fineness) override;

    //! \brief Adjoin an inner approximation using the set's bounding box.
    //! \details Matches GridTreePaving's extent selection and then
    //! delegates to the open-set overload.
    Void adjoin_inner_approximation(
        EffectiveEuclideanSetInterface const& set, Nat fineness) override;

    //! \brief Adjoin a cell from the same grid.
    Void adjoin(GridCell const& cell) override;

    //! \brief Remove a cell from the same grid.
    Void remove(GridCell const& cell) override;

    //! \brief Return an immutable union with a paving on the same grid and manager.
    RobddPaving join(RobddPaving const& other) const;

    //! \brief Return an immutable intersection with a paving on the same grid and manager.
    RobddPaving intersection(RobddPaving const& other) const;

    //! \brief Return an immutable set difference with a paving on the same grid and manager.
    RobddPaving difference(RobddPaving const& other) const;

    //! \brief Test subset inclusion for compatible roots.
    Bool subset(RobddPaving const& other) const;

    //! \brief Test subset inclusion against an arbitrary subpaving.
    Bool subset(SubPavingInterface const& other) const override;

    //! \brief Test whether two compatible pavings intersect.
    Bool intersects(RobddPaving const& other) const;

    //! \brief Test intersection against an arbitrary subpaving.
    Bool intersects(SubPavingInterface const& other) const override;

    //! \brief Test geometric set equality against an arbitrary subpaving.
    Bool equals(SubPavingInterface const& other) const override;

    //! \brief Test whether the paving covers a geometric cell completely.
    Bool superset(GridCell const& cell) const override;

    //! \brief Return a detached subpaving restricted to a descendant cell.
    //! \details Repeatedly cofactoring each binary subdivision bit avoids
    //! materializing an explicit tree. The result has its own root metadata
    //! and shares only immutable ROBDD nodes with the original. A child
    //! with no remaining supported variables is represented by a terminal.
    //! \pre The requested cell is contained in root_cell() on the same grid.
    RobddPaving subpaving(GridCell const& cell) const;

    //! \brief Return a value-like snapshot restricted to one child cell.
    //! \details Delegates to subpaving(root_cell().split(right)).
    //! This is intentionally detached, unlike a writable SubPavingInterface
    //! branch view whose mutations must propagate to the parent.
    RobddPaving branch(Bool right) const;

  protected:
    //! \brief Create a live, non-owning child view through SubPavingInterface.
    SubPavingInterface* _branch(Bool right) const override;
    ForwardConstantIteratorInterface<GridCell>* _begin() const override;
    ForwardConstantIteratorInterface<GridCell>* _end() const override;

  private:
    struct CopyState {
        RobddManager* manager;
        GridCell root_cell;
        Robdd root;
        SizeType depth;
    };
    static CopyState _snapshot_copy(RobddPaving const& source);
    explicit RobddPaving(CopyState state);
    Void _adjoin_unlocked(RobddPaving const& source);
    // The caller must stabilize *this* and the source. No owner-lock
    // acquisition is allowed in these helpers: internal mutations already
    // hold the destination mutex; public value operations use snapshots.
    RobddPaving _join_unlocked(RobddPaving const& other) const;
    RobddPaving _intersection_unlocked(RobddPaving const& other) const;
    RobddPaving _difference_unlocked(RobddPaving const& other) const;

    OutputStream& _write(OutputStream& os) const override;
    Void _check_compatible(RobddPaving const& other) const;
    GridCell _common_root(RobddPaving const& other) const;
    std::pair<Robdd,SizeType> _lift_to(GridCell const& ancestor) const;

    RobddManager* _manager;
    GridCell _root_cell;
    Robdd _root;
    SizeType _depth;
    mutable std::mutex _adjoin_mutex;
};

} // namespace Ariadne

#endif // ARIADNE_PAVING_ROBDD_PAVING_HPP
