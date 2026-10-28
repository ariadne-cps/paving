#include "paving/set_approximation.hpp"
#include "paving/paver.hpp"
#include "paving/grid_paving.hpp"
#include "paving/grid_cell.hpp"
#include "paving/paving_interface.hpp"
#include "set/function_set.hpp"
#include "set/affine_set.hpp"
#include "solving/linear_simplex_algorithm.hpp"
#include "utility/array.hpp"
#include "solving/linear_interior_point_method.hpp"
#include "logging/logging.hpp"
#include "numeric/numeric.hpp"
#include "algebra/vector.hpp"
#include "algebra/matrix.hpp"

namespace Ariadne {

// Feasibility data local to paving. Set exposes its coefficients directly.
struct PavingFeasibilityData {
    Matrix<FloatDP> A = Matrix<FloatDP>(0u,0u,dp);
    Vector<FloatDP> b = Vector<FloatDP>(0u,dp);
    Vector<FloatDP> l = Vector<FloatDP>(0u,dp);
    Vector<FloatDP> u = Vector<FloatDP>(0u,dp);
    Vector<FloatDP> c = Vector<FloatDP>(0u,dp);
    Array<Slackness> vt;
    Array<SizeType> p;
    Matrix<FloatDPBounds> B = Matrix<FloatDPBounds>(0u,0u,dp);
};


// Implemented in the set module; this transformation is independent of paving.
ExactBoxType over_approximation(const RealBox&);

Void adjoin_outer_approximation_to(PavingInterface& paving, const ConstrainedImageSet& original, Nat fineness) {
    ValidatedConstrainedImageSet set(over_approximation(original.domain()),original.function(),original.constraints());
    return adjoin_outer_approximation_to(paving,set,fineness);
}

GridTreePaving outer_approximation(const ValidatedConstrainedImageSet& set, const Grid& grid, Nat fineness) {
    GridTreePaving paving(grid);
    adjoin_outer_approximation_to(paving,set,fineness);
    return paving;
}

Void adjoin_outer_approximation_to(PavingInterface& paving, const ValidatedConstrainedImageSet& set, Nat fineness) {
    SubdivisionPaver().adjoin_outer_approximation(paving,set,fineness);
    paving.recombine();
}


GridTreePaving
outer_approximation(const ValidatedAffineConstrainedImageSet& set, const Grid& g, Nat depth) {
    GridTreePaving r(g);
    adjoin_outer_approximation_to(r,set,depth);
    return r;
}


static Void _adjoin_outer_approximation_to(PavingInterface& paving, PavingFeasibilityData& lp, const Vector<FloatDPError>& errors, GridCell& cell, Nat fineness, const UpperBoxType* enclosure)
{

    // No need to check if cell is already part of the set
    if(paving.superset(cell)) {
        return;
    }

    // Find concrete cell box
    const ExactBoxType bx=cell.box();

    // A validated enclosure is a cheap, sound rejection certificate.  In
    // particular, a cell touching the enclosure is not rejected.
    if(enclosure && definitely(disjoint(bx,*enclosure))) { return; }

    // Make part of linear program dependent on cell
    for(SizeType i=0; i!=cell.dimension(); ++i) {
        //lp.l[i]=bx[i].lower_bound();
        //lp.u[i]=bx[i].upper_bound();
        lp.l[i]=FloatDP(sub(down,bx[i].lower_bound(),errors[i].raw()));
        lp.u[i]=FloatDP(add(up,bx[i].upper_bound(),errors[i].raw()));
    }

    Int cell_depth=cell.depth();
    Int maximum_depth=static_cast<Int>(fineness*cell.dimension());

    // Check for disjointness using linear program
    LinearInteriorPointSolver optimiser;
    ValidatedKleenean feasible=optimiser.feasible(lp.l,lp.u,lp.A,lp.b);
    if(definitely(!feasible)) { return; }

    // If not disjoint, and we are at maximum depth, adjoin the set.
    // Otherwise, split and continue.

    if(cell_depth>=maximum_depth) {
        paving.adjoin(cell);
    } else {
        GridCell subcell1 = cell.split(0);
        GridCell subcell2 = cell.split(1);
        _adjoin_outer_approximation_to(paving,lp,errors,subcell1,fineness,enclosure);
        _adjoin_outer_approximation_to(paving,lp,errors,subcell2,fineness,enclosure);
    }

}





Void
adjoin_outer_approximation_to(PavingInterface& paving, const ValidatedAffineConstrainedImageSet& set, Nat fineness)
{
    ARIADNE_PRECONDITION(set.dimension()==paving.dimension());

    const UpperBoxType enclosure=set.bounding_box();
    GridCell bounding_cell=GridCell::smallest_enclosing_primary_cell(enclosure,paving.grid());

    // Create linear program
    PavingFeasibilityData lp;
    set.construct_linear_constraints(lp.A,lp.b,lp.l,lp.u);
    Vector<FloatDPError> errors(set.dimension(),dp);
    for(SizeType i=0; i!=set.dimension(); ++i) {
        errors[i]=set.space_models()[i].error();
    }
    _adjoin_outer_approximation_to(paving,lp,errors,bounding_cell,fineness,&enclosure);
}




static Void _robust_adjoin_outer_approximation_to(PavingInterface& paving, PavingFeasibilityData& lp, const Vector<FloatDPError>& errors, GridCell& cell, Nat fineness)
{
    const SizeType nx=cell.dimension();
    const SizeType ne=lp.A.column_size()-lp.A.row_size()-1u;

    if(paving.superset(cell)) {
        return;
    }

    const ExactBoxType& bx=cell.box();
    for(SizeType i=0; i!=nx; ++i) {
        lp.l[ne+i]=bx[i].lower_bound();
        lp.u[ne+i]=bx[i].upper_bound();
    }

    Int cell_depth=cell.depth();
    Int maximum_depth=static_cast<Int>(fineness*cell.dimension());

    // Robust intersection is equivalent to feasibility with the robustness
    // variable constrained to be nonnegative.  The previous implementation
    // started it at -1 and then optimised -r until r>=0, which needlessly
    // exercised degenerate simplex pivots.
    LinearInteriorPointSolver optimiser;
    ValidatedKleenean feasible=optimiser.feasible(lp.l,lp.u,lp.A,lp.b);
    if(definitely(not feasible)) {
        return;
    }

    if(cell_depth>=maximum_depth) {
        paving.adjoin(cell);
    } else {
        GridCell subcell1=cell.split(0);
        GridCell subcell2=cell.split(1);
        _adjoin_outer_approximation_to(paving,lp,errors,subcell1,fineness,nullptr);
        _adjoin_outer_approximation_to(paving,lp,errors,subcell2,fineness,nullptr);
    }
}


Void
robust_adjoin_outer_approximation_to(PavingInterface& paving, const ValidatedAffineConstrainedImageSet& set, Nat fineness) {
    LOGGING_SCOPE_CREATE;

    ARIADNE_PRECONDITION(set.dimension()==paving.dimension());

    GridCell bounding_cell=GridCell::smallest_enclosing_primary_cell(set.bounding_box(),paving.grid());

    // Set up linear programming problem.
    // We have parameter e and point x, which need to satisfy
    //  x=Ge+h;  Ce+d>=0; lb<=x<=ub, -1<=e<=+1
    // Add slack variables for the inequality constraints Ce+s=d; s>=0
    // The standard form is then
    //  -Ge+x=h,  -Ce+s=d; -1<=e<=+1, lb<=x<=ub, 0<=s
    // The only dependence on the cell is in the inequality constraints for x

    // Set up linear program Ax=b; l<=x<=u.
    // Order variables as e,x,s

    // Spacial dimension nx; parameter dimension np; number of constraints nc
    const SizeType nx=set.dimension();
    const SizeType ne=set.number_of_parameters();
    const SizeType nc=set.number_of_constraints();

    // Create linear program
    PavingFeasibilityData lp;
    lp.A.resize(nx+nc,nx+ne+nc+1u);
    lp.b.resize(nx+nc);
    lp.c.resize(nx+ne+nc+1u);
    lp.l.resize(nx+ne+nc+1u);
    lp.u.resize(nx+ne+nc+1u);
    lp.vt.resize(ne+nx+nc+1u);
    lp.p.resize(ne+nx+nc+1u);

    // Make part of linear program only dependent on set
    // Need to set all values since matrix is uninitialised
    for(SizeType i=0; i!=nx; ++i) {
        for(SizeType j=0; j!=ne; ++j) {
            lp.A[i][j]=-set.space_models()[i].gradient(j);
        }
        for(SizeType j=0; j!=nx; ++j) {
            lp.A[i][ne+j]=0;
        }
        lp.A[i][ne+i]=+1;
        for(SizeType j=0; j!=nc; ++j) {
            lp.A[i][ne+nx+j]=0;
        }
        lp.b[i]=set.space_models()[i].value();
    }
    for(SizeType i=0; i!=nc; ++i) {
        for(SizeType j=0; j!=ne; ++j) {
            lp.A[nx+i][j]=set.constraint_models()[i].function().gradient(j);
        }
        for(SizeType j=0; j!=nx; ++j) {
            lp.A[nx+i][ne+j]=0;
        }
        for(SizeType j=0; j!=nc; ++j) {
            lp.A[nx+i][ne+nx+j]=0;
        }
        lp.A[nx+i][nx+ne+i]=+1;
        lp.b[nx+i]=-set.constraint_models()[i].function().value();
    }
    for(SizeType i=0; i!=ne; ++i) {
        lp.l[i]=-1;
        lp.u[i]=+1;
    }
    for(SizeType i=0; i!=nc; ++i) {
        lp.l[ne+nx+i]=-set.constraint_models()[i].upper_bound().value();
        lp.u[ne+nx+i]=-set.constraint_models()[i].lower_bound().value();
    }


    // Make part of linear program used for robustness
    for(SizeType i=0; i!=ne+nx+nc; ++i) {
        lp.c[i]=0;
    }
    for(SizeType i=0; i!=nx+nc; ++i) {
        lp.A[i][ne+nx+nc]=1;
    }
    lp.c[ne+nx+nc]=-1;
    lp.l[ne+nx+nc]=0;
    lp.u[ne+nx+nc]=infty;
    lp.vt[ne+nx+nc]=Slackness::LOWER;
    lp.p[ne+nx+nc]=ne+nx+nc;


    // Make part of linear program dependent on cell
    const ExactBoxType bx=bounding_cell.box();
    for(SizeType i=0; i!=nx; ++i) {
        lp.l[ne+i]=bx[i].lower_bound();
        lp.u[ne+i]=bx[i].upper_bound();
    }

    // Take x and s variables to be basic, so the initial basis matrix is the
    // identity
    for(SizeType i=0; i!=ne; ++i) {
        lp.vt[i]=Slackness::LOWER;
        lp.p[nx+nc+i]=i;
    }
    for(SizeType i=0; i!=nx+nc; ++i) {
        lp.vt[ne+i]=Slackness::BASIS;
        lp.p[i]=ne+i;
    }
    lp.B=Matrix<FloatDPBounds>::identity(nx+nc,dp);

    Vector<FloatDPError> errors(set.dimension(),dp);
    for(SizeType i=0; i!=set.dimension(); ++i) {
        errors[i]=set.space_models()[i].error();
    }

    LOGGING_PRINTLN("A="<<lp.A<<"\nb="<<lp.b<<"\nl="<<lp.l<<"\nu="<<lp.u);

    _robust_adjoin_outer_approximation_to(paving,lp,errors,bounding_cell,fineness);
}


} // namespace Ariadne
