/***************************************************************************
 *            paving/cached_binary_tree_paving.hpp
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

/** @file paving/cached_binary_tree_paving.hpp
 *  @brief Binary-tree pavings with an optional compiled ROBDD query cache.
 */

#ifndef ARIADNE_PAVING_CACHED_BINARY_TREE_PAVING_HPP
#define ARIADNE_PAVING_CACHED_BINARY_TREE_PAVING_HPP

#include <cstdint>
#include <memory>
#include <mutex>
#include <vector>

#include "paving/binary_tree_paving.hpp"
#include "paving/robdd_paving.hpp"

namespace Ariadne {

//! Mutable binary-tree backend with an optional canonical ROBDD query cache.
//! Tree geometry remains authoritative and all PavingInterface mutations
//! invalidate the cache before writing. The ROBDD is lazily compiled upon
//! first query of each version only when automatic_cache is enabled.
//! Legacy constructors keep explicitly compiled caching by default.
//! Like other BinaryTreePaving views, borrowed subpavings require caller
//! synchronization; using a borrowed mutable view across compile_cache()
//! is not safe. Do not bypass this class's mutators via a cast to its base.
class CachedBinaryTreePaving : public BinaryTreePaving {
  public:
    explicit CachedBinaryTreePaving(Grid const& grid,Bool enabled=false,
                                    Bool automatic_cache=false);
    explicit CachedBinaryTreePaving(BinaryTreePaving const& source,
                                    Bool automatic_cache=false);
    CachedBinaryTreePaving(CachedBinaryTreePaving const& source);
    /** @brief Perform the specified paving operation. */
    CachedBinaryTreePaving& operator=(CachedBinaryTreePaving const&)=delete;
    /** @brief Return a copy of the paving object. */
    CachedBinaryTreePaving* clone() const override;

    using BinaryTreePaving::superset;
    /** @brief Perform the specified paving operation. */
    Bool superset(GridCell const& cell) const override;
    /** @brief Perform the specified paving operation. */
    Bool superset(ExactBoxType const& box) const override;

    //! Compile a snapshot outside the tree mutex and publish if unchanged.
    //! Returns false if a concurrent mutation overtook this compilation.
    Bool compile_cache();
    Bool automatic_cache() const { return _automatic_cache; }
    /** @brief Test the stated geometric property. */
    Bool has_cache() const;
    /** @brief Perform the specified paving operation. */
    std::uint64_t version() const;
    /** @brief Manage the compiled query cache. */
    SizeType cached_nodes() const;
    /** @brief Return the requested grid or paving property. */
    std::unique_ptr<BinaryTreePaving> tree_snapshot() const;

    // The full polymorphic mutation contract plus legacy tree mutations.
    Void adjoin(GridCell const& cell) override;
    /** @brief Modify the paving or its subdivision. */
    Void adjoin(std::vector<GridCell> const& cells) override;
    /** @brief Modify the paving or its subdivision. */
    Void adjoin(SubPavingInterface const& other) override;
    /** @brief Modify the paving or its subdivision. */
    Void restrict(SubPavingInterface const& other) override;
    /** @brief Modify the paving or its subdivision. */
    Void remove(SubPavingInterface const& other) override;
    /** @brief Modify the paving or its subdivision. */
    Void remove(GridCell const& cell) override;
    /** @brief Perform the specified paving operation. */
    Void set_root_cell(Bool enabled) override;
    /** @brief Perform the specified paving operation. */
    Void mince(Nat fineness) override;
    /** @brief Perform the specified paving operation. */
    Void recombine() override;
    /** @brief Modify the paving or its subdivision. */
    Void restrict_to_extent(Nat extent) override;
    /** @brief Modify the paving or its subdivision. */
    Void adjoin_outer_approximation(EffectiveEuclideanCompactSetInterface const& set,Nat fineness) override;
    /** @brief Modify the paving or its subdivision. */
    Void adjoin_outer_approximation(ValidatedEuclideanCompactSetInterface const& set,Nat fineness) override;
    /** @brief Modify the paving or its subdivision. */
    Void adjoin_outer_approximation(UpperBoxType const& box,Nat fineness) override;
    /** @brief Modify the paving or its subdivision. */
    Void adjoin_lower_approximation(EffectiveEuclideanOvertSetInterface const& set,Nat extent,Nat fineness) override;
    /** @brief Modify the paving or its subdivision. */
    Void adjoin_inner_approximation(EffectiveEuclideanOpenSetInterface const& set,Nat extent,Nat fineness) override;
    /** @brief Modify the paving or its subdivision. */
    Void adjoin_inner_approximation(EffectiveEuclideanSetInterface const& set,Nat fineness) override;
    /** @brief Modify the paving or its subdivision. */
    Void clear();
    /** @brief Perform the specified paving operation. */
    Void mince_to_depth(Nat depth);
    /** @brief Perform the specified paving operation. */
    Void mince_to_tree_depth(Nat depth);
    /** @brief Modify the paving or its subdivision. */
    Void subdivide(ApproximateDouble width);
    /** @brief Modify the paving or its subdivision. */
    Void adjoin_over_approximation(ExactBoxType const& box,Nat fineness);
    /** @brief Modify the paving or its subdivision. */
    Void adjoin_lower_approximation(EffectiveEuclideanOvertSetInterface const& set,ExactBoxType const& bounding,Nat fineness);
    /** @brief Modify the paving or its subdivision. */
    Void adjoin_lower_approximation(EffectiveEuclideanLocatedSetInterface const& set,Nat fineness);
    /** @brief Modify the paving or its subdivision. */
    Void adjoin_inner_approximation(EffectiveEuclideanOpenSetInterface const& set,ExactBoxType const& bounding,Nat fineness);
    /** @brief Modify the paving or its subdivision. */
    Void adjoin_inner_approximation(LowerBoxType const& box,Nat fineness);
    /** @brief Modify the paving or its subdivision. */
    Void adjoin(BinaryTreeSubpaving const& other);
    /** @brief Modify the paving or its subdivision. */
    Void restrict(BinaryTreeSubpaving const& other);
    /** @brief Modify the paving or its subdivision. */
    Void remove(BinaryTreeSubpaving const& other);
    //! Borrowed branch creation invalidates the cache before returning it.
    BinaryTreeSubpaving branch(Bool right) const;
    /** @brief Perform the specified paving operation. */
    template<class Callback> Void with_locked_branch(Bool right,Callback&& cb) {
        /** @brief Perform the specified paving operation. */
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        invalidate_unlocked();
        /** @brief Perform the specified paving operation. */
        BinaryTreePaving::with_locked_branch(right,std::forward<Callback>(cb));
    }

  protected:
    /** @brief Perform the specified paving operation. */
    SubPavingInterface* _branch(Bool right) const override;

  private:
    /** @brief Represent a geometric paving component. */
    struct Compiled;
    /** @brief Manage the compiled query cache. */
    Void invalidate_unlocked() const;

    // Recursive to handle the tree's virtual overload-forwarding paths.
    mutable std::recursive_mutex _mutex;
    mutable std::mutex _compile_mutex;
    Bool const _automatic_cache;
    mutable std::uint64_t _version=0u;
    mutable std::shared_ptr<Compiled const> _compiled;
};

} // namespace Ariadne

#endif // ARIADNE_PAVING_CACHED_BINARY_TREE_PAVING_HPP
