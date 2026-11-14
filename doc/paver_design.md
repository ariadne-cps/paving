# Paver design and optimization journal

## Purpose and maintenance rules

This is the continuing engineering journal for `AffinePaver`,
`SubdivisionPaver`, `ReducePaver`, and `ConstraintPaver` in
`ariadne-cps/paving`. Record the *reason*, implementation, measured result,
semantic checks, and disposition of every significant change. Failed
experiments remain documented. Do not rewrite earlier measurements to
make a later approach look better.

Related documentation:

- [Binary tree design](binary_tree_design.md): explicit tree structure,
  ownership, and concurrency.
- [ROBDD paving design](robdd_paving_design.md): alternate representation.
- [Comparative Paver benchmark](../README.md#comparative-paver-benchmark):
  commands, workloads, profiling counters, and geometric verification.

A Git commit named below is a repository revision, not a guarantee that
its tests passed; read the verification status separately.

## Semantic and performance requirements

1. A Paver produces a certified **outer approximation**. Preserve its
   fineness, rejection/coverage predicates, and treatment of equality on
   cell boundaries. Touching a boundary is *not* certified separation.
2. Preserve the parameter-splitting decision, including Jacobian-based
   sensitivity ordering, strict tie-breaking, and intrinsic-error stop
   conditions, unless a separately validated algorithm change is intended.
3. Compare actual geometry by bidirectional **exact cell containment**;
   matching volume or cell count alone is insufficient. Test repeated
   unions, existing enabled regions, degenerate intervals, constraints,
   and shifted/scaled grids.
4. Match `GridCell`'s midpoint calculation, physical coordinate conversion,
   and directed rounding at every depth. Avoid replacement predicates
   whose behavior differs at equality.
5. Optimize normal (uninstrumented) execution; the `--diagnostics`
   replay is separately instrumented. Report medians and MADs as well as
   timing buckets and operation counts; do not treat profiler totals as
   equivalent to regular medians.
6. Keep benchmarking reproducible: compare candidate and baseline on
   the same machine and build configuration, preferably alternating
   revisions. A five-sample run is useful screening, not proof of a
   small speed difference. `SOFT LIMIT` rows are incomplete samples.
7. All supported paving backends are designed for thread-safe public
   operations. Respect each backend's synchronization and object-lifetime
   contracts; thread safety and *parallel speedup* are different concerns.

## Algorithm overview and measured trade-offs

| Paver | Approach | Main limitation or advantage observed |
| --- | --- | --- |
| `SubdivisionPaver` | Test constraints and mapped range over parameter boxes; if range is not small enough, use Lipschitz/Jacobian sensitivity to split a parameter axis, then recursively adjoin exact image boxes. | Fastest in the four selected 3D workloads. `Adjoin` and `Sensitivity` dominate the runtime. |
| `ReducePaver` | Tighten domains via repeated box/hull reduction before paving. | Can produce *strictly tighter* outer approximations for constrained domains, but reduction dominates runtime. |
| `ConstraintPaver` | Visit combinations of parameter domains and output cells, with sensitivity and certified separation. | Same output geometry as Subdivision in the four sampled cases, but more recursive work and expensive sensitivity evaluation. |
| `AffinePaver` | Use affine approximations with subdivision and an explicit depth limit. | Depth-cap and unresolved-domain results must be reported; do not silently label incomplete accuracy as achieved. |

`OptimalConstraintPaver` was removed as redundant in
[`76917b26`](https://github.com/ariadne-cps/paving/commit/76917b26f12a5bfbf0cee2a60b9d5921da5cc8f6).
The currently selectable strategies are `affine`, `subdivision`,
`reduce`, and `constraint`.

### Geometric comparisons from the November 2026 3D benchmark

At dimension 3, fineness 4:

| Case | Subdivision cells / outer volume | Reduce cells / outer volume | Constraint relation |
| --- | --- | --- | --- |
| `uv-triangle` | 701 / 0.826172 | 701 / 0.826172 | Equal to both |
| `correlated-strip` | 706 / 0.940430 | 616 / 0.738281 | Equal to Subdivision; Reduce strictly contained |
| `product-cap` | 627 / 0.760254 | 549 / 0.632812 | Equal to Subdivision; Reduce strictly contained |
| `folded-quadratic` | 1065 / 1.423828 | 1065 / 1.423828 | Equal to both |

All relationships above were reported using pairwise exact cell
coverage, not inferred from the volumes. `ReducePaver` is not
interchangeable with Subdivision when tighter geometry matters. At the
specified budget, `ReducePaver` and `ConstraintPaver` have sometimes
completed only 3/5 samples of `folded-quadratic`; such rows are
`SOFT LIMIT`, not fully sampled timings.

## Instrumentation and test infrastructure

- [`6e54b0fc`](https://github.com/ariadne-cps/paving/commit/6e54b0fc1997bf596a17ecb0307a91a901b7005a):
  create the shared-workload comparative benchmark.
- [`48551489`](https://github.com/ariadne-cps/paving/commit/48551489fd11):
  add geometric containment, outer/set volume ratios, and affine depth
  diagnostics.
- [`f5aa7b27`](https://github.com/ariadne-cps/paving/commit/f5aa7b27c52b):
  add mapped, constrained, and degenerate workloads.
- [`164eec1b`](https://github.com/ariadne-cps/paving/commit/164eec1b531b):
  enable choosing particular Pavers independently.
- [`0e9b053f`](https://github.com/ariadne-cps/paving/commit/0e9b053f2532cebebbc7632946c2f234548c24d4):
  introduce untimed diagnostic replays and operation counters.
- [`e51be63c`](https://github.com/ariadne-cps/paving/commit/e51be63c1e8df9e6fc4d46bced4406a19cf1e8d5):
  refine constraint counts and timed operation buckets, including
  separately measured map, constraints, sensitivity, and insertion.
- [`ad7df7b2`](https://github.com/ariadne-cps/paving/commit/ad7df7b2d2b3b89e2edf8553673b07c3d7c38d7d):
  profile box versus hull contractor costs; hoist
  `ConstraintPaver`'s joint function/constraint construction out of its
  recursive inner loop.

These facilities are necessary to attribute changes to a hotspot
rather than merely to whole-suite elapsed time.

## SubdivisionPaver optimization chronology

The following table collects representative **user-supplied five-sample
Release benchmark medians (milliseconds)**, dimension 3, fineness 4.
All measurements use the same four workload names, but the runs were
not an interleaved A/B experiment. Treat differences of a few percent
as provisional until repeated under controlled conditions.

| Revision / strategy | UV triangle | Correlated strip | Product cap | Folded quadratic | Decision |
| --- | ---: | ---: | ---: | ---: | --- |
| Before incremental lattice geometry | 366.916 | 244.208 | 197.258 | 917.630 | Historical baseline |
| `108baf96` + `fb1c36e5`: propagate lattice box | 104.536 | 63.513 | 47.344 | 252.244 | **Keep** |
| `47329a7a`: propagate physical box too | 79.997 | 44.811 | 32.479 | 181.178 | **Keep** |
| `2654c0c4`: mutate/restore split intervals in-place | 62.743 | 36.094 | 25.514 | 151.580 | **Keep** |
| `d7a79eb3`: classify split axis incrementally | 64.687 | 37.707 | 26.837 | 159.013 | **Keep provisionally**; 3D advantage unproven |
| `5e55ede4`: prune disabled work early | 57.147 | 32.033 | 22.368 | 137.479 | **Keep** |
| `3dedecad`: remove intermediate Jacobian interval matrix | 63.228 | 32.788 | 22.949 | 137.059 | **Revert requested**; no measured consistent benefit |

The formatting-only `0222cd87` (`Apply format`) descends from
`5e55ede4`. It is the code baseline for the Jacobian experiment.
The journal is added before reverting the experiment, so the unfavorable
result is retained permanently.

### Earlier negative experiment: batched box insertion

- [`5e02f687`](https://github.com/ariadne-cps/paving/commit/5e02f6875dd6e274eb5efe7fbb197b4800cf79f7):
  batch cells generated by Subdivision before inserting them into the
  paving.
- [`c06f3df5`](https://github.com/ariadne-cps/paving/commit/c06f3df5cb1a5b706753dd53c4262e84339d3d02):
  correct virtual-base downcasts encountered by that implementation.
- [`a729d405`](https://github.com/ariadne-cps/paving/commit/a729d40598bffda28559928b51397a29ed713630):
  **revert** the batching after a measured slowdown.

**Lesson:** reducing call count does not necessarily reduce the work
done by tree alignment, allocation, geometry, and union. Avoid
reintroducing batching without demonstrating a net win and preserving
exact geometry.

### Successful changes to exact-box insertion

1. [`ad7df7b2`](https://github.com/ariadne-cps/paving/commit/ad7df7b2d2b3b89e2edf8553673b07c3d7c38d7d):
   specialize `ExactBoxSetType` insertion with the generic
   separation/coverage and depth semantics. Retain generic-set fallback.
2. [`108baf96`](https://github.com/ariadne-cps/paving/commit/108baf96994f67ebe919e4466496c0d490499330):
   propagate lattice bounds recursively instead of reconstructing the
   whole binary word and geometry at each node.
   [`fb1c36e5`](https://github.com/ariadne-cps/paving/commit/fb1c36e56ef8fbed81da185c52203e5815bf1227)
   fixes an MSVC `size_t`-to-`dimension_type` warning-as-error.
3. [`47329a7a`](https://github.com/ariadne-cps/paving/commit/47329a7a21798ec044455ddb54b6667f20c3989b):
   propagate physical bounds as well; convert only the one split
   boundary rather than every coordinate at each node.
4. [`2654c0c4`](https://github.com/ariadne-cps/paving/commit/2654c0c43f9f2a1ffa4d04a7b562059e8e35d069):
   share mutable boxes between sibling recursive visits; save and
   restore the old interval on the split axis. Avoid four full box
   copies per branching node.
5. [`39420766`](https://github.com/ariadne-cps/paving/commit/3942076601676dd1a17eaa348aaadd88ab8fb0fd):
   attempt per-coordinate incremental classification; initially
   failed to compile because unqualified `covers`/`separated` resolved
   against class members instead of free interval predicates.
   [`d7a79eb3`](https://github.com/ariadne-cps/paving/commit/d7a79eb3c9bbbebebe8167770478ab978a481156)
   qualifies calls with `Ariadne::`. The subsequent 3D benchmark did
   **not** establish a gain for this step in isolation (approximately
   3%-5% higher medians, alongside general runtime noise).
6. [`5e55ede4`](https://github.com/ariadne-cps/paving/commit/5e55ede4951377d11fed608fdd6e7353fdd46257):
   avoid recursive calls and split-axis mutations for already-enabled
   children and children proven separated by a strict split-plane
   comparison. Preserve touching-boundary behavior. The next benchmark
   showed approximately **12%-17% lower** total medians versus the
   preceding sample, and approximately **22%-26% lower** `Adjoin` times.

Relevant regression tests in `tests/test_paver.cpp` compare the
specialized and reference insertion paths by bidirectional cell
containment and verify physical-cell reconstruction, restoration after
recursion, split-plane classification, enabled siblings, degenerate
boxes, and shifted/scaled grids.

### Inconclusive/negative sensitivity optimization

[`3dedecad`](https://github.com/ariadne-cps/paving/commit/3dedecad7078c2682cd9a96d1af8e53c8e33ac32)
introduced a `SubdivisionPaver`-local sensitivity routine that skips
materializing an intermediate matrix of upper intervals after
computing the validated Jacobian. It keeps the same numerical
conversion, accumulation order, and tie-breaking in source, and
introduces `test_subdivision_lipschitz_exact_equivalence`.

The subsequent 3D benchmark yielded **63.228 / 32.788 / 22.949 /
137.059 ms**, versus **57.147 / 32.033 / 22.368 / 137.479 ms** before
the change. `Sensitivity (ms)` was **30.075 / 7.357 / 5.626 /
58.209** versus **27.571 / 7.110 / 5.643 / 57.982**.
Other Pavers' timings also shifted, so this is not proof that
avoiding the matrix is intrinsically slower. It is, however, **not
evidence of a performance win**. Dominant cost is likely Jacobian
evaluation, which this experiment does not eliminate. Decision:
**revert only the experiment**, retaining formatting and all prior
geometry optimizations.

## Other Pavers: findings and future targets

### ReducePaver

Under the 3D fineness-4 benchmark, instrumented `Box red. (ms)`
was approximately **250 / 247 / 164 / 491**, respectively, for the
four selected shapes (one replay). Hull reduction is also substantial.
Contractor precision can give a smaller outer set, especially for
`correlated-strip` and `product-cap`. A future experiment should
profile redundant contractor work while verifying that the tighter
approximation is not silently weakened. No new reducer algorithm has
been implemented or benchmark-validated in this optimization sequence.

### ConstraintPaver

The one-time joint map/constraint function construction
(`ad7df7b2`) eliminated redundant setup inside recursive work. The
diagnostics separately report `Joint (ms)`, sensitivity, map range,
and exact insertion. In the sampled workloads, sensitivity is a
primary runtime cost (for example approximately 178 ms on
`uv-triangle` and 424 ms on `folded-quadratic` in one replay).
Do not equate its traversal and contraction behavior with Subdivision's.

### AffinePaver

Affinely approximate the set, with a bounded subdivision search.
Continue reporting `Depth cap` and `Unresolved` separately from
completed approximations. The four-case speed comparison above does
not provide comparable affine timing measurements.

## 2026-11-13 - Reverted ThreadManager-backed parallel SubdivisionPaver

**Disposition: REVERTED after user benchmark and failing CI.**
The experiment was introduced in
[`0ec1663b`](https://github.com/ariadne-cps/paving/commit/0ec1663bd3485aa5f46ae7604490c1f6b7264f14)
after the sequential optimization baseline
[`142888ba`](https://github.com/ariadne-cps/paving/commit/142888baede341a2251fdfde72b7e3f0c0eb7e2b).
It consumed the existing process-wide
`ThreadManager::instance().concurrency()` setting, rather than adding
a separate worker parameter. Concurrency zero used the original
sequential recursion. Positive values generated a bounded parameter
frontier (up to 32 subdomains) and submitted tasks via `TaskGroup`.
The tasks shared a thread-safe paving, whose public outer-approximation
insertion holds an owner mutex for the duration of each insertion.
The implementation and benchmark CLI extension have been removed;
`ThreadManager` and other paving backends' thread safety remain intact.

### User-measured benchmark

Release, dimension 3, fineness 4, five samples per case.
Numbers are **median wall-clock milliseconds**; every row completed
5/5 samples. The `concurrency` numbers were from separate runs, not
an interleaved randomized A/B sequence:

| Shape | 0 (sequential) | 2 | 4 | 8 |
| --- | ---: | ---: | ---: | ---: |
| `uv-triangle` | **57.172** | 60.778 | 62.386 | 73.925 |
| `correlated-strip` | **31.960** | 35.411 | 36.034 | 40.471 |
| `product-cap` | **22.324** | 23.882 | 26.398 | 30.905 |
| `folded-quadratic` | 136.435 | **133.838** | 141.553 | 166.690 |

At concurrency 2 the total slowdown is about 6.3%, 10.8%, and
7.0% on the first three cases; folded-quadratic improved by about
1.9%. At concurrency 8, all four slow down by about 22%–38%.
All runs reported the same cell counts and outer volumes as the
sequential baseline: 701/0.826172, 706/0.940430,
627/0.760254, and 1065/1.423828, respectively.
These aggregates do **not** by themselves establish exact geometric
equivalence. No repeatable broad speedup was established.

### Validation and lessons

The `0ec1663b` CI had successful Ubuntu Release builds but
failed multiple Ubuntu Debug builds and the AddressSanitizer/UBSan
and ThreadSanitizer build workflows. The precise compilation
diagnostics were not obtained, so do not label those failures as
specific compiler or race bugs without investigating the logs.
The failed checks are an independent reason not to promote the
experiment.

Potential throughput limits include (not yet separately measured):
task scheduling overhead, load imbalance, repeated parameter-domain
objects, and serialization in
`BinaryTreePaving::adjoin_outer_approximation` under
`_adjoin_mutex`. Thread safety guarantees correctness of supported
concurrent public operations; it does not guarantee speedup.

**Rollback contract:** restore `include/paving/paver.hpp`,
`src/paver.cpp`, `tests/test_paver.cpp`,
`benchmarks/benchmark_pavers.cpp`, and `README.md` byte-for-byte to
`142888ba`. Retain this journal, the earlier
[`0222cd87`](https://github.com/ariadne-cps/paving/commit/0222cd875b0f1f17825ca299f44c72d4fbb6bdb9)
`Apply format` commit, and all successful sequential optimizations.
Do not resume SubdivisionPaver parallelization without a new explicit
decision and a redesigned, separately validated approach.

## 2026-11-14 - ConstraintPaver domain-evaluation reuse (first experiment)

**Status: source-level change, benchmark and build validation pending.**
The interval constraint recursion previously recomputed the combined
map/constraint range and Jacobian-based sensitivity whenever an *output
cell* was split, even though the parameter domain had not changed.
An optional per-domain evaluation record now shares these two results
among descendants created by output-cell splitting. A parameter-domain
split starts with a fresh record. Results remain lazy: a previously covered
cell triggers neither computation, and a separated cell need not compute
sensitivity.

No geometric decision rule was intentionally changed: the same
certified disjointness predicate, sensitivity selection, intrinsic-error
criterion, output-cell depth, and cell insertion remain in place.
Diagnostics count actual evaluations (cache misses) rather than each
recursive visit; visits and split counts retain their previous meaning.
Tests add upper bounds on evaluation counts derived from the number
of parameter domains created.

**Validation required:** compile and run `test_paver` on supported
platforms; compare the candidate against the previous main commit using
bidirectional exact cell containment for all benchmark cases, including
constrained, degenerate and shifted/scaled grids. In Release mode,
alternate five-sample baseline/candidate runs of
`benchmark_pavers --dimensions 3 --finenesses 4
--shapes uv-triangle,correlated-strip,product-cap,folded-quadratic
--pavers constraint --samples 5 --max-case-ms 0 --diagnostics`.
Record medians, MADs, evaluator counts and timing buckets before
claiming an improvement. Revert if there is a correctness regression or
no reproducible wall-clock benefit.

## 2026-11-14 - ConstraintPaver domain-evaluation reuse (first measurements)

**Disposition: KEEP provisionally (strong performance gain, full correctness validation pending).**
User-supplied "before" and "after" Release measurements on dimension 3,
fineness 4, with five completed samples per shape, `--max-case-ms 0`
and `--diagnostics`, using only `--pavers constraint`. The source
change is commit [`d047cb6b`](https://github.com/ariadne-cps/paving/commit/d047cb6b3507e51b59b52327dec0adf8f45fa498).
Both runs completed 5/5 samples; there were no soft-budget rows.
The reported runs are not documented as interleaved A/B runs.

### User-supplied wall-clock timings

| Case | Before median (ms) | Before MAD (ms) | After median (ms) | After MAD (ms) | Median reduction | Speedup |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| `uv-triangle` | 294.600 | 1.609 | 193.748 | 0.699 | 34.2% | 1.52x |
| `correlated-strip` | 290.806 | 1.683 | 200.252 | 3.361 | 31.1% | 1.45x |
| `product-cap` | 227.488 | 0.782 | 156.831 | 0.301 | 31.1% | 1.45x |
| `folded-quadratic` | 689.069 | 2.939 | 431.226 | 5.097 | 37.4% | 1.60x |

### Per-invocation diagnostic replay counts

| Case | Visits (both) | Parameter / cell splits (both) | Map evals before -> after | Constraint evals before -> after | Sensitivity before -> after |
| --- | ---: | ---: | ---: | ---: | ---: |
| `uv-triangle` | 90535 | 22568 / 22699 | 78735 -> 41295 | 0 -> 0 | 48651 -> 25284 |
| `correlated-strip` | 102049 | 26086 / 24938 | 88185 -> 45876 | 88185 -> 45876 | 54876 -> 28965 |
| `product-cap` | 82267 | 21333 / 19800 | 71599 -> 37925 | 71599 -> 37925 | 44247 -> 23613 |
| `folded-quadratic` | 154829 | 36609 / 40805 | 130393 -> 63012 | 0 -> 0 | 83246 -> 43519 |

For each case, disjoint rejection, previously covered, and adjoin counts
also remained identical: `uv-triangle` (30083 / 11801 / 3384),
`correlated-strip` (33308 / 13865 / 3852), `product-cap`
(27351 / 10669 / 3114), and `folded-quadratic` (47146 / 24437 / 5832).
The joint-function construction count stayed at one per invocation.

### Instrumented operation times (not wall-clock medians)

| Case | Map or joint before -> after (ms) | Sensitivity before -> after (ms) | Other before -> after (ms) |
| --- | ---: | ---: | ---: |
| `uv-triangle` | 25.466 -> 13.244 | 179.896 -> 93.607 | 89.949 -> 88.481 |
| `correlated-strip` | 36.922 -> 19.030 | 155.920 -> 82.022 | 101.641 -> 98.740 |
| `product-cap` | 23.877 -> 12.160 | 125.703 -> 66.698 | 80.755 -> 77.999 |
| `folded-quadratic` | 90.143 -> 44.450 | 430.431 -> 225.351 | 165.572 -> 161.375 |

The reduction in map/constraint evaluation and Jacobian sensitivity
work is consistent with caching across output-cell children. The
recursive traversal itself has not shrunk. `Other (ms)` is now a
notable target for profiling; it includes recursion, bookkeeping,
geometric box construction and profiling overhead, so attributing it
to a single bottleneck without further measurements is unjustified.

**Geometry and verification:** cell counts and approximate outer
volumes stayed identical (`uv-triangle` 701 / 0.826172,
`correlated-strip` 706 / 0.940430, `product-cap` 627 / 0.760254,
`folded-quadratic` 1065 / 1.423828). Exact baseline/candidate
bidirectional cell containment is **not established** by these
aggregates or by the single-Paver benchmark output. The successful
benchmark execution does not establish that all `test_paver` cases
and supported-platform builds have passed. Retain the experiment
pending geometry equivalence and test-suite verification; repeat
controlled interleaved A/B timings if small subsequent changes are
to be ranked.

## 2026-11-14 - ConstraintPaver traversal-cost profiling (second experiment)

**Status: diagnostic-only instrumentation, user benchmark pending.**
After the domain-evaluation cache reduced wall-clock medians by
31.1%-37.4% in four 3D workloads, unaccounted `Other (ms)` remained
at approximately 78-161 ms in the separate diagnostic replay.
This is a new attribution experiment, **not** an algorithmic speedup.

Instrument the existing `ConstraintPaver` recursion with four
mutually exclusive timing buckets:

1. `superset(cell)` before considering a cell;
2. `cell.box()` when the cell is not already covered;
3. construction of `product(cell_box, constraint_bounds)`;
4. `definitely(disjoint(combined_range, target_box))`.

The separated expression preserves the previous order, exact box
representation, and certified predicate. The ordinary path still
uses the same function, cell, constraint bounds, recursion and
sensitivity logic. The additional diagnostic counters check that
coverage tests equal visits and box/product/disjoint operation counts
agree. The benchmark prints a separate `ConstraintPaver traversal
breakdown` with counts and timing buckets. Its `Other (ms)` now
subtracts these new times; **do not compare that column directly to
the preceding measurements**. The new fine-grained timers themselves
add measurement overhead to diagnostic replays.

**Next validation:** compile and run `test_paver`, then compare
the four workloads with the previous commit using unchanged
uninstrumented wall-clock samples and geometric checks. Run
`benchmark_pavers --dimensions 3 --finenesses 4
--shapes uv-triangle,correlated-strip,product-cap,folded-quadratic
--pavers constraint --samples 5 --max-case-ms 0 --diagnostics`.
Use the new traversal breakdown to identify which operation
dominates, then choose a single targeted optimization.
Do not assume cheaper cell geometry or faster coverage queries
without first observing these costs. Exact cross-revision geometric
containment remains a separate outstanding verification.

## 2026-11-14 - ConstraintPaver: skip provably redundant coverage queries

**Status: committed candidate; benchmarks and tests pending.** Following
the traversal-only profiling on the four dimension-3, fineness-4 cases,
the measured wall-clock medians were 193.173 / 196.969 / 156.321 /
423.669 ms (`uv-triangle`, `correlated-strip`, `product-cap`,
`folded-quadratic`). The traversal diagnostic replay recorded:

| Case | Visits | Parameter splits | `superset(cell)` before (ms) | `cell.box()` + product + disjoint (ms) | Baseline `Other` (ms) |
| --- | ---: | ---: | ---: | ---: | ---: |
| `uv-triangle` | 90535 | 22568 | 28.403 | 4.264 | 61.580 |
| `correlated-strip` | 102049 | 26086 | 31.938 | 5.039 | 68.973 |
| `product-cap` | 82267 | 21333 | 25.904 | 3.987 | 54.451 |
| `folded-quadratic` | 154829 | 36609 | 50.368 | 7.046 | 113.622 |

The timings above belong to an **instrumented replay**, not the
standard medians. `superset(cell)` accounts for around 12-16% of the
profiled total; the three other explicit geometry operations together
cost much less, so reducing box construction first is unlikely to pay.

An internal `known_uncovered` flag is propagated **only** from a
parameter-splitting parent to its first recursive child. Both visit
the *same output cell*, and no paving mutation occurs between the
parent's unsuccessful `superset(cell)` query and that first visit.
Therefore the first child's identical coverage query is redundant.
The second child continues to check coverage because the first child
may have enabled cells. Both children of a **cell split** still check
coverage, since a parent not covered can have individually covered
subcells. This skips one check per parameter split but changes no
constraint, sensitivity, disjointness, splitting, insertion, or
geometric equality predicate. It requires no new state in the backend
and preserves the generic `PavingInterface` dispatch.

The instrumented invariant becomes
`coverage_checks + parameter_splits == visited_nodes`, replacing
`coverage_checks == visited_nodes`. The four previous runs imply
candidate coverage-check counts of 67967, 75963, 60934 and 118220
respectively **if the traversal counts remain unchanged**. These
are predictions to validate, not observed candidate measurements.

**Validation:** build and run `test_paver`, then run
`benchmark_pavers --dimensions 3 --finenesses 4
--shapes uv-triangle,correlated-strip,product-cap,folded-quadratic
--pavers constraint --samples 5 --max-case-ms 0 --diagnostics`
and compare exact resulting pavings across revisions by bidirectional
cell containment (not only cell counts and volume). Check both fresh
and pre-enabled pavings, since a second parameter child can observe
insertions by its first sibling. Revert if the gain is not repeatable
or any geometric regression occurs.

## 2026-11-14 - Measured results of redundant-coverage elimination

**Disposition: KEEP on main based on user-supplied performance evidence;
cross-revision exact geometry comparison and complete test status remain
unverified.** The candidate is commit
[`900f0f6`](https://github.com/ariadne-cps/paving/commit/900f0f6ff00025a22527ffb6ba8fca2b76d4466a).
Release, dimension 3, fineness 4, five completed samples per case,
`--diagnostics`, `--pavers constraint`. The preceding measurements
were from the profiling-only revision `1674b28`; these runs were not
documented as interleaved A/B trials.

| Case | Before median (ms) | After median (ms) | After MAD (ms) | Reduction | Superset before -> after (ms) | Coverage checks before -> after |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| `uv-triangle` | 193.173 | 185.474 | 0.186 | 4.0% | 28.403 -> 22.045 | 90535 -> 67967 |
| `correlated-strip` | 196.969 | 190.181 | 0.330 | 3.4% | 31.938 -> 25.104 | 102049 -> 75963 |
| `product-cap` | 156.321 | 149.547 | 0.165 | 4.3% | 25.904 -> 19.625 | 82267 -> 60934 |
| `folded-quadratic` | 423.669 | 412.313 | 0.077 | 2.7% | 50.368 -> 39.390 | 154829 -> 118220 |

The measured coverage counts exactly match `visits - parameter_splits`
(22568, 26086, 21333, 36609 split domains respectively).
All four cases retained identical visits, parameter splits, cell splits,
map/constraint evaluations, sensitivity evaluations, rejections, already
covered counts and adjoins. Final cell counts and approximate volumes
were unchanged: `uv-triangle` 701 / 0.826172; `correlated-strip` 706 / 0.940430; `product-cap` 627 / 0.760254; `folded-quadratic` 1065 / 1.423828.

The reduced `superset` cost and improved wall-clock medians are
consistent with the intended removal of one redundant query per parameter
split, without a change in traversal. The remaining `Other` times
are 61.574 / 69.050 / 54.381 / 113.169 ms; this is still an unassigned
mixture of recursion, bookkeeping, splitting and measurement overhead,
not evidence of any single bottleneck. The detailed geometry operations
are each small compared with coverage and sensitivity.

**Outstanding:** exact baseline/candidate bidirectional cell containment
(including pre-enabled pavings), platform builds, and full `test_paver`
results were not supplied. Equal aggregate geometry and identical
algorithm counters do not establish exact cell equivalence. Do not
report these as passed.


## 2026-11-14 - Reachability-style enclosure-sequence benchmark

**Status: implementation submitted for build and runtime validation;
no timings have been measured yet.**

Motivation: the original `benchmark_pavers` times one isolated set in
an empty paving, whereas `HybridReachabilityAnalyser` adjoins many
enclosures to an evolving paving. On SPRE22, `ConstraintPaver` was
reported faster than `SubdivisionPaver`, reversing the standalone
3D benchmark ranking. This alone does not prove a per-call speed
advantage: the Pavers can produce different approximations, changing
later reachability work. The published `arch/spre22` branch still
selects the default `SubdivisionPaver`, so the locally measured
comparison was not independently reproduced here.

A standalone `benchmark_paver_sequences` has been added without
changing the original benchmark. It constructs a deterministic
five-state `(t,x,y,vx,vy)` sequence, with grid scale
`(1,200,200,20,20)`, quadratic cross-terms, parameter constraints,
and validated Taylor-model functions. Two scenarios (`overlap` and
`sweep`) and two modes (`isolated` and `accumulating`) distinguish
nonlinear-function cost from existing-coverage work. Each strategy
receives the same prebuilt sequence; only calls to the Paver run
inside the timed callback. Per-sample geometry is checked by
bidirectional cell containment. The benchmark separately reports
pairwise Paver geometry and isolated-versus-accumulated geometry,
as these may differ. Optional diagnostic replay breaks down each
enclosure's work.

**Important limitation:** this is an SPRE22-inspired **surrogate**,
not a serialized fixture captured from an actual evolver; matching
dimension and scale does not prove the distributions of parameter
ranks, Taylor-model coefficients, constraints, and overlap match
SPRE22. It must not be cited as a measured explanation of the
original observation. Capturing actual enclosure sequences at the
Evolver/Paver boundary would be the next fidelity step, preferably
with a fixture format that does not introduce an Evolution dependency
into Paving.

**Validation outstanding:** build `benchmark_paver_sequences` in
Release and Debug and run small `--verify-only` cases, including
both scenarios and modes, then five-sample controlled comparisons
at fineness 2 and optionally 3. Check nonemptiness, deterministic
exact geometry per strategy, geometric relations across strategies,
and the isolated/accumulating relationship before interpreting
speed ratios. Record the measured times and profiles here only
after they have been obtained; do not assume a winner.
