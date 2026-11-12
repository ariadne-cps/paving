/***************************************************************************
 *            src/cached_binary_tree_paving.cpp
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

/** @file src/cached_binary_tree_paving.cpp
 *  @brief Implement cached binary tree paving operations.
 */

#include "paving/cached_binary_tree_paving.hpp"

#include <limits>
#include <stdexcept>
#include <utility>

namespace Ariadne {

// The BDD manager outlives every ROBDD handle within this immutable snapshot.
struct CachedBinaryTreePaving::Compiled {
    RobddManager manager;
    RobddPaving paving;
    std::uint64_t version;

    Compiled(BinaryTreePaving const& snapshot,std::uint64_t revision)
        : manager(),paving(manager,static_cast<BinaryTreeSubpaving const&>(snapshot)),
          version(revision) { }
};

CachedBinaryTreePaving::CachedBinaryTreePaving(
        Grid const& grid,Bool enabled,Bool automatic_cache)
    : BinaryTreePaving(grid,enabled),_automatic_cache(automatic_cache) { }

CachedBinaryTreePaving::CachedBinaryTreePaving(
        BinaryTreePaving const& source,Bool automatic_cache)
    : BinaryTreePaving(source),_automatic_cache(automatic_cache) { }

CachedBinaryTreePaving::CachedBinaryTreePaving(CachedBinaryTreePaving const& source)
    : BinaryTreePaving(static_cast<BinaryTreePaving const&>(source)),
      _automatic_cache(source._automatic_cache) { }

CachedBinaryTreePaving* CachedBinaryTreePaving::clone() const {
    return new CachedBinaryTreePaving(*this);
}

Void CachedBinaryTreePaving::invalidate_unlocked() const {
    _version=PavingDetail::checked_next_revision(_version);
    _compiled.reset();
}

Void CachedBinaryTreePaving::adjoin(GridCell const& cell) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::adjoin(cell);
}

Void CachedBinaryTreePaving::adjoin(std::vector<GridCell> const& cells) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::adjoin(cells);
}

Void CachedBinaryTreePaving::adjoin(SubPavingInterface const& other) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::adjoin(other);
}

Void CachedBinaryTreePaving::restrict(SubPavingInterface const& other) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::restrict(other);
}

Void CachedBinaryTreePaving::remove(SubPavingInterface const& other) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::remove(other);
}

Void CachedBinaryTreePaving::remove(GridCell const& cell) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::remove(cell);
}

Void CachedBinaryTreePaving::set_root_cell(Bool enabled) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::set_root_cell(enabled);
}

Void CachedBinaryTreePaving::mince(Nat fineness) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::mince(fineness);
}

Void CachedBinaryTreePaving::recombine() {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::recombine();
}

Void CachedBinaryTreePaving::restrict_to_extent(Nat extent) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::restrict_to_extent(extent);
}

Void CachedBinaryTreePaving::adjoin_outer_approximation(EffectiveEuclideanCompactSetInterface const& set,Nat fineness) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::adjoin_outer_approximation(set,fineness);
}

Void CachedBinaryTreePaving::adjoin_outer_approximation(ValidatedEuclideanCompactSetInterface const& set,Nat fineness) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::adjoin_outer_approximation(set,fineness);
}

Void CachedBinaryTreePaving::adjoin_outer_approximation(UpperBoxType const& box,Nat fineness) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::adjoin_outer_approximation(box,fineness);
}

Void CachedBinaryTreePaving::adjoin_lower_approximation(EffectiveEuclideanOvertSetInterface const& set,Nat extent,Nat fineness) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::adjoin_lower_approximation(set,extent,fineness);
}

Void CachedBinaryTreePaving::adjoin_inner_approximation(EffectiveEuclideanOpenSetInterface const& set,Nat extent,Nat fineness) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::adjoin_inner_approximation(set,extent,fineness);
}

Void CachedBinaryTreePaving::adjoin_inner_approximation(EffectiveEuclideanSetInterface const& set,Nat fineness) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::adjoin_inner_approximation(set,fineness);
}

Void CachedBinaryTreePaving::clear() {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::clear();
}

Void CachedBinaryTreePaving::mince_to_depth(Nat depth) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::mince_to_depth(depth);
}

