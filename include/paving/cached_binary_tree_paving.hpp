/***************************************************************************
 *            cached_binary_tree_paving.hpp
 *
 *  Explicit, thread-safe Tree -> ROBDD query cache for Ariadne Paving.
 *  Part of Ariadne Paving, GPL-3.0-or-later.
 ****************************************************************************/
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
    CachedBinaryTreePaving& operator=(CachedBinaryTreePaving const&)=delete;
    CachedBinaryTreePaving* clone() const override;

    using BinaryTreePaving::superset;
    Bool superset(GridCell const& cell) const override;
    Bool superset(ExactBoxType const& box) const override;

    //! Compile a snapshot outside the tree mutex and publish if unchanged.
    //! Returns false if a concurrent mutation overtook this compilation.
    Bool compile_cache();
    Bool automatic_cache() const { return _automatic_cache; }
    Bool has_cache() const;
    std::uint64_t version() const;
    SizeType cached_nodes() const;
    std::unique_ptr<BinaryTreePaving> tree_snapshot() const;

    // The full polymorphic mutation contract plus legacy tree mutations.
    Void adjoin(GridCell const& cell) override;
    Void adjoin(std::vector<GridCell> const& cells) override;
    Void adjoin(SubPavingInterface const& other) override;
    Void restrict(SubPavingInterface const& other) override;
    Void remove(SubPavingInterface const& other) override;
    Void remove(GridCell const& cell) override;
    Void set_root_cell(Bool enabled) override;
    Void mince(Nat fineness) override;
    Void recombine() override;
    Void restrict_to_extent(Nat extent) override;
    Void adjoin_outer_approximation(EffectiveEuclideanCompactSetInterface const& set,Nat fineness) override;
    Void adjoin_outer_approximation(ValidatedEuclideanCompactSetInterface const& set,Nat fineness) override;
    Void adjoin_outer_approximation(UpperBoxType const& box,Nat fineness) override;
    Void adjoin_lower_approximation(EffectiveEuclideanOvertSetInterface const& set,Nat extent,Nat fineness) override;
    Void adjoin_inner_approximation(EffectiveEuclideanOpenSetInterface const& set,Nat extent,Nat fineness) override;
    Void adjoin_inner_approximation(EffectiveEuclideanSetInterface const& set,Nat fineness) override;
    Void clear();
    Void mince_to_depth(Nat depth);
    Void mince_to_tree_depth(Nat depth);
    Void subdivide(ApproximateDouble width);
    Void adjoin_over_approximation(ExactBoxType const& box,Nat fineness);
    Void adjoin_lower_approximation(EffectiveEuclideanOvertSetInterface const& set,ExactBoxType const& bounding,Nat fineness);
    Void adjoin_lower_approximation(EffectiveEuclideanLocatedSetInterface const& set,Nat fineness);
    Void adjoin_inner_approximation(EffectiveEuclideanOpenSetInterface const& set,ExactBoxType const& bounding,Nat fineness);
    Void adjoin_inner_approximation(LowerBoxType const& box,Nat fineness);
    Void adjoin(BinaryTreeSubpaving const& other);
    Void restrict(BinaryTreeSubpaving const& other);
    Void remove(BinaryTreeSubpaving const& other);
    //! Borrowed branch creation invalidates the cache before returning it.
    BinaryTreeSubpaving branch(Bool right) const;
    template<class Callback> Void with_locked_branch(Bool right,Callback&& cb) {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        invalidate_unlocked();
        BinaryTreePaving::with_locked_branch(right,std::forward<Callback>(cb));
    }

  protected:
    SubPavingInterface* _branch(Bool right) const override;

  private:
    struct Compiled;
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
