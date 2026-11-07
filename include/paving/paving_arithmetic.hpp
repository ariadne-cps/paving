/***************************************************************************
 *            paving_arithmetic.hpp
 *
 *  Copyright  2026  Luca Geretti
 *  This file is part of Ariadne Paving, distributed under the GNU GPL
 *  version 3 or (at your option) any later version.
 ****************************************************************************/
#ifndef ARIADNE_PAVING_ARITHMETIC_HPP
#define ARIADNE_PAVING_ARITHMETIC_HPP

#include <limits>
#include "utility/typedefs.hpp"
#include "utility/macros.hpp"

namespace Ariadne::PavingDetail {

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
Index checked_index(SizeType value) {
    ARIADNE_PRECONDITION_MSG(
        value<=static_cast<SizeType>(std::numeric_limits<Index>::max()),
        "Paving index exceeds the destination integer range");
    return static_cast<Index>(value);
}

template<class Index> requires (std::numeric_limits<Index>::digits >=
                               std::numeric_limits<SizeType>::digits)
Index checked_index(SizeType value) {
    return static_cast<Index>(value);
}

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
