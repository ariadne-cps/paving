/***************************************************************************
 *            paving/paving_interface.hpp
 *
 *  Copyright  2011-20  Pieter Collins
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

/** @file paving/paving_interface.hpp
 *  @brief Abstract interfaces and handles for paving representations.
 */

#ifndef ARIADNE_PAVING_INTERFACE_HPP
#define ARIADNE_PAVING_INTERFACE_HPP

#include <iosfwd>
#include <limits>
#include <utility>
#include <vector>

#include "geometry/declarations.hpp"
#include "geometry/set_interface.hpp"
#include "paving/grid_cell.hpp"
#include "paving/paving_arithmetic.hpp"

namespace Ariadne {

/** @brief Grid supports abstract interfaces and handles for paving representations. */
class Grid;
/** @brief GridCell supports abstract interfaces and handles for paving representations. */
class GridCell;

#define ARIADNE_ABSTRACT_METHOD                                                                                  \
    {                                                                                                            \
        throw std::runtime_error(StringType("ERROR: Unimplemented abstract method ") + ARIADNE_PRETTY_FUNCTION); \
    }

// A continuous predicate taking values in a three-valued logic.
// NOTE: Corresponds to your SetCheckerInterface; I think that this name is better.
template<class BS>
class PredicateInterface {
  public:
    /** @brief Perform the paving operation. */
    virtual ValidatedKleenean check(const BS&) = 0;
};

template<class T>
class ForwardConstantIteratorHandle;

template<class T>
class ForwardConstantIteratorInterface {
    friend class ForwardConstantIteratorHandle<T>;

  public:
    /** @brief Destroy this object. */
    virtual ~ForwardConstantIteratorInterface() = default;

  private:
    /** @brief Perform the paving operation. */
    virtual ForwardConstantIteratorInterface<T>* clone() const = 0;
    /** @brief Perform the paving operation. */
    virtual Void increment() = 0;
    /** @brief Perform the paving operation. */
    virtual const T& dereference() const = 0;
    /** @brief Perform the paving operation. */
    virtual Bool equals(const ForwardConstantIteratorInterface<T>&) const = 0;
    /** @brief Perform the paving operation. */
    virtual OutputStream& _write(OutputStream&) const = 0;
    friend OutputStream& operator<<(OutputStream& os, const ForwardConstantIteratorInterface<T>& self) {
        self._write(os);
        return os;
    }
};

//! \brief A generic forward Iterator through constant data.
//! \details Since the Iterator is through constant data, the data may be safely be returned by value rather than by reference,
//! though this is not strictly standards-conforming, and means that operator->() cannot be provided.
template<class T>
class ForwardConstantIteratorHandle {
    ForwardConstantIteratorInterface<T>* _ptr;

  public:
    ~ForwardConstantIteratorHandle() { delete _ptr; }
    ForwardConstantIteratorHandle(ForwardConstantIteratorInterface<T>* p) : _ptr(p) {}
    ForwardConstantIteratorHandle(const ForwardConstantIteratorInterface<T>& p) : _ptr(p.clone()) {}
    ForwardConstantIteratorHandle(const ForwardConstantIteratorHandle<T>& other) : _ptr(other._ptr->clone()) {}
    ForwardConstantIteratorHandle<T>& operator=(const ForwardConstantIteratorHandle<T>& other) {
        if (_ptr != other._ptr) {
            auto* copied = other._ptr->clone();
            delete _ptr;
            _ptr = copied;
        }
        return *this;
    }

