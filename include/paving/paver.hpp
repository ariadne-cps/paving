/***************************************************************************
 *            paving/paver.hpp
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

/** @file paving/paver.hpp
 *  @brief Algorithms constructing outer approximations to geometric sets.
 */

#ifndef ARIADNE_PAVER_HPP
#define ARIADNE_PAVER_HPP

#include "paving/paver_interface.hpp"
#include "utility/handle.hpp"

namespace Ariadne {

template<class X>
class Vector;

/** @brief Paver supports algorithms constructing outer approximations to geometric sets. */
class Paver
    : public Handle<const PaverInterface> {
  public:
    typedef PaverInterface::SetType SetType;
    using Handle<const PaverInterface>::Handle;
    /** @brief Modify the paving representation. */
    Void adjoin_outer_approximation(PavingInterface& paving, const SetType& set, Nat fineness) const {
        this->_ptr->adjoin_outer_approximation(paving, set, fineness);
    }
    friend OutputStream& operator<<(OutputStream& os, const Paver& pv) { return pv._ptr->_write(os); }
};

//! \brief A class for computing outer approximations to sets defined by functions.
class AffinePaver : public PaverInterface {
  public:
    /** @brief Modify the paving representation. */
    Void adjoin_outer_approximation(PavingInterface& paving, const SetType& set, Nat fineness) const;
    /** @brief Perform the paving operation. */
    OutputStream& _write(OutputStream& os) const;
};

//! \brief A class for computing outer approximations to sets defined by functions.
class SubdivisionPaver : public PaverInterface {
  public:
    /** @brief Modify the paving representation. */
    Void adjoin_outer_approximation(PavingInterface& paving, const SetType& set, Nat fineness) const;
    /** @brief Modify the paving representation. */
    Void adjoin_outer_approximation_recursion(PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness, const Vector<FloatDP>& max_errors) const;
    /** @brief Perform the paving operation. */
    OutputStream& _write(OutputStream& os) const;
};

//! \brief A class for computing outer approximations to sets defined by functions.
class ReducePaver : public PaverInterface {
  public:
    /** @brief Modify the paving representation. */
    Void adjoin_outer_approximation(PavingInterface& paving, const SetType& set, Nat fineness) const;
    /** @brief Perform the paving operation. */
    OutputStream& _write(OutputStream& os) const;
};

//! \brief A class for computing outer approximations to sets defined by functions.
class ConstraintPaver : public PaverInterface {
  public:
    /** @brief Modify the paving representation. */
    Void adjoin_outer_approximation(PavingInterface& paving, const SetType& set, Nat fineness) const;
    /** @brief Perform the paving operation. */
    OutputStream& _write(OutputStream& os) const;
};

//! \brief A class for computing outer approximations to sets defined by functions.
class OptimalConstraintPaver : public PaverInterface {
  public:
    /** @brief Modify the paving representation. */
    Void adjoin_outer_approximation(PavingInterface& paving, const SetType& set, Nat fineness) const;
    /** @brief Perform the paving operation. */
    OutputStream& _write(OutputStream& os) const;
};

} // namespace Ariadne

#endif /* ARIADNE_PAVER_HPP */
