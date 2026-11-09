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


def test_polymorphic_paving_backends_and_shared_manager():
    from pathlib import Path
    import sys

    # The public tutorial itself is the regression fixture: both backends
    # execute exactly the same algorithm and verify set identities and views.
    tutorial_dir = Path(__file__).resolve().parents[1] / "tutorials"
    sys.path.insert(0, str(tutorial_dir))
    try:
        from tutorial_paving import demonstrate
    finally:
        sys.path.remove(str(tutorial_dir))

    grid = Grid(1)
    root = GridCell(grid, 0, BinaryWord(""))
    first = demonstrate("Binary tree", Paving.tree(grid), Paving.tree(grid), root)

    manager = RobddManager()
    robdd_first = Paving.robdd(grid, manager)
    robdd_second = Paving.robdd(grid, manager)
    del manager
    second = demonstrate("ROBDD (shared manager)", robdd_first, robdd_second, root)

    # Representation-independent properties, not backend leaf-count equality.
    left = root.split(False)
    right = root.split(True)
    for result in (first, second):
        united, common, difference_set = result
        assert united.superset(left.split(False))
        assert united.superset(left.split(True))
        assert united.superset(right.split(False))
        assert common.superset(right.split(False))
        assert difference_set.superset(left.split(False))
