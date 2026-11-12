/***************************************************************************
 *            src/robdd_paving.cpp
 *
 *  Copyright  2026  Luca Geretti
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

/** @file src/robdd_paving.cpp
 *  @brief Implement robdd paving operations.
 */

#include <algorithm>
#include <cfenv>
#include <exception>
#include <functional>
#include <limits>
#include <memory>
#include <optional>
#include <typeinfo>
#include <stdexcept>
#include <optional>
#include <utility>
#include <vector>

#include "utility/macros.hpp"
#include "paving/binary_tree_paving.hpp"
#include "set/function_set.hpp"
#include "paving/grid_cell_alignment.hpp"
#include "paving/paving_cell_operations.hpp"
#include "paving/robdd_paving.hpp"
#include "parallel_classification.hpp"
#include "prepared_separation.hpp"
#include "threading/floating_point_environment.hpp"
#include "threading/task_group.hpp"
#include "threading/thread_manager.hpp"

namespace Ariadne {

namespace {

RobddManager& checked_manager(std::shared_ptr<RobddManager> const& manager) {
    ARIADNE_PRECONDITION_MSG(manager != nullptr,
        "A shared ROBDD paving requires a non-null manager");
    return *manager;
}

// Dense Boolean prefix trie with stable integer indices. This avoids
// allocating a separate heap node for every path bit. Vector growth never
// invalidates an index, including when inserting a late ancestor.
struct CellPrefixTrie {
    static constexpr SizeType missing=std::numeric_limits<SizeType>::max();
    struct Node {
        Bool covered=false;
        SizeType low=missing;
        SizeType high=missing;
    };

    std::vector<Node> nodes{Node{}};

    void insert(BinaryWord const& path) {
        SizeType current=0u;
        for(SizeType i=0u;i<path.size();++i) {
            if(nodes[current].covered) { return; }
            // Do not retain a Node reference across emplace_back(), which
            // may reallocate the arena and invalidate all references.
            Bool branch=path[i];
            SizeType child=branch ? nodes[current].high : nodes[current].low;
            if(child==missing) {
                child=nodes.size();
                nodes.emplace_back();
                if(branch) { nodes[current].high=child; }
                else { nodes[current].low=child; }
            }
            current=child;
        }
        // A late covering ancestor subsumes descendants. Orphaned arena
        // nodes are ignored by the encoder and freed with this local trie.
        nodes[current].covered=true;
        nodes[current].low=missing;
        nodes[current].high=missing;
    }
};

Robdd encode_cell_trie(
        RobddManager& manager,CellPrefixTrie const& trie,
        SizeType index,SizeType level) {
    if(index==CellPrefixTrie::missing) { return manager.constant(false); }
    auto const& node=trie.nodes[index];
    if(node.covered) { return manager.constant(true); }
    if(node.low==CellPrefixTrie::missing
       and node.high==CellPrefixTrie::missing)
        return manager.constant(false);

    Robdd low=encode_cell_trie(manager,trie,node.low,level+1u);
    Robdd high=encode_cell_trie(manager,trie,node.high,level+1u);
    // A canonical ROBDD removes nodes with equal children. Avoid a
    // manager call (and its validation/interning work) in this case.
    if(low==high) { return low; }
    return manager.make_node(BddVariable(level),low,high);
}

// Encode the explicit leaf/branch representation in natural variable order.
// A uniform leaf becomes a terminal, independently of its logical depth.
Robdd encode_tree(
        RobddManager& manager, BinaryTreeNode const* node, SizeType level) {
    // BinaryTreeSubpaving validates the root; non-leaves own both children.
    if (node->is_leaf()) {
        return manager.constant(node->is_enabled());
    }
    SizeType next=PavingDetail::checked_add(
        level,1u,std::numeric_limits<SizeType>::max()-1u);
    Robdd low = encode_tree(manager,node->left_node(),next);
    Robdd high = encode_tree(manager,node->right_node(),next);
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
        node->mince(PavingDetail::checked_index<Nat>(depth-level));
        return node;
    }

