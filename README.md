# Ariadne Paving

[![macOS Status](https://github.com/ariadne-cps/paving/actions/workflows/macos.yml/badge.svg)](https://github.com/ariadne-cps/paving/actions/workflows/macos.yml)
[![Ubuntu Status](https://github.com/ariadne-cps/paving/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/ariadne-cps/paving/actions/workflows/ubuntu.yml)
[![Windows Status](https://github.com/ariadne-cps/paving/actions/workflows/win.yml/badge.svg)](https://github.com/ariadne-cps/paving/actions/workflows/win.yml)
[![Sanitizers Status](https://github.com/ariadne-cps/paving/actions/workflows/sanitizers.yml/badge.svg)](https://github.com/ariadne-cps/paving/actions/workflows/sanitizers.yml)
[![Coverage Status](https://github.com/ariadne-cps/paving/actions/workflows/coverage.yml/badge.svg)](https://github.com/ariadne-cps/paving/actions/workflows/coverage.yml)
[![codecov](https://codecov.io/gh/ariadne-cps/paving/branch/main/graph/badge.svg)](https://codecov.io/gh/ariadne-cps/paving)

Ariadne Paving is the standalone C++20 discrete paving layer of Ariadne. It provides binary trees, grids, cells, grid-tree pavings, set approximation and subdivision algorithms.
Although independently maintained and usable, this repository is also part of the [Ariadne repository structure](https://github.com/ariadne-cps/ariadne/blob/main/doc/structure.md).

## Layout

- Public headers: `include/paving/`, with umbrella header `include/ariadne-paving.hpp`.
- C++ implementations: `src/`.
- Design journal: [`doc/robdd_paving_design.md`](doc/robdd_paving_design.md).
- C++ tests: `tests/` (66 independently registered fixture cases across eight executables).
- Python bindings, tests and tutorial: `python/bindings/`, `python/tests/` and `python/tutorials/`.
- Standalone installed C++ example: `tutorials/tutorial_paving/`.

## Dependencies

Paving depends directly on:

- [ariadne-cps/set](https://github.com/ariadne-cps/set) (Git submodule `ss/set`), providing set representations and numerical solving dependencies.
- [ariadne-cps/bdd](https://github.com/ariadne-cps/bdd) (Git submodule `ss/bdd`), providing binary decision diagrams.
- [ariadne-cps/build](https://github.com/ariadne-cps/build) (Git submodule `ss/cfg`), providing shared CMake infrastructure.
- [ariadne-cps/testing](https://github.com/ariadne-cps/testing) (Git submodule `ss/tst`), providing independent `ContextFixture` regression cases and diagnostics.

Set provides Linear Programming, Constraint Solving, Function, Geometry and the lower layers transitively. BDD provides its Threading and other lower dependencies transitively. The CMake build includes and links both Set and BDD; existing paving binary-tree algorithms are not replaced by the BDD implementation.

## Paving representations

The two representation-specific C++ names are:

- `BinaryTreePaving` (header
  `paving/binary_tree_paving.hpp`): the public name of the existing
  `GridTreePaving` implementation, which remains used by legacy clients.
- `RobddPaving` (header `paving/robdd_paving.hpp`):
  an experimental ROBDD-backed paving snapshot.

The types do not yet offer identical functionality. In particular,
the ROBDD prototype does not yet implement the complete
`PavingInterface`; the alias does not create a new concrete class.

The two backends share representation-independent geometric alignment
through `paving/grid_cell_alignment.hpp`, including paths relative to a
primary-cell extent, common root cells and relative subdivision paths.

## Experimental ROBDD paving snapshot

`RobddPaving` is an additive prototype for comparing immutable
ROBDD storage with the current explicit binary-tree backend. It imports a
`GridTreeSubpaving`, retains the geometric `GridCell` root and a logical
subdivision depth, and stores membership in a shared `RobddManager`.
It supports finite-depth membership, counting and visiting cells,
compact enabled-leaf counting/iteration (`enabled_leaf_count`,
`for_each_enabled_leaf`, `size`, `begin`, `end`), immutable
union/intersection/difference,
inclusion/intersection predicates, logical mincing and recombination,
in-place union/intersection/difference, adding or removing `GridCell` objects,
`clear()`/`set_root_cell()`, and re-materialization as a `BinaryTreeNode`.
Compact leaves are the leaves of the recombined ordered binary-tree
interpretation, not all fixed-depth cells or the leaves of an
unrecombined historical tree.

The prototype also accepts `SubPavingInterface const&` inputs for
import, union, intersection, difference, set predicates and equality.
It implements `superset(GridCell)` and a native `branch(Bool)`
that restricts the ROBDD to a child cell without constructing an
explicit binary tree. This does not yet give it the full virtual
`PavingInterface` contract.

The manager must outlive snapshots. Both operands in a set operation must
share one manager and lie on the same grid; roots are aligned even when
their paths and primary-cell extents differ. Projection, geometry queries
and integration with `GridTreeCursor` are deliberately deferred. Existing paving APIs and
representation choices remain unchanged.

## Build

```bash
git clone --recurse-submodules https://github.com/ariadne-cps/paving.git
cd paving
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel --target tests
ctest --test-dir build --output-on-failure
```

For installation and Python bindings:

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --parallel --target ariadne-paving bindings
cmake --install build-release --prefix /path/to/install
```

## Python bindings

Python 3.11 or newer and a compatible development environment are required for the Python extension. Paving contributes grid and paving operations to `pyariadne_paving` alongside the Set and BDD Python modules and their dependencies.

## Tutorials

- C++: `tutorials/tutorial_paving/tutorial_paving.cpp`, built separately against an installed `ariadne-paving`.
- Python: `python/tutorials/tutorial_paving.py`.

## Contribution guidelines

Please contact the developer: Luca Geretti <luca.geretti@univr.it>.