    const T& operator*() const { return _ptr->dereference(); }
    const T* operator->() const { return &_ptr->dereference(); }
    ForwardConstantIteratorHandle<T>& operator++() {
        _ptr->increment();
        return *this;
    }
    ForwardConstantIteratorHandle<T> operator++(Int) {
        ForwardConstantIteratorHandle<T> tmp(*this);
        this->_ptr->increment();
        return tmp;
    }
    Bool operator==(const ForwardConstantIteratorHandle<T>& other) const { return this->_ptr->equals(*other._ptr); }
    Bool operator!=(const ForwardConstantIteratorHandle<T>& other) const { return !this->_ptr->equals(*other._ptr); }
    friend OutputStream& operator<<(OutputStream& os, const ForwardConstantIteratorHandle<T>& self) { return os << *self._ptr; }
    // friend OutputStream& operator<<(OutputStream& os, const ForwardConstantIteratorHandle<T>& self) { self._ptr->_write(os); return os; }
};

//! \brief A prototype interface for sets
//! \details Since the Iterator is through constant data, the data may be safely be returned by value rather than by reference,
//! though this is not strictly standards-conforming, and means that operator->() cannot be provided.
template<class BS>
class DenotableSetInterface {
  public:
    /** @brief Return the requested paving property. */
    inline ForwardConstantIteratorHandle<BS> begin() const;
    /** @brief Return the requested paving property. */
    inline ForwardConstantIteratorHandle<BS> end() const;
    /** @brief Modify the paving representation. */
    virtual Void adjoin(const BS&) = 0;
    /** @brief Modify the paving representation. */
    virtual Void adjoin(const DenotableSetInterface<BS>&) = 0;

