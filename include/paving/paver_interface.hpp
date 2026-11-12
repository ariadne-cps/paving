/***************************************************************************
 *            paving/paver_interface.hpp
 *
 *  Copyright  2011-20  Pieter Collins
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

/** @file paving/paver_interface.hpp
 *  @brief Abstract interface for geometric paving algorithms.
 */

#ifndef ARIADNE_PAVER_INTERFACE_HPP
#define ARIADNE_PAVER_INTERFACE_HPP

#include "geometry/declarations.hpp"
#include "utility/writable.hpp"

namespace Ariadne {

/** @brief PavingInterface supports abstract interface for geometric paving algorithms. */
class PavingInterface;
/** @brief ValidatedConstrainedImageSet supports abstract interface for geometric paving algorithms. */
class ValidatedConstrainedImageSet;

//! \brief A class for computing outer approximations to sets defined by functions.
class PaverInterface : public virtual WritableInterface {
  public:
    typedef ValidatedConstrainedImageSet SetType;

  public:
    /** @brief Modify the paving representation. */
    virtual Void adjoin_outer_approximation(PavingInterface& paving, const SetType& set, Nat fineness) const = 0;
};

} // namespace Ariadne

#endif /* ARIADNE_PAVER_INTERFACE_HPP */
