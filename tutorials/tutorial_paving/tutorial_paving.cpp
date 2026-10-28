#include <iostream>
#include <string>

#include "ariadne-paving.hpp"
#include "paving/set_approximation.hpp"
#include "set/curve.hpp"

using namespace Ariadne;
using std::cout;

namespace {

void section(const std::string& title) {
    cout << "\n== " << title << " ==\n";
}

template<class T>
void show(const std::string& label, const T& value) {
    cout << label << ": " << value << "\n";
}

} // namespace

int main() {
    section("Points and boxes: cheap geometric enclosures");

    ExactBoxType workspace({
        {-2.0_x, 2.0_x},
        {-1.5_x, 1.5_x}
    });
    ExactBoxType observation({
        {-0.75_x, 1.25_x},
        {-0.5_x, 1.0_x}
    });
    FloatDPPoint sample({ExactDouble(0.5), ExactDouble(0.25)}, dp);

    show("workspace", workspace);
    show("sample", sample);
    show("sample dimension", sample.dimension());
    show("sample bounding box", sample.bounding_box());
    show("workspace dimension", workspace.dimension());
    show("workspace centre", workspace.centre());
    show("workspace widths", workspace.widths());
    show("workspace radii", workspace.radii());
    show("workspace semiperimeter", workspace.semiperimeter());
    show("workspace area", workspace.measure());

    // Geometric predicates are deliberately richer than Boolean tests.
    // For exact objects they can certify relations such as containment,
    // separation and overlap without replacing the object by a sample point.
    show("workspace contains sample", workspace.contains(sample));
    show("observation inside workspace", observation.inside(workspace));
    show("workspace covers observation", workspace.covers(observation));

    // Boxes also form a small enclosure algebra.  intersection() keeps only
    // common points, hull() returns a coordinate-aligned enclosure of both
    // operands, and split() is the primitive behind recursive subdivision.
    ExactBoxType overlap = intersection(workspace, observation);
    ExactBoxType envelope = hull(workspace, ExactBoxType({
        {1.0_x, 3.0_x},
        {-0.25_x, 0.5_x}
    }));
    auto halves = observation.split();

    show("intersection", overlap);
    show("hull", envelope);
    show("lower half after split", halves.first);
    show("upper half after split", halves.second);

    section("Implicit regions: geometry defined by constraints");

    // Some sets are easier to describe by a condition than by a
    // parametrisation.  The following set is the unit disk, encoded as
    // 0 <= x^2+y^2 <= 1.  ConstraintSet represents the preimage itself.
    auto q = EffectiveScalarMultivariateFunction::coordinates(2);
    EffectiveVectorMultivariateFunction radius_squared({
        sqr(q[0]) + sqr(q[1])
    });
    ConstraintSet unit_disk(
        radius_squared,
        RealBox(1u, RealInterval(0, 1))
    );

    // Intersecting with a box turns an unbounded implicit description into
    // a bounded located set.  This makes bounding boxes and image operations
    // available while preserving the defining constraint.
    RealBox search_window({
        {-1.25_x, 1.25_x},
        {-1.25_x, 1.25_x}
    });
    BoundedConstraintSet bounded_disk = intersection(search_window, unit_disk);

    show("implicit unit disk", unit_disk);
    show("constraint count", unit_disk.number_of_constraints());
    show("bounded disk", bounded_disk);
    show("bounded disk domain", bounded_disk.domain());
    show("bounded disk bounding box", bounded_disk.bounding_box());
    show("disk overlaps observation", bounded_disk.overlaps(observation));

    // Images convert an implicitly defined feasible region into a
    // parametrised geometric set.  This is a common pattern after solving a
    // feasibility problem: preserve the original admissible region, then map
    // it into the variables that are useful downstream.
    EffectiveVectorMultivariateFunction shear({
        q[0] + q[1] / 2,
        q[1]
    });
    ConstrainedImageSet sheared_disk = image(bounded_disk, shear);
    show("sheared disk", sheared_disk);
    show("sheared disk bounding box", sheared_disk.bounding_box());

    auto sheared_parts = sheared_disk.split(0);
    show("first split image", sheared_parts.first.bounding_box());
    show("second split image", sheared_parts.second.bounding_box());

    section("Validated parametrised sets: dependence plus constraints");

    // Here the parameters u=(u0,u1) range over a box and are mapped through a
    // nonlinear function.  Unlike a plain bounding box, the representation
    // keeps the parameter dependence, so later algorithms can subdivide,
    // propagate constraints and build validated approximations.
    auto u = ValidatedScalarMultivariateFunction::coordinates(2);
    ValidatedVectorMultivariateFunction parametrisation({
        u[0] + u[1],
        u[0] * u[1]
    });
    ExactBoxType parameter_domain = ExactBoxType::unit_box(2);
    ValidatedConstrainedImageSet image_set(parameter_domain, parametrisation);

    show("parameter domain", image_set.domain());
    show("parametrisation", image_set.function());
    show("image dimension", image_set.dimension());
    show("number of parameters", image_set.number_of_parameters());
    show("initial bounding box", image_set.bounding_box());

    // Constraints may live either in parameter space or in the image space.
    // Space constraints are automatically composed with the parametrisation.
    image_set.new_parameter_constraint(u[0] + u[1] <= ExactNumber(1.25_x));

    auto x = ValidatedScalarMultivariateFunction::coordinates(2);
    ValidatedExactConstraint state_constraint = x[0] - x[1] <= ExactNumber(1.5_x);
    image_set.new_space_constraint(state_constraint);

    show("constrained image set", image_set);
    show("constraint count", image_set.number_of_constraints());
    show("constraint bounds", image_set.constraint_bounds());
    show("bounding box before reduction", image_set.bounding_box());

    // reduce() contracts the parameter domain by propagating the constraints.
    // It does not change the represented set; it changes only the internal
    // domain used for subsequent computations.
    image_set.reduce();
    show("original parameter domain", image_set.domain());
    show("reduced parameter domain", image_set.reduced_domain());
    show("bounding box after reduction", image_set.bounding_box());
    show("is empty", image_set.is_empty());

    // Splitting and restriction are the two basic ways to localise a
    // nonlinear set.  Splitting is algorithmic; restriction is useful when
    // the user already knows which part of parameter space is relevant.
    auto image_parts = image_set.split(0);
    show("first validated split", image_parts.first.bounding_box());
    show("second validated split", image_parts.second.bounding_box());

    ExactBoxType left_parameter_domain({
        {-1.0_x, 0.0_x},
        {-1.0_x, 1.0_x}
    });
    ValidatedConstrainedImageSet restricted =
        image_set.restriction(left_parameter_domain);
    show("restricted parameter domain", restricted.domain());
    show("restricted image bounds", restricted.bounding_box());

    // Predicates act on the represented set rather than just on its bounding
    // box.  Results are validated logical values and may therefore be
    // indeterminate if the requested relation cannot be certified.
    ExactBoxType query_box({
        {-0.5_x, 0.5_x},
        {-0.5_x, 0.5_x}
    });
    show("overlaps query box", image_set.overlaps(query_box));
    show("separated from query box", image_set.separated(query_box));
    show("inside workspace", image_set.inside(workspace));

    section("Affine over-approximation: cheaper validated geometry");

    // An affine constrained image preserves correlations that a box loses,
    // while being cheaper to manipulate than a nonlinear constrained image.
    // It is therefore a useful hand-off representation for downstream
    // geometry algorithms.
    ValidatedAffineConstrainedImageSet affine_set(
        image_set.affine_over_approximation()
    );
    show("affine over-approximation", affine_set);
    show("affine dimension", affine_set.dimension());
    show("affine is bounded", affine_set.is_bounded());
    show("affine is empty", affine_set.is_empty());
    show("affine bounding box", affine_set.bounding_box());

    section("Interpolated curves: geometry from sampled trajectories");

    // InterpolatedCurve is useful when the data already comes as samples,
    // e.g. a simulation trace or a sequence of measured states.  The curve
    // retains the parameter values and linearly interpolates between points.
    FloatDPPoint p0({ExactDouble(0.0), ExactDouble(0.0)}, dp);
    FloatDPPoint p1({ExactDouble(0.5), ExactDouble(0.25)}, dp);
    FloatDPPoint p2({ExactDouble(1.0), ExactDouble(1.0)}, dp);

    InterpolatedCurve trajectory(FloatDP(0.0_x, dp), p0);
    trajectory.insert(FloatDP(0.5_x, dp), p1);
    trajectory.insert(FloatDP(1.0_x, dp), p2);

    show("trajectory", trajectory);
    show("stored samples", trajectory.size());
    show("trajectory dimension", trajectory.dimension());
    show("trajectory bounding box", trajectory.bounding_box());

    section("Grid cells and paving: finite set approximations");

    // A Grid fixes the coordinate system used by discrete set
    // approximations.  GridTreePaving then stores active cells in a binary
    // tree.  Fineness controls how far the approximation is subdivided.
    Grid grid(2);
    show("grid", grid);
    show("grid origin", grid.origin());
    show("grid lengths", grid.lengths());

    GridCell primary = GridCell::smallest_enclosing_primary_cell(
        image_set.bounding_box(), grid);
    show("smallest enclosing primary cell", primary);
    show("primary cell box", primary.box());

    GridTreePaving coarse = outer_approximation(image_set, grid, 3);
    coarse.recombine();

    GridTreePaving fine = outer_approximation(image_set, grid, 5);
    fine.recombine();

    show("coarse paving cells", coarse.size());
    show("coarse paving measure", coarse.measure());
    show("coarse paving bounds", coarse.bounding_box());
    show("fine paving cells", fine.size());
    show("fine paving measure", fine.measure());
    show("fine paving bounds", fine.bounding_box());

    // Pavings support ordinary set algebra provided that they share a grid.
    // A second paving built from an observation box can therefore be used as
    // a discrete mask or obstacle.
    GridTreePaving observation_paving(grid);
    observation_paving.adjoin_over_approximation(observation, 4);
    observation_paving.recombine();

    GridTreePaving common = intersection(fine, observation_paving);
    GridTreePaving outside_observation = difference(fine, observation_paving);
    GridTreePaving combined = join(fine, observation_paving);

    show("intersection cells", common.size());
    show("difference cells", outside_observation.size());
    show("union cells", combined.size());

    section("Choosing a geometry representation");
    cout
        << "Point / Box: exact locations and cheap coordinate-aligned enclosures.\n"
        << "ConstraintSet / BoundedConstraintSet: implicit feasible regions.\n"
        << "ConstrainedImageSet: effective images of constrained regions.\n"
        << "ValidatedConstrainedImageSet: nonlinear dependence with rigorous predicates.\n"
        << "ValidatedAffineConstrainedImageSet: cheaper correlated enclosure.\n"
        << "InterpolatedCurve: sampled paths and trajectories.\n"
        << "GridTreePaving: finite cell representation for subdivision and set algebra.\n";

    return 0;
}
