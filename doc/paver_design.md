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

### First build feedback: effective-function constant type

The first macOS arm64 Release compile failed in
`benchmark_paver_sequences.cpp` at the time-map expression
`k * 0.125_x + u[0] * 0.0625_x`. The left term has type
`ApproximateDouble`, which cannot be added directly to an
`EffectiveScalarMultivariateFunction`. Replace it with an explicit
effective constant function for the exact rational offset `k/8`
(`EffectiveScalarMultivariateFunction::constant(5u, Real(k)/Real(8))`).
This changes only construction of the deterministic fixture, not
Paver algorithms or benchmark measurement logic. **Build status
remains unverified until the user recompiles.**


### First measured SPRE22-inspired sequence (2026-11-14)

**User-supplied results; interpretation: surrogate is too easy to
represent the reported SPRE22 reversal.** Build succeeded on user's
macOS arm64 Release environment after the symbolic-constant fix.
Configuration: `--finenesses 2 --scenarios overlap --modes accumulating
--steps 6 --samples 5 --pavers subdivision,constraint --diagnostics`.
All ten samples completed.

| Paver | Median (ms) | MAD (ms) | Cells | Outer volume |
| --- | ---: | ---: | ---: | ---: |
| Subdivision | 0.992 | 0.022 | 16 | 250000 |
| Constraint | 3.120 | 0.245 | 16 | 250000 |

The final pavings were **exactly geometrically equal**, verified by
bidirectional cell coverage. Constraint was approximately 3.15x
slower on this surrogate. Subdivision performed only 3 recursive
visits, 1 parameter split, 3 map evaluations and 1 sensitivity
evaluation on **each** of the six inputs. Constraint performed
191, 107, 139, 107, 175 and 107 visits, with **zero parameter
splits on all six**, and 0 / 4 / 4 / 4 / 4 / 4 covered-cell
hits. Each input produced a small number of coarse output cells.
The tiny amount of mathematical work in Subdivision and lack of
parameter subdivision in Constraint make this fixture a poor proxy
for actual reachability enclosures. The result does not falsify
the reported advantage of Constraint in SPRE22.

**Next controlled check:** run the same sequence at fineness 4,
matching the SPRE22 analyser configuration, with diagnostics and
a complete sample budget. Inspect per-stage visits, sensitivity
evaluations and output-cell growth before redesigning the fixture.
If the workload is still trivial, collect representative actual
validated enclosure inputs at the Evolver-to-Paver boundary (including
domain, function, constraints, scaling, and prior paving occupancy)
rather than tuning an artificial polynomial until it favours a
particular algorithm. Do not treat the standalone wall-clock ratio
as end-to-end reachability speed; the latter also depends on how
many cells are evolved at subsequent steps.


### Second measured SPRE22-inspired sequence: fineness 4 (2026-11-14)

**User-supplied Release data; keep workload and results, but do not
interpret this as a same-precision speed comparison.** Configuration:
`--finenesses 4 --scenarios overlap --modes accumulating --steps 6
--samples 5 --pavers subdivision,constraint --max-case-ms 0 --diagnostics`.
All ten samples completed. Exact bidirectional cell containment
reported `B strictly contains A`: Constraint is the strictly
larger (less tight) outer paving.

| Paver | Median (ms) | MAD (ms) | Cells after recombination | Outer volume |
| --- | ---: | ---: | ---: | ---: |
| Subdivision | 4403.839 | 5.457 | 571 | 10620.117188 |
| Constraint | 349.812 | 0.516 | 1048 | 22888.183594 |

Constraint's median is **12.59x faster**, but its outer volume
is **2.155x** Subdivision's (115.5% greater), with 1.835x as
many final cells. The speed ratio measures distinct approximation
qualities, not equal accuracy.

