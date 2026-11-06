/***************************************************************************
 *            robdd_paving.cpp
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

#include <algorithm>
#include <limits>
#include <optional>
#include <utility>
#include <vector>

#include "utility/macros.hpp"
#include "paving/grid_paving.hpp"
#include "paving/grid_cell_alignment.hpp"
#include "paving/paving_cell_operations.hpp"
#include "paving/robdd_paving.hpp"

namespace Ariadne {

namespace {

// Encode the explicit leaf/branch representation in natural variable order.
// A uniform leaf becomes a terminal, independently of its logical depth.
Robdd encode_tree(
        RobddManager& manager, BinaryTreeNode const* node, SizeType level) {
    ARIADNE_PRECONDITION_MSG(node != nullptr,
        "Cannot encode a null explicit paving tree");
    if (node->is_leaf()) {
        return manager.constant(node->is_enabled());
    }
    ARIADNE_PRECONDITION_MSG(level < std::numeric_limits<SizeType>::max()-1u,
        "Explicit tree exceeds the valid ROBDD variable index range");
    Robdd low = encode_tree(manager,node->left_node(),level+1u);
    Robdd high = encode_tree(manager,node->right_node(),level+1u);
    return manager.make_node(BddVariable(level),low,high);
}

std::unique_ptr<BinaryTreeNode> expand_tree(
        RobddManager& manager, Robdd const& root,
        SizeType level, SizeType depth) {
    if (root.is_false()) {
        return std::make_unique<BinaryTreeNode>(false);
    }
    if (root.is_true()) {
        auto node = std::make_unique<BinaryTreeNode>(true);
        SizeType remaining = depth-level;
        ARIADNE_PRECONDITION_MSG(
            remaining <= static_cast<SizeType>(std::numeric_limits<Nat>::max()),
            "Explicit binary-tree expansion depth exceeds Nat capacity");
        node->mince(static_cast<Nat>(remaining));
        return node;
    }

    ARIADNE_PRECONDITION_MSG(level < depth,
        "ROBDD support exceeds the snapshot's logical depth");
    auto top = manager.top_variable(root);
    ARIADNE_PRECONDITION_MSG(top.has_value() and top->index() >= level,
        "ROBDD variables must follow the subdivision order");
    Robdd low = top->index() == level ? manager.low(root) : root;
    Robdd high = top->index() == level ? manager.high(root) : root;
    auto low_tree = expand_tree(manager,low,level+1u,depth);
    auto high_tree = expand_tree(manager,high,level+1u,depth);

    auto node = std::make_unique<BinaryTreeNode>(false);
    node->split();
    node->left_node()->copy_from(low_tree.get());
    node->right_node()->copy_from(high_tree.get());
    return node;
}

// Enumerate enabled leaves in the ordered binary-tree interpretation.
// A true terminal closes a leaf, but skipped variables still introduce
// distinct left and right subtrees. Do not materialize an explicit tree.
void visit_enabled_leaf_paths(
        RobddManager& manager, Robdd const& root,
        SizeType level, SizeType depth, BinaryWord& path,
        std::function<void(BinaryWord const&)> const& visitor) {
    if (root.is_false()) { return; }
    if (root.is_true()) {
        visitor(path);
        return;
    }
    ARIADNE_PRECONDITION_MSG(level < depth,
        "ROBDD leaf traversal exceeded the declared logical depth");
    auto top = manager.top_variable(root);
    ARIADNE_PRECONDITION_MSG(top.has_value() and top->index() >= level,
        "ROBDD variable order is invalid during paving leaf traversal");

    Robdd low = top->index() == level ? manager.low(root) : root;
    Robdd high = top->index() == level ? manager.high(root) : root;
    path.push_back(false);
    visit_enabled_leaf_paths(manager,low,level+1u,depth,path,visitor);
    path.back() = true;
    visit_enabled_leaf_paths(manager,high,level+1u,depth,path,visitor);
    path.pop_back();
}

// Incremental depth-first iterator for the compact ordered tree represented
// by a semantic ROBDD. The frontier contains only pending subproblems and
// never materializes the expanded decision tree or its complete cell set.
class RobddPavingLeafIterator final
    : public ForwardConstantIteratorInterface<GridCell> {
    struct Pending {
        Robdd root;
        BinaryWord path;
        SizeType level;
    };

    RobddManager* _manager;
    GridCell _root_cell;
    Robdd _root;
    SizeType _depth;
    std::vector<Pending> _pending;
    std::optional<GridCell> _current;

  public:
    RobddPavingLeafIterator(
            RobddManager& manager, GridCell const& cell,
            Robdd const& root, SizeType depth, Bool past_end)
        : _manager(&manager), _root_cell(cell), _root(root), _depth(depth) {
        if (not past_end) {
            _pending.push_back(Pending{root,BinaryWord{},0u});
            _advance();
        }
    }

  private:
    Void _advance() {
        _current.reset();
        while (not _pending.empty()) {
            Pending next = std::move(_pending.back());
            _pending.pop_back();
            if (next.root.is_false()) { continue; }
            if (next.root.is_true()) {
                BinaryWord word = _root_cell.word();
                word.append(next.path);
                _current.emplace(
                    _root_cell.grid(),_root_cell.root_extent(),word);
                return;
            }

            ARIADNE_PRECONDITION_MSG(next.level < _depth,
                "ROBDD iterator encountered a variable past its depth");
            auto top = _manager->top_variable(next.root);
            ARIADNE_PRECONDITION_MSG(
                top.has_value() and top->index() >= next.level,
                "ROBDD iterator requires increasing variable indices");

            Robdd low = top->index() == next.level
                ? _manager->low(next.root) : next.root;
            Robdd high = top->index() == next.level
                ? _manager->high(next.root) : next.root;

            BinaryWord right_path = next.path;
            right_path.push_back(true);
            _pending.push_back(Pending{
                high,std::move(right_path),next.level+1u});
            next.path.push_back(false);
            _pending.push_back(Pending{
                low,std::move(next.path),next.level+1u});
        }
    }

    RobddPavingLeafIterator* clone() const override {
        return new RobddPavingLeafIterator(*this);
    }

    Void increment() override {
        ARIADNE_PRECONDITION_MSG(_current.has_value(),
            "Cannot increment a past-the-end ROBDD paving iterator");
        _advance();
    }

    GridCell const& dereference() const override {
        ARIADNE_PRECONDITION_MSG(_current.has_value(),
            "Cannot dereference a past-the-end ROBDD paving iterator");
        return *_current;
    }

    Bool equals(
            ForwardConstantIteratorInterface<GridCell> const& other) const override {
        auto const* rhs = dynamic_cast<RobddPavingLeafIterator const*>(&other);
        if (rhs == nullptr) { return false; }
        if (_manager != rhs->_manager or _root != rhs->_root
                or _root_cell != rhs->_root_cell or _depth != rhs->_depth) {
            return false;
        }
        if (not _current.has_value() or not rhs->_current.has_value()) {
            return not _current.has_value() and not rhs->_current.has_value();
        }
        return *_current == *rhs->_current;
    }

    OutputStream& _write(OutputStream& os) const override {
        os << "RobddPavingLeafIterator(";
        if (_current.has_value()) {
            os << *_current;
        } else {
            os << "end";
        }
        return os << ")";
    }
};

} // anonymous namespace

RobddPaving* RobddPaving::clone() const {
    return new RobddPaving(*this);
}

SubPavingInterface* RobddPaving::_branch(Bool right) const {
    return new PavingCellViewAdapter<RobddPaving>(
        const_cast<RobddPaving&>(*this),_root_cell.split(right));
}

ForwardConstantIteratorInterface<GridCell>* RobddPaving::_begin() const {
    return new RobddPavingLeafIterator(
        *_manager,_root_cell,_root,_depth,false);
}

ForwardConstantIteratorInterface<GridCell>* RobddPaving::_end() const {
    return new RobddPavingLeafIterator(
        *_manager,_root_cell,_root,_depth,true);
}

OutputStream& RobddPaving::_write(OutputStream& os) const {
    os << "RobddPaving(" << _root_cell << ",depth=" << _depth << ")";
    return os;
}

RobddPaving::RobddPaving(
        RobddManager& manager, GridTreeSubpaving const& source)
    : RobddPaving(
        manager,source.root_cell(),
        encode_tree(manager,source.binary_tree(),0u),source.tree_depth()) { }

RobddPaving::RobddPaving(
        RobddManager& manager, SubPavingInterface const& source)
    : RobddPaving(manager,source.root_cell(),manager.constant(false),0u) {
    for (auto cell=source.begin(), end=source.end(); cell!=end; ++cell) {
        adjoin(*cell);
    }
}

RobddPaving::RobddPaving(
        RobddManager& manager, GridCell const& root_cell,
        Robdd const& root, SizeType depth)
    : _manager(&manager), _root_cell(root_cell), _root(root), _depth(depth) {
    ARIADNE_PRECONDITION_MSG(depth < std::numeric_limits<SizeType>::max(),
        "ROBDD paving subdivision depth exceeds the valid index range");
    auto variables = manager.support(root); // Validates manager identity.
    ARIADNE_PRECONDITION_MSG(
        variables.empty() or variables.back().index() < depth,
        "ROBDD root refers to a subdivision outside the declared depth");
}

Bool RobddPaving::is_enabled(BinaryWord const& path) const {
    ARIADNE_PRECONDITION_MSG(path.size() == _depth,
        "Paving membership requires a path of the snapshot's logical depth");
    return _manager->evaluate(_root,path);
}

GridCell RobddPaving::smallest_enclosing_primary_cell(
        UpperBoxType const& box) const {
    ARIADNE_PRECONDITION_MSG(box.dimension() == dimension(),
        "Cannot find an enclosing primary cell for a box of another dimension");
    return GridCell::smallest_enclosing_primary_cell(box,grid());
}

UpperBoxType RobddPaving::bounding_box() const {
    auto current=begin(), finish=end();
    if (current == finish) {
        return ExactBoxType(dimension());
    }

    UpperBoxType bounds=current->box();
    for (++current; current!=finish; ++current) {
        UpperBoxType cell_box=current->box();
        for (DimensionType i=0u; i<dimension(); ++i) {
            if (cell_box[i].lower_bound().raw() < bounds[i].lower_bound().raw()) {
                bounds[i].set_lower_bound(cell_box[i].lower_bound());
            }
            if (cell_box[i].upper_bound().raw() > bounds[i].upper_bound().raw()) {
                bounds[i].set_upper_bound(cell_box[i].upper_bound());
            }
        }
    }
    return bounds;
}

ValidatedLowerKleenean RobddPaving::inside(
        ExactBoxType const& box) const {
    ARIADNE_PRECONDITION_MSG(box.dimension() == dimension(),
        "Paving and query-box dimensions must agree");
    ExactBoxType narrowed=cast_exact_box(narrow(box));
    for (auto cell=begin(), last=end(); cell!=last; ++cell) {
        if (not cell->box().subset(narrowed)) { return false; }
    }
    return true;
}

ValidatedLowerKleenean RobddPaving::separated(
        ExactBoxType const& box) const {
    ARIADNE_PRECONDITION_MSG(box.dimension() == dimension(),
        "Paving and query-box dimensions must agree");
    ExactBoxType widened=cast_exact_box(widen(box));
    for (auto cell=begin(), last=end(); cell!=last; ++cell) {
        if (cell->box().overlaps(widened)) { return false; }
    }
    return true;
}

ValidatedLowerKleenean RobddPaving::overlaps(
        ExactBoxType const& box) const {
    ARIADNE_PRECONDITION_MSG(box.dimension() == dimension(),
        "Paving and query-box dimensions must agree");
    ExactBoxType narrowed=cast_exact_box(narrow(box));
    for (auto cell=begin(), last=end(); cell!=last; ++cell) {
        if (cell->box().overlaps(narrowed)) { return true; }
    }
    return false;
}

SizeType RobddPaving::enabled_cell_count() const {
    return _manager->model_count(_root,_depth);
}

Void RobddPaving::for_each_enabled_cell(
        std::function<void(GridCell const&)> const& visitor) const {
    ARIADNE_PRECONDITION_MSG(static_cast<Bool>(visitor),
        "Enabled-cell visitor must be nonempty");
    _manager->for_each_satisfying_assignment(
        _root,_depth,[&](BinaryWord const& assignment) {
            BinaryWord path = _root_cell.word();
            path.append(assignment);
            GridCell cell(_root_cell.grid(),_root_cell.root_extent(),path);
            visitor(cell);
        });
}

SizeType RobddPaving::enabled_leaf_count() const {
    SizeType count=0u;
    BinaryWord path;
    visit_enabled_leaf_paths(*_manager,_root,0u,_depth,path,
        [&](BinaryWord const&) {
            ARIADNE_PRECONDITION_MSG(
                count < std::numeric_limits<SizeType>::max(),
                "Enabled-leaf count exceeds SizeType capacity");
            ++count;
        });
    return count;
}

RobddPaving::ConstIterator RobddPaving::begin() const {
    return ConstIterator(new RobddPavingLeafIterator(
        *_manager,_root_cell,_root,_depth,false));
}

RobddPaving::ConstIterator RobddPaving::end() const {
    return ConstIterator(new RobddPavingLeafIterator(
        *_manager,_root_cell,_root,_depth,true));
}

Void RobddPaving::for_each_enabled_leaf(
        std::function<void(GridCell const&)> const& visitor) const {
    ARIADNE_PRECONDITION_MSG(static_cast<Bool>(visitor),
        "Enabled-leaf visitor must be nonempty");
    BinaryWord path;
    visit_enabled_leaf_paths(*_manager,_root,0u,_depth,path,
        [&](BinaryWord const& relative_path) {
            BinaryWord complete_path = _root_cell.word();
            complete_path.append(relative_path);
            GridCell cell(_root_cell.grid(),_root_cell.root_extent(),complete_path);
            visitor(cell);
        });
}

Void RobddPaving::mince(Nat fineness) {
    // GridTreeSubpaving::mince uses this same geometric target, but
    // physically splits leaves. A ROBDD retains canonical shared nodes
    // and records the additional subdivisions only in its logical depth.
    constexpr SizeType limit=std::numeric_limits<SizeType>::max();
    SizeType extent=static_cast<SizeType>(_root_cell.root_extent());
    SizeType requested=static_cast<SizeType>(fineness);
    SizeType dimensions=static_cast<SizeType>(dimension());
    ARIADNE_PRECONDITION_MSG(extent <= limit-requested,
        "Requested paving fineness overflows the primary-cell level");
    SizeType levels=extent+requested;
    ARIADNE_PRECONDITION_MSG(
        dimensions == 0u or levels <= limit/dimensions,
        "Requested paving fineness overflows subdivision depth");
    SizeType absolute_depth=levels*dimensions;
    SizeType prefix_depth=_root_cell.word().size();
    SizeType desired=absolute_depth>prefix_depth
        ? absolute_depth-prefix_depth : 0u;
    mince_to_depth(desired);
}

Void RobddPaving::mince_to_depth(SizeType new_depth) {
    ARIADNE_PRECONDITION_MSG(new_depth < std::numeric_limits<SizeType>::max(),
        "Requested subdivision depth exceeds the valid index range");
    _depth = std::max(_depth,new_depth);
}

Void RobddPaving::recombine() {
    auto variables = _manager->support(_root);
    _depth = variables.empty() ? 0u : variables.back().index()+1u;
}

std::unique_ptr<BinaryTreeNode> RobddPaving::to_binary_tree() const {
    return expand_tree(*_manager,_root,0u,_depth);
}

Void RobddPaving::set_root_cell(Bool enabled) {
    _root = _manager->constant(enabled);
    _depth = 0u;
}

Void RobddPaving::clear() {
    set_root_cell(false);
}

Void RobddPaving::adjoin(RobddPaving const& other) {
    *this = join(other);
}

Void RobddPaving::adjoin(SubPavingInterface const& other) {
    ARIADNE_PRECONDITION_MSG(grid() == other.grid(),
        "Cannot adjoin a subpaving on another grid");
    adjoin(RobddPaving(*_manager,other));
}

Void RobddPaving::restrict(RobddPaving const& other) {
    *this = intersection(other);
}

Void RobddPaving::restrict(SubPavingInterface const& other) {
    ARIADNE_PRECONDITION_MSG(grid() == other.grid(),
        "Cannot restrict a paving with a subpaving on another grid");
    restrict(RobddPaving(*_manager,other));
}

Void RobddPaving::remove(RobddPaving const& other) {
    *this = difference(other);
}

Void RobddPaving::remove(SubPavingInterface const& other) {
    ARIADNE_PRECONDITION_MSG(grid() == other.grid(),
        "Cannot remove a subpaving on another grid");
    remove(RobddPaving(*_manager,other));
}

Void RobddPaving::adjoin_outer_approximation(
        EffectiveEuclideanCompactSetInterface const& set, Nat fineness) {
    ARIADNE_PRECONDITION_MSG(set.dimension() == dimension(),
        "Cannot approximate a set of another dimension");
    Nat extent=GridCell::smallest_enclosing_primary_cell_extent(
        set.bounding_box(),grid());
    Nat dimensions=static_cast<Nat>(dimension());
    constexpr SizeType limit=std::numeric_limits<SizeType>::max();
    constexpr Nat nat_limit=std::numeric_limits<Nat>::max();
    ARIADNE_PRECONDITION_MSG(dimensions <= nat_limit and
        extent <= nat_limit-dimensions,
        "Outer approximation primary-cell extent overflows");
    Nat enclosing_extent=extent+dimensions;
    ARIADNE_PRECONDITION_MSG(enclosing_extent <= nat_limit-fineness,
        "Outer approximation fineness overflows");
    SizeType levels=static_cast<SizeType>(enclosing_extent)+fineness;
    ARIADNE_PRECONDITION_MSG(
        dimensions == 0u or levels <= limit/dimensions,
        "Outer approximation subdivision depth overflows");
    SizeType target_depth=levels*dimensions;
    GridCell enclosing_root(grid(),enclosing_extent,BinaryWord{});
    auto const* open_set=dynamic_cast<
        EffectiveEuclideanOpenSetInterface const*>(
            static_cast<EuclideanSetInterfaceBase const*>(&set));

    // Perform geometric exploration before changing the ROBDD root.
    std::vector<GridCell> selected;
    std::function<Void(GridCell const&,SizeType)> visit =
        [&](GridCell const& cell, SizeType depth) {
            if (definitely(set.separated(cell.box()))) { return; }
            if (superset(cell)) { return; }
            if ((open_set and definitely(open_set->covers(cell.box())))
                    or depth==target_depth) {
                selected.push_back(cell);
                return;
            }
            visit(cell.split(false),depth+1u);
            visit(cell.split(true),depth+1u);
        };
    visit(enclosing_root,0u);
    for (GridCell const& cell : selected) { adjoin(cell); }
}

Void RobddPaving::adjoin_outer_approximation(
        UpperBoxType const& box, Nat fineness) {
    ARIADNE_PRECONDITION_MSG(box.dimension() == dimension(),
        "Cannot approximate a box of another dimension");
    ExactBoxSetType set=cast_exact_box(box);
    EffectiveEuclideanCompactSetInterface const& compact=set;
    adjoin_outer_approximation(compact,fineness);
}

Void RobddPaving::adjoin_inner_approximation(
        EffectiveEuclideanOpenSetInterface const& set,
        Nat extent, Nat fineness) {
    ARIADNE_PRECONDITION_MSG(set.dimension() == dimension(),
        "Cannot approximate a set of another dimension");
    constexpr SizeType limit=std::numeric_limits<SizeType>::max();
    ARIADNE_PRECONDITION_MSG(extent <= limit-fineness,
        "Inner approximation fineness overflows");
    SizeType levels=static_cast<SizeType>(extent)+fineness;
    SizeType dimensions=static_cast<SizeType>(dimension());
    ARIADNE_PRECONDITION_MSG(
        dimensions == 0u or levels <= limit/dimensions,
        "Inner approximation subdivision depth overflows");
    SizeType target_depth=levels*dimensions;
    GridCell enclosing_root(grid(),extent,BinaryWord{});
    auto const* closed_set=dynamic_cast<
        EffectiveEuclideanClosedSetInterface const*>(&set);

    // Only positively certified cells may enter an inner approximation.
    // Never expand a cell already fully enabled by the current paving.
    std::vector<GridCell> selected;
    std::function<Void(GridCell const&,SizeType)> visit =
        [&](GridCell const& cell, SizeType depth) {
            if (superset(cell)) { return; }
            if (definitely(set.covers(cell.box()))) {
                selected.push_back(cell);
                return;
            }
            if (closed_set and
                    definitely(closed_set->separated(cell.box()))) { return; }
            if (depth==target_depth) { return; }
            visit(cell.split(false),depth+1u);
            visit(cell.split(true),depth+1u);
        };
    visit(enclosing_root,0u);
    for (GridCell const& cell : selected) { adjoin(cell); }
}

Void RobddPaving::adjoin_inner_approximation(
        EffectiveEuclideanSetInterface const& set, Nat fineness) {
    ARIADNE_PRECONDITION_MSG(set.dimension() == dimension(),
        "Cannot approximate a set of another dimension");
    ExactBoxType bounding=cast_exact_box(set.bounding_box());
    Nat extent=GridCell::smallest_enclosing_primary_cell_extent(
        bounding,grid());
    EffectiveEuclideanOpenSetInterface const& open_set=set;
    adjoin_inner_approximation(open_set,extent,fineness);
}

Void RobddPaving::adjoin(GridCell const& cell) {
    RobddPaving singleton(*_manager,cell,_manager->constant(true),0u);
    adjoin(singleton);
}

Void RobddPaving::remove(GridCell const& cell) {
    RobddPaving singleton(*_manager,cell,_manager->constant(true),0u);
    remove(singleton);
}

Void RobddPaving::_check_compatible(
        RobddPaving const& other) const {
    ARIADNE_PRECONDITION_MSG(_manager == other._manager,
        "ROBDD paving set operations require the same manager");
    ARIADNE_PRECONDITION_MSG(_root_cell.grid() == other._root_cell.grid(),
        "ROBDD paving set operations require identical grids");
}

GridCell RobddPaving::_common_root(
        RobddPaving const& other) const {
    _check_compatible(other);
    return common_root_cell(_root_cell,other._root_cell);
}

std::pair<Robdd,SizeType> RobddPaving::_lift_to(
        GridCell const& ancestor) const {
    BinaryWord path = relative_cell_path(ancestor,_root_cell);
    SizeType offset = path.size();
    ARIADNE_PRECONDITION_MSG(
        offset < std::numeric_limits<SizeType>::max() - _depth,
        "Aligned paving depth exceeds the valid ROBDD variable range");
    ARIADNE_PRECONDITION_MSG(
        offset <= static_cast<SizeType>(std::numeric_limits<std::ptrdiff_t>::max()),
        "Aligned paving prefix exceeds the BDD shift-offset range");

    Robdd lifted = _manager->shift(
        _root,static_cast<std::ptrdiff_t>(offset));
    Robdd disabled = _manager->constant(false);
    for (SizeType i=offset; i!=0u; ) {
        --i;
        Bool right = path[i];
        lifted = right
            ? _manager->make_node(BddVariable(i),disabled,lifted)
            : _manager->make_node(BddVariable(i),lifted,disabled);
    }
    return std::make_pair(lifted,offset+_depth);
}

RobddPaving RobddPaving::join(
        RobddPaving const& other) const {
    GridCell common = _common_root(other);
    auto first = _lift_to(common);
    auto second = other._lift_to(common);
    return RobddPaving(
        *_manager,common,_manager->disjunction(first.first,second.first),
        std::max(first.second,second.second));
}

RobddPaving RobddPaving::intersection(
        RobddPaving const& other) const {
    GridCell common = _common_root(other);
    auto first = _lift_to(common);
    auto second = other._lift_to(common);
    return RobddPaving(
        *_manager,common,_manager->conjunction(first.first,second.first),
        std::max(first.second,second.second));
}

RobddPaving RobddPaving::difference(
        RobddPaving const& other) const {
    GridCell common = _common_root(other);
    auto first = _lift_to(common);
    auto second = other._lift_to(common);
    return RobddPaving(
        *_manager,common,
        _manager->conjunction(first.first,_manager->negation(second.first)),
        std::max(first.second,second.second));
}

Bool RobddPaving::subset(
        RobddPaving const& other) const {
    GridCell common = _common_root(other);
    auto first = _lift_to(common);
    auto second = other._lift_to(common);
    return _manager->conjunction(
        first.first,_manager->negation(second.first)).is_false();
}

Bool RobddPaving::intersects(
        RobddPaving const& other) const {
    GridCell common = _common_root(other);
    auto first = _lift_to(common);
    auto second = other._lift_to(common);
    return not _manager->conjunction(
        first.first,second.first).is_false();
}

Bool RobddPaving::subset(SubPavingInterface const& other) const {
    ARIADNE_PRECONDITION_MSG(grid() == other.grid(),
        "Cannot compare pavings from different grids");
    return subset(RobddPaving(*_manager,other));
}

Bool RobddPaving::intersects(SubPavingInterface const& other) const {
    ARIADNE_PRECONDITION_MSG(grid() == other.grid(),
        "Cannot compare pavings from different grids");
    return intersects(RobddPaving(*_manager,other));
}

Bool RobddPaving::equals(SubPavingInterface const& other) const {
    ARIADNE_PRECONDITION_MSG(grid() == other.grid(),
        "Cannot compare pavings from different grids");
    RobddPaving imported(*_manager,other);
    return subset(imported) and imported.subset(*this);
}

Bool RobddPaving::superset(GridCell const& cell) const {
    ARIADNE_PRECONDITION_MSG(grid() == cell.grid(),
        "Cannot test cell containment across different grids");
    RobddPaving required(*_manager,cell,_manager->constant(true),0u);
    return required.subset(*this);
}

RobddPaving RobddPaving::subpaving(GridCell const& cell) const {
    BinaryWord path=relative_cell_path(_root_cell,cell);
    RobddPaving restricted(*this);
    for (SizeType i=0u; i<path.size(); ++i) {
        restricted=restricted.branch(path[i]);
    }
    return restricted;
}

RobddPaving RobddPaving::branch(Bool right) const {
    Robdd restricted = _root;
    SizeType child_depth = 0u;
    if (_depth != 0u) {
        restricted = _manager->restrict(_root,BddVariable(0u),right);
        restricted = _manager->shift(restricted,-1);
        child_depth = _depth-1u;
    }
    return RobddPaving(*_manager,_root_cell.split(right),restricted,child_depth);
}

} // namespace Ariadne
