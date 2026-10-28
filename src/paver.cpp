/***************************************************************************
 *            paving/paver.cpp
 *
 *  Copyright  2011-20  Pieter Collins
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne.
 *
 *  Ariadne is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "function/functional.hpp"
#include "paving/paver.hpp"
#include "paving/set_approximation.hpp"

#include "macro/general.hpp"
#include "logging/logging.hpp"
#include "algebra/polynomial.hpp"
#include "function/function.hpp"
#include "function/taylor_function.hpp"
#include "function/procedure.hpp"
#include "nonlinear-programming/nonlinear_programming.hpp"
#include "nonlinear-programming/nonlinear_interior_point_method.hpp"
#include "constraint-solving/constraint_solver.hpp"
#include "set/function_set.hpp"
#include "set/affine_set.hpp"
#include "paving/paving_interface.hpp"
#include "paving/grid_paving.hpp"
#include "set/affine_set.hpp"
#include "algebra/algebra.hpp"


namespace Ariadne {

Pair<SizeType,FloatDPError> nonlinearity_index_and_error(const ValidatedVectorMultivariateFunction& function, const ExactBoxType& domain);
Pair<SizeType,FloatDPError> lipschitz_index_and_error(const ValidatedVectorMultivariateFunction& function, const ExactBoxType& domain);
inline Pair<SizeType,FloatDPError> lipschitz_index_and_error(const ValidatedVectorMultivariateFunction& function, const UpperBoxType& domain) {
    return lipschitz_index_and_error(function,cast_exact_box(domain));
}

namespace {

PositiveFloatDPUpperBound average_width(const UpperBoxType& bx) {
    PositiveFloatDPUpperBound res(0u,double_precision);
    for(SizeType i=0; i!=bx.size(); ++i) {
        res+=bx[i].width();
    }
    // ReducePaver delegates parameter-free maps before entering reduction.
    return res/bx.size();
}

PositiveFloatDPUpperBound maximum_scaled_width(const UpperBoxType& bx, const Vector<PositiveFloatDP>& sf) {
    PositiveFloatDPUpperBound res(0u,double_precision);
    for(SizeType i=0; i!=bx.size(); ++i) {
        res=max(bx[i].width()/sf[i],res);
    }
    return res;
}

PositiveFloatDPUpperBound average_scaled_width(const UpperBoxType& bx, const Vector<PositiveFloatDP>& sf) {
    PositiveFloatDPUpperBound res(0u,double_precision);
    for(SizeType i=0; i!=bx.size(); ++i) {
        res+=(bx[i].width()/sf[i]);
    }
    return bx.size()==0u ? res : res/bx.size();
}


FloatDP average_scaled_width(const UpperBoxType& bx, const Vector<FloatDP>& sf) {
    Vector<PositiveFloatDP> positive_sf(sf.size(),[&](SizeType i){ return cast_positive(sf[i]); },dp);
    return average_scaled_width(bx,positive_sf).raw();
}

} // namespace


OutputStream& AffinePaver::_write(OutputStream& os) const { return os << "AffinePaver()"; }
OutputStream& SubdivisionPaver::_write(OutputStream& os) const { return os << "SubdivisionPaver()"; }
OutputStream& ReducePaver::_write(OutputStream& os) const { return os << "ReducePaver()"; }
OutputStream& ConstraintPaver::_write(OutputStream& os) const { return os << "ConstraintPaver()"; }
OutputStream& OptimalConstraintPaver::_write(OutputStream& os) const { return os << "OptimalConstraintPaver()"; }

Void SubdivisionPaver::adjoin_outer_approximation(PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness) const
{
    Vector<FloatDP> max_errors(paving.dimension(),dp);
    for(SizeType i=0; i!=max_errors.size(); ++i) {
        max_errors[i]=shft(static_cast<FloatDP>(paving.grid().lengths()[i]),-static_cast<int>(fineness));
    }

    this->adjoin_outer_approximation_recursion(paving,set,fineness,max_errors);
}

Void SubdivisionPaver::adjoin_outer_approximation_recursion(PavingInterface& paving, ValidatedConstrainedImageSet const& set, Nat fineness, const Vector<FloatDP>& max_errors) const
{
    // How small an over-approximating box needs to be relative to the cell size
    static const ExactDouble RELATIVE_SMALLNESS=0.5_x;

    ARIADNE_PRECONDITION(paving.dimension()==set.dimension());
    ARIADNE_PRECONDITION(max_errors.size()==paving.dimension());
    for(SizeType i=0; i!=max_errors.size(); ++i) { ARIADNE_PRECONDITION(max_errors[i]>FloatDP(0,dp)); }
    const ExactBoxType& subdomain = set.reduced_domain();
    if(definitely(subdomain.is_empty())) { return; }
    const ValidatedVectorMultivariateFunction& function = set.function();
    const List<ValidatedConstraint>& constraints = set.constraints();

    for(List<ValidatedConstraint>::ConstIterator iter=constraints.begin();
        iter!=constraints.end(); ++iter)
    {
        UpperIntervalType constraint_range=apply(iter->function(),subdomain);
        if( definitely(constraint_range.lower_bound() > iter->bounds().upper_bound() || constraint_range.upper_bound() < iter->bounds().lower_bound() ) ) { return; }
    }

    UpperBoxType range=apply(function,subdomain);
    Bool small=true;
    for(SizeType i=0; i!=range.size(); ++i) {
        if(possibly(hlf(range[i].width())>max_errors[i]*RELATIVE_SMALLNESS)) {
            small=false;
            break;
        }
    }

    if(small) {
        paving.adjoin_outer_approximation(cast_exact_box(range),fineness);
    } else {
        const auto sensitivity=lipschitz_index_and_error(function,subdomain);
        FloatDP minimum_error=max_errors[0];
        for(SizeType i=1u; i!=max_errors.size(); ++i) { minimum_error=min(minimum_error,max_errors[i]); }
        if(sensitivity.first==subdomain.dimension() || sensitivity.second.raw()<=mul(down,minimum_error,FloatDP(RELATIVE_SMALLNESS,dp))) {
            // Further parameter subdivision cannot remove intrinsic output
            // uncertainty. Discretise its enclosing box at the requested grid resolution.
            paving.adjoin_outer_approximation(cast_exact_box(range),fineness);
            return;
        }
        Pair<ValidatedConstrainedImageSet,ValidatedConstrainedImageSet> subsets=set.split(sensitivity.first);
        this->adjoin_outer_approximation_recursion(paving,subsets.first,fineness,max_errors);
        this->adjoin_outer_approximation_recursion(paving,subsets.second,fineness,max_errors);
    }
}



Void AffinePaver::adjoin_outer_approximation(PavingInterface& paving,
                                             const ValidatedConstrainedImageSet& set,
                                             Nat fineness) const
{
    ARIADNE_PRECONDITION(paving.dimension()==set.dimension());
    const ExactBoxType& subdomain = set.reduced_domain();
    if(definitely(subdomain.is_empty())) { return; }
    const ValidatedVectorMultivariateFunction& function = set.function();
    const List<ValidatedConstraint>& constraints = set.constraints();

    // Bound the maximum number of splittings allowed to draw a particular set.
    // Note that this gives rise to possibly 2^MAX_DEPTH split sets!!
    static const Nat MAXIMUM_DEPTH = 16;

    // The basic approximation error when plotting with accuracy=0
    static const ExactDouble BASIC_ERROR = 0.0625_pr;

    const ExactDouble max_error=BASIC_ERROR/TwoExp(static_cast<Int>(fineness));

    // Constraint functions are constructed on the mapping function's domain.
    ValidatedVectorMultivariateFunction fg=join(function,set.constraint_function());

    List<ExactBoxType> subdomains;
    List<ExactBoxType> unsplitdomains;
    List<ExactBoxType> splitdomains;
    unsplitdomains.append(subdomain);
    ExactBoxType splitdomain1,splitdomain2;
    for(Nat i=0; i!=MAXIMUM_DEPTH; ++i) {
        //std::cerr<<"i="<<i<<"\nsubdomains="<<subdomains<<"\nunsplitdomains="<<unsplitdomains<<"\n\n";
        for(SizeType n=0; n!=unsplitdomains.size(); ++n) {
            SizeType k; FloatDPError err(dp);
            make_lpair(k,err)=nonlinearity_index_and_error(fg,unsplitdomains[n]);
            //std::cerr<<"  domain="<<unsplitdomains[n]<<" k="<<k<<" err="<<err<<" max_err="<<max_error<<"\n";
            if(k==subdomain.size() || definitely(err < max_error)) {
                subdomains.append(unsplitdomains[n]);
            } else {
                make_lpair(splitdomain1,splitdomain2)=unsplitdomains[n].split(k);
                splitdomains.append(splitdomain1);
                splitdomains.append(splitdomain2);
            }
        }
        unsplitdomains.swap(splitdomains);
        splitdomains.clear();
        if(unsplitdomains.empty()) { break; }
    }
    subdomains.concatenate(unsplitdomains);
    if(!unsplitdomains.empty()) {
        ARIADNE_WARN("Cannot obtain desired accuracy in outer approximation without excessive splitting.");
    }

    for(SizeType n=0; n!=subdomains.size(); ++n) {
        ValidatedConstrainedImageSet subset(subdomains[n],function,constraints);
        adjoin_outer_approximation_to(paving,subset.affine_over_approximation(),fineness);
    }
}

namespace {

template<class Y, class PR> decltype(auto) make_raw_float(Y const& y, PR pr) { return RawFloat<PR>(y,pr); }

inline Bool strictly_smaller_by_factor(PositiveFloatDPUpperBound x1, PositiveFloatDPUpperBound x2, ExactDouble sf) {
    return x1.raw() < mul(down,x2.raw(),make_raw_float(sf,dp)); }
inline Bool strictly_smaller(PositiveFloatDPUpperBound x1, PositiveFloatDPUpperBound x2) {
    return x1.raw() < x2.raw(); }


FloatDP cell_splitting_radius(GridCell const& cell) {
    // The zero-dimensional cell is a singleton, with no coordinate widths.
    return cell.dimension()==0u ? FloatDP(0,dp) : cell.box().radius().upper().raw();
}

// Adjoin an over-approximation to the solution of $f(dom)$ such that $g(D) in C$ to the paving p, looking only at solutions in b.
Void procedure_constraint_adjoin_outer_approximation_recursion(
        PavingInterface& paving, const ExactBoxType& domain, const ValidatedVectorMultivariateFunction& f,
        const ValidatedVectorMultivariateFunction& g, const ExactBoxType& codomain, const GridCell& cell, Int max_dpth, SizeType splt, const List<ValidatedProcedure>& procedures)
{
    LOGGING_SCOPE_CREATE;

    const SizeType m=domain.size();
    const SizeType nf=f.result_size();
    const SizeType ng=g.result_size();

    const ExactBoxType& cell_box=cell.box();
    const Vector<PositiveFloatDP> scalings=Vector<PositiveFloatDP>(paving.grid().lengths());

    UpperBoxType bbox = apply(f,domain);

    FloatDP domwdth = average_width(domain).raw();
    FloatDP bbxwdth = average_scaled_width(bbox,paving.grid().lengths());
    FloatDP clwdth = average_scaled_width(cell_box,paving.grid().lengths());

    LOGGING_PRINTLN_AT(1,"splt="<<splt<<" dpth="<<cell.depth()<<" max_dpth="<<max_dpth);
    LOGGING_PRINTLN_AT(1,"domwdth="<<domwdth<<" bbxwdth="<<bbxwdth<<" clwdth="<<clwdth<<" dom="<<domain<<" bbox="<<bbox<<" cell="<<cell.box());

    ConstraintSolver constraint_solver;

    if(paving.superset(cell)) {
        LOGGING_PRINTLN("Cell is already a subset of paving");
        return;
    }

    // Try to prove disjointness
    const ExactBoxType& old_domain=domain;
    UpperBoxType new_domain(old_domain);
    PositiveFloatDPUpperBound olddomwdth = average_width(domain);
    PositiveFloatDPUpperBound newdomwdth = olddomwdth;

    static const ExactDouble ACCEPTABLE_REDUCTION_FACTOR = 0.75_x;


    // ExactBoxType reduction steps
    for(SizeType i=0; i!=nf; ++i) {
        for(SizeType j=0; j!=m; ++j) {
            constraint_solver.box_reduce(new_domain,f[i],cell_box[i],j);
            if(definitely(new_domain.is_empty())) { LOGGING_PRINTLN("Proved disjointness using box reduce"); return; }
        }
    }
    for(SizeType i=0; i!=ng; ++i) {
        for(SizeType j=0; j!=m; ++j) {
            constraint_solver.box_reduce(new_domain,g[i],codomain[i],j);
            if(definitely(new_domain.is_empty())) { LOGGING_PRINTLN("Proved disjointness using box reduce"); return; }
        }
    }
    newdomwdth=average_width(new_domain);
    LOGGING_PRINTLN_AT(1,"domwdth="<<newdomwdth<<" olddomwdth="<<olddomwdth<<" dom="<<new_domain<<" box reduce");

    // Hull reduction steps
    do {
        olddomwdth=newdomwdth;
        for(SizeType i=0; i!=nf; ++i) {
            constraint_solver.hull_reduce(new_domain,procedures[i],cell_box[i]);
            if(definitely(new_domain.is_empty())) { LOGGING_PRINTLN("Proved disjointness using hull reduce"); return; }
            //constraint_solver.hull_reduce(new_domain,f[i],cell_box[i]);
        }
        for(SizeType i=0; i!=ng; ++i) {
            constraint_solver.hull_reduce(new_domain,procedures[nf+i],codomain[i]);
            if(definitely(new_domain.is_empty())) { LOGGING_PRINTLN("Proved disjointness using hull reduce"); return; }
            //constraint_solver.hull_reduce(new_domain,g[i],codomain[i]);
        }
        newdomwdth=average_width(new_domain);
        LOGGING_PRINTLN_AT(2,"domwdth="<<newdomwdth<<" dom="<<new_domain);
    // Each reduction above returns immediately if the domain becomes empty.
    } while( strictly_smaller_by_factor(newdomwdth , olddomwdth, ACCEPTABLE_REDUCTION_FACTOR) );

    LOGGING_PRINTLN_AT(1,"new_domain="<<new_domain);


    domwdth = average_scaled_width(new_domain,RawFloatDPVector(new_domain.size(),FloatDP(1.0_x,dp)));
    bbox=apply(f,new_domain);
    bbxwdth=average_scaled_width(bbox,paving.grid().lengths());
    if(definitely(bbox.disjoint(cell_box)) || definitely(codomain.disjoint(apply(g,new_domain)))) {
        LOGGING_PRINTLN("Proved disjointness using image of new domain");
        return;
    }

    LOGGING_PRINTLN_AT(1,"domwdth="<<domwdth<<" bbxwdth="<<bbxwdth<<" clwdth="<<clwdth<<" dom="<<new_domain<<" bbox="<<bbox<<" cell="<<cell.box());

    // Decide whether to split cell or split domain by comparing size of
    // bounding box with the cell and splitting the larger.
    // It seems that a more efficient algorithm results if the domain
    // is only split if the bounding box is much larger, so we preferentiably
    // split the cell unless the bounding box is 4 times as large
    PositiveFloatDPUpperBound bbxmaxwdth = maximum_scaled_width(bbox,scalings);
    PositiveFloatDPUpperBound clmaxwdth = maximum_scaled_width(cell_box,scalings);
    ExactDouble RELATIVE_SPLITTING_SIZE = 4.0_x;

    if( !strictly_smaller_by_factor(bbxmaxwdth, clmaxwdth, RELATIVE_SPLITTING_SIZE) || (cell.depth()>=max_dpth && strictly_smaller(clmaxwdth, bbxmaxwdth)) ) {
        const auto sensitivity=lipschitz_index_and_error(f,new_domain);
        if(sensitivity.first<m && sensitivity.second.raw()>cell_splitting_radius(cell)) {
            LOGGING_PRINTLN("Splitting domain on coordinate "<<sensitivity.first);
            const auto domains=cast_exact_box(new_domain).split(sensitivity.first);
            procedure_constraint_adjoin_outer_approximation_recursion(paving, domains.first, f, g, codomain, cell, max_dpth, splt+1, procedures);
            procedure_constraint_adjoin_outer_approximation_recursion(paving, domains.second, f, g, codomain, cell, max_dpth, splt+1, procedures);
            return;
        }
    }
    if(cell.depth()>=max_dpth) {
        LOGGING_PRINTLN("Adjoining cell "<<cell_box);
        paving.adjoin(cell);
    } else {
        LOGGING_PRINTLN("Splitting cell "<<cell_box);
        Pair<GridCell,GridCell> sb = cell.split();
        procedure_constraint_adjoin_outer_approximation_recursion(paving,cast_exact_box(new_domain),f,g,codomain,sb.first, max_dpth, splt, procedures);
        procedure_constraint_adjoin_outer_approximation_recursion(paving,cast_exact_box(new_domain),f,g,codomain,sb.second, max_dpth, splt, procedures);
    }


}



// The former hot-start solver did not update its feasibility variable:
// it remained zero. Use validated interval subdivision with a sensitivity check, so intrinsic
// uncertainty cannot cause endless parameter splits.
Void interval_constraint_adjoin_outer_approximation_recursion(
    PavingInterface& paving, const ExactBoxType& domain,
    const ValidatedVectorMultivariateFunction& function,
    const ValidatedVectorMultivariateFunction& constraints, const ExactBoxType& bounds,
    const GridCell& cell, Nat fineness)
{
    if(paving.superset(cell)) { return; }
    const auto combined=join(function,constraints);
    if(definitely(disjoint(apply(combined,domain),product(cell.box(),bounds)))) { return; }

    const auto sensitivity=lipschitz_index_and_error(function,domain);
    if(sensitivity.first<domain.dimension() && sensitivity.second.raw()>cell_splitting_radius(cell)) {
        const auto domains=domain.split(sensitivity.first);
        interval_constraint_adjoin_outer_approximation_recursion(paving,domains.first,function,constraints,bounds,cell,fineness);
        interval_constraint_adjoin_outer_approximation_recursion(paving,domains.second,function,constraints,bounds,cell,fineness);
    } else if(cell.depth()>=Int(fineness*cell.dimension())) {
        paving.adjoin(cell);
    } else {
        const auto cells=cell.split();
        interval_constraint_adjoin_outer_approximation_recursion(paving,domain,function,constraints,bounds,cells.first,fineness);
        interval_constraint_adjoin_outer_approximation_recursion(paving,domain,function,constraints,bounds,cells.second,fineness);
    }
}

Void constraint_adjoin_outer_approximation(
    PavingInterface& paving, const ExactBoxType& domain,
    const ValidatedVectorMultivariateFunction& function,
    const ValidatedVectorMultivariateFunction& constraints, const ExactBoxType& bounds, Nat fineness)
{
    const GridCell cell=GridCell::smallest_enclosing_primary_cell(cast_exact_box(apply(function,domain)),paving.grid());
    const ExactBoxType constraint_bounds=intersection(cast_exact_box(widen(apply(constraints,domain),1)),bounds);
    interval_constraint_adjoin_outer_approximation_recursion(paving,domain,function,constraints,constraint_bounds,cell,fineness);
}

Void
procedure_constraint_adjoin_outer_approximation(PavingInterface& p, const ExactBoxType& d, const ValidatedVectorMultivariateFunction& f,
                                                const ValidatedVectorMultivariateFunction& g, const ExactBoxType& c, Nat e)
{
    GridCell b=p.smallest_enclosing_primary_cell(cast_exact_box(apply(f,d)));

    List<ValidatedProcedure> procedures;
    procedures.reserve(f.result_size()+g.result_size());
    for(SizeType i=0; i!=f.result_size(); ++i) { procedures.append(make_procedure(f[i])); }
    for(SizeType i=0; i!=g.result_size(); ++i) { procedures.append(make_procedure(g[i])); }

    Ariadne::procedure_constraint_adjoin_outer_approximation_recursion(p,d,f,g,c,b,static_cast<Int>(e*p.dimension()),0, procedures);
    //std::cerr<<"Computing outer approximation considered a total of "<<COUNT_TESTS<<" domains/cells\n";
    //std::cerr<<"Measure of paving is "<<p.measure()<<"\n";

    p.recombine();
}

} // namespace


Void ReducePaver::adjoin_outer_approximation(PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness) const {
    ARIADNE_PRECONDITION(paving.dimension()==set.dimension());
    if(definitely(set.reduced_domain().is_empty())) { return; }
    // With no parameters there is no domain to reduce. Procedure reduction
    // requires variables, while interval enclosure handles constant images.
    if(set.reduced_domain().dimension()==0u) { return ConstraintPaver().adjoin_outer_approximation(paving,set,fineness); }
    return procedure_constraint_adjoin_outer_approximation(paving,set.reduced_domain(),set.function(),set.constraint_function(),set.constraint_bounds(),fineness);
}

Void ConstraintPaver::adjoin_outer_approximation(PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness) const {
    ARIADNE_PRECONDITION(paving.dimension()==set.dimension());
    if(definitely(set.reduced_domain().is_empty())) { return; }
    return constraint_adjoin_outer_approximation(paving,set.reduced_domain(),set.function(),set.constraint_function(),set.constraint_bounds(),fineness);
}

Void OptimalConstraintPaver::adjoin_outer_approximation(PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness) const {
    // The removed optimisation initialiser was a no-op. Its zero feasibility
    // value caused unconditional recursion. Use validated constraint reduction
    // until an optimisation implementation can supply a feasibility certificate.
    return ReducePaver().adjoin_outer_approximation(paving,set,fineness);
}

} // namespace Ariadne

