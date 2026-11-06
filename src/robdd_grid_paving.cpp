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
        node->mince(depth-level);
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
    ARIADNE_PRECONDITION_MSG(_root_cell == other._root_cell,
        "ROBDD paving set operations require an identical root cell");
}

RobddGridPaving RobddGridPaving::join(
        RobddGridPaving const& other) const {
    _check_compatible(other);
    return RobddGridPaving(
        *_manager,_root_cell,_manager->disjunction(_root,other._root),
        std::max(_depth,other._depth));
}

RobddGridPaving RobddGridPaving::intersection(
        RobddGridPaving const& other) const {
    _check_compatible(other);
    return RobddGridPaving(
        *_manager,_root_cell,_manager->conjunction(_root,other._root),
        std::max(_depth,other._depth));
}

RobddGridPaving RobddGridPaving::difference(
        RobddGridPaving const& other) const {
    _check_compatible(other);
    return RobddGridPaving(
        *_manager,_root_cell,
        _manager->conjunction(_root,_manager->negation(other._root)),
        std::max(_depth,other._depth));
}

Bool RobddGridPaving::subset(
        RobddGridPaving const& other) const {
    _check_compatible(other);
    return _manager->conjunction(
        _root,_manager->negation(other._root)).is_false();
}

Bool RobddGridPaving::intersects(
        RobddGridPaving const& other) const {
    _check_compatible(other);
    return not _manager->conjunction(_root,other._root).is_false();
}

} // namespace Ariadne
