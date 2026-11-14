# Ariadne Paving

[![Format Status](https://github.com/ariadne-cps/paving/actions/workflows/format.yml/badge.svg)](https://github.com/ariadne-cps/paving/actions/workflows/format.yml)
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
- Python bindings, tests and tutorial: `python/bindings/`, `python/tests/` and `tutorial/`.
- Standalone installed C++ example: `tutorial/`.

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
`uniform_cell_paving(fineness)` returns an independent Paving with
those cells materialized as an explicit binary-tree paving. This works for
both binary-tree and ROBDD sources and does not change either source.
In contrast to minimum-fineness enumeration, every returned enabled
leaf has exactly the selected depth. If the requested grid is too coarse
to represent the set exactly, the operation throws rather than silently
performing an outer/inner approximation. A subview cannot use cells
coarser than its own nonempty root. The detached result uses the primary
root cell at the source's root extent, even for child views.

## Comparative Paver benchmark

The standalone `benchmark_pavers` executable compares `AffinePaver`,
`SubdivisionPaver`, `ReducePaver`, and `ConstraintPaver`. The original
five aligned/rotated box and ellipsoid, and ball cases remain shared with
`benchmark_paving_backends` through `benchmarks/paving_benchmark_shapes.hpp`.
Nine **Paver-only** cases extend the workload beyond identity maps:

| Case | Geometry (unit parameter domains) | Exact output volume |
| --- | --- | --- |
| `affine-shear` | (u, v + u/4), remaining coordinates unchanged | 1 |
| `quadratic-shear` | (u, v + (u-1/2)^2/4), remaining unchanged | 1 |
| `folded-quadratic` | (4(u-1/2)^2, v), remaining unchanged | 1 |
| `uv-triangle` | (u, uv), remaining unchanged | 1/2 |
| `affine-projection` | d+1 parameters projected onto d coordinates | 1 |
| `correlated-strip` | Identity, 3/4 <= u+v <= 5/4 | 7/16 |
| `product-cap` | Identity, uv >= 1/4 | 3/4 + ln(1/4)/4 |
| `parabolic-curve` | d-1 parameters mapped to (u, u^2, ...) | 0 |
| `empty-constraints` | Identity, impossible u <= -1 | 0 |

`--dimensions` refers to the **output** dimension (2..6), not necessarily
the number of parameters. The default matrix includes all 14 cases. Use
`--shapes` to select a subset; existing five-shape invocations still work.
The backend comparison still uses exclusively the original five shapes.

Use `--pavers` to choose one or more algorithms without running the
others: `affine`, `subdivision`, `reduce`, `constraint`, or
`all` (the default). Comma-separated selections are validated for
unknown names, empty entries and duplicates. The order is preserved in
both the timing and pairwise comparison tables; a one-Paver run omits
pairwise comparisons. This option is independent of the case matrix.

Build and run manually in Release mode:

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --target benchmark_pavers
./build-release/benchmarks/benchmark_pavers --help
./build-release/benchmarks/benchmark_pavers --dimensions 2 --finenesses 1 --samples 5
./build-release/benchmarks/benchmark_pavers --dimensions 2 --finenesses 2 --shapes affine-shear,uv-triangle,correlated-strip --samples 5
./build-release/benchmarks/benchmark_pavers --dimensions 2 --finenesses 3,4 --shapes uv-triangle,correlated-strip,product-cap,folded-quadratic --pavers subdivision,reduce,constraint --samples 5 --max-case-ms 2000
./build-release/benchmarks/benchmark_pavers --dimensions 3 --finenesses 4 --shapes uv-triangle,correlated-strip,product-cap,folded-quadratic --pavers subdivision,reduce,constraint --samples 5 --max-case-ms 2000 --diagnostics
./build-release/benchmarks/benchmark_pavers --dimensions 2 --finenesses 3 --shapes uv-triangle --pavers constraint --samples 5
./build-release/benchmarks/benchmark_pavers --dimensions 2 --finenesses 1 --shapes parabolic-curve,empty-constraints --pavers subdivision,reduce,constraint --verify-only
```

The per-strategy, per-case `--max-case-ms N` option (default 2000,
`0` disables) sets a **soft budget** for the accumulated *timed* samples.
After each completed invocation the budget is checked; a partial row
reports `SOFT LIMIT` with the actual sample count, e.g. `1/5`. It cannot
interrupt an already running Paver call, so an individual sample can
exceed the budget. Warm-ups are outside that budget and execute once
per strategy. `--verify-only` performs each case once without timing
or budget. Use `--max-case-ms 0` for unrestricted timing.

The report shows median, min, max, mean and MAD by default (customizable
with `--summary`), compact cell counts, approximate outer volume and
analytic set volume, and the outer/set volume ratio. For zero-volume
or empty sets the ratio is `n/a`, rather than a divide-by-zero number.
Known interior witness boxes are checked where available; empty sets
must yield empty pavings and lower-dimensional images must be nonempty.
The necessary volume lower bound and deterministic repeatability are
also checked. These are sanity checks, not a complete proof of soundness.

`AffinePaver` additionally reports split counts, unresolved domains,
and whether its depth limit was reached. The original warning is
preserved; unrelated Pavers display `n/a` for those fields. The second
table compares all six pairs by exact cell coverage (equality, strict
containment or incomparability). Geometry construction, recombination,
result verification and pairwise comparisons are **outside** the timed
region. Never rank strategies by elapsed time alone.

The box-insertion hot path dispatches `ExactBoxSetType` to a
specialized `BinaryTreePaving` traversal, retaining the general compact
set's exact separation and coverage predicates, subdivision depth, and
enabled-cell fast exits. The traversal propagates each tree node's
lattice and physical boxes incrementally instead of reconstructing the
complete binary path or converting all physical coordinates at each node.
Each split converts only the new boundary along its split axis, using
the same midpoint and directed floating-point operations as `GridCell`.
Sibling branches now share the same mutable lattice and physical boxes,
restoring the original split-axis intervals between visits instead of
copying all coordinates four times per node. Certified exclusion
and coverage are tracked incrementally: a non-separated parent can
become separated only along the newly split axis, and a count of axes
not strictly covered determines when a subtree can be enabled in one
step. Thus the remaining classification work per node is independent
of the number of dimensions. Before visiting either child, the traversal
also checks whether its node is already enabled or whether the box lies
strictly beyond its split boundary; such a branch needs no geometry
mutation or recursive call. Equality at the boundary never counts as
separation, preserving the certified outer approximation. The general
compact-set path is unchanged. Tests compare the specialized box path
against an independent regular-located-set wrapper using bidirectional
exact cell containment, check incremental geometry and restoration,
validate split-boundary pruning against full interval separation, and
exercise pre-enabled branches, boundary-touching and degenerate boxes
on unit and shifted/scaled grids.

`ConstraintPaver` constructs the joint map/constraint function only
once per invocation, rather than once per recursive visit. `--diagnostics`
reports this in `Joins` and `Join (ms)`. `ReducePaver` now reports
`Box reducers`/`Hull reducers` and their separate elapsed times.
Their total is the original `Reductions` count and aggregate
contractor time. These refinements do not change the algorithms'
acceptance or subdivision criteria.

Optional `--diagnostics` adds a separate **untimed** pass for
`SubdivisionPaver`, `ReducePaver` and `ConstraintPaver`. It records
recursive visits, parameter and cell splits, map and constraint
evaluations, sensitivity computations, contractor invocations, proven
disjoint rejections, previously covered cells, and paving insertion
calls. For `SubdivisionPaver`, a visit is a parameter subdomain; for
`ReducePaver` and `ConstraintPaver`, a visit is a domain/cell pair.
`Reductions` counts individual box/hull contractor calls, not
high-level reduction iterations. `Constraint reductions` counts the
subset of contractor invocations involving constraint functions, while
`Constraint evals` counts only actual interval evaluations involving
nonempty constraint functions; empty constraints are not counted.
`Map evals` counts selected range evaluations, not scalar arithmetic.
A combined function/constraint enclosure in `ConstraintPaver` is timed
in its own `Joint (ms)` bucket; it is not double-counted as a map or
constraint time. For `ConstraintPaver`, a separate traversal breakdown
reports calls and instrumented time for `superset(cell)`, `cell.box()`,
construction of `product(cell.box(), constraint_bounds)`, and certified
disjointness checks. These four exclusive timings are subtracted from
`Other (ms)` rather than double-counted. `Other (ms)` is therefore
not directly comparable to older reports without this breakdown.
The other timing buckets distinguish map ranges,
constraint ranges, sensitivities, **box/hull** contractor calls, the
one-time joint-function construction, and paving insertions.
`Other (ms)` is the remainder of the instrumented pass (including
traversal, setup, splitting, and profiling overhead), not the cost of a
specific named operation. The added fine-grained timers may themselves
be noticeable on very short operations and belong only to diagnostic
replays, not ordinary timed samples. Timings are inclusive of the clock instrumentation
around each measured operation and should **not** be compared directly
with the standard uninstrumented benchmark medians.

These counts describe workloads, but are not interchangeable units of work
between different strategies.

The instrumented replay uses a fresh empty paving, checks the same
correctness conditions, and verifies *bidirectional exact cell coverage*
against the measured result. No counter increments are included in
reported times. Diagnostics are opt-in because replay may itself be
costly, and are intentionally unavailable for `AffinePaver`, which
retains its dedicated depth-limit diagnostics.

These are manual benchmarks; they are not CTest performance tests.


## Reachability-style Paver sequence benchmark

The standalone `benchmark_paver_sequences` compares `SubdivisionPaver` and
`ConstraintPaver` on **the same ordered sequence of validated 5D
constrained image sets**. The controlled fixture is inspired by
SPRE22's `(t,x,y,vx,vy)` state variables and scaling (grid lengths
`1,200,200,20,20`), with nonlinear correlated Taylor-model maps,
constraints on parameters, and repeated insertions. **It is not a
recording of actual SPRE22 enclosures**, and does not include evolver
or hybrid-transition time. To benchmark that full workload, collect
actual enclosure fixtures separately instead of treating this
surrogate as a direct measurement.

Two independent factors expose why a Paver may win in a reachability
analyser but lose in the original single-set benchmark:

- `--scenarios overlap,sweep`: the output regions largely overlap
  or move to new grid cells over successive enclosures.
- `--modes isolated,accumulating`: every enclosure receives a fresh
  empty paving, or all enclosures adjoin to **the same paving** so a
  Paver can exploit cells already covered by earlier enclosures.

Input generation, Taylor-model construction, paving preparation,
recombination, and geometry verification are outside the timed region.
Only the repeated Paver insertions are timed; `isolated` outputs are
united after timing solely for geometry comparisons. The report shows
median/MAD timing, resulting cells and physical outer volume. It
compares Pavers using bidirectional exact cell containment and also
checks whether `isolated` and `accumulating` produce the same
union for each Paver. Identical runs of one configuration must produce
exactly the same geometry; differences across *different* algorithms
are reported rather than incorrectly assumed to be bugs.

Build in Release mode, then **from the build directory**:

```bash
cmake --build . --target benchmark_paver_sequences
./benchmarks/benchmark_paver_sequences --finenesses 2 --steps 6 --samples 5 --scenarios overlap,sweep --modes isolated,accumulating --pavers subdivision,constraint --max-case-ms 0 -t dark
./benchmarks/benchmark_paver_sequences --finenesses 2 --steps 6 --samples 5 --scenarios overlap --modes accumulating --pavers subdivision,constraint --max-case-ms 0 --diagnostics -t dark
./benchmarks/benchmark_paver_sequences --finenesses 1 --steps 2 --scenarios overlap,sweep --modes isolated,accumulating --verify-only -t dark
```

`--diagnostics` adds a separate, untimed per-enclosure replay, showing
the already-covered checks, recursive visits, parameter splits,
range/sensitivity evaluations, and per-stage Paver times. These
instrumented step times must not be equated with the medians. The
`--max-case-ms` soft budget works per complete sequence sample; it
cannot interrupt an in-progress run. The default is 5000 ms,
`0` disables it. Finenesses 0..4 and 1..24 enclosure steps are
supported. The existing `benchmark_pavers` and its analytic
volume comparisons remain unchanged; this separate benchmark
does **not** claim a known analytic output volume for the 5D
constrained image union.

## Comparative backend benchmark

Build explicitly with:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target benchmark_paving_backends
./build/benchmarks/benchmark_paving_backends --help
./build/benchmarks/benchmark_paving_backends --dimensions 2,3 --finenesses 1,2 --shapes aligned-box,rotated-box,aligned-ellipsoid,rotated-ellipsoid,ball --iterations 1 --samples 5 --concurrency 0
# To add CachedTree:
./build/benchmarks/benchmark_paving_backends --include-cached-tree --dimensions 2 --finenesses 2 --shapes aligned-box --samples 5 --concurrency 0
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
The optional `--include-cached-tree` flag adds a third backend column,
plus a CachedTree/Tree ratio table, without changing the default Tree/ROBDD
baseline. CachedTree `superset_16` uses a verified, precompiled index;
its compilation cost is measured separately by
`benchmark_cached_binary_tree_paving`. Other workloads exercise the
authoritative Tree and its cache-invalidation overhead rather than a
cached-query optimization. CachedTree's displayed node count excludes
its additional ROBDD-cache memory. For the isolated mutation workload,
cache preparation occurs outside the timed interval.
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
callers using `PavingInterface&` or `Paving` to request cell-batch
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
  --samples 7 --min-sample-ms 12 --max-iterations 8192 \
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

The report distinguishes **descriptive all-case** aggregates from
**calibrated-only** aggregates. Calibrated-only ratios exclude a
backend pair unless both sides met `--min-sample-ms`. If a coverage
category has no calibrated pairs it reports `cases=0 geomean_ratio=NA`,
not an artificial ratio. Base backend comparisons primarily on the
calibrated-only tables; a short sample should not reverse the
interpretation of the overall geometric mean.

Machine-readable all-case summaries use
`OUTER_OVERLAP_BACKEND_CASE`, `OUTER_OVERLAP_BACKEND_GEOMETRY`,
`OUTER_OVERLAP_BACKEND_COVERAGE`, `OUTER_OVERLAP_BACKEND_OVERALL`.
The filtered summaries use
`OUTER_OVERLAP_BACKEND_CALIBRATED_GEOMETRY`,
`OUTER_OVERLAP_BACKEND_CALIBRATED_COVERAGE`, and
`OUTER_OVERLAP_BACKEND_CALIBRATED_OVERALL`.

Timing rows report `tree_calibration_met` and `robdd_calibration_met`.
If the iteration cap is reached before `--min-sample-ms`, the benchmark
also emits `OUTER_OVERLAP_CALIBRATION_WARNING`. This is not a
correctness failure, but short samples make microsecond-level ratios
less reliable. With prepared-solver construction deferred, the
reported fully covered 4D/f3 cases take about 4 microseconds.
At that speed 2048 iterations are too few for a 12 ms batch, so the
manual Release example now uses `--max-iterations 8192`.
This higher cap does not force already-slow empty/partial cases to
run longer: calibration ends as soon as the duration threshold is met.

### Concurrent adjoin (initial correctness milestone)

Both `BinaryTreePaving` and `RobddPaving` serialize concurrent `adjoin(GridCell)`, `adjoin(std::vector<GridCell>)`, and same-backend `adjoin(SubPavingInterface)` calls on one destination. A vector batch holds one paving mutex for the full operation. The tree backend additionally locks both owning tree pavings in a deadlock-safe order when merging them; the ROBDD backend snapshots the source before acquiring the destination mutex. Independently owned sources are supported; borrowed source views must remain valid and stable during the operation.

**Scope limit:** This is an incremental thread-safety milestone, not a full `PavingInterface` guarantee. `superset(GridCell)`, `size()`, `root_cell()`, `clone()` and owning snapshot/copy construction can now run alongside `adjoin` on both backends. ROBDD additionally snapshots its Boolean root, depth, enabled-cell counts, compact iterators and enumeration. Other concurrent mutations, explicit tree iterators, borrowed views, generic multi-paving comparisons and reference-valued `grid()` are not yet protected. Do not invoke those APIs concurrently with adjoin on the same paving. `RobddManager` retains its separate synchronization. Regression tests exercise both backend read/write overlap.

## CachedBinaryTreePaving: explicit compiled ROBDD query cache

`CachedBinaryTreePaving` owns the authoritative mutable BinaryTreePaving and
optionally compiles an immutable RobddPaving query index. The class does
not use the name *hybrid*: that term denotes hybrid systems in Ariadne.

`compile_cache()` takes a coherent Tree snapshot under the owner mutex,
constructs the BDD outside that lock, and publishes only if the revision
has not changed. Mutations invalidate the index before changing the Tree.
`superset()` uses the compiled index when available and falls back to
the original Tree when the cache is absent. The index owns its manager
and concurrent readers retain ownership while using it.

This initial prototype is explicit/synchronous; automatic activation and
compilation tasks are deferred until the manual break-even measurement.
Ordinary correctness tests include concurrent readers, writers and
compilation. No performance benchmark is run in CI.

The earlier BDD/Tree 2x2 factorial benchmark has been completed.
The script `benchmarks/run_dependency_ab.sh` was removed from main.
Its original code remains in Git history; methodology, selected pins and
results remain documented in `doc/robdd_paving_design.md`.

### Manual benchmark for the compiled Tree query cache

The dedicated executable `benchmark_cached_binary_tree_paving` uses
Ariadne Benchmarking to measure three independent costs:
copying an authoritative Tree snapshot; encoding that snapshot
into a new-manager ROBDD; and querying equivalent Tree and
compiled-ROBDD representations. It prints `CACHED_TREE_BDD`
records with absolute costs and the estimated number of queries
needed to amortize snapshot plus BDD encoding.

```bash
git pull --ff-only
git submodule update --init --recursive
cmake --build build-release --target benchmark_cached_binary_tree_paving
./build-release/benchmarks/benchmark_cached_binary_tree_paving \
  --depth 9 --queries 128 --samples 7 --iterations 256
```

Queries are checked for semantic equivalence against the Tree.
Compilation and Tree copies are excluded from the per-query timer.
The break-even figure excludes memory overhead, CPU interference
and the likelihood of invalidation between queries. A nonpositive
query saving produces -1 rather than a claimed crossover.

The benchmark runs **manually only** and is not registered with
CTest or executed in GitHub Actions. No automatic ThreadManager
cache-compilation tasks have yet been added.

### Experimental parallel certified outer classification

`RobddPaving::adjoin_outer_approximation_parallel(set, fineness)` is an
explicit opt-in path. It uses the configured shared `ThreadManager` (2-8
bounded worker partitions), a detached ROBDD snapshot for containment
checks, and an immutable prepared `ConstrainedImageSet` contractor.
It classifies the ancestors before forming worker subtrees, ensuring the
same certified pruning decisions as the sequential depth-first traversal.
Each task collects its selected cells in a private vector; the paving is
updated by a single batch union only after all tasks join successfully.
Worker exceptions do not publish partial results. The caller's
floating-point rounding mode is installed and restored on each worker.

The parallel path currently applies only to the exact `ConstrainedImageSet`
type. Derived sets, other compact-set implementations and fewer than two
configured worker threads use the unmodified sequential algorithm; this
avoids silently assuming that arbitrary virtual predicates are thread-safe.
The ordinary `adjoin_outer_approximation` method remains sequential.
The input set must not be mutated while the operation runs. Correctness is
compared against the serial oracle across worker counts, fineness values,
empty, partially populated and fully covered pavings.

This is a correctness milestone. No speedup is claimed without Release
measurements comparing identical inputs at concurrency 0, 2, 4 and 8.

### Manual serial/parallel ROBDD outer-approximation benchmark

Build Release explicitly and run the manual benchmark:

```bash
cmake --build build-release --target benchmark_parallel_outer_paving
./build-release/benchmarks/benchmark_parallel_outer_paving \\
  --dimension 2 --fineness 1,2 --concurrency 0,2,4,8 --samples 5
```

The report uses milliseconds. The benchmark prepares a fresh destination
outside the timed callback for every sample, measures the complete certified
outer-approximation call and verifies exact geometric agreement with an
independent sequential oracle after each call. The worker count comes from
Benchmarking::Matrix and is applied through the shared ThreadManager before
timing; unsupported values are skipped. It is not a CTest workload and never
runs automatically in GitHub Actions.