| Paver | Visits (six steps) | Parameter splits | Map evaluations | Sensitivity evaluations | Covered |
| --- | ---: | ---: | ---: | ---: | ---: |
| Subdivision | 231822 | 115908 | 230472 | 115908 | 0 |
| Constraint | 42616 | 8851 | 17606 | 10257 | 3608 |

The above counts are sums from separate *instrumented* per-step
replays, not a timing decomposition of median runs. The last column
indicates positive already-covered checks for Constraint (about
8.5% of its visits); it does not by itself explain the 12.59x
advantage. The larger effect is different parameter-domain
subdivision and the resulting looser discretization, a natural
consequence of each Paver's decision rules. The pre-recombination
sizes printed in the per-step replay (696 / 1500 by step six) are
not the final sizes reported by the benchmark (571 / 1048).

**Interpretation:** the switch from fineness 2 (equal paving;
Subdivision faster) to fineness 4 (Constraint much faster but
strictly less precise) confirms that this surrogate has a strongly
fineness-dependent quality/time tradeoff. It reproduces a runtime
reversal, **not** a like-for-like quality win, nor an isolated
explanation for SPRE22's full reachability time. The underlying
SPRE22 analyser can produce different future cell populations
depending on Paver choice, so downstream evolution cost must also
be measured separately.

**Next experiment:** execute the same fineness-4, six-enclosure,
`overlap` case in `isolated` mode, with otherwise identical
options, to test how much the accumulating paving affects either
Paver. Then compare `sweep` and (preferably) captured actual
SPRE22 enclosure sequences. To compare computational efficiency
at similar tightness, construct a time-versus-volume/Paver frontier
over multiple fineness settings; never call two runs equal-quality
solely because their fineness settings match.


### Third measured SPRE22-inspired sequence: isolated versus accumulating

**User-supplied Release data (2026-11-14)**. Identical fineness-4,
six-enclosure overlap sequence with the prior run, but using
`--modes isolated`; five complete timed samples for each Paver.

| Paver | Isolated median / MAD (ms) | Accumulating median / MAD (ms) | Isolated / accumulating |
| --- | ---: | ---: | ---: |
| Subdivision | 4361.872 / 2.405 | 4403.839 / 5.457 | 0.9905 |
| Constraint | 356.267 / 0.141 | 349.812 / 0.516 | 1.0185 |

Constraint is 12.24x faster in isolated mode and 12.59x faster in
accumulating mode. Differences between modes are small compared with
the Paver-to-Paver difference; the executions were not documented as
randomized/interleaved comparisons.

| Diagnostic replay total | Isolated | Accumulating |
| --- | ---: | ---: |
| Constraint visited nodes | 43468 | 42616 |
| Constraint parameter splits | 9018 | 8851 |
| Constraint already covered | 3338 | 3608 |
| Subdivision visited nodes | 231822 | 231822 |
| Subdivision parameter splits | 115908 | 115908 |

The isolated mode still accumulates cells *within each Paver call*;
it only begins a fresh paving for each enclosure. Thus its 3338
Constraint already-covered hits are expected and are not evidence
that the mode accidentally reuses earlier enclosure output.

For both modes, final results report Subdivision 571 cells and outer
volume 10620.117188, Constraint 1048 cells and outer volume
22888.183594. The within-mode pairwise geometry check reports that
Constraint strictly contains Subdivision. The separate runs did not
execute a direct exact isolated-vs-accumulating geometric check,
because the benchmark only compares modes when selected in the
*same invocation*. Equal aggregate size/volume across the runs is
not proof of geometric identity.

**Conclusion:** previous-enclosure paving occupancy is not the
dominant source of Constraint's roughly 12x advantage on this
surrogate. Parameter-splitting strategy and *different geometric
tightness* remain the primary candidates. No changes to either Paver
are justified by these measurements alone.

**Next comparison:** run `--modes isolated,accumulating` together
to get the direct cross-mode containment table, and compare a
time-versus-volume frontier across finenesses or record actual SPRE22
enclosure sequences to investigate representativeness. Higher
fineness on a surrogate does not by itself establish comparable
accuracy between strategies.


