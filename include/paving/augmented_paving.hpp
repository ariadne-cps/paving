/***************************************************************************
 *            paving/augmented_paving.hpp
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

/** @file paving/augmented_paving.hpp
 *  @brief Pavings augmented with auxiliary coordinate mappings.
 */

#ifndef ARIADNE_PAVING_AUGMENTED_PAVING_HPP
#define ARIADNE_PAVING_AUGMENTED_PAVING_HPP

#include <iosfwd>
#include "function/function.hpp"
#include "geometry/set_interface.hpp"
#include "paving/paving_interface.hpp"
#include "paving/binary_tree_paving.hpp"

namespace Ariadne {

typedef EffectiveVectorMultivariateFunction Mapping;

//! \ingroup DynamicsModule
//! \brief A backend-independent paving of states with an auxiliary mapping.
class AugmentedPaving
{
    Paving _state_set;
    EffectiveVectorMultivariateFunction _auxiliary_mapping;
  public:
    virtual ~AugmentedPaving() = default;

    typedef Grid GridType;
    typedef Paving PavingType;
//    AugmentedPaving(Grid const& g) : _state_set(g), _auxiliary_mapping(0,g.dimension()) { }
//    AugmentedPaving(BinaryTreePaving const& gtp) : _state_set(gtp), _auxiliary_mapping(0,gtp.dimension()) { }
    AugmentedPaving(Grid const& g, Mapping const& aux)
        : _state_set(new BinaryTreePaving(g)), _auxiliary_mapping(aux) { }
    AugmentedPaving(BinaryTreePaving const& gtp, Mapping const& aux)
        : _state_set(new BinaryTreePaving(gtp)), _auxiliary_mapping(aux) { }
    //! Caller-selected backend is retained across all subsequent operations.
    AugmentedPaving(Paving const& paving, Mapping const& aux)
        : _state_set(paving), _auxiliary_mapping(aux) {
        ARIADNE_PRECONDITION(paving.dimension()==aux.argument_size());
    }

    //! The state paving, without its auxiliary graph.
    Paving& state_paving() { return this->_state_set; }
    Paving const& state_paving() const { return this->_state_set; }
    //! Legacy reachability terminology, retained for existing callers.
    Paving& state_set() { return this->state_paving(); }
    Paving const& state_set() const { return this->state_paving(); }
    //! Outer paving of {(x,f(x)) : x is contained in the state paving}.
    BinaryTreePaving state_auxiliary_paving() const {
        // Explicit tree materialization for the geometric product/drawing,
        // never a replacement of the authoritative state representation.
        BinaryTreePaving tree(this->_state_set.grid());
        for(auto it=this->_state_set.begin(),end=this->_state_set.end();
            it!=end;++it) { tree.adjoin(*it); }
        /** @brief Perform the specified paving operation. */
        Grid auxiliary_grid(this->_auxiliary_mapping.result_size());
        return outer_skew_product(tree,auxiliary_grid,this->_auxiliary_mapping);
    }
    BinaryTreePaving state_auxiliary_set() const { return this->state_auxiliary_paving(); }
    Mapping const& auxiliary_mapping() const { return this->_auxiliary_mapping; }
    Mapping const& auxiliary_function() const { return this->_auxiliary_mapping; }
    Mapping const& auxiliary_data() const { return this->_auxiliary_mapping; }

    Bool is_empty() const { return this->_state_set.is_empty(); }
    SizeType size() const { return this->_state_set.size(); }
    Paving::ConstIterator begin() const { return this->_state_set.begin(); }
    Paving::ConstIterator end() const { return this->_state_set.end(); }

    Grid grid() const { return this->_state_set.grid(); }

    Void clear() { this->_state_set.clear(); }
    /** @brief Modify the paving or its subdivision. */
    Void adjoin(AugmentedPaving const& other) {
        this->_state_set.adjoin(static_cast<PavingInterface const&>(other._state_set)); }
    /** @brief Modify the paving or its subdivision. */
    Void restrict(AugmentedPaving const& other) {
        this->_state_set.restrict(static_cast<PavingInterface const&>(other._state_set)); }
    /** @brief Modify the paving or its subdivision. */
    Void remove(AugmentedPaving const& other) {
        this->_state_set.remove(static_cast<PavingInterface const&>(other._state_set)); }

    /** @brief Modify the paving or its subdivision. */
    Void adjoin_inner_approximation(EffectiveEuclideanSetInterface const& set, Nat fineness) {
        this->_state_set.adjoin_inner_approximation(set,fineness); }
    /** @brief Modify the paving or its subdivision. */
    Void adjoin_lower_approximation(EffectiveEuclideanOvertSetInterface const& set, Nat extent, Nat fineness) {
        this->_state_set.adjoin_lower_approximation(set,extent,fineness); }
    /** @brief Modify the paving or its subdivision. */
    Void adjoin_outer_approximation(EffectiveEuclideanCompactSetInterface const& set, Nat fineness) {
        this->_state_set.adjoin_outer_approximation(set,fineness); }
    /** @brief Modify the paving or its subdivision. */
    Void adjoin_outer_approximation(ValidatedEuclideanCompactSetInterface const& set, Nat fineness) {
        this->_state_set.adjoin_outer_approximation(set,fineness); }
    /** @brief Modify the paving or its subdivision. */
    Void adjoin_outer_approximation(ExactBoxType const& bx, Nat fineness) {
        this->_state_set.adjoin_outer_approximation(widen(bx),fineness); }

    Void restrict_to_extent(Nat level) { this->_state_set.restrict_to_extent(level); }
    Void mince(Nat fineness) { this->_state_set.mince(fineness); }
    Void recombine() { this->_state_set.recombine(); }

    /** @brief Test the stated geometric property. */
    friend Bool subset(AugmentedPaving const& set1, AugmentedPaving const& set2) {
        //ARIADNE_PRECONDITION(same(set1._auxiliary_mapping,set2._auxiliary_mapping));
        return set1._state_set.subset(static_cast<PavingInterface const&>(set2._state_set)); }
    /** @brief Perform the specified paving operation. */
    friend Bool intersect(AugmentedPaving const& set1, AugmentedPaving const& set2) {
        return set1._state_set.intersects(static_cast<PavingInterface const&>(set2._state_set)); }

    virtual AugmentedPaving* clone() const { return new AugmentedPaving(*this); }
    DimensionType dimension() const { return this->_state_set.dimension(); }

    /** @brief Perform the specified paving operation. */
    friend OutputStream& operator<<(OutputStream& os, AugmentedPaving const& set) {
        return os << "AugmentedPaving(" << set._state_set << "," << set._auxiliary_mapping << ")"; }
};


} // namespace Ariadne

#endif // ARIADNE_PAVING_AUGMENTED_PAVING_HPP
