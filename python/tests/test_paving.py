# Copyright 2026 Luca Geretti
# Ariadne is licensed under the GNU GPL version 3 or later.

from pyariadne_paving import *

def test_grid_paving():
    grid = Grid(2)
    paving = BinaryTreePaving(grid)
    assert paving.dimension() == 2

def test_set_approximation_bindings():
    box = FloatDPExactBox([{exact(-1): exact(1)}])
    set_value = ValidatedConstrainedImageSet(box)
    grid = Grid(1)
    outer = set_value.outer_approximation(grid, 0)
    assert outer.dimension() == 1
    assert not outer.is_empty()
    other = BinaryTreePaving(grid)
    set_value.adjoin_outer_approximation_to(other, 0)
    assert not other.is_empty()
    direct = outer_approximation(set_value, grid, 0)
    assert not direct.is_empty()


def test_polymorphic_nested_branch_live_view():
    # Validate the binding's value transport, not the standalone tutorial.
    grid = Grid(1)
    root = GridCell(grid, 0, BinaryWord(""))
    left_left = root.split(False).split(False)

    def check_live_view(paving):
        paving.adjoin(left_left)
        snapshot = Paving(paving)
        left_view = paving.branch(False)
        left_view.branch(False).set_root_cell(False)
        assert not paving.superset(left_left)
        assert snapshot.superset(left_left)

    check_live_view(Paving.tree(grid))
    manager = RobddManager()
    robdd = Paving.robdd(grid, manager)
    del manager
    check_live_view(robdd)
