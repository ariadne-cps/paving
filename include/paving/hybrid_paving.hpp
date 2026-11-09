/***************************************************************************
 *            hybrid_paving.hpp
 *
 *  Explicit, thread-safe Tree -> ROBDD query cache for Ariadne Paving.
 *  Part of Ariadne Paving, GPL-3.0-or-later.
 ****************************************************************************/
#ifndef ARIADNE_PAVING_HYBRID_PAVING_HPP
#define ARIADNE_PAVING_HYBRID_PAVING_HPP

#include <cstdint>
#include <memory>
#include <mutex>
#include <vector>

#include "paving/binary_tree_paving.hpp"
#include "paving/robdd_paving.hpp"

namespace Ariadne {

//! \brief Mutable BinaryTreePaving with an optional immutable ROBDD query index.
//! \details The tree is authoritative. compile_cache() explicitly snapshots
//! the tree and builds a canonical ROBDD *without holding the owner lock*.
//! The compiled index is published only if no mutation has intervened.
//! Every mutator invalidates the index BEFORE touching the tree, even when
//! that mutation subsequently throws. This protects against partial writes.
//!
//! Queries are linearizable: a cache hit observes the version current when
//! its shared index was acquired, while misses query the tree under the lock.
//! A query overlapping a mutation may observe the earlier version, but a
//! query started after a completed mutation never uses its stale cache.
//!
//! No mutable tree reference escapes this type. All mutation MUST go through
//! its methods; externally mutating an earlier source tree is independent.
//! Do not destroy the wrapper while another thread is accessing it.
class HybridPaving {
  public:
    explicit HybridPaving(Grid const& grid,Bool enabled=false);
    explicit HybridPaving(BinaryTreePaving const& source);

    //! \brief Return whether a cell is fully contained (Tree fallback).
    Bool superset(GridCell const& cell) const;

    //! \brief Compile the current Tree version, if not already compiled.
    //! \returns True if a cache for the current version is available;
    //! false if the source changed while this compilation was in progress.
    //! Only one compilation of an instance runs at a time.
    Bool compile_cache();

    //! \brief Structural changes invalidate the cached ROBDD first.
    Void adjoin(GridCell const& cell);
    Void adjoin(std::vector<GridCell> const& cells);
    Void remove(GridCell const& cell);
    Void recombine();
    Void clear();

    //! \brief Snapshot detached from all future mutations.
    std::unique_ptr<BinaryTreePaving> tree_snapshot() const;

    Bool has_cache() const;
    std::uint64_t version() const;
    SizeType cached_nodes() const;

  private:
    struct Compiled;
    Void invalidate_unlocked();

    mutable std::mutex _mutex;
    mutable std::mutex _compile_mutex;
    BinaryTreePaving _tree;
    std::uint64_t _version=0u;
    std::shared_ptr<Compiled const> _compiled;
};

} // namespace Ariadne

#endif // ARIADNE_PAVING_HYBRID_PAVING_HPP
