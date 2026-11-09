#ifndef ARIADNE_PAVING_PREPARED_SEPARATION_HPP
#define ARIADNE_PAVING_PREPARED_SEPARATION_HPP

#include "set/function_set.hpp"
#include <typeinfo>

namespace Ariadne::PavingDetail {

// Preparation snapshots the base implementation. Derived virtual predicates
// must retain their dispatch, and the solver requires positive dimension.
inline ConstrainedImageSet const* prepared_separation_source(
        EffectiveEuclideanCompactSetInterface const& set) {
    auto const* constrained=dynamic_cast<ConstrainedImageSet const*>(&set);
    if(constrained and typeid(*constrained)==typeid(ConstrainedImageSet)
       and constrained->dimension()!=0u) return constrained;
    return nullptr;
}

}
#endif