## 2026-11-14 - Cross-fineness time/tightness frontier for enclosure sequences

**Status: source implemented; compilation and new benchmark execution
pending.** The user-supplied fineness-4 overlap/accumulating case
showed that ConstraintPaver is about 12.59x faster than
SubdivisionPaver, while the former's final outer volume is 2.155x
larger. The corresponding isolated run preserved the large runtime
gap. This is a different-accuracy tradeoff, not a same-quality speedup.

Extend the existing `benchmark_paver_sequences` (without modifying
the original `benchmark_pavers`) to retain each sample configuration's
median runtime and exact output paving. After all timed configurations,
compare outputs with **identical scenario, accumulation mode, enclosure
count and sample count** but *different* fineness settings. Print:

- A `Time/tightness frontier` with absolute medians, final cells,
  absolute outer volume, volume relative to the smallest measured
  volume for the same workload, and a descriptive Pareto status.
- Pairwise `Cross-fineness geometric comparisons` using bidirectional
  exact cell containment, rather than inferring from volumes.

A point is `dominated` only if a complete-sample competitor is no
slower and its entire paving is geometrically included; strictness
comes from a faster median or strict geometric containment.
Incomplete `SOFT LIMIT` samples must be displayed as unranked.
Do not attach statistical significance to small median changes, and
do not regard identical fineness as identical geometric precision.
`--verify-only` still checks geometry but cannot rank times.

**Validation command (from Release build directory):**

```bash
cmake --build . --target benchmark_paver_sequences
./benchmarks/benchmark_paver_sequences --finenesses 2,3,4 --steps 6 --samples 5 --scenarios overlap --modes accumulating --pavers subdivision,constraint --max-case-ms 0 -t dark
```

Test smaller `--finenesses 1,2` and `--verify-only` first, then the
full `2,3,4` frontier. This remains a synthetic 5D surrogate, not
a replay of real SPRE22 enclosures; further representativeness work
requires capturing actual evolver-to-Paver inputs.

## 2026-11-14 - Remove arbitrary benchmark fineness ceilings

**Status: source updated; build and performance validation pending.**
The earlier time/tightness frontier for six 5D overlap enclosures
compared both algorithms at finenesses 2, 3, and 4. At fineness 4
Constraint was 12.56x faster but produced a strictly larger outer
approximation. Follow-up must compare *both* algorithms at fineness
5 under identical input, grid, scenario, accumulation mode, and
sample count; cross-fineness frontiers are supplementary.

Previously both `benchmark_paver_sequences` and
`benchmark_pavers` rejected `fineness > 4` independently of the
implementation. Remove the arbitrary check in **both**
benchmarks, preserving their unrelated dimension/sample
validations. The command-line parameter remains `unsigned`;
the value is forwarded unmodified to each Paver. No change is
made to the algorithms and no Paver receives a benchmark-specific
upper fineness limit.

The paired benchmark command is:

```bash
./benchmarks/benchmark_paver_sequences --finenesses 4,5 --steps 6 --samples 5 --scenarios overlap --modes accumulating --pavers subdivision,constraint --max-case-ms 0 -t dark
```

This evaluates all four Paver/fineness configurations and
reports both same-fineness and cross-fineness exact geometric
relations. Equal fineness is the appropriate same-setting
comparison but does not guarantee equal geometric tightness.
Because a fineness increment can cause superlinear/exponential
growth in recursive work, no runtime estimate or completion
guarantee is claimed. The benchmark's soft case budget cannot
cancel an individual long-running computation.


## 2026-11-14 - Investigating ConstraintPaver f=4 quality discontinuity

**Status: source-level hypothesis, not yet experimentally isolated.
No algorithmic changes.** The user's six-enclosure
`overlap/accumulating` sequence at f=4 and f=5, run with
only `--pavers constraint`, yielded:

