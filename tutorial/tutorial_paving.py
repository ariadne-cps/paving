#!/usr/bin/env python3
"""The same backend-independent paving workflow as the C++ tutorial."""

from pyariadne_paving import (
    BinaryWord, Grid, GridCell, Paving, RobddManager,
    join, intersection, difference,
)


def demonstrate(backend, first, second, root):
    """Use only Paving and SubPaving, never a concrete backend."""
    left = root.split(False)
    right = root.split(True)
    left_left = left.split(False)
    left_right = left.split(True)
    right_left = right.split(False)

    first.adjoin([left_left, right_left])
    second.adjoin([left_right, right_left])

    united = join(first, second)
    common = intersection(first, second)
    difference_set = difference(first, second)

    if not all(united.superset(cell) for cell in
               (left_left, left_right, right_left)):
        raise AssertionError("Incorrect union")
    if not (common.superset(right_left) and
            not common.superset(left_left) and
            not common.superset(left_right)):
        raise AssertionError("Incorrect intersection")
    if not (difference_set.superset(left_left) and
            not difference_set.superset(right_left)):
        raise AssertionError("Incorrect difference")

    print(f"\n== {backend} ==")
    print(f"union: {united.size()} cells, "
          f"intersection: {common.size()} cells, "
          f"difference: {difference_set.size()} cells")
    print("enabled cells:", *list(united))

    # A Paving copy is independent. A SubPaving is a live view of its owner.
    snapshot = Paving(first)
    left_view = first.branch(False)
    left_view.branch(False).set_root_cell(False)
    if first.superset(left_left):
        raise AssertionError("SubPaving did not modify its owner")
    if not snapshot.superset(left_left):
        raise AssertionError("Copy unexpectedly changed")
    print("live view modified original; copy remained unchanged")

    return united, common, difference_set


def main():
    grid = Grid(1)
    root = GridCell(grid, 0, BinaryWord(""))

    demonstrate("Binary tree", Paving.tree(grid), Paving.tree(grid), root)

    # Both ROBDD-backed pavings use one canonical manager, retained by Paving.
    manager = RobddManager()
    first = Paving.robdd(grid, manager)
    second = Paving.robdd(grid, manager)
    del manager
    demonstrate("ROBDD (shared manager)", first, second, root)


if __name__ == "__main__":
    main()
