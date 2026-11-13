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

#include <cstdint>
#include <vector>

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

/** @brief Statistics for one execution of the affine outer approximation.
 *  Unresolved domains are retained conservatively when the split-depth cap is hit.
 */
struct AffinePaverDiagnostics {
    Nat depth_limit = 0u;
    Nat completed_levels = 0u;
    SizeType split_domains = 0u;
    SizeType unresolved_domains = 0u;

    /** @brief Whether the requested affine error was not achieved by the depth cap. */
    bool depth_limit_reached() const { return unresolved_domains != 0u; }
};

//! \brief A class for computing outer approximations to sets defined by functions.
class AffinePaver : public PaverInterface {
  public:
    /** @brief Modify the paving representation. */
    Void adjoin_outer_approximation(PavingInterface& paving, const SetType& set, Nat fineness) const;
    /** @brief Run the same algorithm while collecting depth-limit diagnostics. */
    Void adjoin_outer_approximation(PavingInterface& paving, const SetType& set, Nat fineness,
                                    AffinePaverDiagnostics& diagnostics) const;
    /** @brief Perform the paving operation. */
    OutputStream& _write(OutputStream& os) const;

  private:
    Void _adjoin_outer_approximation(PavingInterface& paving, const SetType& set, Nat fineness,
                                     AffinePaverDiagnostics* diagnostics) const;
};

/** @brief Work counters for one instrumented Paver execution.
 *  Visits are recursive calls (parameter subdomains in SubdivisionPaver,
 *  domain/cell pairs in ReducePaver and ConstraintPaver). Evaluation counts
 *  denote vector/interval range evaluations, not scalar arithmetic steps.
 */
struct PaverDiagnostics {
    SizeType visited_nodes = 0u;
    SizeType parameter_splits = 0u;
    SizeType cell_splits = 0u;
    SizeType map_evaluations = 0u;
    SizeType constraint_evaluations = 0u;
    SizeType sensitivity_evaluations = 0u;
    SizeType reduction_calls = 0u;
    SizeType box_reduction_calls = 0u;
    SizeType hull_reduction_calls = 0u;
    SizeType constraint_reduction_calls = 0u;
    SizeType join_constructions = 0u;
    SizeType disjoint_rejections = 0u;
    SizeType already_covered = 0u;
    SizeType adjoin_calls = 0u;

    // Exclusive measured durations of selected operations in the instrumented
    // replay (nanoseconds). A joint f/g range evaluation has its own bucket;
    // it is not counted twice as an individual f or g evaluation.
    std::uint64_t map_range_ns = 0u;
    std::uint64_t constraint_range_ns = 0u;
    std::uint64_t joint_range_ns = 0u;
    std::uint64_t sensitivity_ns = 0u;
    std::uint64_t contractor_ns = 0u;
    std::uint64_t box_reduction_ns = 0u;
    std::uint64_t hull_reduction_ns = 0u;
    std::uint64_t join_ns = 0u;
    std::uint64_t adjoin_ns = 0u;
};

//! \brief A class for computing outer approximations to sets defined by functions.
class SubdivisionPaver : public PaverInterface {
  public:
    /** @brief Modify the paving representation. */
    Void adjoin_outer_approximation(PavingInterface& paving, const SetType& set, Nat fineness) const;
    /** @brief Run with per-execution operation counters. */
    Void adjoin_outer_approximation(PavingInterface& paving, const SetType& set,
                                    Nat fineness, PaverDiagnostics& diagnostics) const;
    /** @brief Modify the paving representation using specified output errors. */
    Void adjoin_outer_approximation_recursion(PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness, const Vector<FloatDP>& max_errors) const;
    /** @brief Perform the paving operation. */
    OutputStream& _write(OutputStream& os) const;

  private:
    Void _adjoin_outer_approximation(PavingInterface& paving, const SetType& set,
                                     Nat fineness, PaverDiagnostics* diagnostics) const;
    Void _adjoin_outer_approximation_recursion(PavingInterface& paving,
                                               const ValidatedConstrainedImageSet& set,
                                               Nat fineness, const Vector<FloatDP>& max_errors,
                                               PaverDiagnostics* diagnostics,
                                               std::vector<ValidatedConstrainedImageSet>* frontier = nullptr,
                                               Nat remaining_frontier_depth = 0u) const;
};

//! \brief A class for computing outer approximations to sets defined by functions.
class ReducePaver : public PaverInterface {
  public:
    /** @brief Modify the paving representation. */
    Void adjoin_outer_approximation(PavingInterface& paving, const SetType& set, Nat fineness) const;
    /** @brief Run with per-execution operation counters. */
    Void adjoin_outer_approximation(PavingInterface& paving, const SetType& set,
                                    Nat fineness, PaverDiagnostics& diagnostics) const;
    /** @brief Perform the paving operation. */
    OutputStream& _write(OutputStream& os) const;

  private:
    Void _adjoin_outer_approximation(PavingInterface& paving, const SetType& set,
                                     Nat fineness, PaverDiagnostics* diagnostics) const;
};

//! \brief A class for computing outer approximations to sets defined by functions.
class ConstraintPaver : public PaverInterface {
  public:
    /** @brief Modify the paving representation. */
    Void adjoin_outer_approximation(PavingInterface& paving, const SetType& set, Nat fineness) const;
    /** @brief Run with per-execution operation counters. */
    Void adjoin_outer_approximation(PavingInterface& paving, const SetType& set,
                                    Nat fineness, PaverDiagnostics& diagnostics) const;
    /** @brief Perform the paving operation. */
    OutputStream& _write(OutputStream& os) const;

  private:
    Void _adjoin_outer_approximation(PavingInterface& paving, const SetType& set,
                                     Nat fineness, PaverDiagnostics* diagnostics) const;
};

} // namespace Ariadne

#endif /* ARIADNE_PAVER_HPP */