| Constraint fineness | Median (ms) | MAD (ms) | Cells | Outer volume |
| ---: | ---: | ---: | ---: | ---: |
| 4 | 351.877 | 0.134 | 1048 | 22888.183594 |
| 5 | 2637.805 | 4.515 | 5031 | 4156.589508 |

Five completed samples each. Exact cell containment reports
Constraint f=4 **strictly contains** Constraint f=5.
The f=5 output has 81.8% less volume and costs 7.50x as much.
The separate Subdivision f=5 run was reported to take an
impractically long time; **no complete f=5 Subdivision
measurement or fair paired speed ratio is available**.

The quality gap at f=4 has a concrete possible contributor
in the current `ConstraintPaver` *interval recursion*:

- In `src/paver.cpp`, `cell_splitting_radius(cell)` returns
  `cell.box().radius().upper().raw()`: **the maximum physical
  half-width among all output dimensions**.
- In `set/src/function_set.cpp`,
  `lipschitz_index_and_error(function, domain)` computes each
  parameter column's **sum of absolute interval Jacobian
  coefficients across output dimensions**, multiplied by the
  parameter-domain radius. It does not divide output
  coordinates by the Paver grid lengths.
- The recursion splits parameters while the scalar Lipschitz
  estimate exceeds that maximum output-cell radius. Otherwise,
  it splits output cells until `cell.depth() >= fineness *
  cell.dimension()`, and then adds any cell not certified
  disjoint. Thus this threshold affects how much parameter
  refinement precedes conservative inclusion of unresolved
  output cells.
- For the surrogate grid with lengths
  `(1,200,200,20,20)`, at depth `fineness*5` an output
  cell's physical half-widths are `(1,200,200,20,20)/2^(f+1)`.
  At f=4 these are
  `(0.03125,6.25,6.25,0.625,0.625)`: the single
  decision radius is **6.25**. At f=5 it is **3.125**.
  The scalar split decision is therefore driven by the
  largest spatial scale and is not equivalent to checking
  adequacy for every output coordinate in normalized
  grid units.

**Hypothesis:** this heterogeneous-unit scalar comparison
can stop parameter subdivision while some fine-scale output
coordinates remain poorly resolved, causing a large outer
paving at f=4. The tightened threshold at f=5 can trigger
further domain splits and a large improvement in coverage.
This is consistent with observed behavior, but no direct
counterfactual has isolated its contribution; cell-depth
discretization itself also doubles per-coordinate resolution.
The unscaled global error norm may be a weak metric without
being an unsound outer-approximation test.

**Next verification (before changing the algorithm):**
instrument a separate diagnostic-only, grid-normalized
sensitivity criterion (Jacobian output rows divided by
corresponding grid lengths, cell radii compared in lattice
units), and report how frequently it would request a
parameter split where the existing scalar criterion does
not. Inspect the diagnostic counts at f=3, f=4 and f=5
and run a controlled A/B candidate only if the discrepancy
explains the observed quality jump. Keep exact bidirectional
cell containment and performance measurements; preserve
certified outer approximation and boundary semantics.

The benchmark's measured component is `ConstraintPaver`,
not `ConstraintSolver`. The code path selected by the
benchmark does not construct or invoke `ConstraintSolver`.

## 2026-11-14 - Instrument coordinate-wise stopping diagnostics

**Status: diagnostic-only experiment, compilation and new benchmark
execution pending; no changes to Paver decisions or geometry.**

To test the previous source-level hypothesis, add a cached,
per-parameter-domain interval-Jacobian sensitivity vector,
calculated **only when `PaverDiagnostics*` is supplied**.
For each output coordinate `i`, compute
`E_i = max_j mag(J_ij) * radius(D_j)` using
`jacobian_range(function, domain)` with validated interval
magnitudes. The *coordinate-wise candidate* split test is
`exists i: E_i > radius(cell.box()[i])`. These comparisons
are in physical units separately per output coordinate, so
they are equivalent to dividing the left and right sides
by the corresponding grid length. Using actual cell radii
also accounts for incomplete rounds of cell splitting.