    // The constructor validates support < depth. Nonterminal nodes have
    // a top variable, and manager-owned edges strictly increase indices.
    // Taking an edge only at its own level preserves level <= top < depth.
    auto top = manager.top_variable(root);
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
        SizeType level, BinaryWord& path,
        std::function<void(BinaryWord const&)> const& visitor) {
    if (root.is_false()) { return; }
    if (root.is_true()) {
        visitor(path);
        return;
    }
    // As in expand_tree, a nonterminal has top >= level by ordered edges.
    auto top = manager.top_variable(root);

    Robdd low = top->index() == level ? manager.low(root) : root;
    Robdd high = top->index() == level ? manager.high(root) : root;
    path.push_back(false);
    visit_enabled_leaf_paths(manager,low,level+1u,path,visitor);
    path.back() = true;
    visit_enabled_leaf_paths(manager,high,level+1u,path,visitor);
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

    std::shared_ptr<RobddManager> _manager_owner;
    RobddManager* _manager;
    GridCell _root_cell;
    Robdd _root;
    SizeType _depth;
    std::vector<Pending> _pending;
    std::optional<GridCell> _current;

  public:
    RobddPavingLeafIterator(
            std::shared_ptr<RobddManager> manager_owner,
            RobddManager& manager, GridCell const& cell,
            Robdd const& root, SizeType depth, Bool past_end)
        : _manager_owner(std::move(manager_owner)),
          _manager(&manager), _root_cell(cell), _root(root), _depth(depth) {
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

            // Each pending item preserves the same ordered-edge invariant
            // as expand_tree; the snapshotted root already validates depth.
            auto top = _manager->top_variable(next.root);

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

GridCell RobddPaving::root_cell() const {
    std::lock_guard<std::mutex> lock(_adjoin_mutex);
    return _root_cell;
}

DimensionType RobddPaving::dimension() const {
    std::lock_guard<std::mutex> lock(_adjoin_mutex);
    return _root_cell.grid().dimension();
}

Robdd RobddPaving::robdd() const {
    std::lock_guard<std::mutex> lock(_adjoin_mutex);
    return _root;
}

SizeType RobddPaving::depth() const {
    std::lock_guard<std::mutex> lock(_adjoin_mutex);
    return _depth;
}

Bool RobddPaving::is_empty() const {
    std::lock_guard<std::mutex> lock(_adjoin_mutex);
    return _root.is_false();
}

RobddPaving* RobddPaving::clone() const {
    return new RobddPaving(*this);
}

SubPavingInterface* RobddPaving::_branch(Bool right) const {
    return new PavingCellViewAdapter<RobddPaving>(
        const_cast<RobddPaving&>(*this),root_cell().split(right));
}

ForwardConstantIteratorInterface<GridCell>* RobddPaving::_begin() const {
    CopyState state=_snapshot_copy(*this);
    return new RobddPavingLeafIterator(
        state.manager_owner,*state.manager,state.root_cell,state.root,state.depth,false);
}

ForwardConstantIteratorInterface<GridCell>* RobddPaving::_end() const {
    CopyState state=_snapshot_copy(*this);
    return new RobddPavingLeafIterator(
        state.manager_owner,*state.manager,state.root_cell,state.root,state.depth,true);
}

OutputStream& RobddPaving::_write(OutputStream& os) const {
    CopyState state=_snapshot_copy(*this);
    os << "RobddPaving(" << state.root_cell << ",depth=" << state.depth << ")";
    return os;
}

RobddPaving::CopyState RobddPaving::_snapshot_copy(
        RobddPaving const& source) {
    std::lock_guard<std::mutex> lock(source._adjoin_mutex);
    return CopyState{source._manager_owner,source._manager,
                     source._root_cell,source._root,source._depth};
}

RobddPaving::RobddPaving(CopyState state)
    : _manager_owner(std::move(state.manager_owner)),
      _manager(state.manager),_root_cell(std::move(state.root_cell)),
      _root(state.root),_depth(state.depth) { }

RobddPaving::RobddPaving(RobddPaving const& other)
    : RobddPaving(_snapshot_copy(other)) { }

RobddPaving& RobddPaving::operator=(RobddPaving const& other) {
    if(this!=&other) {
        CopyState state=_snapshot_copy(other);
        std::lock_guard<std::mutex> lock(_adjoin_mutex);
        ARIADNE_PRECONDITION_MSG(_root_cell.grid()==state.root_cell.grid(),
            "Cannot assign a paving from a different grid");
        _manager_owner=std::move(state.manager_owner);
        _manager=state.manager;
        _root_cell=std::move(state.root_cell);
        _root=state.root;
        _depth=state.depth;
    }
    return *this;
}

RobddPaving::RobddPaving(std::shared_ptr<RobddManager> manager,
                             Grid const& grid)
    : RobddPaving(checked_manager(manager),
                  GridCell(grid,0u,BinaryWord()),
                  checked_manager(manager).constant(false),0u) {
    _manager_owner=std::move(manager);
}

RobddPaving::RobddPaving(
        RobddManager& manager, BinaryTreeSubpaving const& source)
    : RobddPaving(
        manager,source.root_cell(),
        encode_tree(manager,source.binary_tree(),0u),source.tree_depth()) { }

RobddPaving::RobddPaving(
        RobddManager& manager, SubPavingInterface const& source)
    : RobddPaving(manager,source.root_cell(),manager.constant(false),0u) {
    // Explicit conversion from a representation-neutral subpaving:
    // collect its enabled cells and compile one canonical ROBDD batch.
    // Do not perform one singleton union for every leaf.
    std::vector<GridCell> cells;
    for (auto cell=source.begin(), end=source.end(); cell!=end; ++cell) {
        cells.push_back(*cell);
    }
    adjoin(cells);
}

RobddPaving::RobddPaving(
        std::shared_ptr<RobddManager> manager,
        SubPavingInterface const& source)
    : RobddPaving(checked_manager(manager),source) {
    _manager_owner=std::move(manager);
}

RobddPaving::RobddPaving(
        RobddManager& manager, GridCell const& root_cell,
        Robdd const& root, SizeType depth)
    : _manager(&manager), _root_cell(root_cell), _root(root), _depth(PavingDetail::checked_decision_depth(depth)) {
    auto variables = manager.support(root); // Validates manager identity.
    ARIADNE_PRECONDITION_MSG(
        variables.empty() or variables.back().index() < depth,
        "ROBDD root refers to a subdivision outside the declared depth");
}


RobddPaving::RobddPaving(
        std::shared_ptr<RobddManager> manager,
        GridCell const& root_cell, Robdd const& root, SizeType depth)
    : RobddPaving(checked_manager(manager),root_cell,root,depth) {
    _manager_owner=std::move(manager);
}

RobddPaving RobddPaving::from_cells(
        RobddManager& manager,GridCell const& root_cell,
        std::vector<GridCell> const& cells) {
    // Normalize each cell and insert its relative path immediately,
    // without retaining a second vector of allocated BinaryWords.
    // Only local arena memory changes during this loop: the manager and
    // destination paving remain untouched until ALL cells validate.
    // A GridCell may have a different primary extent but still
    // represent a descendant of the supplied geometric root.
    CellPrefixTrie trie;
    SizeType maximum_depth=0u;
    for(GridCell const& cell:cells) {
        ARIADNE_PRECONDITION_MSG(root_cell.grid()==cell.grid(),
            "Bulk ROBDD cells must lie on the same grid");
        BinaryWord path=relative_cell_path(root_cell,cell);
        maximum_depth=std::max(maximum_depth,
            PavingDetail::checked_decision_depth(path.size()));
        trie.insert(path);
    }
    Robdd root=encode_cell_trie(manager,trie,0u,0u);
    return RobddPaving(manager,root_cell,root,maximum_depth);
}

RobddPaving RobddPaving::from_cells(
        std::shared_ptr<RobddManager> manager, GridCell const& root_cell,
        std::vector<GridCell> const& cells) {
    RobddPaving result=from_cells(checked_manager(manager),root_cell,cells);
    result._manager_owner=std::move(manager);
    return result;
}

Bool RobddPaving::is_enabled(BinaryWord const& path) const {
    CopyState state=_snapshot_copy(*this);
    ARIADNE_PRECONDITION_MSG(path.size() == state.depth,
        "Paving membership requires a path of the snapshot's logical depth");
    return state.manager->evaluate(state.root,path);
}

GridCell RobddPaving::smallest_enclosing_primary_cell(
        UpperBoxType const& box) const {
    // Copy root geometry under the owner mutex; grid() returns an unsafe
    // borrowed reference if concurrent assignment is allowed.
    GridCell geometric_root=root_cell();
    ARIADNE_PRECONDITION_MSG(box.dimension() == geometric_root.grid().dimension(),
        "Cannot find an enclosing primary cell for a box of another dimension");
    return GridCell::smallest_enclosing_primary_cell(box,geometric_root.grid());
}

UpperBoxType RobddPaving::bounding_box() const {
    RobddPaving snapshot(*this);
    auto current=snapshot.begin(), finish=snapshot.end();
    if (current == finish) {
        return ExactBoxType(snapshot.dimension());
    }

    UpperBoxType bounds=current->box();
    for (++current; current!=finish; ++current) {
        UpperBoxType cell_box=current->box();
        for (DimensionType i=0u; i<snapshot.dimension(); ++i) {
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
    RobddPaving snapshot(*this);
    ARIADNE_PRECONDITION_MSG(box.dimension() == snapshot.dimension(),
        "Paving and query-box dimensions must agree");
    ExactBoxType narrowed=cast_exact_box(narrow(box));
    for (auto cell=snapshot.begin(), last=snapshot.end(); cell!=last; ++cell) {
        if (not cell->box().subset(narrowed)) { return false; }
    }
    return true;
}

ValidatedLowerKleenean RobddPaving::separated(
        ExactBoxType const& box) const {
    RobddPaving snapshot(*this);
    ARIADNE_PRECONDITION_MSG(box.dimension() == snapshot.dimension(),
        "Paving and query-box dimensions must agree");
    ExactBoxType widened=cast_exact_box(widen(box));
    for (auto cell=snapshot.begin(), last=snapshot.end(); cell!=last; ++cell) {
        if (cell->box().overlaps(widened)) { return false; }
    }
    return true;
}

ValidatedLowerKleenean RobddPaving::overlaps(
        ExactBoxType const& box) const {
    RobddPaving snapshot(*this);
    ARIADNE_PRECONDITION_MSG(box.dimension() == snapshot.dimension(),
        "Paving and query-box dimensions must agree");
    ExactBoxType narrowed=cast_exact_box(narrow(box));
    for (auto cell=snapshot.begin(), last=snapshot.end(); cell!=last; ++cell) {
        if (cell->box().overlaps(narrowed)) { return true; }
    }
    return false;
}

SizeType RobddPaving::enabled_cell_count() const {
    CopyState state=_snapshot_copy(*this);
    return state.manager->model_count(state.root,state.depth);
}

Void RobddPaving::for_each_enabled_cell(
        std::function<void(GridCell const&)> const& visitor) const {
    ARIADNE_PRECONDITION_MSG(static_cast<Bool>(visitor),
        "Enabled-cell visitor must be nonempty");
    CopyState state=_snapshot_copy(*this);
    state.manager->for_each_satisfying_assignment(
        state.root,state.depth,[&](BinaryWord const& assignment) {
            BinaryWord path = state.root_cell.word();
            path.append(assignment);
            GridCell cell(state.root_cell.grid(),state.root_cell.root_extent(),path);
            visitor(cell);
        });
}

SizeType RobddPaving::enabled_leaf_count() const {
    CopyState state=_snapshot_copy(*this);
    SizeType count=0u;
    BinaryWord path;
    visit_enabled_leaf_paths(*state.manager,state.root,0u,path,
        [&](BinaryWord const&) {
            count=PavingDetail::checked_add(count,1u);
        });
    return count;
}

RobddPaving::ConstIterator RobddPaving::begin() const {
    return ConstIterator(_begin());
}

RobddPaving::ConstIterator RobddPaving::end() const {
    return ConstIterator(_end());
}

Void RobddPaving::for_each_enabled_leaf(
        std::function<void(GridCell const&)> const& visitor) const {
    ARIADNE_PRECONDITION_MSG(static_cast<Bool>(visitor),
        "Enabled-leaf visitor must be nonempty");
    CopyState state=_snapshot_copy(*this);
    BinaryWord path;
    visit_enabled_leaf_paths(*state.manager,state.root,0u,path,
        [&](BinaryWord const& relative_path) {
            BinaryWord complete_path = state.root_cell.word();
            complete_path.append(relative_path);
            GridCell cell(state.root_cell.grid(),state.root_cell.root_extent(),complete_path);
            visitor(cell);
        });
}

Void RobddPaving::mince(Nat fineness) {
    std::lock_guard<std::mutex> lock(_adjoin_mutex);
    // BinaryTreeSubpaving::mince uses this same geometric target, but
    // physically splits leaves. A ROBDD retains canonical shared nodes
    // and records the additional subdivisions only in its logical depth.
    SizeType extent=static_cast<SizeType>(_root_cell.root_extent());
    // Already holding the owner mutex: avoid a reentrant dimension().
    SizeType dimensions=_root_cell.grid().dimension();
    SizeType absolute_depth=PavingDetail::checked_product(
        PavingDetail::checked_add(extent,fineness),dimensions);
    SizeType prefix_depth=_root_cell.word().size();
    SizeType desired=absolute_depth>prefix_depth
        ? absolute_depth-prefix_depth : 0u;
    _depth=std::max(_depth,PavingDetail::checked_decision_depth(desired));
}

Void RobddPaving::mince_to_depth(SizeType new_depth) {
    std::lock_guard<std::mutex> lock(_adjoin_mutex);
    _depth = std::max(_depth,PavingDetail::checked_decision_depth(new_depth));
}

Void RobddPaving::recombine() {
    std::lock_guard<std::mutex> lock(_adjoin_mutex);
    auto variables = _manager->support(_root);
    _depth = variables.empty() ? 0u : variables.back().index()+1u;
}

std::unique_ptr<BinaryTreeNode> RobddPaving::to_binary_tree() const {
    CopyState state=_snapshot_copy(*this);
    return expand_tree(*state.manager,state.root,0u,state.depth);
}

Void RobddPaving::set_root_cell(Bool enabled) {
    std::lock_guard<std::mutex> lock(_adjoin_mutex);
    _root = _manager->constant(enabled);
    _depth = 0u;
}

Void RobddPaving::clear() {
    set_root_cell(false);
}

Void RobddPaving::_adjoin_unlocked(RobddPaving const& other) {
    RobddPaving united=_join_unlocked(other);
    _root_cell=std::move(united._root_cell);
    _root=united._root;
    _depth=united._depth;
}

Void RobddPaving::adjoin(RobddPaving const& other) {
    // Snapshot the source first, then lock the destination: never hold
    // locks for two pavings while performing a potentially expensive union.
    RobddPaving source(other);
    std::lock_guard<std::mutex> lock(_adjoin_mutex);
    _adjoin_unlocked(source);
}

// Only same-backend pavings and their live child views may participate in
// paving set operations. Cross-backend conversion remains an explicit,
// external migration operation, never an implicit set-operation fallback.
RobddPaving robdd_operand(SubPavingInterface const& other) {
    if (auto const* paving=dynamic_cast<RobddPaving const*>(&other)) {
        return *paving;
    }
    if (auto const* view=
            dynamic_cast<PavingCellViewAdapter<RobddPaving> const*>(&other)) {
        RobddPaving owner_snapshot(view->owner());
        return owner_snapshot.subpaving(view->root_cell());
    }
    throw std::runtime_error("ROBDD paving operations require a ROBDD-backed operand");
}


Void RobddPaving::adjoin(std::vector<GridCell> const& cells) {
    if(cells.empty()) { return; }
    std::lock_guard<std::mutex> lock(_adjoin_mutex);
    GridCell root=_root_cell;
    for(GridCell const& cell:cells) {
        ARIADNE_PRECONDITION_MSG(grid()==cell.grid(),
            "Cannot bulk-adjoin cells from another grid");
        root=common_root_cell(root,cell);
    }
    RobddPaving additions=from_cells(*_manager,root,cells);
    if(_root.is_false() and _depth==0u) {
        // Only a genuinely depth-zero empty paving can discard its old
        // metadata. A minced empty paving must retain its logical depth.
        _root_cell=std::move(additions._root_cell);
        _root=additions._root;
        _depth=additions._depth;
    } else {
        _adjoin_unlocked(additions);
    }
}

Void RobddPaving::adjoin(SubPavingInterface const& other) {
    // The same-backend operand is snapshotted before destination locking.
    // Grid and manager compatibility are validated inside the locked union.
    adjoin(robdd_operand(other));
}

Void RobddPaving::restrict(RobddPaving const& other) {
    if(this==&other) {
        std::lock_guard<std::mutex> lock(_adjoin_mutex);
        return;
    }
    // Match adjoin's snapshot-source policy. The manager handles shared
    // canonical nodes, while this lock protects the whole paving state.
    RobddPaving source(other);
    std::lock_guard<std::mutex> lock(_adjoin_mutex);
    RobddPaving result=_intersection_unlocked(source);
    _root_cell=std::move(result._root_cell);
    _root=result._root;
    _depth=result._depth;
}

Void RobddPaving::restrict(SubPavingInterface const& other) {
    if(auto const* owner=dynamic_cast<RobddPaving const*>(&other)) {
        restrict(*owner);
        return;
    }
    restrict(robdd_operand(other));
}

Void RobddPaving::remove(RobddPaving const& other) {
    if(this==&other) {
        std::lock_guard<std::mutex> lock(_adjoin_mutex);
        _root=_manager->constant(false);
        // A set difference keeps the geometric root and logical depth.
        return;
    }
    RobddPaving source(other);
    std::lock_guard<std::mutex> lock(_adjoin_mutex);
    RobddPaving result=_difference_unlocked(source);
    _root_cell=std::move(result._root_cell);
    _root=result._root;
    _depth=result._depth;
}

Void RobddPaving::remove(SubPavingInterface const& other) {
    if(auto const* owner=dynamic_cast<RobddPaving const*>(&other)) {
        remove(*owner);
        return;
    }
    remove(robdd_operand(other));
}

Void RobddPaving::adjoin_outer_approximation(
        EffectiveEuclideanCompactSetInterface const& set, Nat fineness) {
    // The optional snapshot is built only when a candidate actually
    // requires classification. A fully covered paving skips solver setup.
    std::optional<PreparedConstrainedImageSetSeparation> prepared;
    auto const* constrained=PavingDetail::prepared_separation_source(set);
    bool const can_prepare=constrained!=nullptr;
    // Geometric metadata is captured as a value, never as a borrowed grid.
    GridCell geometric_root=root_cell();
    Grid grid_value=geometric_root.grid();
    ARIADNE_PRECONDITION_MSG(set.dimension() == grid_value.dimension(),
        "Cannot approximate a set of another dimension");
    Nat extent=GridCell::smallest_enclosing_primary_cell_extent(
        set.bounding_box(),grid_value);
    Nat dimensions=PavingDetail::checked_index<Nat>(grid_value.dimension());
    Nat enclosing_extent=PavingDetail::checked_index<Nat>(
        PavingDetail::checked_add(extent,dimensions));
    SizeType levels=PavingDetail::checked_add(enclosing_extent,fineness,
        std::numeric_limits<Nat>::max());
    SizeType target_depth=PavingDetail::checked_product(levels,dimensions);
    GridCell enclosing_root(grid_value,enclosing_extent,BinaryWord{});
    auto const* open_set=dynamic_cast<
        EffectiveEuclideanOpenSetInterface const*>(
            static_cast<EuclideanSetInterfaceBase const*>(&set));

    // Selections are applied only after the traversal, so the current
    // paving stays unchanged while testing candidate cells. If it starts
    // empty, no candidate can already be covered: skip the check entirely.
    Bool check_existing=not is_empty();
    std::vector<GridCell> selected;
    std::function<Void(GridCell const&,SizeType)> visit =
        [&](GridCell const& cell, SizeType depth) {
            // Existing coverage makes this candidate redundant, so no
            // expensive geometric separation/covers query is needed.
            if (check_existing and superset(cell)) { return; }
            if (can_prepare and not prepared)
                prepared.emplace(constrained->prepare_separation());
            Bool const excluded=prepared
                ? definitely(prepared->separated(cell.box()))
                : definitely(set.separated(cell.box()));
            if(excluded) { return; }
            if ((open_set and definitely(open_set->covers(cell.box())))
                    or depth==target_depth) {
                selected.push_back(cell);
                return;
            }
            visit(cell.split(false),depth+1u);
            visit(cell.split(true),depth+1u);
        };
    visit(enclosing_root,0u);
    adjoin(selected);
}


Void RobddPaving::adjoin_outer_approximation_parallel(
        EffectiveEuclideanCompactSetInterface const& set,Nat fineness) {
    // Only the exact base ConstrainedImageSet has the immutable prepared
    // solver snapshot needed for concurrent certified classification.
    // Derived virtual predicates (or other compact set kinds) are not
    // assumed thread-safe; retain their exact sequential dispatch.
    ThreadManager& threading=ThreadManager::instance();
    if(threading.concurrency()<2u or typeid(set)!=typeid(ConstrainedImageSet)) {
        adjoin_outer_approximation(set,fineness);
        return;
    }
    auto const& constrained=dynamic_cast<ConstrainedImageSet const&>(set);
    RobddPaving const existing(*this);
    GridCell const geometric_root=existing.root_cell();
    Grid const grid_value=geometric_root.grid();
    ARIADNE_PRECONDITION_MSG(set.dimension()==grid_value.dimension(),
        "Cannot approximate a set of another dimension");

    Nat extent=GridCell::smallest_enclosing_primary_cell_extent(
        set.bounding_box(),grid_value);
    Nat dimensions=PavingDetail::checked_index<Nat>(grid_value.dimension());
    Nat enclosing_extent=PavingDetail::checked_index<Nat>(
        PavingDetail::checked_add(extent,dimensions));
    SizeType levels=PavingDetail::checked_add(enclosing_extent,fineness,
        std::numeric_limits<Nat>::max());
    SizeType target_depth=PavingDetail::checked_product(levels,dimensions);
    GridCell const enclosing_root(grid_value,enclosing_extent,BinaryWord{});
    Bool const check_existing=not existing.is_empty();

    // Preserve the serial early return on an already-covered search root.
    if(check_existing and existing.superset(enclosing_root)) return;
    // Prepared data own a snapshot; each separated() call constructs its
    // own ConstraintSolver and scratch domain, even across worker threads.
    PreparedConstrainedImageSetSeparation const prepared=
        constrained.prepare_separation();

    // Bound task count and subdivision fan-out; never create a recursive
    // task per cell. Frontier ancestors are classified sequentially first,
    // preserving all early pruning before splitting into parallel branches.
    SizeType partition_depth=0u;
    SizeType desired=1u;
    SizeType const maximum=std::min<SizeType>(8u,threading.concurrency());
    while(desired<maximum and partition_depth<target_depth) {
        desired*=2u;
        ++partition_depth;
    }

    std::vector<GridCell> frontier;
    std::vector<GridCell> selected_prefix;
    std::function<void(GridCell const&,SizeType)> build_frontier=
        [&](GridCell const& cell,SizeType depth) {
            if(check_existing and existing.superset(cell)) return;
            if(definitely(prepared.separated(cell.box()))) return;
            if(depth==target_depth) {
                selected_prefix.push_back(cell);
                return;
            }
            if(depth==partition_depth) {
                frontier.push_back(cell);
                return;
            }
            build_frontier(cell.split(false),depth+1u);
            build_frontier(cell.split(true),depth+1u);
        };
    build_frontier(enclosing_root,0u);

    // Each task owns an output buffer and a separate rounding environment.
    // Read-only sharing is restricted to the immutable prepared snapshot
    // and an independent, stable paving snapshot.
    int const caller_rounding=PavingDetail::require_rounding_mode(std::fegetround());
    std::vector<std::vector<GridCell>> buffers(frontier.size());
    auto classify=[&](SizeType index) {
        ScopedRoundingMode rounding(std::optional<int>{caller_rounding});
        auto& local=buffers[index];
        std::function<void(GridCell const&,SizeType)> visit=
            [&](GridCell const& cell,SizeType depth) {
                if(check_existing and existing.superset(cell)) return;
                if(definitely(prepared.separated(cell.box()))) return;
                if(depth==target_depth) {
                    local.push_back(cell);
                    return;
                }
                visit(cell.split(false),depth+1u);
                visit(cell.split(true),depth+1u);
            };
        // The frontier cell was already checked: its children are exactly
        // the next two candidates in the serial depth-first traversal.
        auto const& root=frontier[index];
        visit(root.split(false),partition_depth+1u);
        visit(root.split(true),partition_depth+1u);
    };

    PavingDetail::run_parallel_classification(frontier.size(),classify,threading);

    // No mutation or partial publication occurs before *all* workers join.
    // Merge in the serial left-to-right subtree order, with one atomic batch
    // union on the paving. Concurrent external adjoins remain valid.
    SizeType total=selected_prefix.size();
    for(auto const& buffer:buffers) total+=buffer.size();
    std::vector<GridCell> selected;
    selected.reserve(total);
    selected.insert(selected.end(),selected_prefix.begin(),selected_prefix.end());
    for(auto const& buffer:buffers)
        selected.insert(selected.end(),buffer.begin(),buffer.end());
    adjoin(selected);
}

Void RobddPaving::adjoin_outer_approximation(
        ValidatedEuclideanCompactSetInterface const& set, Nat fineness) {
    // Classify cells directly; never materialize an explicit binary tree.
    Grid grid_value=root_cell().grid();
    ARIADNE_PRECONDITION_MSG(set.dimension()==grid_value.dimension(),
        "Validated compact set dimension differs from the paving grid");
    Nat extent=GridCell::smallest_enclosing_primary_cell_extent(
        set.bounding_box(),grid_value);
    Nat dimensions=PavingDetail::checked_index<Nat>(grid_value.dimension());
    Nat enclosing_extent=PavingDetail::checked_index<Nat>(
        PavingDetail::checked_add(extent,dimensions));
    SizeType levels=PavingDetail::checked_add(enclosing_extent,fineness,
        std::numeric_limits<Nat>::max());
    SizeType target_depth=PavingDetail::checked_product(levels,dimensions);
    GridCell search_root(grid_value,enclosing_extent,BinaryWord{});
    Bool existing=not is_empty();
    std::vector<GridCell> selected;
    std::function<Void(GridCell const&,SizeType)> visit=
        [&](GridCell const& cell,SizeType depth) {
            if(existing and superset(cell)) { return; }
            if(definitely(set.separated(cell.box()))) { return; }
            if(depth==target_depth) { selected.push_back(cell); return; }
            visit(cell.split(false),depth+1u);
            visit(cell.split(true),depth+1u);
        };
    visit(search_root,0u);
    adjoin(selected);
}

Void RobddPaving::adjoin_lower_approximation(
        EffectiveEuclideanOvertSetInterface const& set,
        Nat extent, Nat fineness) {
    Grid grid_value=root_cell().grid();
    ARIADNE_PRECONDITION_MSG(set.dimension()==grid_value.dimension(),
        "Overt set dimension differs from the paving grid");
    SizeType levels=PavingDetail::checked_add(extent,fineness);
    SizeType target_depth=PavingDetail::checked_product(
        levels,grid_value.dimension());
    GridCell search_root(grid_value,extent,BinaryWord{});
    auto const* open_set=dynamic_cast<
        EffectiveEuclideanOpenSetInterface const*>(&set);
    Bool existing=not is_empty();
    std::vector<GridCell> selected;
    std::function<Void(GridCell const&,SizeType)> visit=
        [&](GridCell const& cell,SizeType depth) {
            if(existing and superset(cell)) { return; }
            if(open_set and definitely(open_set->covers(cell.box()))) {
                selected.push_back(cell); return;
            }
            if(not definitely(set.overlaps(cell.box()))) { return; }
            if(depth==target_depth) { selected.push_back(cell); return; }
            visit(cell.split(false),depth+1u);
            visit(cell.split(true),depth+1u);
        };
    visit(search_root,0u);
    adjoin(selected);
}

Void RobddPaving::restrict_to_extent(Nat extent) {
    GridCell allowed_cell(root_cell().grid(),extent,BinaryWord{});
    RobddPaving allowed(*_manager,allowed_cell,_manager->constant(true),0u);
    restrict(allowed);
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
    GridCell geometric_root=root_cell();
    Grid grid_value=geometric_root.grid();
    ARIADNE_PRECONDITION_MSG(set.dimension() == grid_value.dimension(),
        "Cannot approximate a set of another dimension");
    SizeType levels=PavingDetail::checked_add(extent,fineness);
    SizeType dimensions=grid_value.dimension();
    SizeType target_depth=PavingDetail::checked_product(levels,dimensions);
    GridCell enclosing_root(grid_value,extent,BinaryWord{});
    auto const* closed_set=dynamic_cast<
        EffectiveEuclideanClosedSetInterface const*>(&set);

    // Only positively certified cells may enter an inner approximation.
    // While the candidate cells are collected, the paving root is unchanged.
    // An initially empty paving cannot cover any candidate.
    Bool check_existing=not is_empty();
    std::vector<GridCell> selected;
    std::function<Void(GridCell const&,SizeType)> visit =
        [&](GridCell const& cell, SizeType depth) {
            if (check_existing and superset(cell)) { return; }
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
    adjoin(selected);
}

Void RobddPaving::adjoin_inner_approximation(
        EffectiveEuclideanSetInterface const& set, Nat fineness) {
    GridCell geometric_root=root_cell();
    ARIADNE_PRECONDITION_MSG(set.dimension() == geometric_root.grid().dimension(),
        "Cannot approximate a set of another dimension");
    ExactBoxType bounding=cast_exact_box(set.bounding_box());
    Nat extent=GridCell::smallest_enclosing_primary_cell_extent(
        bounding,geometric_root.grid());
    EffectiveEuclideanOpenSetInterface const& open_set=set;
    adjoin_inner_approximation(open_set,extent,fineness);
}

Void RobddPaving::adjoin(GridCell const& cell) {
    // Assignment may replace _manager. Capture it and publish the new
    // root in one owner critical section, without entering a public lock.
    std::lock_guard<std::mutex> lock(_adjoin_mutex);
    RobddPaving singleton(*_manager,cell,_manager->constant(true),0u);
    _adjoin_unlocked(singleton);
}

Void RobddPaving::remove(GridCell const& cell) {
    std::lock_guard<std::mutex> lock(_adjoin_mutex);
    RobddPaving singleton(*_manager,cell,_manager->constant(true),0u);
    RobddPaving result=_difference_unlocked(singleton);
    _root_cell=std::move(result._root_cell);
    _root=result._root;
    _depth=result._depth;
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
    SizeType lifted_depth=PavingDetail::checked_decision_depth(
        PavingDetail::checked_add(offset,_depth));
    Robdd lifted = _manager->shift(
        _root,PavingDetail::checked_index<std::ptrdiff_t>(offset));
    Robdd disabled = _manager->constant(false);
    for (SizeType i=offset; i!=0u; ) {
        --i;
        Bool right = path[i];
        lifted = right
            ? _manager->make_node(BddVariable(i),disabled,lifted)
            : _manager->make_node(BddVariable(i),lifted,disabled);
    }
    return std::make_pair(lifted,lifted_depth);
}

RobddPaving RobddPaving::join(
        RobddPaving const& other) const {
    if(this==&other) { return RobddPaving(*this); }
    // Capture each mutable owner before calling the lock-free algebra core.
    RobddPaving lhs(*this);
    RobddPaving rhs(other);
    return lhs._join_unlocked(rhs);
}

RobddPaving RobddPaving::_join_unlocked(
        RobddPaving const& other) const {
    GridCell common = _common_root(other);
    auto first = _lift_to(common);
    auto second = other._lift_to(common);
    RobddPaving result(
        *_manager,common,_manager->disjunction(first.first,second.first),
        std::max(first.second,second.second));
    result._manager_owner=_manager_owner ? _manager_owner : other._manager_owner;
    return result;
}

RobddPaving RobddPaving::intersection(
        RobddPaving const& other) const {
    if(this==&other) { return RobddPaving(*this); }
    // Capture each mutable owner before calling the lock-free algebra core.
    RobddPaving lhs(*this);
    RobddPaving rhs(other);
    return lhs._intersection_unlocked(rhs);
}

RobddPaving RobddPaving::_intersection_unlocked(
        RobddPaving const& other) const {
    GridCell common = _common_root(other);
    auto first = _lift_to(common);
    auto second = other._lift_to(common);
    RobddPaving result(
        *_manager,common,_manager->conjunction(first.first,second.first),
        std::max(first.second,second.second));
    result._manager_owner=_manager_owner ? _manager_owner : other._manager_owner;
    return result;
}

RobddPaving RobddPaving::difference(
        RobddPaving const& other) const {
    if(this==&other) {
        RobddPaving result(*this);
        result._root=result._manager->constant(false);
        return result;
    }
    // Capture each mutable owner before calling the lock-free algebra core.
    RobddPaving lhs(*this);
    RobddPaving rhs(other);
    return lhs._difference_unlocked(rhs);
}

RobddPaving RobddPaving::_difference_unlocked(
        RobddPaving const& other) const {
    GridCell common = _common_root(other);
    auto first = _lift_to(common);
    auto second = other._lift_to(common);
    RobddPaving result(
        *_manager,common,
        _manager->conjunction(first.first,_manager->negation(second.first)),
        std::max(first.second,second.second));
    result._manager_owner=_manager_owner ? _manager_owner : other._manager_owner;
    return result;
}

Bool RobddPaving::subset(
        RobddPaving const& other) const {
    if(this==&other) { return true; }
    RobddPaving lhs(*this);
    RobddPaving rhs(other);
    GridCell common=lhs._common_root(rhs);
    auto first=lhs._lift_to(common);
    auto second=rhs._lift_to(common);
    return lhs._manager->conjunction(
        first.first,lhs._manager->negation(second.first)).is_false();
}

Bool RobddPaving::intersects(
        RobddPaving const& other) const {
    if(this==&other) { return !is_empty(); }
    RobddPaving lhs(*this);
    RobddPaving rhs(other);
    GridCell common=lhs._common_root(rhs);
    auto first=lhs._lift_to(common);
    auto second=rhs._lift_to(common);
    return not lhs._manager->conjunction(
        first.first,second.first).is_false();
}

Bool RobddPaving::subset(SubPavingInterface const& other) const {
    if(this==&other) { return true; }
    // The backend compatibility check uses two detached owner snapshots.
    return subset(robdd_operand(other));
}

Bool RobddPaving::intersects(SubPavingInterface const& other) const {
    if(this==&other) { return !is_empty(); }
    return intersects(robdd_operand(other));
}

Bool RobddPaving::equals(SubPavingInterface const& other) const {
    if(this==&other) { return true; }
    // Snapshot this owner only once: equality must not compare two
    // versions of a concurrently changing paving.
    RobddPaving lhs(*this);
    RobddPaving rhs=robdd_operand(other);
    return lhs.subset(rhs) and rhs.subset(lhs);
}

Bool RobddPaving::superset(ExactBoxType const& box) const {
    // Follow canonical ROBDD decisions without materializing a tree.
    RobddPaving snapshot(*this);
    GridCell root=snapshot.root_cell();
    ARIADNE_PRECONDITION_MSG(box.dimension()==root.dimension(),
        "Cannot query a box of another dimension");
    if(not box.subset(root.box())) { return false; }
    SizeType depth=snapshot.depth();
    std::function<Bool(GridCell const&,SizeType)> visit=
        [&](GridCell const& cell,SizeType level)->Bool {
            if(not cell.box().overlaps(box)) { return true; }
            if(snapshot.superset(cell)) { return true; }
            if(level==depth) { return false; }
            return visit(cell.split(false),level+1u)
               and visit(cell.split(true),level+1u);
        };
    return visit(root,0u);
}

Bool RobddPaving::superset(GridCell const& cell) const {
    std::lock_guard<std::mutex> lock(_adjoin_mutex);
    ARIADNE_PRECONDITION_MSG(grid() == cell.grid(),
        "Cannot test cell containment across different grids");
    if(_root.is_false()) { return false; }

    // A dyadic cell is either a descendant of the paving's root, an
    // ancestor, or disjoint. Compare their normalized prefixes directly
    // without constructing a common GridCell or any concatenated/sliced
    // BinaryWord. On the usual same-primary-extent query, this path makes
    // no binary-word allocations at all.
    BinaryWord const& root_word=_root_cell.word();
    BinaryWord const& query_word=cell.word();
    Nat root_extent=_root_cell.root_extent();
    Nat query_extent=cell.root_extent();

    BinaryWord alignment;
    SizeType root_prefix=0u;
    SizeType query_prefix=0u;
    if(root_extent>query_extent) {
        // The queried word starts with the primary-cell path that
        // descends from root_extent to query_extent.
        alignment=GridCell::primary_cell_path(
            _root_cell.grid().dimension(),root_extent,query_extent);
        query_prefix=alignment.size();
    } else if(query_extent>root_extent) {
        // The root word starts with the primary-cell path that
        // descends from query_extent to root_extent.
        alignment=GridCell::primary_cell_path(
            _root_cell.grid().dimension(),query_extent,root_extent);
        root_prefix=alignment.size();
    }

    SizeType root_length=root_prefix+root_word.size();
    SizeType query_length=query_prefix+query_word.size();
    if(query_length<root_length) { return false; }
    auto root_bit=[&](SizeType index)->Bool {
        return index<root_prefix
            ?alignment[index] : root_word[index-root_prefix];
    };
    auto query_bit=[&](SizeType index)->Bool {
        return index<query_prefix
            ?alignment[index] : query_word[index-query_prefix];
    };
    for(SizeType i=0u;i<root_length;++i) {
        if(root_bit(i)!=query_bit(i)) { return false; }
    }

    Robdd value=_root;
    for(SizeType i=0u;i<query_length-root_length;++i) {
        if(value.is_terminal()) { return value.is_true(); }
        auto top=_manager->top_variable(value);
        // Terminals were handled above. An edge is consumed only when
        // its index equals i, so strict BDD ordering preserves top >= i.
        if(top->index()==i) {
            Bool branch=query_bit(root_length+i);
            value=branch?_manager->high(value):_manager->low(value);
        }
    }
    return value.is_true();
}

RobddPaving RobddPaving::subpaving(GridCell const& cell) const {
    RobddPaving restricted(*this);
    BinaryWord path=relative_cell_path(restricted._root_cell,cell);
    for (SizeType i=0u; i<path.size(); ++i) {
        restricted=restricted.branch(path[i]);
    }
    return restricted;
}

RobddPaving RobddPaving::branch(Bool right) const {
    RobddPaving source(*this);
    Robdd restricted=source._root;
    SizeType child_depth=0u;
    if(source._depth!=0u) {
        restricted=source._manager->restrict(source._root,BddVariable(0u),right);
        restricted=source._manager->shift(restricted,-1);
        child_depth=source._depth-1u;
    }
    RobddPaving result(*source._manager,source._root_cell.split(right),
        restricted,child_depth);
    result._manager_owner=source._manager_owner;
    return result;
}

} // namespace Ariadne