  protected:
    /** @brief Perform the paving operation. */
    virtual ForwardConstantIteratorInterface<BS>* _begin() const = 0;
    /** @brief Perform the paving operation. */
    virtual ForwardConstantIteratorInterface<BS>* _end() const = 0;
};

/** @brief Paving supports abstract interfaces and handles for paving representations. */
class Paving;
/** @brief SubPaving supports abstract interfaces and handles for paving representations. */
class SubPaving;

/** @brief PavingInterface supports abstract interfaces and handles for paving representations. */
class PavingInterface;
/** @brief SubPavingInterface supports abstract interfaces and handles for paving representations. */
class SubPavingInterface;

//! \ingroup DiscretisedSetSubModule
//! \brief A prototype interface for subpavings.
//! \details A sub-paving is a paving-type set which is the restriction of another paving to a cell, called the \em root_cell of the subpaving.
//! In general, a sub-paving supports the non-modifying operations of a paving.
//!
//! The only modifying operations on a sub-paving which are logically allowable are those which apply to sub-cells of the root cell.
//! These operations modify the paving itself.
//!
//! \internal
//! NOTE: One difficulty with sub-pavings is that returning a sub-paving requires using either a raw pointer to SubPavingInterface with corresponding memory issues,
//! a shared pointer, which requires using pointer syntax, or a handle, which requires forwarding functions but is cleanest for users.
//! NOTE: Since operations modifying a sub-paving modify the original paving, we need to be careful with constantness, in particular when making copies.
//! This may be an issue in the branch method or when implementing Handle classes.
class SubPavingInterface
    : public virtual LocatedSetInterface<ValidatedTag, RealVector> {
    friend class SubPaving;
    friend class Paving;

  public:
    typedef ForwardConstantIteratorHandle<GridCell> ConstIterator;

  public:
    //! \brief A constant Iterator through the cells of the paving.
    inline ForwardConstantIteratorHandle<GridCell> begin() const { return this->_begin(); }
    inline ForwardConstantIteratorHandle<GridCell> end() const { return this->_end(); }
    //! \brief A sub-paving obtained by considering one of the immediate subcells of the root cell.
    inline SubPaving branch(Bool left_or_right) const;

    //! \brief Visit enabled cells at exactly the requested grid-relative fineness.
    //! \details Each emitted GridCell has word length
    //! (root_extent + fineness) * dimension. When the set contains finer
    //! detail, adjacent enabled leaves are coalesced only if they fully
    //! cover their parent target cell. An incompletely covered target cell
    //! is an error: silently rounding would alter the represented geometry.
    //! This method never alters the stored representation or logical depth.
    //! It works through the virtual set/iterator APIs in any backend,
    //! including borrowed child views; callbacks must not modify the owner.
    //! \note The output may be exponentially large. In particular, a
    //! requested depth above a non-primary view's root cannot describe
    //! that nonempty view as whole cells, and is rejected.
    template<class Visitor>
    Void for_each_enabled_cell_at_exact_fineness(
        Nat fineness, Visitor&& visitor) const;

    //! \brief Materialize a detached paving with enabled leaves at one
    //! exact grid-relative fineness.
    //! \details Returns a Paving owning an explicit binary-tree
    //! snapshot, even when the source is an ROBDD; the source is unmodified.
    //! The returned root is the primary cell of the source's root extent,
    //! which may be larger than a child-view root. Throws if exact
    //! coarsening would alter the set.
    Paving uniform_cell_paving(Nat fineness) const;

  public:
    //! \brief A dynamically-allocated copy.
    virtual SubPavingInterface* clone() const = 0;

    //! \brief The dimension of the paving.
    virtual DimensionType dimension() const = 0;

    //! \brief The underlying grid of the paving.
    virtual const Grid& grid() const = 0;

    //! \brief Number of enabled cells in this backend's current leaf iteration.
    //! \details A representation-dependent count, not a geometric invariant:
    //! a minced tree can expose more leaves than a canonical ROBDD.
    virtual SizeType size() const = 0;

    //! \brief The root cell of the (sub)paving. All enabled cells are subcells of the root cell.
    virtual GridCell root_cell() const = 0;
    //! \brief Tests if the cell \a c is contained in one of the cells of the subpaving.
    virtual Bool superset(const GridCell& c) const = 0; // Could also be called "contains"

    //! \brief Tests geometric equality with a subpaving of the same backend.
    //! \details Equality is independent of redundant subdivisions. Set
    //! operands from different backends are not interchangeable.
    virtual Bool equals(const SubPavingInterface&) const = 0;
    //! \brief Tests if the paving is a subset of another paving on the same Grid.
    virtual Bool subset(const SubPavingInterface&) const = 0;
    //! \brief Tests if the paving intersects another paving on the same Grid.
    //! Tests intersection of interiors, so returns false if the pavings touch but do not overlap.
    virtual Bool intersects(const SubPavingInterface&) const = 0;

    //! \brief Modify the underlying paving so that the root cell of the subpaving is either enabled or disabled.
    virtual Void set_root_cell(Bool onoff) = 0;

    virtual UpperBoxType bounding_box() const = 0;                              // Inherited from CompactSetInterface
    virtual ValidatedLowerKleenean inside(const ExactBoxType& bx) const = 0;    // Inherited from CompactSetInterface
    virtual ValidatedLowerKleenean separated(const ExactBoxType& bx) const = 0; // Inherited from ClosedSetInterface
    virtual ValidatedLowerKleenean overlaps(const ExactBoxType& bx) const = 0;  // Inherited from OvertSetInterface

    //! \brief Optional representation refinement preserving the geometric set.
    //! \details Explicit trees split stored enabled leaves, while a reduced
    //! backend may only adjust logical depth or leave a child view unchanged.
    //! The resulting size() and iterator granularity are backend-dependent.
    //! Fineness is relative to the grid's zero-level cell, not this root.
    //! \note Legacy operation; use for_each_enabled_cell_at_fineness()
    //! from paving_cell_operations.hpp for non-mutating refinement.
    virtual Void mince(Nat fineness) = 0;
    //! \brief Remove redundant representation subdivisions where supported.
    //! \details Preserves geometry. Recombining a child view must not change
    //! sibling geometry. Canonical views may have nothing to recombine.
    //! \note Legacy representation operation, not geometric normalization.
    virtual Void recombine() = 0;

  protected:
    virtual SubPavingInterface* _branch(Bool left_or_right) const = 0; // Could also be called "child" or "subpaving"
    /** @brief Perform the paving operation. */
    virtual ForwardConstantIteratorInterface<GridCell>* _begin() const = 0;
    /** @brief Perform the paving operation. */
    virtual ForwardConstantIteratorInterface<GridCell>* _end() const = 0;

  private:
    /** @brief Perform the paving operation. */
    virtual OutputStream& _write(OutputStream&) const = 0;
};

template<class Visitor>
Void SubPavingInterface::for_each_enabled_cell_at_exact_fineness(
    Nat fineness, Visitor&& visitor) const {
    const GridCell root = this->root_cell();
    const SizeType extent = static_cast<SizeType>(root.root_extent());
    const SizeType requested = static_cast<SizeType>(fineness);
    const SizeType dimensions = static_cast<SizeType>(this->dimension());
    const SizeType target = PavingDetail::checked_cell_depth(extent, requested, dimensions);

    // A non-primary subview may begin below the requested cell size.
    // A nonempty clipped region cannot be represented by a larger
    // target cell without adding geometry outside its own root.
    if (root.word().size() > target) {
        ARIADNE_PRECONDITION_MSG(this->begin() == this->end(),
                                 "Exact cell fineness is coarser than the nonempty view root");
        return;
    }

    auto ancestor_at_target = [&](const GridCell& cell) {
        BinaryWord word = cell.word();
        while (word.size() > target) {
            word.pop_back();
        }
        return GridCell(cell.grid(), cell.root_extent(), word);
    };

    // Prevalidate all fine cells so that a rejected coarsening does not
    // call the visitor with a partially emitted result. A full target
    // cell can be reconstructed from multiple smaller enabled cells.
    Bool validated = false;
    GridCell previous = root;
    for (auto it = this->begin(), end = this->end(); it != end; ++it) {
        if (it->word().size() <= target) {
            continue;
        }
        GridCell parent = ancestor_at_target(*it);
        if (not validated or parent != previous) {
            ARIADNE_PRECONDITION_MSG(this->superset(parent),
                                     "Requested exact fineness would discard finer set geometry");
            previous = parent;
            validated = true;
        }
    }

    // Leaf traversal in both backends follows left-to-right depth-first
    // order, so descendants of the same target cell are consecutive.
    Bool emitted = false;
    GridCell last = root;
    std::vector<GridCell> pending;
    for (auto it = this->begin(), end = this->end(); it != end; ++it) {
        if (it->word().size() > target) {
            GridCell parent = ancestor_at_target(*it);
            if (not emitted or parent != last) {
                visitor(parent);
                last = parent;
                emitted = true;
            }
        } else {
            pending.push_back(*it);
            while (not pending.empty()) {
                GridCell cell = std::move(pending.back());
                pending.pop_back();
                if (cell.word().size() == target) {
                    visitor(cell);
                    last = cell;
                    emitted = true;
                } else {
                    pending.push_back(cell.split(true));
                    pending.push_back(cell.split(false));
                }
            }
        }
    }
}

inline Bool operator==(const SubPavingInterface& p1, const SubPavingInterface& p2) { return p1.equals(p2); }
inline Bool subset(const SubPavingInterface& p1, const SubPavingInterface& p2) { return p1.subset(p2); }
inline Bool intersect(const SubPavingInterface& p1, const SubPavingInterface& p2) { return p1.intersects(p2); }

//! \ingroup DiscretisedSetSubModule
//! \brief A generic forward Iterator through constant data.
//! \details Since the Iterator is through constant data, the data may be safely be returned by value rather than by reference,
//! though this is not strictly standards-conforming, and means that operator->() cannot be provided.
class SubPaving {
    SubPavingInterface* _ptr;