The existing criterion is untouched:
`lipschitz_index_and_error(function, domain).second >
cell_splitting_radius(cell)`, i.e. the largest parameter-column
sum in physical output units compared with the maximum
physical half-width among the output cell coordinates.
Both decisions are checked at the same surviving nodes,
after the original sensitivity calculation. The diagnostics
record `Tests`, `Jac evals`, `Legacy splits`,
`Axis suggests`, `Axis only`, `Axis only at cap`
and `Legacy only`. In particular `Axis only at cap`
counts terminal cells where the actual code includes a
cell yet the coordinate-wise criterion suggests that
additional parameter refinement might be useful.

The vector is cached within the existing
`ConstraintDomainEvaluation` object and shared across
output-cell children; parameter splitting resets it.
No extra Jacobian evaluation is performed in normal
non-diagnostic samples. The benchmark's separate
instrumented replay still validates bidirectional exact
geometric equality against the normal result.

**Interpretation:** the coordinate-wise sensitivity is a
diagnostic heuristic, not yet a validated alternative
stopping criterion. A suggestion is not a proof that a
cell is disjoint; a new algorithm must still preserve
certified exclusion tests and boundary behavior.
The max-over-parameter Jacobian bound may fail to account
for simultaneous multi-parameter contributions. Do not
infer an improvement in quality or runtime solely from
high `Axis only` counts.

**Next validation (from the build directory):**

```bash
cmake --build . --target benchmark_paver_sequences benchmark_pavers test_paver
./tests/test_paver
./benchmarks/benchmark_paver_sequences --finenesses 3,4,5 --steps 6 --samples 5 --scenarios overlap --modes accumulating --pavers constraint --max-case-ms 0 --diagnostics -t dark
```

Compare `Axis only at cap` across finenesses 3, 4 and 5
before deciding whether a grid-aware criterion warrants
a separately benchmarked algorithm experiment.


## 2026-11-14 - Measured axis-sensitivity counterfactual: f=4 hypothesis narrowed

**User-supplied measurements; status: diagnostic-only hypothesis
partially contradicted; do not change the Paver yet.**
The new instrumented benchmark ran to completion on the user's
Release build for six `overlap/accumulating` enclosures, five
samples each, Constraint only, at fineness 3, 4, and 5.
The extra Jacobian evaluations appear only in an instrumented
replay, so per-step replay timings must not be compared directly
with ordinary sample medians.

| Fine | Median (ms) | Cells after recombination | Outer volume | `Axis only` (sum) | `Axis only at cap` (sum) | `Legacy only` (sum) |
| ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| 3 | 8.827 | 39 | 93750.000000 | 76 | **0** | 0 |
| 4 | 364.589 | 1048 | 22888.183594 | 1900 | **0** | 702 |
| 5 | 2767.528 | 5031 | 4156.589508 | 24148 | **8717** | 4617 |

The total `Tests` counts were 1010, 22805, 175804,
respectively, and the `Jac evals` counts were 106, 10257,
75623. For f=4 the axis-based test requests parameter splits
at 1900 nodes where the current criterion does not; **none**
is at the output-cell depth cap. This contradicts the
initially suggested direct explanation that the scalar criterion
prematurely inserts f=4 *terminal cells* despite the
coordinate-wise diagnostic asking for parameter refinement.
It does **not** rule out an indirect effect of earlier
split decisions if the algorithm were changed: this experiment
did not run an alternative algorithm or compare its geometry.

For f=5, the 8717 `Axis only at cap` suggestions are
substantial, but they do not prove that further parameter
subdivision would allow any particular terminal cell
to be removed. Nor is the coordinate-wise diagnostic
necessarily a sound aggregate Lipschitz bound for
simultaneous parameter variations; it uses `max_j` rather
than summing all contributions.

