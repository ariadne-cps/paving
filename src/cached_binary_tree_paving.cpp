/***************************************************************************
 *            cached_binary_tree_paving.cpp
 *
 *  Explicit Tree -> ROBDD query-cache prototype.
 *  Part of Ariadne Paving, GPL-3.0-or-later.
 ****************************************************************************/
#include "paving/cached_binary_tree_paving.hpp"

#include <limits>
#include <stdexcept>
#include <utility>

namespace Ariadne {

// The manager MUST precede the paving: reverse member destruction destroys
// the handles before their manager. Publishing const Compiled prevents
// mutation of its ROBDD after construction.
struct CachedBinaryTreePaving::Compiled {
    RobddManager manager;
    RobddPaving paving;
    std::uint64_t version;

    Compiled(BinaryTreePaving const& snapshot,std::uint64_t revision)
        : manager(),
          paving(manager,static_cast<BinaryTreeSubpaving const&>(snapshot)),
          version(revision) { }
};

CachedBinaryTreePaving::CachedBinaryTreePaving(Grid const& grid,Bool enabled)
    : _tree(grid,enabled) { }

CachedBinaryTreePaving::CachedBinaryTreePaving(BinaryTreePaving const& source)
    : _tree(source) { }

Void CachedBinaryTreePaving::invalidate_unlocked() {
    if(_version==std::numeric_limits<std::uint64_t>::max())
        throw std::overflow_error("CachedBinaryTreePaving revision counter exhausted");
    ++_version;
    _compiled.reset();
}

Void CachedBinaryTreePaving::adjoin(GridCell const& cell) {
    std::lock_guard<std::mutex> lock(_mutex);
    invalidate_unlocked();
    _tree.adjoin(cell);
}

Void CachedBinaryTreePaving::adjoin(std::vector<GridCell> const& cells) {
    std::lock_guard<std::mutex> lock(_mutex);
    invalidate_unlocked();
    _tree.adjoin(cells);
}

Void CachedBinaryTreePaving::remove(GridCell const& cell) {
    std::lock_guard<std::mutex> lock(_mutex);
    invalidate_unlocked();
    _tree.remove(cell);
}

Void CachedBinaryTreePaving::recombine() {
    std::lock_guard<std::mutex> lock(_mutex);
    invalidate_unlocked();
    _tree.recombine();
}

Void CachedBinaryTreePaving::clear() {
    std::lock_guard<std::mutex> lock(_mutex);
    invalidate_unlocked();
    _tree.clear();
}

Bool CachedBinaryTreePaving::compile_cache() {
    // Only one compilation per instance; mutations do not acquire this
    // lock and remain free to proceed while ROBDD construction is running.
    std::lock_guard<std::mutex> compiling(_compile_mutex);
    std::unique_ptr<BinaryTreePaving> snapshot;
    std::uint64_t revision;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if(_compiled) return true;
        revision=_version;
        snapshot=std::make_unique<BinaryTreePaving>(_tree);
    }

    // No owner lock is held while building the canonical ROBDD.
    // The temporary snapshot cannot change during this traversal.
    std::shared_ptr<Compiled const> candidate=
        std::make_shared<Compiled>(*snapshot,revision);

    {
        std::lock_guard<std::mutex> lock(_mutex);
        if(_version!=revision) return false;
        _compiled=std::move(candidate);
        return true;
    }
}

Bool CachedBinaryTreePaving::superset(GridCell const& cell) const {
    std::shared_ptr<Compiled const> compiled;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        compiled=_compiled;
        if(not compiled) {
            // The tree itself has an internal mutex; hold the outer lock
            // too so this query cannot overlap any wrapper mutation.
            return _tree.superset(cell);
        }
    }
    // Shared ownership keeps both paving AND manager alive if a writer
    // invalidates the cache while the query is still evaluating.
    return compiled->paving.superset(cell);
}

std::unique_ptr<BinaryTreePaving> CachedBinaryTreePaving::tree_snapshot() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return std::make_unique<BinaryTreePaving>(_tree);
}

Bool CachedBinaryTreePaving::has_cache() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return static_cast<Bool>(_compiled!=nullptr);
}

std::uint64_t CachedBinaryTreePaving::version() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _version;
}

SizeType CachedBinaryTreePaving::cached_nodes() const {
    std::shared_ptr<Compiled const> compiled;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        compiled=_compiled;
    }
    return compiled ? compiled->manager.node_count() : 0u;
}

} // namespace Ariadne