  public:
    typedef ForwardConstantIteratorHandle<GridCell> ConstIterator;

  public:
    ~SubPaving() { delete _ptr; }
    SubPaving(SubPavingInterface* p) : _ptr(p) {}
    SubPaving(const SubPavingInterface& p) : _ptr(p.clone()) {}
    // Copying snapshots a subpaving; moving transfers the live view.
    // This matters for Python value returns: copying would detach the view
    // from its original paving when crossing the binding boundary.
    SubPaving(const SubPaving& other) : _ptr(other._ptr->clone()) {}
    SubPaving(SubPaving&& other) noexcept : _ptr(std::exchange(other._ptr, nullptr)) {}
    SubPaving& operator=(const SubPaving& other) {
        if (_ptr != other._ptr) {
            auto* copied = other._ptr->clone();
            delete _ptr;
            _ptr = copied;
        }
        return *this;
    }
    SubPaving& operator=(SubPaving&& other) noexcept {
        if (this != &other) {
            delete _ptr;
            _ptr = std::exchange(other._ptr, nullptr);
        }
        return *this;
    }
    operator SubPavingInterface&() { return *this->_ptr; }
    operator const SubPavingInterface&() const { return *this->_ptr; }

    const Grid& grid() const { return this->_ptr->grid(); }
    SizeType size() const { return this->_ptr->size(); }
    /** @brief Return the requested paving property. */
    GridCell root_cell() const { return this->_ptr->root_cell(); };
    Bool superset(const GridCell& c) const { return this->_ptr->superset(c); }
    Bool subset(const SubPavingInterface& p) const { return this->_ptr->subset(p); }
    Bool intersects(const SubPavingInterface& p) const { return this->_ptr->intersects(p); }
    Void set_root_cell(Bool onoff) { return this->_ptr->set_root_cell(onoff); }
    //! \brief Create a live child view of the underlying paving.
    SubPaving branch(Bool right) const;

