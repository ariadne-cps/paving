/***************************************************************************
 *            robdd_grid_paving.cpp
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

#include "utility/macros.hpp"
#include "paving/grid_paving.hpp"
#include "paving/robdd_grid_paving.hpp"

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

} // anonymous namespace

RobddGridPaving::RobddGridPaving(
        RobddManager& manager, GridTreeSubpaving const& source)
    : RobddGridPaving(
        manager,source.root_cell(),
        encode_tree(manager,source.binary_tree(),0u),source.tree_depth()) { }

RobddGridPaving::RobddGridPaving(
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

Bool RobddGridPaving::is_enabled(BinaryWord const& path) const {
    ARIADNE_PRECONDITION_MSG(path.size() == _depth,
        "Paving membership requires a path of the snapshot's logical depth");
    return _manager->evaluate(_root,path);
}

SizeType RobddGridPaving::enabled_cell_count() const {
    return _manager->model_count(_root,_depth);
}

Void RobddGridPaving::for_each_enabled_cell(
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

Void RobddGridPaving::mince_to_depth(SizeType new_depth) {
    ARIADNE_PRECONDITION_MSG(new_depth < std::numeric_limits<SizeType>::max(),
        "Requested subdivision depth exceeds the valid index range");
    _depth = std::max(_depth,new_depth);
}

Void RobddGridPaving::recombine() {
    auto variables = _manager->support(_root);
    _depth = variables.empty() ? 0u : variables.back().index()+1u;
}

std::unique_ptr<BinaryTreeNode> RobddGridPaving::to_binary_tree() const {
    return expand_tree(*_manager,_root,0u,_depth);
}

Void RobddGridPaving::_check_compatible(
        RobddGridPaving const& other) const {
    ARIADNE_PRECONDITION_MSG(_manager == other._manager,
        "ROBDD paving set operations require the same manager");
    ARIADNE_PRECONDITION_MSG(_root_cell.grid() == other._root_cell.grid(),
        "ROBDD paving set operations require identical grids");
}

GridCell RobddGridPaving::_common_root(
        RobddGridPaving const& other) const {
    _check_compatible(other);
    Nat extent = std::max(
        _root_cell.root_extent(),other._root_cell.root_extent());
    auto align_path = [extent](GridCell const& cell) {
        BinaryWord prefix = GridCell::primary_cell_path(
            cell.grid().dimension(),extent,cell.root_extent());
        prefix.append(cell.word());
        return prefix;
    };
    BinaryWord first = align_path(_root_cell);
    BinaryWord second = align_path(other._root_cell);
    BinaryWord prefix;
    SizeType limit = std::min(first.size(),second.size());
    for (SizeType i=0u; i<limit and first[i]==second[i]; ++i) {
        prefix.push_back(first[i]);
    }
    return GridCell(_root_cell.grid(),extent,prefix);
}

std::pair<Robdd,SizeType> RobddGridPaving::_lift_to(
        GridCell const& ancestor) const {
    ARIADNE_PRECONDITION_MSG(ancestor.grid() == _root_cell.grid(),
        "Cannot lift an ROBDD paving to another grid");
    ARIADNE_PRECONDITION_MSG(
        ancestor.root_extent() >= _root_cell.root_extent(),
        "Common root extent must enclose the source primary cell");

    BinaryWord path = GridCell::primary_cell_path(
        _root_cell.grid().dimension(),
        ancestor.root_extent(),_root_cell.root_extent());
    path.append(_root_cell.word());
    ARIADNE_PRECONDITION_MSG(
        ancestor.word().is_prefix(path),
        "Common root must contain the source paving root");
    SizeType offset = path.size() - ancestor.word().size();
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
        Bool right = path[ancestor.word().size()+i];
        lifted = right
            ? _manager->make_node(BddVariable(i),disabled,lifted)
            : _manager->make_node(BddVariable(i),lifted,disabled);
    }
    return std::make_pair(lifted,offset+_depth);
}

RobddGridPaving RobddGridPaving::join(
        RobddGridPaving const& other) const {
    GridCell common = _common_root(other);
    auto first = _lift_to(common);
    auto second = other._lift_to(common);
    return RobddGridPaving(
        *_manager,common,_manager->disjunction(first.first,second.first),
        std::max(first.second,second.second));
}

RobddGridPaving RobddGridPaving::intersection(
        RobddGridPaving const& other) const {
    GridCell common = _common_root(other);
    auto first = _lift_to(common);
    auto second = other._lift_to(common);
    return RobddGridPaving(
        *_manager,common,_manager->conjunction(first.first,second.first),
        std::max(first.second,second.second));
}

RobddGridPaving RobddGridPaving::difference(
        RobddGridPaving const& other) const {
    GridCell common = _common_root(other);
    auto first = _lift_to(common);
    auto second = other._lift_to(common);
    return RobddGridPaving(
        *_manager,common,
        _manager->conjunction(first.first,_manager->negation(second.first)),
        std::max(first.second,second.second));
}

Bool RobddGridPaving::subset(
        RobddGridPaving const& other) const {
    GridCell common = _common_root(other);
    auto first = _lift_to(common);
    auto second = other._lift_to(common);
    return _manager->conjunction(
        first.first,_manager->negation(second.first)).is_false();
}

Bool RobddGridPaving::intersects(
        RobddGridPaving const& other) const {
    GridCell common = _common_root(other);
    auto first = _lift_to(common);
    auto second = other._lift_to(common);
    return not _manager->conjunction(
        first.first,second.first).is_false();
}

} // namespace Ariadne
