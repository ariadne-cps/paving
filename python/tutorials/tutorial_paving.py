#!/usr/bin/python3

from pyariadne import *


def section(title):
    print(f"\n== {title} ==")


def show(label, value):
    print(f"{label}: {value}")


def tutorial_paving():
    dp = DoublePrecision()

    section("Boxes as the basic geometric enclosure")

    workspace = FloatDPExactBox([
        {exact(-2.0): exact(2.0)},
        {exact(-1.5): exact(1.5)},
    ])
    observation = FloatDPExactBox([
        {exact(-0.75): exact(1.25)},
        {exact(-0.5): exact(1.0)},
    ])
    sample = FloatDPPoint([FloatDP(exact(0.5), dp), FloatDP(exact(0.25), dp)])

    show("workspace", workspace)
    show("observation", observation)
    show("dimension", workspace.dimension())
    show("centre", workspace.centre())
    show("widths", workspace.widths())
    show("radii", workspace.radii())
    show("semiperimeter", workspace.semiperimeter())
    show("area", workspace.measure())
    show("sample", sample)
    show("sample dimension", sample.dimension())
    show("workspace contains sample", workspace.contains(sample))
    show("observation inside workspace", observation.inside(workspace))
    show("workspace covers observation", workspace.covers(observation))

    overlap = intersection(workspace, observation)
    envelope = hull(
        workspace,
        FloatDPExactBox([
            {exact(1.0): exact(3.0)},
            {exact(-0.25): exact(0.5)},
        ]),
    )
    lower, upper = observation.split()

    show("intersection", overlap)
    show("hull", envelope)
    show("lower half after split", lower)
    show("upper half after split", upper)

    section("Sets defined by functions and constraints")

    u0 = EffectiveScalarMultivariateFunction.coordinate(2, 0)
    u1 = EffectiveScalarMultivariateFunction.coordinate(2, 1)
    parametrisation = EffectiveVectorMultivariateFunction([
        u0 + u1,
        u0 * u1,
    ])
    parameter_domain = FloatDPExactBox([
        {exact(-1): exact(1)},
        {exact(-1): exact(1)},
    ])
    image_set = ValidatedConstrainedImageSet(parameter_domain, parametrisation)

    show("parameter domain", parameter_domain)
    show("parametrisation", parametrisation)
    show("image set", image_set)
    show("image dimension", image_set.dimension())
    show("number of parameters", image_set.number_of_parameters())
    show("bounding box", image_set.bounding_box())

    # Parameter and space constraints are demonstrated in the C++ tutorial.
    # The Python tutorial adds them after the validated constraint operators
    # are exposed directly, rather than relying on an unsafe conversion from
    # EffectiveConstraint to ValidatedConstraint.

    section("Affine over-approximation")

    affine_set = image_set.affine_over_approximation()
    show("affine over-approximation", affine_set)
    show("affine bounding box", affine_set.bounding_box())

    section("Grid cells and paving")

    grid = Grid(2)
    show("grid", grid)
    show("grid origin", grid.origin())
    show("grid lengths", grid.lengths())

    primary = smallest_enclosing_primary_cell(image_set.bounding_box(), grid)
    show("smallest enclosing primary cell", primary)
    show("primary cell box", primary.box())

    coarse = image_set.outer_approximation(grid, 3)
    coarse.recombine()

    fine = image_set.outer_approximation(grid, 5)
    fine.recombine()

    show("coarse paving cells", coarse.size())
    show("coarse paving measure", coarse.measure())
    show("coarse paving bounds", coarse.bounding_box())
    show("fine paving cells", fine.size())
    show("fine paving measure", fine.measure())
    show("fine paving bounds", fine.bounding_box())

    common = intersection(coarse, fine)
    show("intersection of pavings", common)
    show("intersection cells", common.size())

    section("When to use each representation")
    print("Box: fast coordinate-aligned enclosure and predicates.")
    print("ValidatedConstrainedImageSet: keeps nonlinear parameter dependence and constraints.")
    print("ValidatedAffineConstrainedImageSet: rigorous affine surrogate for cheaper geometry.")
    print("GridTreePaving: finite cell representation for subdivision and set algebra.")


if __name__ == "__main__":
    tutorial_paving()
