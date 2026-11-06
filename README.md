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

Both backends implement the polymorphic `PavingInterface`, while
representation-specific operations and performance characteristics differ.
The `BinaryTreePaving` name aliases the established concrete implementation;
the ROBDD backend is an independent concrete implementation.

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
explicit binary tree. It also supports `bounding_box()`, `inside()`,
`separated()` and `overlaps()` through compact-leaf iteration,
matching the binary-tree backend's conservative box semantics.
Its polymorphic `PavingInterface` implementation also provides same-backend
mutable child views, geometric set predicates, and generic approximations.
Set operations do not implicitly convert between backends.

The manager must outlive snapshots. Both operands in a set operation must
share one manager and lie on the same grid; roots are aligned even when
their paths and primary-cell extents differ. Projection, geometry queries
and integration with `GridTreeCursor` are deliberately deferred. Existing paving APIs and
representation choices remain unchanged.

## Representation-independent cell refinement

The free function `for_each_enabled_cell_at_fineness(paving,fineness,visitor)`
is available in `paving/paving_cell_operations.hpp` (also via the umbrella
header). It visits enabled cells at **at least** the requested grid-relative
fineness, in left-before-right order. Already finer cells remain fine;
there is no implicit coarsening. It does not mutate the paving or change
its stored subdivision depth and can stream the refined cells without
materializing an expanded tree or ROBDD. The number of visited cells can
still be exponential in the requested fineness.

Legacy `mince()` and `recombine()` are representation operations, not
portable guarantees about `size()` or the cells returned by `begin()`.
In particular, a tree may retain redundant subdivisions while ROBDD
iteration remains canonical. The number of leaves exposed by `size()`
therefore need not match between backends before tree recombination.

For exact, homogeneous cells the polymorphic SubPavingInterface exposes
`for_each_enabled_cell_at_exact_fineness(fineness,visitor)`, and
`uniform_cell_paving(fineness)` returns an independent PavingHandle with
those cells materialized as an explicit binary-tree paving. This works for
both binary-tree and ROBDD sources and does not change either source.
In contrast to minimum-fineness enumeration, every returned enabled
leaf has exactly the selected depth. If the requested grid is too coarse
to represent the set exactly, the operation throws rather than silently
performing an outer/inner approximation. A subview cannot use cells
coarser than its own nonempty root. The detached result uses the primary
root cell at the source's root extent, even for child views.


## Comparative backend benchmark

Build explicitly with:


```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target benchmark_paving_backends
./build/benchmarks/benchmark_paving_backends --help
./build/benchmarks/benchmark_paving_backends --dimensions 2,3 --finenesses 1,2 --shapes aligned-box,rotated-box,aligned-ellipsoid,rotated-ellipsoid,ball --iterations 1 --samples 5
```

The CLI, logger, fixed-width result table, median timing and
geometric-mean speed ratio follow ariadne-cps/numeric benchmarks.
Results identify geometry, dimension, grid-relative fineness, backend,
operation, compact leaf count and node count. The ratio is ROBDD/tree:
above 1 means the tree is faster. The --max-depth-bits option skips
combinations with an excessive theoretical subdivision depth. Manager
nodes are shared and must not be read as exclusive ROBDD memory use.
The first version measures warmed manager behavior, not manager startup,
and refuses to compare approximations that disagree geometrically.
A small CTest smoke exercise checks build/invocation without being used
as a performance result.

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