    ForwardConstantIteratorHandle<GridCell> begin() const { return this->_ptr->_begin(); }
    ForwardConstantIteratorHandle<GridCell> end() const { return this->_ptr->_end(); }

    /** @brief Perform the paving operation. */
    UpperBoxType bounding_box() const { return this->_ptr->bounding_box(); };
    ValidatedLowerKleenean inside(const ExactBoxType& bx) const { return this->_ptr->inside(bx); }
    ValidatedLowerKleenean separated(const ExactBoxType& bx) const { return this->_ptr->separated(bx); }
    ValidatedLowerKleenean overlaps(const ExactBoxType& bx) const { return this->_ptr->overlaps(bx); }

    Void mince(Nat fineness) { this->_ptr->mince(fineness); }
    Void recombine() { this->_ptr->recombine(); }
    template<class Visitor>
    Void for_each_enabled_cell_at_exact_fineness(
        Nat fineness, Visitor&& visitor) const {
        this->_ptr->for_each_enabled_cell_at_exact_fineness(
            fineness, std::forward<Visitor>(visitor));
    }
    /** @brief Perform the paving operation. */
    Paving uniform_cell_paving(Nat fineness) const;

  private:
    OutputStream& _write(OutputStream& os) const { return this->_ptr->_write(os); }

