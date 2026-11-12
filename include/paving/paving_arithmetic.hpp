/***************************************************************************
 *            paving/paving_arithmetic.hpp
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

/** @file paving/paving_arithmetic.hpp
 *  @brief Checked size and revision arithmetic for paving representations.
 */

#ifndef ARIADNE_PAVING_ARITHMETIC_HPP
#define ARIADNE_PAVING_ARITHMETIC_HPP

#include <limits>
#include <cstdint>
#include <stdexcept>
#include "utility/typedefs.hpp"
#include "utility/macros.hpp"

namespace Ariadne::PavingDetail {

/** @brief Compute the requested paving geometry or checked value. */
inline std::uint64_t checked_next_revision(std::uint64_t revision) {
    if(revision==std::numeric_limits<std::uint64_t>::max())
        throw std::overflow_error("CachedBinaryTreePaving revision counter exhausted");
    return revision+1u;
}

// Keep arithmetic on logical metadata separate from allocation/traversal.
// The explicit limit also covers Nat, cell-word and BDD index domains.
inline SizeType checked_add(SizeType left, SizeType right,
        SizeType limit=std::numeric_limits<SizeType>::max()) {
    ARIADNE_PRECONDITION_MSG(left<=limit,
        "Paving size or depth exceeds the supported range");
    ARIADNE_PRECONDITION_MSG(right<=limit-left,
        "Paving size or depth addition overflows");
    return left+right;
}

inline SizeType checked_product(SizeType left, SizeType right,
        SizeType limit=std::numeric_limits<SizeType>::max()) {
    if (right==0u) { return 0u; }
    ARIADNE_PRECONDITION_MSG(left<=limit/right,
        "Paving subdivision depth multiplication overflows");
    return left*right;
}

// Express narrowing as a type-level contract. Same-width unsigned
// conversions cannot fail, including when Nat == SizeType.
template<class Index> requires (std::numeric_limits<Index>::digits <
                               std::numeric_limits<SizeType>::digits)
/** @brief Compute the requested paving geometry or checked value. */
Index checked_index(SizeType value) {
    ARIADNE_PRECONDITION_MSG(
        value<=static_cast<SizeType>(std::numeric_limits<Index>::max()),
        "Paving index exceeds the destination integer range");
    return static_cast<Index>(value);
}

template<class Index> requires (std::numeric_limits<Index>::digits >=
                               std::numeric_limits<SizeType>::digits)
/** @brief Compute the requested paving geometry or checked value. */
Index checked_index(SizeType value) {
    return static_cast<Index>(value);
}

/** @brief Compute the requested paving geometry or checked value. */
inline SizeType checked_decision_depth(SizeType depth) {
    ARIADNE_PRECONDITION_MSG(depth<std::numeric_limits<SizeType>::max(),
        "Paving subdivision depth exceeds the valid BDD index range");
    return depth;
}

inline SizeType checked_cell_depth(
        SizeType extent, SizeType fineness, SizeType dimensions) {
    SizeType levels=checked_add(extent,fineness);
    constexpr SizeType limit=static_cast<SizeType>(std::numeric_limits<Int>::max());
    ARIADNE_PRECONDITION_MSG(dimensions<=limit,
        "Paving dimension exceeds the valid GridCell word range");
    // GridCell::split additionally needs room for one dimension cycle.
    return checked_product(levels,dimensions,limit-dimensions);
}

} // namespace Ariadne::PavingDetail
#endif