Void CachedBinaryTreePaving::mince_to_tree_depth(Nat depth) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::mince_to_tree_depth(depth);
}

Void CachedBinaryTreePaving::subdivide(ApproximateDouble width) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::subdivide(width);
}

Void CachedBinaryTreePaving::adjoin_over_approximation(ExactBoxType const& box,Nat fineness) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::adjoin_over_approximation(box,fineness);
}

Void CachedBinaryTreePaving::adjoin_lower_approximation(EffectiveEuclideanOvertSetInterface const& set,ExactBoxType const& bounding,Nat fineness) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::adjoin_lower_approximation(set,bounding,fineness);
}

Void CachedBinaryTreePaving::adjoin_lower_approximation(EffectiveEuclideanLocatedSetInterface const& set,Nat fineness) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::adjoin_lower_approximation(set,fineness);
}

Void CachedBinaryTreePaving::adjoin_inner_approximation(EffectiveEuclideanOpenSetInterface const& set,ExactBoxType const& bounding,Nat fineness) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::adjoin_inner_approximation(set,bounding,fineness);
}

Void CachedBinaryTreePaving::adjoin_inner_approximation(LowerBoxType const& box,Nat fineness) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::adjoin_inner_approximation(box,fineness);
}

Void CachedBinaryTreePaving::adjoin(BinaryTreeSubpaving const& other) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::adjoin(other);
}

Void CachedBinaryTreePaving::restrict(BinaryTreeSubpaving const& other) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::restrict(other);
}

Void CachedBinaryTreePaving::remove(BinaryTreeSubpaving const& other) {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    BinaryTreePaving::remove(other);
}

BinaryTreeSubpaving CachedBinaryTreePaving::branch(Bool right) const {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    return BinaryTreePaving::branch(right);
}

SubPavingInterface* CachedBinaryTreePaving::_branch(Bool right) const {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    invalidate_unlocked();
    return BinaryTreePaving::_branch(right);
}

Bool CachedBinaryTreePaving::compile_cache() {
    std::lock_guard<std::mutex> compiling(_compile_mutex);
    std::unique_lock<std::recursive_mutex> lock(_mutex);
    if(_compiled) return true;
    std::uint64_t const revision=_version;
    auto snapshot=std::make_unique<BinaryTreePaving>(
        static_cast<BinaryTreePaving const&>(*this));
    lock.unlock();
    // Never hold _mutex while building canonical ROBDD nodes.
    auto candidate=std::make_shared<Compiled const>(*snapshot,revision);
    lock.lock();
    if(_version!=revision) return false;
    _compiled=std::move(candidate);
    return true;
}

Bool CachedBinaryTreePaving::superset(GridCell const& cell) const {
    if(_automatic_cache and not has_cache())
        (void)const_cast<CachedBinaryTreePaving*>(this)->compile_cache();
    std::shared_ptr<Compiled const> compiled;
    {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        compiled=_compiled;
        if(not compiled) return BinaryTreePaving::superset(cell);
    }
    return compiled->paving.superset(cell);
}

Bool CachedBinaryTreePaving::superset(ExactBoxType const& box) const {
    if(_automatic_cache and not has_cache())
        (void)const_cast<CachedBinaryTreePaving*>(this)->compile_cache();
    std::shared_ptr<Compiled const> compiled;
    {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        compiled=_compiled;
        if(not compiled) {
            BinaryTreePaving snapshot(
                static_cast<BinaryTreePaving const&>(*this));
            return snapshot.BinaryTreePaving::superset(box);
        }
    }
    return compiled->paving.superset(box);
}

std::unique_ptr<BinaryTreePaving> CachedBinaryTreePaving::tree_snapshot() const {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    return std::make_unique<BinaryTreePaving>(
        static_cast<BinaryTreePaving const&>(*this));
}

Bool CachedBinaryTreePaving::has_cache() const {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    return static_cast<Bool>(_compiled!=nullptr);
}

std::uint64_t CachedBinaryTreePaving::version() const {
    std::lock_guard<std::recursive_mutex> lock(_mutex);
    return _version;
}

SizeType CachedBinaryTreePaving::cached_nodes() const {
    std::shared_ptr<Compiled const> compiled;
    {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        compiled=_compiled;
    }
    return compiled ? compiled->manager.node_count() : 0u;
}

} // namespace Ariadne