  public:
    friend OutputStream& operator<<(OutputStream& os, const SubPaving& self) { return self._write(os); }
};

inline SubPaving SubPavingInterface::branch(Bool left_or_right) const { return this->_branch(left_or_right); }
inline SubPaving SubPaving::branch(Bool right) const { return this->_ptr->branch(right); }

//! \ingroup DiscretisedSetSubModule
//! \brief A paving, which is a set constructed as a finite union of non-overlapping cells, where the possible cells are obtained by successive subdivision.
class PavingInterface
    : public virtual SubPavingInterface {
  public:
    using SubPavingInterface::superset;
    virtual GridCell smallest_enclosing_primary_cell(const UpperBoxType& bx) const = 0; // Useful query, but can also be implemented at the Grid level.
    virtual Void adjoin_cells(const PredicateInterface<ExactBoxType>&, const Nat) { ARIADNE_ABSTRACT_METHOD; }
    //! Full geometric box coverage, including adjacent enabled cells.
    virtual Bool superset(const ExactBoxType& box) const = 0;
    /** @brief Modify the paving representation. */
    virtual Void adjoin_outer_approximation(const EffectiveEuclideanCompactSetInterface& set, const Nat fineness) = 0;
    /** @brief Modify the paving representation. */
    virtual Void adjoin_outer_approximation(const UpperBoxType& set, const Nat fineness) = 0;
    /** @brief Modify the paving representation. */
    virtual Void adjoin_outer_approximation(const ValidatedEuclideanCompactSetInterface& set, Nat fineness) = 0;
    /** @brief Modify the paving representation. */
    virtual Void adjoin_lower_approximation(const EffectiveEuclideanOvertSetInterface& set, Nat extent, Nat fineness) = 0;
    /** @brief Modify the paving representation. */
    virtual Void restrict_to_extent(Nat extent) = 0;
    /** @brief Modify the paving representation. */
    virtual Void adjoin_inner_approximation(const EffectiveEuclideanOpenSetInterface& set, const Nat extent, const Nat fineness) = 0;
    /** @brief Modify the paving representation. */
    virtual Void adjoin_inner_approximation(const EffectiveEuclideanSetInterface& set, const Nat fineness) = 0;
    /** @brief Modify the paving representation. */
    virtual Void adjoin(const SubPavingInterface& paving) = 0;
    /** @brief Modify the paving representation. */
    virtual Void restrict(const SubPavingInterface& paving) = 0;
    /** @brief Modify the paving representation. */
    virtual Void remove(const SubPavingInterface& paving) = 0;
    virtual Void adjoin(const GridCell& cell) = 0; // Deprecated?
    //! \brief Adjoin a sequence of cells, allowing backend-specific batching.
    //! \details BinaryTreePaving and RobddPaving override this operation so
    //! concurrent adjoin calls on one destination are serialized and a
    //! complete batch is published under one paving lock. The default loop
    //! is not an atomic batch for third-party backends.
    virtual Void adjoin(const std::vector<GridCell>& cells) {
        for (const GridCell& cell: cells) {
            this->adjoin(cell);
        }
    }
    virtual Void remove(const GridCell& cell) = 0; // Deprecated?
};

//! \ingroup DiscretisedSetSubModule
class Paving {
    PavingInterface* _ptr;

  public:
    typedef ForwardConstantIteratorHandle<GridCell> ConstIterator;

  public:
    ~Paving() { delete _ptr; }
    Paving(PavingInterface* p) : _ptr(p) {}
    Paving(const SubPavingInterface& p) : _ptr(&dynamic_cast<PavingInterface&>(*p.clone())) {}
    Paving(const Paving& other) : _ptr(&dynamic_cast<PavingInterface&>(*other._ptr->clone())) {}
    Paving& operator=(const Paving& other) {
        if (_ptr != other._ptr) {
            auto* copied = &dynamic_cast<PavingInterface&>(*other._ptr->clone());
            delete _ptr;
            _ptr = copied;
        }
        return *this;
    }
    operator PavingInterface&() { return *this->_ptr; }
    operator const PavingInterface&() const { return *this->_ptr; }

    const Grid& grid() const { return this->_ptr->grid(); }
    DimensionType dimension() const { return this->_ptr->dimension(); }
    Bool is_empty() const { return this->_ptr->size() == 0u; }
    Void clear() { this->_ptr->set_root_cell(false); }
    Void mince(Nat fineness) { this->_ptr->mince(fineness); }
    Void recombine() { this->_ptr->recombine(); }
    SizeType size() const { return this->_ptr->size(); }
    GridCell root_cell() const { return this->_ptr->root_cell(); }
    Bool superset(const GridCell& c) const { return this->_ptr->superset(c); }
    Bool superset(const ExactBoxType& box) const { return this->_ptr->superset(box); }
    Bool subset(const SubPavingInterface& p) const { return this->_ptr->subset(p); }
    Bool intersects(const SubPavingInterface& p) const { return this->_ptr->intersects(p); }
    Void set_root_cell(Bool onoff) { return this->_ptr->set_root_cell(onoff); }

    ForwardConstantIteratorHandle<GridCell> begin() const { return this->_ptr->_begin(); }
    ForwardConstantIteratorHandle<GridCell> end() const { return this->_ptr->_end(); }
    /** @brief Perform the paving operation. */
    UpperBoxType bounding_box() const { return this->_ptr->bounding_box(); };
    ValidatedLowerKleenean inside(const ExactBoxType& bx) const { return this->_ptr->inside(bx); }
    ValidatedLowerKleenean separated(const ExactBoxType& bx) const { return this->_ptr->separated(bx); }
    ValidatedLowerKleenean overlaps(const ExactBoxType& bx) const { return this->_ptr->overlaps(bx); }

