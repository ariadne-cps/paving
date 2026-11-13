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