**Five-dimensional cell-volume accounting.**
The synthetic grid has side lengths `(1,200,200,20,20)`,
whose product is 16000000. Full-depth cell volume is
`16000000 / 2^(5*f)`. The reported pre-recombination
cell totals were 192, 1500 and 8717 at f=3, 4 and 5:
their products with the respective single-cell volumes
488.28125, 15.2587890625, 0.476837158203125 reproduce
the exact total outer volumes (up to decimal printing).
From f=4 to f=5 the number of occupied depth-level
cells grows 5.81x while each cell's volume shrinks 32x,
yielding the observed **81.8% volume reduction**.
This is a plausible ordinary grid-refinement effect;
the headline 81.8% decrease alone is not an algorithmic
anomaly.

**The distinct f=4 quality gap remains important.**
On this same f=4 surrogate, the previously reported
Subdivision paving had 696 pre-recombination fine cells
(571 after recombination), whereas Constraint had 1500
(1048 after recombination). The 804 additional depth-4
cells account for the exact 2.155x outer-volume ratio
`22888.183594 / 10620.117188`, because both cover the
same-sized depth-4 cells. Those 804 cells are **not
certified false positives** merely because the
Subdivision paving is a smaller certified outer
approximation. They are candidates for improved
separation/refinement.

**Next investigation:** focus on what happens to the cells
in `Constraint f=4 \ Subdivision f=4` (set difference).
For each, use independent certified separation checks
and record whether the cause is insufficient parameter
subdivision, weak joint map/constraint interval exclusion,
or unresolved boundary intersection. Any proposed
criterion change should then be tested A/B with exact
geometric containment, preserving the original
certified outer-approximation semantics. Neither Paver
has been modified in response to these measurements.


## 2026-11-14 - Certified exclusion probe for Constraint-only cells

**Status: standalone benchmark diagnostic implemented, compilation and
runtime validation pending; no changes to either Paver algorithm.**

At five-dimensional fineness 4 on the user-supplied six-enclosure
SPRE22-inspired sequence, `ConstraintPaver` gives 1500 uniform
fine cells prior to recombination, versus 696 from
`SubdivisionPaver`. Since the latter is exactly contained in
the former on this case, 804 uniform f=4 cells lie in
`Constraint \ Subdivision`. Merely comparing the two outputs
does not establish which of these cells can be removed without
violating outer-approximation soundness.

New optional `benchmark_paver_sequences --difference-probe`
performs a *separate*, untimed and non-mutating experiment,
only for configurations running **both** Pavers at the same
fineness. Construct the exact set difference using the paving
library's `difference(constraint, subdivision)`; enumerate
its enabled cells at exact requested fineness using
`for_each_enabled_cell_at_exact_fineness` (regardless of
their underlying recombined representation). Verify
Subdivision is geometrically contained in Constraint.
Record the exact number of exclusive cells, not inferred
from total cell count or volume.

For each investigated cell, build the validated combined
map `join(set.function(), set.constraint_function())`,
and `set.constraint_bounds()` for each original
enclosure in the sequence. Perform certified interval
separation using
`definitely(disjoint(apply(combined, domain),
product(cell.box(), constraint_bounds)))`.
If the initial test fails, bisect the *parameter domain*
in a fixed cyclic coordinate order, rechecking subdomains
up to configurable depth. A cell can be proved excluded
from the **union of enclosures** only if the separation
proof succeeds for every enclosure. Otherwise report
`Unresolved`: non-disjoint interval bounds at finite
depth are **not proof of intersection**. No boundary
touching is treated as definite separation.

