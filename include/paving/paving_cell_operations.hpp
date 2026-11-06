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
#include <memory>
#include <utility>
#include <vector>

#include "utility/macros.hpp"
#include "paving/grid_cell.hpp"
#include "paving/paving_interface.hpp"

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

//! \brief A snapshot-based iterator over cells clipped to a subpaving view.
//! \details A cloned iterator shares its immutable cell snapshot. End
//! comparisons work across independently created iterator snapshots of
//! the same owning paving and view root.
class PavingCellViewIterator final
    : public ForwardConstantIteratorInterface<GridCell> {
  public:
    PavingCellViewIterator(
            void const* owner, GridCell const& root,
            std::shared_ptr<std::vector<GridCell> const> cells,
            Bool past_end)
        : _owner(owner),_root(root),_cells(std::move(cells)),
          _position(past_end ? _cells->size() : 0u) { }

    PavingCellViewIterator* clone() const override {
        return new PavingCellViewIterator(*this);
    }

    Void increment() override {
        ARIADNE_PRECONDITION_MSG(_position < _cells->size(),
            "Cannot advance a past-the-end cell-view iterator");
        ++_position;
    }

    GridCell const& dereference() const override {
        ARIADNE_PRECONDITION_MSG(_position < _cells->size(),
            "Cannot dereference a past-the-end cell-view iterator");
        return (*_cells)[_position];
    }

    Bool equals(ForwardConstantIteratorInterface<GridCell> const& other)
            const override {
        auto const* rhs=dynamic_cast<PavingCellViewIterator const*>(&other);
        if (rhs==nullptr or _owner!=rhs->_owner or _root!=rhs->_root) {
            return false;
        }
        Bool ended=_position==_cells->size();
        Bool other_ended=rhs->_position==rhs->_cells->size();
        if (ended or other_ended) { return ended and other_ended; }
        return _position==rhs->_position and
            (*_cells)[_position]==(*rhs->_cells)[rhs->_position];
    }

    OutputStream& _write(OutputStream& os) const override {
        os << "PavingCellViewIterator(";
        if (_position==_cells->size()) { os << "end"; }
        else { os << (*_cells)[_position]; }
        return os << ")";
    }

  private:
    void const* _owner;
    GridCell _root;
    std::shared_ptr<std::vector<GridCell> const> _cells;
    SizeType _position;
};

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

    //! \brief Standard forward iterator over a cell snapshot of this view.
    //! \details The returned iterator retains the cells it observed at
    //! creation, even if the owner later changes. The owner and view
    //! must not be destroyed during comparisons with fresh iterators.
    using ConstIterator=ForwardConstantIteratorHandle<GridCell>;

    ConstIterator begin() const {
        auto cells=std::make_shared<std::vector<GridCell>>();
        for_each_enabled_leaf([&](GridCell const& cell) {
            cells->push_back(cell);
        });
        return ConstIterator(new PavingCellViewIterator(
            _paving,_root,cells,false));
    }

    ConstIterator end() const {
        auto cells=std::make_shared<std::vector<GridCell>>();
        return ConstIterator(new PavingCellViewIterator(
            _paving,_root,cells,true));
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

    //! \brief Bounding box of the enabled region restricted to this view.
    //! \details The empty view has an empty box of ambient dimension.
    UpperBoxType bounding_box() const {
        UpperBoxType bounds(dimension());
        Bool found=false;
        for_each_enabled_leaf([&](GridCell const& cell) {
            if (not found) {
                bounds=cell.box();
                found=true;
                return;
            }
            for (DimensionType i=0u; i<dimension(); ++i) {
                if (cell.box()[i].lower_bound().raw() <
                        bounds[i].lower_bound().raw()) {
                    bounds[i].set_lower_bound(cell.box()[i].lower_bound());
                }
                if (cell.box()[i].upper_bound().raw() >
                        bounds[i].upper_bound().raw()) {
                    bounds[i].set_upper_bound(cell.box()[i].upper_bound());
                }
            }
        });
        return bounds;
    }

    //! \brief Check inclusion using the same narrowed query box as
    //! GridTreeSubpaving::inside.
    ValidatedLowerKleenean inside(ExactBoxType const& box) const {
        ARIADNE_PRECONDITION_MSG(box.dimension() == dimension(),
            "View and query-box dimensions must agree");
        ExactBoxType narrowed=cast_exact_box(narrow(box));
        Bool contained=true;
        for_each_enabled_leaf([&](GridCell const& cell) {
            if (not cell.box().subset(narrowed)) { contained=false; }
        });
        return contained;
    }

    //! \brief Check separation against the conservatively widened box.
    ValidatedLowerKleenean separated(ExactBoxType const& box) const {
        ARIADNE_PRECONDITION_MSG(box.dimension() == dimension(),
            "View and query-box dimensions must agree");
        ExactBoxType widened=cast_exact_box(widen(box));
        Bool disjoint=true;
        for_each_enabled_leaf([&](GridCell const& cell) {
            if (cell.box().overlaps(widened)) { disjoint=false; }
        });
        return disjoint;
    }

    //! \brief Check interior overlap against the narrowed query box.
    ValidatedLowerKleenean overlaps(ExactBoxType const& box) const {
        ARIADNE_PRECONDITION_MSG(box.dimension() == dimension(),
            "View and query-box dimensions must agree");
        ExactBoxType narrowed=cast_exact_box(narrow(box));
        Bool intersects=false;
        for_each_enabled_leaf([&](GridCell const& cell) {
            if (cell.box().overlaps(narrowed)) { intersects=true; }
        });
        return intersects;
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

//! \brief Polymorphic bridge for a mutable cell view of a canonical paving.
//! \details The live view always borrows its owner. clone() instead creates a
//! detached owner snapshot of the selected cell and returns that snapshot as
//! SubPavingInterface. In particular, copied SubPavingHandles are independent,
//! while branch() continues to refer to the current owner.
//! \tparam Paving A PavingInterface implementation with a detached
//! subpaving(GridCell) operation and canonical, representation-only mincing.
//! \warning The owner must outlive this live adapter and its child views.
template<class Paving>
class PavingCellViewAdapter final : public SubPavingInterface {
  public:
    PavingCellViewAdapter(Paving& paving, GridCell const& root)
        : _paving(&paving), _view(paving,root) { }

    SubPavingInterface* clone() const override {
        return new Paving(_paving->subpaving(_view.root_cell()));
    }

    //! \brief Expose the concrete owning backend to same-backend dispatch.
    //! \details Allows PavingInterface operations to accept only their own
    //! backend's views, without importing or converting other backends.
    Paving const& owner() const { return *_paving; }

    DimensionType dimension() const override { return _view.dimension(); }
    Grid const& grid() const override { return _view.grid(); }
    SizeType size() const override { return _view.size(); }
    GridCell root_cell() const override { return _view.root_cell(); }
    Bool superset(GridCell const& cell) const override {
        return _view.superset(cell);
    }

    Bool equals(SubPavingInterface const& other) const override {
        return _paving->subpaving(_view.root_cell()).equals(other);
    }

    Bool subset(SubPavingInterface const& other) const override {
        return _paving->subpaving(_view.root_cell()).subset(other);
    }

    Bool intersects(SubPavingInterface const& other) const override {
        return _paving->subpaving(_view.root_cell()).intersects(other);
    }

    Void set_root_cell(Bool enabled) override {
        _view.set_root_cell(enabled);
    }

    UpperBoxType bounding_box() const override {
        return _view.bounding_box();
    }

    ValidatedLowerKleenean inside(ExactBoxType const& box) const override {
        return _view.inside(box);
    }

    ValidatedLowerKleenean separated(ExactBoxType const& box) const override {
        return _view.separated(box);
    }

    ValidatedLowerKleenean overlaps(ExactBoxType const& box) const override {
        return _view.overlaps(box);
    }

    Void mince(Nat fineness) override {
        // A canonical ROBDD already represents arbitrary refinement of
        // each enabled terminal without modifying the DAG. The view has
        // no persistent depth metadata: mincing it is geometrically a
        // no-op, unlike physically splitting a binary-tree subpaving.
        (void)fineness;
    }

    Void recombine() override {
        // The BDD manager already canonicalizes the DAG. Recombining
        // this view cannot change the owner or any of its sibling cells.
    }

  protected:
    SubPavingInterface* _branch(Bool right) const override {
        return new PavingCellViewAdapter(
            *_paving,_view.root_cell().split(right));
    }

    ForwardConstantIteratorInterface<GridCell>* _begin() const override {
        auto cells=std::make_shared<std::vector<GridCell>>();
        _view.for_each_enabled_leaf([&](GridCell const& cell) {
            cells->push_back(cell);
        });
        return new PavingCellViewIterator(
            _paving,_view.root_cell(),cells,false);
    }

    ForwardConstantIteratorInterface<GridCell>* _end() const override {
        auto cells=std::make_shared<std::vector<GridCell>>();
        return new PavingCellViewIterator(
            _paving,_view.root_cell(),cells,true);
    }

  private:
    OutputStream& _write(OutputStream& os) const override {
        return os << "PavingCellViewAdapter(" << _view.root_cell() << ")";
    }

    Paving* _paving;
    PavingCellView<Paving> _view;
};

} // namespace Ariadne

#endif // ARIADNE_PAVING_CELL_OPERATIONS_HPP
