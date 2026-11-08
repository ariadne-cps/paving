# Ariadne Paving

[![macOS Status](https://github.com/ariadne-cps/paving/actions/workflows/macos.yml/badge.svg)](https://github.com/ariadne-cps/paving/actions/workflows/macos.yml)
[![Ubuntu Status](https://github.com/ariadne-cps/paving/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/ariadne-cps/paving/actions/workflows/ubuntu.yml)
[![Windows Status](https://github.com/ariadne-cps/paving/actions/workflows/win.yml/badge.svg)](https://github.com/ariadne-cps/paving/actions/workflows/win.yml)
[![Sanitizers Status](https://github.com/ariadne-cps/paving/actions/workflows/sanitizers.yml/badge.svg)](https://github.com/ariadne-cps/paving/actions/workflows/sanitizers.yml)
[![Coverage Status](https://github.com/ariadne-cps/paving/actions/workflows/coverage.yml/badge.svg)](https://github.com/ariadne-cps/paving/actions/workflows/coverage.yml)
[![codecov](https://codecov.io/gh/ariadne-cps/paving/branch/main/graph/badge.svg)](https://codecov.io/gh/ariadne-cps/paving)

Ariadne Paving is the standalone C++20 discrete paving layer of Ariadne. It provides grids, cells, grid-tree pavings, set approximation and subdivision algorithms, and consumes the standalone Tree module for the binary-tree node representation.
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
- [ariadne-cps/tree](https://github.com/ariadne-cps/tree) (Git submodule `ss/tre`), owning `BinaryTreeNode` and its independent regression tests.
- [ariadne-cps/build](https://github.com/ariadne-cps/build) (Git submodule `ss/cfg`), providing shared CMake infrastructure.
- [ariadne-cps/testing](https://github.com/ariadne-cps/testing) (Git submodule `ss/tst`), providing independent `ContextFixture` regression cases and diagnostics.

Set provides Linear Programming, Constraint Solving, Function, Geometry and the lower layers transitively. BDD provides its Threading and other lower dependencies transitively. The CMake build includes and links both Set and BDD; existing paving binary-tree algorithms are not replaced by the BDD implementation.

## Binary tree node as an independent data structure

`BinaryTreeNode` (defined in `tree/binary_tree.hpp`) is an owning node
and its rooted subtree, not a geometric paving type. It describes Boolean
membership of abstract binary prefixes: an enabled leaf covers all
continuations of its path. Set operations do not require a `Grid`,
`GridCell`, or `PavingInterface`. The node depends on Foundation logical
values and Utility paths and types, not directly on Function or Set.

The node remains a sequential, unsynchronized primitive. Child pointers
are borrowed; coordinating concurrent access and parallel subtree work
is the responsibility of a higher-level owner. The extraction into `ariadne-cps/tree` preserves the `BinaryTreeNode` name and behavior.

See [the Tree design](https://github.com/ariadne-cps/tree/blob/main/doc/binary_tree_design.md) for invariants,
ownership, semantics, limitations and the completed extraction.

## Paving representations

The two representation-specific C++ names are:

- `BinaryTreePaving` (header
  `paving/binary_tree_paving.hpp`): the concrete binary-tree-backed paving implementation.
- `RobddPaving` (header `paving/robdd_paving.hpp`):
  an experimental ROBDD-backed paving snapshot.

Both backends implement the polymorphic `PavingInterface`, while
representation-specific operations and performance characteristics differ.
`BinaryTreePaving` and `RobddPaving` are independent concrete implementations.

The two backends share representation-independent geometric alignment
through `paving/grid_cell_alignment.hpp`, including paths relative to a
primary-cell extent, common root cells and relative subdivision paths.

## Experimental ROBDD paving snapshot

`RobddPaving` is an additive prototype for comparing immutable
ROBDD storage with the current explicit binary-tree backend. It imports a
`BinaryTreeSubpaving`, retains the geometric `GridCell` root and a logical
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
and integration with `BinaryTreePavingCursor` are deliberately deferred. Existing paving APIs and
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
./build/benchmarks/benchmark_paving_backends --dimensions 2,3 --finenesses 1,2 --shapes aligned-box,rotated-box,aligned-ellipsoid,rotated-ellipsoid,ball --iterations 1 --samples 5 --concurrency 0
```

The CLI, logger, fixed-width result table, median timing and
geometric-mean speed ratio follow ariadne-cps/numeric benchmarks.
ThreadingCommandLineInterface provides all logging options and
--concurrency/-c (0 or max or a numeric concurrency setting). The BDD
algorithms in this benchmark are currently sequential; a nonzero
concurrency setting configures ThreadManager but does not enable
parallel BDD operations. Timings therefore must not be interpreted
as multithreaded speedups.
Results identify geometry, dimension, grid-relative fineness, backend,
operation, compact leaf count and node count. The ratio is ROBDD/tree:
above 1 means the tree is faster. The --max-depth-bits option skips
combinations with an excessive theoretical subdivision depth. Manager
nodes are shared and must not be read as exclusive ROBDD memory use.
The first version measures warmed manager behavior, not manager startup,
and refuses to compare approximations that disagree geometrically.
For each geometry/dimension/fineness case, an independent memory
table estimates physical node payload: BinaryTreeNode count multiplied
by sizeof(BinaryTreeNode), and reachable distinct ROBDD decision nodes
(including complemented-edge sharing) multiplied by the layout estimate
of RobddManager::Node. It also prints the manager-wide allocated node
payload separately. These are **indicative node storage**, NOT measured
RSS or total heap use: allocator overhead, node-vector capacity, unique
table and ITE cache memory are excluded. Manager counts are snapped
before correctness verification or benchmark timings. The public BDD
cofactor API is used for node counting, so no BDD library modification
is required.

These benchmarks are standalone executables, deliberately excluded from
the `tests` target and CTest. Build and run them explicitly.

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

### Accurate geometry, calibration and warm/cold construction

Benchmark geometry now uses exactly representable dyadic constants.
Aligned/rotated box and ellipsoid pairs use the same principal widths
and volumes, with a rational 3-4-5 orthogonal rotation of the first
two coordinate axes. Additional axes remain identical between pairs.
This is deliberate: decimal literals such as 0.2_x are not exactly
representable and can fail the ExactDouble precondition.

Defaults focus on nondegenerate cases: dimensions 2,3,4 and
finenesses 2,3, with --max-depth-bits 26 and --min-leaves 2.
A geometry producing fewer than the requested minimum leaves is
reported as SKIP and excluded from time aggregates; specify
--min-leaves 0 to include coarse cases. The effective subdivision bit
bound is conservative, so raising it for more 4D/5D cases may be costly.
Run one configuration at a time if its feasibility is unknown.

Each backend/operation is timed using at least --iterations operations,
calibrated by doubling the batch size until its elapsed time exceeds
--min-sample-ms (default 12), up to --max-iterations (default 32768).
There is one warm-up/calibration phase and an odd number of timed
samples (default 7); median milliseconds are reported per operation,
along with the calibrated iteration count. --iterations specifies a
minimum, not the final exact repetition count.

The distinct outer_warm and outer_cold operations avoid mixing a
reused ROBDD manager with a fresh one. In outer_cold every invocation
constructs and destroys a new manager; in outer_warm it reuses a
populated manager. Geometry construction occurs before timing in both
modes. Memory table columns distinguish BDD N (nodes reachable from
the root), Mgr N (fresh manager after initial construction), and Mgr
warm (manager nodes remaining after all timed measurements). These
manager node counts are not an allocation size or full heap profile.


### Manager allocation diagnostics and isolated mutations

The benchmark now requires the pinned BDD version exposing
RobddManager::statistics(). Its snapshot reports the actual number
and capacity of canonical decision nodes in the manager, along with
the unique-table and ITE-cache element counts and the ABI-dependent
sizeof each element payload. A dedicated table compares statistics
before geometry validation/timing with those after the workload.
The total estimated payload includes vector capacity and table
value payloads; it excludes standard-library tree links, allocator
bookkeeping and other heap allocations. The separate root-reachable
ROBDD node count remains the indicator of logical DAG compression.

The copy workload constructs a copy without a following size().
The adjoin_recombine workload prepares independent paving copies
outside each timed interval, then times just the in-place adjoin and
recombine. Timed samples accumulate as many intervals as needed in
chunks of at most 256 live copies, avoiding the prior 256-operation
limit. Copy/destruction overhead is not included. Results from older
copy_adjoin_cell tests are not directly comparable.

The untimed retention experiments compare TWO independent 2D
managers: SAME rebuilds the identical compact box, while VARIED
builds distinct exact dyadic translations of one box. Every paving
is destroyed before the next manager snapshot. --retention-steps
(1..64, default 12) and --retention-fineness (1..6, default 3)
control both runs. Each table row prints root-reachable node count
before destruction and retained manager allocations afterwards.
Distinct exact shapes may have the same coarse outer approximation.
Use --no-retention to skip both experiments.
Retained allocations do not necessarily indicate a memory leak;
the current manager intentionally keeps canonical nodes and caches
until the manager itself is destroyed.

### Precomputed cell replay

The replay_cells operation uses the exact enabled cells extracted from
the completed source tree before any timing. Both backends receive
the same prepared cells, and reconstruction is checked for geometric
equivalence against the source using a fresh independent ROBDD manager.
The timed operation creates a new paving, inserts the prepared cells
and recombines. Geometric constraint solving and cell enumeration
are excluded. --max-replay-cells (default 16384) skips the replay
operation for excessive leaf counts without truncating the data.
The outer_warm workload now returns a constructed paving instead of
calling size(), so its sink resembles replay_cells. outer_cold still
calls size() before returning because its manager must be destroyed,
and that distinction should be taken into account when comparing
the cold and warm construction timings.

### Batch ROBDD construction from precomputed cells

The native ROBDD backend now exposes
RobddPaving::from_cells(manager,root_cell,cells) and
RobddPaving::adjoin_cells_bulk(cells). A temporary prefix trie collects
the cells, collapses duplicates and nested ancestor/descendant cells,
then uses the manager's canonical make_node() operations in one
bottom-up pass. Skipped Boolean variables and complemented-edge
canonicalization remain owned by the manager. The batch union first
validates the grid and common root; it preserves the original paving
when inputs are invalid. When the previous paving is empty but has
been explicitly minced, the batch union retains that logical depth.

Both outer and inner compact-set approximation now use batch
insertion of the cells selected by their existing certified geometric
visitors. The geometric acceptance/rejection decisions and their
coverage targets are unchanged; the only change is that the selected
cells enter the ROBDD as one union rather than successive singleton
pavings and Boolean unions.

The benchmark retains replay_cells as the incremental reference and
adds replay_bulk using the exact same prepared cell sequence, outside
any geometric constraint checks. It validates the two variants against
the binary-tree oracle before timing. A new table measures fresh
isolated manager nodes, ITE cache entries and estimated payload for
both replay methods (plus reachable nodes), not totals inflated by
warm-manager repeated runs. The tree timings for replay_bulk provide
the same baseline tree insertion workload as replay_cells, allowing
comparison of incremental/batch ROBDD independently of geometric
classification. All reported memory is indicative and does not
include allocator bookkeeping or full resident memory.

### Direct read-only ROBDD cell-containment query

RobddPaving::superset(GridCell) performs a geometric-root containment
check, normalizing cell representations across primary extents with
common_root_cell and relative_cell_path. It then follows the queried
binary prefix through the ROBDD's low/high cofactors, skipping variables
absent from the canonical DAG. It returns true only when the entire
remaining Boolean subfunction is the true terminal. This preserves
semantics for partial/nonempty subtrees, complemented edges and variable
skips without constructing an intermediate paving or adding canonical
nodes/ITE cache entries to the manager.

During inner and outer certified approximation, selected cells are
collected before any root mutation. If the existing root is empty, every
superset query is redundant and is now skipped. If nonempty, the same
geometric pruning condition remains in place, but its containment
check is read-only.

The expanded backend regression suite independently compares direct
superset queries against the explicit tree for all 256 occupancy
patterns of three decision variables, including parent/descendant
queries, different root extents, minced logical depth and complemented
Boolean functions. For direct queries it checks that manager nodes,
capacity, unique entries and ITE cache entries do not increase.

### Polymorphic batch insertion and explicit subpaving import

The virtual `PavingInterface::adjoin(std::vector<GridCell> const&)` allows
callers using `PavingInterface&` or `PavingHandle` to request cell-batch
insertion. The default implementation forwards each cell to the
single-cell overload, which preserves the existing binary-tree
semantics without requiring an additional concrete override.
The `RobddPaving` implementation overrides this method to build a
single canonical ROBDD and take at most one union.

`adjoin(SubPavingInterface const&)` on ROBDD-backed pavings is
different: it retains the direct Boolean union of an already-encoded
ROBDD (or native view) and rejects foreign-backend operands. For
**explicit conversion** from a generic subpaving,
`RobddPaving(manager, SubPavingInterface const&)` now collects enabled
cells from that source and calls batch `adjoin(cells)` once, instead
of repeatedly adding singleton cells. The existing tree-specific
conversion already uses direct tree encoding and is unchanged.

### Read-only cell-containment microbenchmark

`benchmark_paving_superset` isolates the cost of
`superset(GridCell)`, separately from geometric approximation.
It measures a binary-tree oracle, the previous common-root/relative-path
alignment (reference only, local to the benchmark), and the optimized
production ROBDD lookup. Queries are prepared before timing and
classified as internal, external, ancestor or descendant, including
queries crossing primary-cell extents. Every case must agree with the
tree and the manager statistics must not change during the timed
read-only queries.

The optimized lookup compares root and query prefix bits directly,
without allocating or concatenating binary words for the common
same-primary-extent case. When extents differ, it constructs only
the necessary primary-cell alignment prefix. A single traversal then
reads the remaining query bits as ROBDD cofactors, including skipped
variables and complemented edges.

To run the microbenchmark in a Release build:

```sh
cmake --build build-release --target benchmark_paving_superset
./build-release/benchmarks/benchmark_paving_superset \
  --dimension 2 --probes-per-class 64 \
  --samples 7 --min-sample-ms 12 --concurrency 0
```

Use --dimension (1..6), --probes-per-class (4..1024),
--iterations, --samples, --min-sample-ms and --max-iterations to
control the workload. Times are medians in nanoseconds per query,
calibrated separately by category/implementation. This benchmark is not
registered with CTest; invoke it explicitly when needed.


### Outer approximation with pre-existing geometric coverage

Use the opt-in `--compare-outer-overlap` mode of
`benchmark_paving_backends` to measure full outer-approximation
operations on three kinds of destination: **empty**, **partial** (every
second compact leaf of an independently classified approximation), and
**full** (the enclosing search-root cell already enabled). The same
shape, grid and fineness are used in all three scenarios.

The benchmark verifies each scenario against a separate reference
constructed using the original virtual `separated()` and `covers()`
classification, comparing both backend outputs by bidirectional
geometric containment. It skips a scenario family when fewer than two
reference leaves exist. `--verify-only` runs these checks without
timings. No speedup is inferred from correctness alone.

```sh
cmake --build build-release --target benchmark_paving_backends
./build-release/benchmarks/benchmark_paving_backends \
  --compare-outer-overlap --verify-only \
  --dimensions 2 --finenesses 2 --shapes aligned-box,ball \
  --max-depth-bits 8 --concurrency 0

./build-release/benchmarks/benchmark_paving_backends \
  --compare-outer-overlap \
  --dimensions 4 --finenesses 3 --max-depth-bits 28 \
  --shapes aligned-box,rotated-box,aligned-ellipsoid,rotated-ellipsoid,ball \
  --samples 7 --min-sample-ms 12 --max-iterations 2048 \
  --concurrency 0
```

Each independently calibrated sample times only the in-place outer
approximation call. Destination copies are prepared outside the timed
interval, while preparation of the effective-set geometric predicate
is included within the timed call. Recombination, setup and destruction
are excluded equally. The ROBDD manager is reused and warmed, not
cold-started. Outputs `OUTER_OVERLAP_VERIFIED`,
`OUTER_OVERLAP_TIMING` and `OUTER_OVERLAP_RELATIVE` separate
semantic checks, measured timings and ratios to the initially empty
case. Such ratios compare destination **states**, not the old and new
algorithm, and are not automatically attributable to the recent
early-containment optimization.

The benchmark ends with a **ROBDD/BinaryTree summary**: a matrix of
ratios for every geometry/dimension/fineness with separate `empty`,
`partial`, `full` columns; a geometric-mean ratio grouped by geometry
type (across all selected dimensions/finenesses and coverage states);
a geometric-mean ratio grouped by initial coverage (across all
geometries); and one equal-case-weighted overall geometric mean.
The summary also reports minima/maxima, win counts for each backend,
ties and uncalibrated counts. **Ratio > 1 means BinaryTree faster;
ratio < 1 means ROBDD faster.** These are geometric means of
pairwise median ratios, not a ratio of summed runtimes, and do not
compare different code versions.

The machine-readable summaries are prefixed
`OUTER_OVERLAP_BACKEND_CASE`, `OUTER_OVERLAP_BACKEND_GEOMETRY`,
`OUTER_OVERLAP_BACKEND_COVERAGE` and
`OUTER_OVERLAP_BACKEND_OVERALL`.

Timing rows report `tree_calibration_met` and `robdd_calibration_met`.
If the iteration cap is reached before `--min-sample-ms`, the benchmark
also emits `OUTER_OVERLAP_CALIBRATION_WARNING`. This is not a
correctness failure, but short samples make microsecond-level ratios
less reliable. Increasing `--max-iterations` permits the fast full
scenario to reach the minimum duration without forcing expensive
empty/partial scenarios to run longer.

### Concurrent adjoin (initial correctness milestone)

Both `BinaryTreePaving` and `RobddPaving` serialize concurrent `adjoin(GridCell)`, `adjoin(std::vector<GridCell>)`, and same-backend `adjoin(SubPavingInterface)` calls on one destination. A vector batch holds one paving mutex for the full operation. The tree backend additionally locks both owning tree pavings in a deadlock-safe order when merging them; the ROBDD backend snapshots the source before acquiring the destination mutex. Independently owned sources are supported; borrowed source views must remain valid and stable during the operation.

**Scope limit:** This is an incremental thread-safety milestone, not a full `PavingInterface` guarantee. `superset(GridCell)`, `size()`, `root_cell()`, `clone()` and owning snapshot/copy construction can now run alongside `adjoin` on both backends. ROBDD additionally snapshots its Boolean root, depth, enabled-cell counts, compact iterators and enumeration. Other concurrent mutations, explicit tree iterators, borrowed views, generic multi-paving comparisons and reference-valued `grid()` are not yet protected. Do not invoke those APIs concurrently with adjoin on the same paving. `RobddManager` retains its separate synchronization. Regression tests exercise both backend read/write overlap.
