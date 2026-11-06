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

RobddPaving::RobddPaving(
        RobddManager& manager, GridTreeSubpaving const& source)
    : RobddPaving(
        manager,source.root_cell(),
        encode_tree(manager,source.binary_tree(),0u),source.tree_depth()) { }

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

std::pair<Robdd,SizeType> RobddPaving::_lift_to(
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

} // namespace Ariadne