    // GridCell smallest_enclosing_primary_cell(const ExactBoxType& bx) const { return this->_ptr->smallest_enclosing_primary_cell(); }
    Void adjoin_cells(const PredicateInterface<ExactBoxType>& predicate, const Nat fineness) { return this->_ptr->adjoin_cells(predicate, fineness); }
    Void adjoin_outer_approximation(const EffectiveEuclideanCompactSetInterface& set, const Nat fineness) { return this->_ptr->adjoin_outer_approximation(set, fineness); }
    Void adjoin_outer_approximation(const UpperBoxType& set, Nat fineness) { this->_ptr->adjoin_outer_approximation(set, fineness); }
    Void adjoin_outer_approximation(const ValidatedEuclideanCompactSetInterface& set, Nat fineness) { this->_ptr->adjoin_outer_approximation(set, fineness); }
    Void adjoin_lower_approximation(const EffectiveEuclideanOvertSetInterface& set, Nat extent, Nat fineness) { this->_ptr->adjoin_lower_approximation(set, extent, fineness); }
    Void restrict_to_extent(Nat extent) { this->_ptr->restrict_to_extent(extent); }
    Void adjoin_inner_approximation(const EffectiveEuclideanOpenSetInterface& set, const Nat extent, const Nat fineness) { return this->_ptr->adjoin_inner_approximation(set, extent, fineness); }
    Void adjoin_inner_approximation(const EffectiveEuclideanSetInterface& set, const Nat fineness) { return this->_ptr->adjoin_inner_approximation(set, fineness); }
    Void adjoin(const SubPavingInterface& paving) { return this->_ptr->adjoin(paving); }
    Void restrict(const SubPavingInterface& paving) { return this->_ptr->restrict(paving); }
    Void remove(const SubPavingInterface& paving) { return this->_ptr->remove(paving); }
    Void adjoin(const GridCell& cell) { return this->_ptr->adjoin(cell); }
    /** @brief Modify the paving representation. */
    Void adjoin(const std::vector<GridCell>& cells) {
        return this->_ptr->adjoin(cells);
    }
    Void remove(const GridCell& cell) { return this->_ptr->remove(cell); }
    template<class Visitor>
    Void for_each_enabled_cell_at_exact_fineness(
        Nat fineness, Visitor&& visitor) const {
        this->_ptr->for_each_enabled_cell_at_exact_fineness(
            fineness, std::forward<Visitor>(visitor));
    }
    /** @brief Perform the paving operation. */
    Paving uniform_cell_paving(Nat fineness) const {
        return this->_ptr->uniform_cell_paving(fineness);
    }

    friend OutputStream& operator<<(OutputStream& os, const Paving& self) { return os << *self._ptr; }
};

inline Paving SubPaving::uniform_cell_paving(Nat fineness) const {
    return this->_ptr->uniform_cell_paving(fineness);
}

/** @brief Perform the paving operation. */
inline Paving join(const SubPavingInterface& p1, const SubPavingInterface& p2) {
    Paving r(p1);
    r.adjoin(p2);
    return r;
}
/** @brief Perform the paving operation. */
inline Paving geometric_union(const SubPavingInterface& p1, const SubPavingInterface& p2) {
    Paving r(p1);
    r.adjoin(p2);
    return r;
}
/** @brief Perform the paving operation. */
inline Paving intersection(const SubPavingInterface& p1, const SubPavingInterface& p2) {
    Paving r(p1);
    r.restrict(p2);
    return r;
}
/** @brief Perform the paving operation. */
inline Paving difference(const SubPavingInterface& p1, const SubPavingInterface& p2) {
    Paving r(p1);
    r.remove(p2);
    return r;
}

} // namespace Ariadne

#endif // ARIADNE_PAVING_INTERFACE_HPP