The table records `Extra cells`, `Probed`, `Root
excluded` (all enclosure roots disjoint), `Refined
excluded` (at least one enclosure requires domain
bisecting), `Unresolved`, `Range evals`, and the
maximum parameter split depth. `--difference-cells 64`
samples 64 approximately evenly spaced cells
deterministically from the enumeration; `0` probes all
of them. `--difference-depth 6` bounds each
unresolved search branch by six parameter bisections;
increasing it can cause exponential work. Probes
run after measurement or `--verify-only` and are
never part of Paver wall-clock samples.

The procedure deliberately uses only the public
validated map/constraint interval API, not the
internal `ConstraintPaver` recursion or its
stopping criterion. Consequently it can independently
certify *some* lost precision and reveal whether
more parameter refinement would help, but it is
not a proof that the legacy algorithm's particular
decision was wrong. A cell that is certified excluded
by the deeper probe is a candidate for removal by a
future certified refinement algorithm, not a mutation
performed by the diagnostic.

**Validation commands (from Release build directory):**

```bash
cmake --build . --target benchmark_paver_sequences
./benchmarks/benchmark_paver_sequences --finenesses 4 --steps 6 --samples 1 --scenarios overlap --modes accumulating --pavers subdivision,constraint --verify-only --difference-probe --difference-cells 64 --difference-depth 6 -t dark
./benchmarks/benchmark_paver_sequences --finenesses 4 --steps 6 --samples 1 --scenarios overlap --modes accumulating --pavers subdivision,constraint --verify-only --difference-probe --difference-cells 0 --difference-depth 6 -t dark
```

Validate exact-geometry pairwise comparison and the
expected count of 804 exclusive uniform cells for this
fixture. Do not use the sampling result to infer the
number of all excluded cells; use `--difference-cells 0`
for exhaustive enumeration. Repeat with different
depths to determine whether unresolved classifications
are caused by a weak interval enclosure, a shallow
split budget, or possible true/boundary intersection.
Keep the historical benchmarks and both Paver algorithms
unchanged.


### First measured certified difference-cell exclusion (2026-11-14)

**User-supplied diagnostic replay** of the six-enclosure,
five-dimensional `overlap/accumulating` sequence at fineness 4,
using `--verify-only --difference-probe --difference-cells 64
--difference-depth 6`. The two Paver outputs were 571 and
1048 recombined cells, respectively, and the exact geometric
relation was `Subdivision strictly contained in Constraint`.
The exact-fineness difference contains **804** cells.

| Metric | Observed |
| --- | ---: |
| Difference cells at exact fineness 4 | 804 |
| Deterministically sampled cells | 64 |
| Certified excluded at root domain | 0 |
| Certified excluded after parameter bisections | **15** |
| Unresolved after maximum 6 parameter bisections/branch | 49 |
| Validated combined range evaluations | 1133 |

The 15 refined-excluded cells are certified disjoint from
**all six original enclosures**, hence unnecessary in a
sound outer approximation of their union. This demonstrates
recoverable excess geometry without sacrificing certified
correctness. The result does not show that the original
Paver made an unsound decision: conservative unresolved-cell
inclusion is permissible. The 49 unresolved cells may be
truly intersected, near a boundary, or require stronger
exclusion/deeper subdivision. The 64 samples are
deterministic systematic samples, **not a randomized sample**:
do not extrapolate the 15/64 fraction to all 804 cells.

The zero root exclusions and positive refined exclusions
provide direct evidence that parameter bisection can
strengthen interval separation in this case. It does
not yet identify which parameter-splitting heuristic
best recovers precision or its incremental computational
cost inside the full Paver.

**Next controlled checks:** probe the same deterministic
64 cells with `--difference-depth 0,2,4,6,8` in separate
otherwise identical `--verify-only` runs and verify
that certified-excluded counts never decrease with
increasing depth. Then use `--difference-cells 0`
for exhaustive depth-6 certification of the 804 cells.
The current implementation's numeric depth parameter
bounds per-branch recursion, not total evaluations.
Avoid modifying the Paver until this evidence supports
a targeted, certified refinement rule that is measured
against the previous geometry and runtime.
