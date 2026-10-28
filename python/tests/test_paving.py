# Copyright 2026 Luca Geretti
# Ariadne is licensed under the GNU GPL version 3 or later.

from pyariadne import *

def test_grid_paving():
    grid = Grid(2)
    paving = GridTreePaving(grid)
    assert paving.dimension() == 2

def test_set_approximation_bindings():
    box = FloatDPExactBox([{exact(-1): exact(1)}])
    set_value = ValidatedConstrainedImageSet(box)
    grid = Grid(1)
    outer = set_value.outer_approximation(grid, 0)
    assert outer.dimension() == 1
    assert not outer.is_empty()
    other = GridTreePaving(grid)
    set_value.adjoin_outer_approximation_to(other, 0)
    assert not other.is_empty()
    direct = outer_approximation(set_value, grid, 0)
    assert not direct.is_empty()
