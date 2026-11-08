# ROBDD-backed paving: M5 integration journal

## Scope and invariants

This is an additive, paving-local experiment. `ariadne-cps/bdd` continues to
provide only general-purpose Boolean functions, canonical ROBDD nodes,
model counting and satisfying-assignment enumeration. The geometric
interpretation is entirely owned by `ariadne-cps/paving`.

The new `RobddGridPaving` snapshot stores a `GridCell` root, a
logical subdivision depth, a semantic `Robdd` handle, and a pointer to
the shared `RobddManager`. The manager must outlive the snapshot. Variable
indices start at zero and correspond to successive binary splits below
the geometric root. The BDD root is immutable; changing a snapshot's
logical depth does not change its Boolean function.

A `BinaryTreeNode` leaf that is true or false becomes a corresponding
BDD terminal, independent of its depth. An explicit branch at level
`i` becomes `make_node(BddVariable(i),low,high)`. Reduction can remove
identical branches without removing the logical subdivision level.
The maximal depth of the imported explicit tree is retained separately.

A finite-depth membership test evaluates the Boolean root under the
path's bits. Cell enumeration appends a satisfying assignment to the
stored geometric root word and constructs `GridCell` objects. Model
counts are interpreted as enabled *cells at the declared subdivision
depth*, not counts of explicit tree leaves.

A snapshot may be converted back to `BinaryTreeNode`, expanding
skipped variables while preserving false terminals and reproducing
the historical true-leaf mincing behavior. Conversion is potentially
exponential; this is a compatibility oracle, not the intended final
representation.

## Supported operations (first M5 increment)

- Import a `BinaryTreeSubpaving` without retaining its mutable tree pointers.
- Query membership at exact logical depth and count/visit enabled cells.
- Adjust logical depth (`mince_to_depth`) and reduce it to the
  maximum supported variable index (`recombine`) without BDD mutation.
- Convert back to an explicit `BinaryTreeNode`.
- Boolean union, intersection, difference, subset and overlap tests.
  Operands must use the same BDD manager and **identical root cells**.
  Binary functions are evaluated at a logical depth equal to the maximum
  of the operand depths. The results are immutable snapshots.

The existing `BinaryTreePaving`, `BinaryTreeSubpaving`, iterators and
`BinaryTreePavingCursor` remain unchanged. This prototype does not yet implement
variable-index alignment between distinct geometric roots, geometric
queries, projections, or generic `PavingInterface` polymorphism.

## Dependency integration

The `ss/bdd` gitlink is updated inside `paving` from
`8dfa9fa866d31e11cd9727308dcfc95b14c62c48` to
`7ae56c9a2046386c473be9c38ceffbf172901004`, enabling M1-M4 APIs.
No commit is made to the `bdd` repository. The BDD submodule's Build,
Testing, PythonCommon and Threading dependency pins match the previously
used BDD version, so no additional direct submodule updates are required.

## Verification and next steps

The dedicated C++ regression imports a mixed-depth explicit tree and
checks round-trip membership for all eight paths, counted/enumerated
cells, geometry prefixes, logical mincing/recombination, terminals,
complemented roots, and Boolean set operations against explicit-tree
membership. Manager/root mismatch and invalid-depth cases are checked.

Tests must be executed by the `paving` GitHub CI (all platform,
sanitizer and coverage jobs); local compilation has not been established
by the connector-only integration workflow. If CI reports a source or
semantic failure, diagnose whether the fault is a test oracle, the
new paving adapter, or a dependency before changing an expectation.

Next milestone: real `BinaryTreePaving` parity under root-cell
alignment/height changes, projection and efficient path-based snapshot
iteration. Do not replace pointer-based cursors with raw pointers into
an immutable ROBDD DAG.

## 2026-11-06 - Explicit backend naming

Standardized the public representation-specific C++ names:
`BinaryTreePaving` for the existing explicit binary tree backend,
and `RobddGridPaving` for the new ROBDD prototype.

`BinaryTreePaving` is currently a type alias for the longstanding
`BinaryTreePaving`, declared in
`include/paving/binary_tree_binary_tree_paving.hpp`. The old name remains valid,
preserving the numerous existing references and virtual interface
implementations. No legacy class or algorithm was changed.

Renamed the experimental ROBDD implementation and its source, public
header, dedicated tests, CMake references and umbrella inclusion to
`RobddGridPaving` / `robdd_grid_paving`. This is a naming change only:
the new prototype still does not implement `PavingInterface` and is not
yet interchangeable with the tree-based class. A compile-time test
confirms the intended tree-backend alias.

No change was made to `ariadne-cps/bdd`, the pinned BDD version, or
the semantics of grid paving operations. Build and regression validation
are pending the CI for the naming commit.

## 2026-11-06 - M5: align ROBDD paving roots for set operations

Set operations now support different root cells on the same grid. Both
pavings must share the same `RobddManager`; grid mismatches remain a
precondition violation. The common geometric root is the longest common
prefix of both root paths after expressing them from the higher of the
two primary-cell extents. Extent alignment uses
`GridCell::primary_cell_path` and preserves the established alternating
primary-cell subdivision convention.

To express a source paving from that common root, its ROBDD variable
indices are uniformly shifted by the length of its root-path suffix;
that suffix is encoded as a chain of canonical decision nodes with a
false sibling at each step. Missing Boolean variables remain don't-care
choices. This makes union, intersection, difference, subset and
intersection detection available on roots that coincide, are nested,
or are disjoint, even across primary-cell extents, without expanding
the explicit tree.

The logical depth of a combined snapshot is the maximum of the two
adjusted depths. Its cells are defined relative to the new common root.
The geometric interpretation remains entirely in the paving layer:
no BDD API or implementation changes.

Also fixed MSVC /W4 /WX C4267 in `expand_tree` by checking that the
remaining SizeType logical depth is representable by Nat before
calling `BinaryTreeNode::mince` with an explicit cast. No lossy cast
is permitted on overflow.

Tests add same-grid differing roots, adjacent/disjoint roots,
ancestor/descendant containment, cross-extent path alignment, and
grid incompatibility. CI execution is required to confirm the C++
build, test oracle and sanitizer results.

## 2026-11-06 - Correct cross-extent union test oracle

The new `test_distinct_roots_and_extents` initially expected two
enabled cells from the union of an extent-1 root with word `0`
and an extent-0 root with word `0` on a one-dimensional grid.
The test expectation was wrong: relative to their common extent-1
root, the fine root has path `10`, whereas the coarse root has
path `0`. The merged logical depth is two. Consequently, the
coarse root accounts for the two paths `00` and `01`, and
the finer root accounts for `10`: exactly three satisfying
full-depth paths, with `11` disabled.

The test now checks the common root and depth, all four membership
values against an independently constructed `BinaryTreeNode`
reference, round-trip conversion to an explicit tree, and the
ordered enumeration of `00`, `01`, `10`. The asserted cell
count is derived from the explicit reference rather than merely
changed from two to three.

No change was made to the ROBDD root-alignment implementation or
to the `bdd` repository. CI results for the amended test remain
to be confirmed.

## 2026-11-06 - Compact enabled-leaf iteration versus fixed-depth cells

The explicit `BinaryTreeSubpaving::begin/end` iterator visits enabled
leaves, and its `size()` counts those leaves. The ROBDD M5 prototype
previously only enumerated satisfying *full-length* assignments.
These two representations must not silently conflate the number of
compact enabled leaves with the number of cells at a chosen depth.

Added two **paving-level** methods to `RobddGridPaving`:
`enabled_leaf_count()` and
`for_each_enabled_leaf(std::function<void(GridCell const&)>)`.
These traverse the semantic ROBDD directly in low-before-high order.
A false terminal contributes no leaf; a true terminal contributes
one enabled leaf at the current prefix; and variables absent from
the support are expanded into both branches before the next
supported variable. Thus a compact leaf may correspond to many
uniform-depth satisfying assignments, and the same canonical
BDD subfunction can be reached through multiple prefixes.

The interpretation is the fully recombined ordered binary tree:
it deliberately matches `BinaryTreeNode::recombine()` applied to the
finite-depth explicit interpretation, not an arbitrary unrecombined
tree that stores redundant split structure. In particular,
`mince_to_depth` changes the count of **fixed-depth cells** but
does not change the count of **compact enabled leaves**: the
ROBDD snapshot does not retain historical artificial subdivision
structure. Both existing fixed-depth methods are preserved with
their original semantics.

`enabled_leaf_count` detects SizeType overflow. The compact iterator
does not instantiate a `BinaryTreeNode`, and only creates a
`GridCell` when delivering an actual leaf. As with the fixed-depth
iterator, the visitor receives a temporary reference.

Regression cases cover constant true/false, early true terminals,
skipped levels, complemented edges, stable visitation order,
invalid visitor, and invariance of the manager's node count.
All 256 three-variable truth tables are compared against
`BinaryTreeSubpaving`'s leaf iterator after explicit-tree
recombination. A second independent oracle checks all eight
membership assignments and the fixed-depth model count.
The implementation, tests and documentation are modified only
in `ariadne-cps/paving`; `bdd` remains unchanged.

CI compilation and runtime results for this increment must still
be verified before claiming parity. The next step toward a
common `PavingInterface` is deciding what `size()` means for
an immutable ROBDD paving after logical mincing, and
implementing iterator/geometry operations without prematurely
materializing the full decision tree.

## 2026-11-06 - Backend names and long-term public Paving handle

The backend types are now named `BinaryTreePaving` and `RobddPaving`.
Both provide pavings on a `Grid`, which is already required by
`PavingInterface`; the "Grid" qualifier in the implementation names
adds no information. The newly introduced names
`BinaryTreePaving` / `RobddGridPaving` and their headers are
**deleted outright**. There are no transition compatibility headers or
aliases for these two names.

`BinaryTreePaving` is a public type alias of the long-established
`BinaryTreePaving` concrete class. That original class is still used
throughout the paving implementation and downstream reasoning, and
renaming it is a separate legacy migration rather than retention of
one of the new temporary names. `RobddPaving` is a standalone class,
with the same algorithms and constraints as the previous prototype.
The C++ implementation/test files, umbrella header, CMake declarations
and README use the new names.

### Planned public interface, not implemented in this naming change

The intended public value-like type is `Paving`, replacing the
historical `PavingHandle`; `BinaryTreePaving` and `RobddPaving`
are implementation choices at construction. The existing
`PavingInterface` and `PavingHandle` expose polymorphic operations
and copy-by-clone today. To construct `Paving` from
`RobddPaving`, however, the latter first needs a faithful complete
implementation of `PavingInterface`, including the subpaving
interface and mutation semantics. The shared `RobddManager` lifetime,
clone/copy behaviour, and the difference between compact-leaf size and
fixed-depth assignment counts must be resolved explicitly.

Current downstream `ariadne-cps/reasoning` dependencies go beyond
spelling: `Storage::_state_set` owns `BinaryTreePaving` by value,
`Storage::state_set()` returns a concrete reference, and
`Storage::state_auxiliary_set()` uses `outer_skew_product`.
`Storage` also calls functions not currently exposed by
`PavingInterface`, such as `clear`, `restrict_to_extent`,
`adjoin_lower_approximation`, and concrete drawing support.
`LabelledStorage`, `LabelledBinaryTreePaving`,
`SpaceAnalysisTraits<EuclideanSpace>`, and hybrid storage/type
traits require coordinated follow-up changes. The paver integration
already accepts `PavingInterface&`, which is a useful interface
boundary.

This commit does *not* rename `PavingHandle`, modify
`PavingInterface`, change `Reasoning`, or claim that `RobddPaving`
is already interchangeable with `BinaryTreePaving`. No change is
made to `ariadne-cps/bdd`. CI validation is required.

## 2026-11-06 - M5 incremental compact-leaf iterator

`RobddPaving` now exposes the `ForwardConstantIteratorHandle<GridCell>`
iterator protocol used in the paving interfaces, through `begin()`,
`end()` and a `ConstIterator` typedef. Its `size()` returns
`enabled_leaf_count()`; `grid()` and `dimension()` provide the
ordinary geometric metadata accessors.

The implementation is a private `RobddPavingLeafIterator` deriving from
`ForwardConstantIteratorInterface<GridCell>`. It holds the manager
pointer, a snapshot of the Boolean root and root cell, the logical depth,
a stack of pending search frames, and the currently yielded `GridCell`.
The stack always pushes the high branch before the low branch, so each
increment visits the next enabled leaf in the same depth-first order as
the recombined explicit binary tree. False terminals are skipped;
true terminals close a leaf immediately; skipped BDD variables branch
to both valuations. The iterator does not construct a
`BinaryTreeNode` or materialize the entire output sequence.

The iterator owns its traversal state, including on copy/clone, and
checks that the Boolean root, manager, geometry and logical depth match
when comparing two iterator positions. It remains valid if the
`RobddPaving` object is copied or its logical depth is subsequently
changed, provided the owning `RobddManager` remains alive. It does
not own the manager. Past-the-end dereference and increment are
precondition violations.

The legacy `BinaryTreePaving::size()` can count redundant explicitly
minced leaves, whereas the ROBDD `size()` counts the compact
recombined interpretation. They agree after recombination; they are
not universally identical for unnormalized explicit trees. The
fixed-depth `enabled_cell_count()` remains a separate operation.

Regression tests exercise empty, full, early terminal and skipped
variable cases, copy/clone independence, post-increment semantics,
forward iteration through `operator->`, equality and the complete
set of 256 three-variable truth tables against explicit-tree
reference iterators. These changes are a prerequisite for an eventual
`PavingInterface` implementation; they do not yet make
`RobddPaving` derive from it or rename `PavingHandle` to `Paving`.
`Reasoning` and `bdd` remain unchanged. CI validation is pending.

## 2026-11-06 - Extract shared grid-cell root alignment

Extracted representation-independent grid-cell alignment into
`paving/grid_cell_alignment.hpp` and `src/grid_cell_alignment.cpp`.
The three public functions are `cell_path_at_extent`,
`common_root_cell`, and `relative_cell_path`. The first converts a
cell's word to a chosen enclosing primary-cell extent; the second
finds the smallest common binary-prefix ancestor; the third
produces the path suffix from a containing cell to its descendant.

Both storage implementations now use these geometric helpers:
`RobddPaving::_common_root` calls `common_root_cell`, and its
ROBDD-specific lifting uses `relative_cell_path`; the existing
binary-tree helper `common_primary_cell_path` now calls
`cell_path_at_extent` for both inputs. The binary-tree-specific
pointer navigation and ROBDD-specific variable shifting remain
separate, and no shared stateful superclass was introduced.

A dedicated `test_grid_cell_alignment` executable checks common
ancestors, reversed arguments, same and different primary-cell
extents, geometrically equivalent cell representations, multi-
dimensional primary-cell paths, and rejection of invalid grids,
extents and ancestor relations. Existing backend regressions
provide integration-level evidence of semantic preservation.

No changes are made to `PavingInterface`, `PavingHandle`,
`Reasoning` or `ariadne-cps/bdd`. GitHub CI validation for this
refactor is pending.

## 2026-11-06 - In-place paving updates and design-journal relocation

Added value-like in-place operations to `RobddPaving`:
`adjoin(RobddPaving const&)`, `restrict(RobddPaving const&)`,
`remove(RobddPaving const&)`, `adjoin(GridCell const&)`,
`remove(GridCell const&)`, `set_root_cell(Bool)`, and `clear()`.

The set operations compute the immutable result using the existing
`join`/`intersection`/`difference` implementations, then
assign its root, root cell and logical depth to the receiver. This
preserves the independence of copied pavings and prior iterators,
which retain a snapshot of the old Boolean root and metadata.
A cell is converted into a temporary true-terminal paving, and
alignment uses the existing geometric helpers. The manager
continues to own canonical Boolean nodes and must outlive pavings
and their iterators. Self-union, self-intersection and self-
difference are supported. Preconditions on the manager and grid
are checked before assignment; invalid inputs do not change
the receiver.

`set_root_cell` enables or disables the entire geometric root
with a terminal and resets the logical depth to zero.
`clear` calls `set_root_cell(false)` without changing the
root cell. The fixed-depth versus compact-leaf distinction is
preserved.

Regression cases cover cell-by-cell updates compared with the
explicit `BinaryTreePaving`, independent copies and iterators,
cross-extent changes, self-operations, root clearing/enabling,
and invalid manager or grid arguments.

This increment does **not** complete `PavingInterface`: mixed
`SubPavingInterface` operands, subpaving branches, geometric
bounding/overlap predicates, and approximation methods still
need faithful native implementations.

### Journal location

This file moves from `doc/designs/robdd_paving_design.md`
to `doc/robdd_paving_design.md`. It was the only tracked file
in `doc/designs`, so deleting its old path also eliminates the
now-empty directory. There is no compatibility copy.

No changes were made to `ariadne-cps/bdd` or
`ariadne-cps/reasoning`. CI verification remains necessary.

## 2026-11-06 - Native child restriction and representation-neutral operations

Added explicit support for reading and mutating `RobddPaving` through
ordinary `SubPavingInterface const&` inputs. A new constructor
`RobddPaving(RobddManager&, SubPavingInterface const&)` imports the
source's enabled `GridCell` iterator into the target manager. This
does not require an intermediate `BinaryTreeNode` and works with
subpavings of arbitrary geometric root paths and extents. Import
cost is proportional to the number of enabled cells and may be
substantial for large inputs; it is not a constant-time conversion.

The following native representation-neutral overloads are now
available: `adjoin`, `restrict`, `remove`, `subset` and
`intersects` with `SubPavingInterface const&`; `equals`
with `SubPavingInterface const&`; and `superset(GridCell const&)`.
The existing ROBDD-specific overloads remain available, and are
still chosen for ROBDD arguments. All operations check for grid
compatibility and reuse the existing canonical Boolean
set-operation implementations. Equality is checked by inclusion
in both directions. A cell is completely contained exactly when
its true-terminal paving is included in the receiver.

`RobddPaving::branch(Bool)` now returns a value-like child snapshot.
For a nonzero logical depth, it restricts the first ROBDD variable
to the requested branch and uniformly shifts remaining variable
indices down by one, keeping canonical representation and
complemented-edge semantics. At depth zero the root is necessarily
a terminal, which is reused unchanged for both child cells.
The child geometric root is obtained with `GridCell::split`.
The source paving remains unchanged. This operation does not
materialize a binary tree and does not yet implement the virtual
`SubPavingInterface::_branch` adapter or its mutation-through-view
semantics.

Regression tests compare generic import and mixed operations against
the explicit binary tree, verify compressed leaves and distinct
extent roots, check self-contained geometric membership,
reject foreign grids without modifying the receiver, and
exhaustively compare both child restrictions against all 256
three-variable truth tables. They also exercise skipped variables,
complemented references, empty and full terminal roots.

The `PavingHandle` spelling is deliberately retained. This
increment does **not** make `RobddPaving` derive from
`PavingInterface`: geometric predicates, outer/inner approximation
operations, precise `mince(Nat)` semantics and writable subpaving
views require further implementation. `reasoning`, `drawing`
and `bdd` remain unchanged. CI must validate this increment.

## 2026-11-06 - Exact grid-box queries without tree materialization

Added `bounding_box()`, `inside(ExactBoxType const&)`,
`separated(ExactBoxType const&)` and
`overlaps(ExactBoxType const&)` to `RobddPaving`. All four
are implemented over the native incremental compact-leaf
iterator, rather than by converting the entire ROBDD to a
`BinaryTreeNode`.

`bounding_box` computes the coordinate-wise hull of enabled
leaf-cell boxes and returns an empty box of the ambient
dimension for an empty paving. The Boolean predicates preserve
the exact box semantics of `BinaryTreeSubpaving`:
`inside` compares every enabled cell with the narrowed query
box; `separated` requires that no enabled cell's interior
overlaps the widened query box; and `overlaps` succeeds when
an enabled cell overlaps the narrowed query box. The
conservative widen/narrow steps intentionally preserve the
tree backend's handling of boundary contacts. All methods
check dimension compatibility and leave the paving unchanged.

A reference-oracle regression enumerates all 256 Boolean
functions over three subdivision bits and compares the four
queries against a recombined `BinaryTreeSubpaving` using six
geometric boxes, including disjoint, touching, whole-root,
and nested cases. It also checks empty/full roots, invariance
of bounding geometry under logical mincing, and invalid
query dimensions.

These operations bring the ROBDD prototype closer to
`SubPavingInterface`, but do not yet make it implement the
abstract interface or support all approximation routines.
The immutable DAG remains unexpanded by these queries; worst-case
work is proportional to the compact leaves traversed. The
`PavingHandle` name is unchanged. CI verification is pending.

## 2026-11-06 - Independent concrete-backend geometry oracle

Reworked `test_geometric_queries_against_explicit_tree` as
`test_geometric_queries_between_backends`. The previous test
constructed a `BinaryTreeSubpaving` from a `BinaryTreeNode` and then
imported it into `RobddPaving`; this validated the tree-to-ROBDD
conversion alongside the geometry, rather than fully independent
backend construction.

The rewritten test constructs a fresh `BinaryTreePaving` and a
fresh `RobddPaving` for every one of the 256 three-bit truth
tables, and calls `adjoin(GridCell)` on both with the exact same
geometric input cells. Only the binary-tree instance is
`recombine()`d to normalize its explicit leaf representation;
the ROBDD depth is extended to three for the full-depth membership
checks. Neither backend is constructed from or imported through
the other. The test checks identical geometric roots, compact-leaf
counts and traversal order, enabled membership for every
three-bit cell, bounding boxes, and `inside`/`separated`/
`overlaps` for all six original geometric query boxes.

The existing `BinaryTreeSubpaving` cases remain in the suite
solely for scenarios that specifically require importing an
explicit subtree, exercising non-primary root paths, or
checking polymorphism through `SubPavingInterface`.
The backend-to-backend regression does not need that
intermediate representation.

No production code or public APIs were changed in this
increment. The test must pass the usual CI before its
equivalence claims can be considered verified.

## 2026-11-06 - Grid-relative mincing and primary-cell enclosure

Implemented two further `PavingInterface`-compatible operations in
`RobddPaving` without introducing incomplete virtual overrides:

- `mince(Nat fineness)` computes the same subdivision-depth target
  used by `BinaryTreeSubpaving::mince`:
  `max(0,(root_extent+fineness)*dimension-root_word_size)`.
  The operations check size arithmetic for overflow before adjusting
  the declared logical depth. No ROBDD nodes are expanded and existing
  membership is unchanged. Canonical true terminals still represent
  one compact leaf despite the increased number of fixed-depth cells,
  unlike explicitly minced binary-tree storage. This distinction
  remains deliberate and observable through `size()`.
- `smallest_enclosing_primary_cell(UpperBoxType const&)` validates the
  box dimension and delegates to the existing geometric
  `GridCell::smallest_enclosing_primary_cell(box,grid())` function.
  It does not depend on the current Boolean root.

The regression exercises one-, two- and three-dimensional grids,
different primary extents, and nonempty root paths; compares geometric
predicates and root identity with a corresponding
`BinaryTreeSubpaving`; verifies the computed logical depth and stable
compact-leaf count; checks primary-cell enclosure; and rejects
incompatible query dimensions. An explicit tree is physically minced
only for small target depths to prevent accidental exponential
test-memory consumption.

This increment does not make `RobddPaving` a
`PavingInterface` implementation. Before constructing a
`PavingHandle` from this backend, the writable branch-view
semantics and geometric approximation routines still require
careful implementation. `bdd`, `drawing` and `reasoning`
remain unchanged. CI must validate this increment.

## 2026-11-06 - Representation-neutral cell mutations and detached subpaving slices

Revisited the mutable branch-view design before adding a backend-specific
`RobddSubpaving` type. Existing `BinaryTreeSubpaving` borrows the
parent's mutable `BinaryTreeNode`, whereas an ROBDD paving is a
value-like holder of an immutable canonical DAG root. A writable branch
view cannot simply reuse `RobddPaving::branch(Bool)`, since that
returns a detached snapshot. Nor should it be implemented by mutating
the shared BDD nodes. The general writable-view abstraction and
associated ownership/lifetime contract remain to be established.
**No `RobddSubpaving` class or pretend interface implementation
was added.**

As a shared building block for both backends, introduced the
header-only `paving/paving_cell_operations.hpp` utility:
`set_paving_cell(paving,cell,enabled)`. It uses only the common
`grid()`, `root_cell()`, `adjoin(GridCell)` and
`remove(GridCell)` operations. The helper validates equal grids
and verifies that the modified cell lies within the owning paving's
geometric root, then delegates to the selected backend's native
update operation. It is callable with `PavingInterface&`, the
concrete binary-tree backend, or `RobddPaving&` without introducing
an intermediate class hierarchy. The helper cannot replace a
subpaving view by itself; it only implements the cell mutation
primitive needed by one.

Also introduced `RobddPaving::subpaving(GridCell const&)`. It
validates the ancestor/descendant geometry with
`relative_cell_path`, then repeatedly uses the canonical Boolean
cofactor/shift logic of `branch(Bool)` to return a detached
value-like slice at an arbitrary descendant root, including
across primary-cell extents. Each slice retains the manager but
owns its own root metadata; mutating it does not alter the source.
The existing single-bit `branch(Bool)` behavior is unchanged.

The new regression tests apply the generic cell-update primitive
through both `PavingInterface&` (on the tree backend) and
`RobddPaving&`, checking equality of enabled membership and
normalized leaf counts. They reject off-root and incompatible-grid
updates without mutating the paving. Another regression tests
subpaving extraction on all 256 three-bit truth tables and
multiple descendant root paths, independently reconstructs an
expected paving from the cells contained by each descendant,
compares inclusion and compact-leaf counts, and checks
detachment, invalid roots and cross-extent extraction.

The remaining work for `PavingHandle` is a well-defined
mutable subpaving view and full `PavingInterface` approximation
support, not another class name alias. No changes were made to
`bdd`, `drawing` or `reasoning`. CI validation is pending.

## 2026-11-06 - Backend-neutral non-owning writable cell views

Introduced `PavingCellView<Paving>` and the deduction helper
`paving_cell_view(paving,root)` in the existing
`paving/paving_cell_operations.hpp`. This is a lightweight,
non-owning wrapper around a mutable paving and a `GridCell`
contained in its geometric root, using the already shared
`set_paving_cell` operation for mutation.

The API is deliberately small: `root_cell`, `grid`,
`dimension`, `superset(GridCell)`,
`set_root_cell(Bool)` and `branch(Bool)`. A child view
holds the same paving pointer but a child root cell, enabling
nested modifications of precisely the selected region.
The view checks grids and geometric containment. Membership
queries for cells outside the view return false, even when
the owner represents them.

View copies retain the same owner pointer; they are aliases,
not independently owned values. The paving must outlive its
views. In particular, this class does **not** derive from
`SubPavingInterface`, is not a `SubPavingHandle`, and does
not attempt a `clone()` implementation. This distinction
is essential because the existing `SubPavingHandle` copy
constructor delegates to `clone()`, whose current binary-tree
implementation returns an independently owned paving. The
complete polymorphic contract must be addressed separately,
including live iteration, geometric queries, logical mincing,
and lifetime/ownership semantics of views.

The regression exercises the same `PavingCellView` through a
`PavingInterface&` referring to `BinaryTreePaving` and
directly through `RobddPaving&`. It tests nested branches,
root-cell enabling/disabling, parent mutation, alias copies,
out-of-view membership, independent sibling regions, invalid
grid/root requests, and unchanged behavior of both backends.
No changes were made to `bdd`, `drawing` or `reasoning`.
CI validation is required before treating this as verified.

## 2026-11-06 - Read-only traversal of generic writable cell views

Extended the existing `PavingCellView<Paving>` with
`for_each_enabled_leaf(visitor)` and `size()`, using only the
common constant `begin()/end()` iterator and `GridCell` geometry.
For every enabled owner leaf contained in the view, traversal emits
that leaf; when an enabled owner leaf contains the entire view,
traversal emits exactly the view's root cell. This geometric
clipping is required for a subpaving view of an unsplit enabled
ancestor. It does not construct or clone a new paving, and
results reflect the owner's current value at each traversal.
The owner must not be mutated during traversal.

The new exhaustive reference test constructs both backends
independently from all 256 three-bit truth tables, normalizes
the binary tree, and compares the clipped enabled-cell sequences
and counts over eight view root paths. A separate full-terminal
case verifies that an unsplit enabled ancestor is clipped to one
descendant cell and that subsequently removing that cell
changes the visible count in both backends. Neither
`SubPavingInterface` nor `PavingHandle` has changed.
These generic cell views remain non-owning and their copies
remain aliases; they are not polymorphic cloned subpavings.
CI verification is pending.

## 2026-11-06 - Geometry queries on backend-neutral cell views

Extended `PavingCellView<Paving>` with `bounding_box()`,
`inside(ExactBoxType const&)`, `separated(ExactBoxType const&)`
and `overlaps(ExactBoxType const&)`. All operate on the
enabled cells already clipped to the view's root by
`for_each_enabled_leaf`; they never use the unrestricted
owner's geometry and do not materialize a new paving.
The result therefore reflects subsequent owner mutations.

As in `BinaryTreeSubpaving`, inclusion uses the conservatively
narrowed box, separation the widened box and interior overlap
the narrowed box; mismatched query dimensions are rejected.
The bounding box computes the hull of clipped enabled leaves;
an empty region has a dimension-preserving empty box.

The new regression independently constructs
`BinaryTreePaving` and `RobddPaving` from all 256
three-bit truth tables, compares the four view-geometry
operations for eight descendant-root paths and six box
queries, and checks full-terminal clipping, live updates
and dimension errors. No virtual `SubPavingInterface`
adapter or ownership semantics were changed. The next
interface step still requires an explicit independent
`clone()` contract and the remaining approximation API.

No changes were made to `bdd`, `drawing` or
`reasoning`. CI validation is pending.

## 2026-11-06 - Standard constant iterators on generic cell views

Extended `PavingCellView<Paving>` with the conventional
`ConstIterator`, `begin()` and `end()` interface.
The iterators use the existing `ForwardConstantIteratorHandle<GridCell>`
and a concrete `PavingCellViewIterator` implementing the associated
`ForwardConstantIteratorInterface<GridCell>`. On `begin()`,
the view enumerates its clipped enabled cells into an immutable
shared snapshot. This makes iterator copying safe and keeps saved
iterators valid across later mutations of the owning paving.
A fresh `begin()` observes the new owner state. End iterators
compare using the identity of the owner and view root, not the
identity of a separately allocated snapshot; independently
constructed `begin()` and `end()` therefore interoperate.

The test builds the binary-tree and ROBDD backends independently
for every three-bit Boolean function, traverses eight clipped
view regions through the generic iterators and compares cells,
postincrement snapshots and end behavior. It also checks
iterator copies, independently created past-the-end iterators,
and retained snapshots after modifying the owner. The views
remain non-owning, and this change does not add an incomplete
`SubPavingInterface` implementation. `PavingHandle`, `bdd`,
`drawing` and `reasoning` are unchanged. CI verification
is pending.

## 2026-11-06 - Direct ROBDD outer approximation of geometric boxes

Implemented `RobddPaving::adjoin_outer_approximation(UpperBoxType const&,Nat)`
as a first native geometric approximation operation. It reproduces the
primary-cell alignment and target subdivision depth of the explicit
`BinaryTreePaving` backend, and visits cells recursively. Cells proved
separated from the input box are skipped; already-enabled cells are
skipped; cells provably covered by the query set or at the target
depth are staged for insertion. Selected cells are then adjoined to
the canonical ROBDD via existing `adjoin(GridCell)`; no explicit
`BinaryTreeNode` is constructed. Dimension and depth overflow
checks reject invalid inputs before mutating the set.

A cross-backend regression constructs the same outer box approximations
independently in `BinaryTreePaving` and `RobddPaving`, comparing
normalized enabled leaf counts, two-way geometric inclusion of leaves,
and bounding boxes across one- and two-dimensional grids and
multiple fineness levels. It checks idempotence and wrong dimensions.

This does not yet implement the other three abstract geometric
approximation overloads. In particular, `RobddPaving` is not yet
derived from `PavingInterface`. No `RobddSubpaving` was introduced.
`PavingHandle`, `bdd`, `drawing` and `reasoning` are unchanged.
CI must validate this increment.

## 2026-11-06 - Generic compact outer and open/set inner approximations

Completed the remaining geometric-approximation method signatures
needed by `PavingInterface` in the experimental `RobddPaving`:

- `adjoin_outer_approximation(EffectiveEuclideanCompactSetInterface
  const&,Nat)` uses the same conservative separation test and
  optional open-set coverage proof as the explicit-tree backend.
  The existing `UpperBoxType` overload now delegates through
  an `ExactBoxSetType` so that both use identical geometry semantics.
- `adjoin_inner_approximation(EffectiveEuclideanOpenSetInterface
  const&,Nat extent,Nat fineness)` visits subdivisions of the
  requested primary cell, only adjoining regions that are
  `definitely(set.covers(cell.box()))`. As in the tree backend,
  optional closed-set separation prunes disjoint regions; cells
  that remain uncertain at the target depth are excluded.
- `adjoin_inner_approximation(EffectiveEuclideanSetInterface
  const&,Nat)` selects the primary-cell extent from the exact
  bounding box and delegates to the open-set operation.

All three implementations stage geometric cells and then use native
ROBDD insertions, without converting a tree or a subpaving. They
validate dimensions and protect subdivision-depth arithmetic
against overflow. Previously enabled regions are skipped.
The implementation is intended to preserve the observable
geometric result, not internal binary-tree refinement.

The cross-backend regression independently executes each new
overload using geometric box-sets as concrete effective sets,
in one and two dimensions, three explicit primary extents,
multiple fineness levels and multiple query boxes. It checks
two-way geometric cell coverage, normalized leaf counts,
idempotence and wrong-dimension rejection. This checks the
overloads' behavior for box-sets; richer compact/open set
implementations require future dedicated tests.

`RobddPaving` still does **not** derive from `PavingInterface`.
The remaining blocker is the correct generic mutable subpaving
view adapter, including clone semantics and subregion-local
`mince`/`recombine` effects. No changes were made to
`PavingHandle`, `bdd`, `drawing` or `reasoning`.
CI verification is pending.

## 2026-11-06 - Polymorphic ROBDD paving and generic live branch adapter

`RobddPaving` now derives from `PavingInterface` and implements its
remaining virtual hooks: `clone()`, `_branch()`, `_begin()`,
`_end()` and `_write()`. Its `root_cell()` returns `GridCell`
by value as required by the abstract interface. Its existing
native set operations, geometric queries, mincing and
approximation signatures are marked `override`; no alternate
paving type or rename of `PavingHandle` was introduced.

The shared `paving_cell_operations.hpp` provides a
`PavingCellViewAdapter<Paving>` implementing `SubPavingInterface`
for a backend with detached `subpaving(GridCell)` snapshots.
The adapter borrows the mutable owner, while `_branch()`
returns another borrowing adapter for the selected child.
`set_root_cell()` updates only the designated subcell through
the already-tested geometric cell mutation primitive.

The adapter's `clone()` returns a newly owned value of the
underlying paving's `subpaving(root_cell)`. Consequently,
copying a `SubPavingHandle` or constructing `PavingHandle`
from a branch yields an independent paving; modifying that
clone does not affect the original. No new `RobddSubpaving`
backend-specific class is needed. Geometric queries and
snapshot-safe iterators reuse `PavingCellView`. For a
canonical ROBDD, `mince()` and `recombine()` on a view
are geometry-preserving no-ops, intentionally avoiding
modifications to sibling regions or to the owner's global
logical depth. This is *geometric* rather than physical
leaf-count equivalence: the explicit tree can expand its
storage during local mincing, whereas a canonical ROBDD
retains the reduced Boolean DAG.

Cross-backend paving set operations are intentionally
**not** permitted. The temporary generic fallback added to
`BinaryTreeSubpaving::equals`/`subset`/`intersects` was
reverted: those methods retain the same-backend
`BinaryTreeSubpaving` dynamic cast. The ROBDD virtual
`adjoin`, `restrict`, `remove`, `subset`, `intersects`
and `equals` now accept only `RobddPaving` operands or
`PavingCellViewAdapter<RobddPaving>` child views. Different
backends are rejected before mutation, rather than implicitly
converted. The same-ROBDD-manager compatibility restriction
also remains in force.

Geometric cross-backend validation belongs only in external
test/oracle code, where each backend is constructed and queried
independently. The existing explicit constructors from
`BinaryTreeSubpaving` or `SubPavingInterface` remain available
as **opt-in migration/conversion**, not as a mechanism for
automatic mixed-backend set operations.

The integration regression constructs both backends through
`PavingHandle`, compares independent geometric observations through
`PavingInterface`, tests cross-backend set-operation rejection,
checks cloned handle independence and
mutable nested child views, preserves sibling geometry
through local refinement/recombination, exercises virtual
iteration and verifies constructing an independent
`PavingHandle` from a `SubPavingInterface`. The
`RobddManager` must outlive all ROBDD pavings and
iterators; a live view's owner must outlive the view.
No updates to `bdd`, `drawing` or `reasoning`.
This increment requires CI verification.

## 2026-11-06 - Corrected backend isolation for set operations

Architectural constraint: `BinaryTreePaving` and `RobddPaving`
are alternate implementations of the same interface, **not**
interoperable paving set operands. Cross-backend comparison of
observable outputs in tests is valid, as are external operations
and explicit conversion for migration. Mixed-backend
`adjoin`, `restrict`, `remove`, `subset`,
`equals` and `intersects` are prohibited. The correction
removes the temporarily introduced tree-side generic fallback,
rejects foreign inputs in ROBDD virtual overrides, and rewrites
the tests to verify both rejection and same-backend virtual
operations, including live child views. No change to
`PavingHandle` or the BDD manager is needed.

## 2026-11-06 - Sequence-level equivalence via isolated polymorphic handles

Added `test_polymorphic_sequences_backend_isolation`, an exhaustive
integration regression that exercises each backend through a separate
`PavingHandle` / `PavingInterface` reference without ever using a
different-backend paving as a set operand. For all 256 Boolean truth
tables over three subdivision bits, in one and two dimensions, the
test runs the same sequence independently on both backends:

1. construct and adjoin initial enabled cells;
2. clone both handles to preserve an immutable baseline;
3. restrict against independently constructed same-backend masks;
4. remove an independently constructed same-backend mask;
5. adjoin that same-backend mask again;
6. create live child views, clone their values independently, and
   modify the original child region without touching siblings;
7. destroy borrowed child handles before recombining tree nodes.

After each stage the oracle compares only external observations:
root grid and cell, number of normalized enabled leaves, full
bounding box (when nonempty), membership of all eight depth-three
cells and geometric `inside`, `separated`, `overlaps` queries.
This intentionally does *not* call `subset`, `intersects`,
`equals`, `adjoin`, `restrict` or `remove` with operands
from different backends. Same-backend subset predicates are tested
separately. Cloned handles are checked against each other as
independent geometric outputs after the owners have been mutated.

The existing negative regression covers forbidden mixed-backend
operations. This sequence-level test complements, rather than
replaces, the individual geometric truth-table checks and
the polymorphic clone and branch tests. No production or public
interface code changed in this increment. The comparison benchmark
will be introduced only after functional equivalence is validated
by the full CI matrix. CI validation of this test is pending.

## 2026-11-06 - Separate backend-neutral tests and rename the tree suite

Reorganized the C++ regression suites without changing the behavioral
assertions of existing test cases:

- `tests/test_binary_tree_paving.cpp` was renamed to
  `tests/test_binary_tree_paving.cpp`. The fixture class is now
  `TestBinaryTreePaving` and the test suite is registered as
  `test_binary_tree_paving`; method names and their assertions stay
  unchanged. No alias for the retired test target is introduced.
- `tests/test_paving_backends.cpp` is a new standalone regression
  executable with fixture `TestPavingBackends`. Seventeen methods
  previously registered in `test_robdd_paving.cpp` were moved here,
  preserving their bodies and relative registration order. They cover
  tree-oracle migration, cross-backend geometric comparisons and
  sequence-level equivalence using independent backends.
- `tests/test_robdd_paving.cpp` retains five ROBDD-specific tests:
  `test_terminals_and_complemented_roots`,
  `test_set_operations_and_preconditions`,
  `test_distinct_roots_and_extents`,
  `test_compact_leaf_iteration`, and
  `test_detached_subpaving_for_descendants`.

`tests/CMakeLists.txt` replaces the old `test_grid_paving` executable
with `test_binary_tree_paving` and adds `test_paving_backends` to
`UNIT_TESTS`. Both are registered through the same `CTest` loop,
including Windows path setup and the existing `paving` test label.
All 22 original ROBDD test methods are accounted for (17 moved, five
retained), and no comparison logic was weakened or converted into
mixed-backend set operations.

This change is solely test organization. The production
`BinaryTreePaving` class and its public API retain their existing
names, as does the independent `test_grid_paving_extensions` suite.
No changes to the `bdd`, `drawing` or `reasoning` repositories.
The rename and test relocation require CI validation.

## 2026-11-06 - Polymorphic equivalence across nonzero primary extents

Extended `test_paving_backends.cpp` with
`test_polymorphic_alignment_across_primary_extents`. This is a
focused cross-backend regression covering one, two and three
dimensions at primary extents 1 and 2. Both implementations are
constructed independently with the same geometric root through
`PavingHandle`. The test enables cells at both the owner's
primary extent and zero extent, so the ROBDD must align
subdivision variables and the binary tree must align its
nodes across primary-cell boundaries.

After each mutation sequence, an external observer compares
geometric properties: normalized enabled-leaf counts,
bounding boxes, mutual coverage of each backend's enabled
cells by the other (using only `superset(GridCell)`),
and membership of eight probe subcells at two primary
extents. No mixed-backend paving set operation is used.

The test also checks that live `SubPavingHandle` child and
grandchild views propagate local changes to the original
owners, that copied child handles retain independent
snapshots, that sibling regions remain enabled, and that
copies of the original `PavingHandle` remain unchanged.
As required by the binary-tree ownership rules, borrowed
subpaving handles are destroyed before recombining the tree.

This supplements the exhaustive extent-zero truth-table
regressions with a different geometric alignment path;
it is not a performance benchmark and does not change
any production API. The full CI matrix must validate it.

## 2026-11-06 - Fix crash when branching a uniform tree leaf

The new `test_polymorphic_alignment_across_primary_extents` uncovered
a real `BinaryTreeSubpaving` bug: after a uniform enabled region is
recombined to a leaf, the tree's virtual `_branch()` unconditionally
returned `child_node(side)`, which is null for any leaf.
`SubPavingHandle` consequently exposed a live view with a null
`BinaryTreeNode*`; mutating its root dereferenced the null pointer.
The reported crash followed the check
`robdd_snapshot.superset(left)`, immediately before taking and
mutating the nested child views. The ROBDD snapshot was not the
origin of this failure.

Fixed both `BinaryTreeSubpaving::_branch(Bool)` and the concrete
`BinaryTreeSubpaving::branch(Bool)`: each splits the current node
when necessary before constructing the borrowed child view.
`BinaryTreeNode::split()` copies a uniform leaf's enabled/disabled
state into its two children, so this changes only the *tree's
representation*, never its geometric set. This preserves the
intended mutable branch-view contract even when the owning tree is
fully enabled or fully disabled and avoids changing ROBDD logic.
The header documents this representation-only split and the
borrowed-pointer lifetime constraint.

Added `test_branch_from_uniform_leaf` to the backend-local
`test_binary_tree_paving.cpp`, covering virtual and concrete
branch creation on both enabled and disabled uniform roots,
nested mutable branches, sibling preservation and recombination.
The existing `test_paving_backends.cpp` cross-backend scenario
remains intact, with additional nested-view root and membership
assertions. Grandchild handles are now explicitly destroyed before
their ancestor is cleared: clearing an ancestor can delete tree
nodes borrowed by the descendants, which must not subsequently
be accessed.

This is a genuine production tree bug exposed by the new test,
not a wrong assertion or an allowable backend-representation
difference. No test scenario was removed or weakened. The
fix preserves the agreed same-backend restriction for all
paving set operations. CI validation is required before
declaring the regression repaired.

## 2026-11-06 - Polymorphic pure set-algebra regression

Added `test_polymorphic_pure_algebra_with_isolated_backends` to
`tests/test_paving_backends.cpp`. It exercises the four free
operations provided by `paving_interface.hpp`:
`join`, `geometric_union`, `intersection` and
`difference`. Each operation is executed independently
on `PavingHandle` operands of one backend, with an
identically constructed pair of the other backend acting
only as an external geometric oracle. The test checks
all eight depth-three cell membership decisions against
Boolean truth-table algebra for selected empty, full,
regular and irregular patterns in one and two dimensions.

Unlike the existing tests of in-place set operations, this
checks that free functions create independently owned
results: mutating a returned paving does not change either
original operand or another returned result. It also covers
cloning a live child `SubPavingInterface` as the first
operand of a pure operation, including a uniform enabled
tree root (the scenario that exposed the previous null
child-node bug). Same-backend set operands include views;
foreign backends are explicitly rejected for all pure
operations, including a foreign operand paired with a
same-backend child view.

This test does not change the interface or allow
cross-backend `subset`/`equals`/`intersects` or mixed
set operations. It uses only independent geometric
queries to compare the two outcomes. CI is required to
validate the new scenarios. The `mince`/`recombine`
representation-versus-interface contract remains separate
from this regression.

## 2026-11-06 - Representation-neutral polymorphic refinement contract

SubPavingInterface represents a geometric set, not an explicit tree
decomposition. The legacy mince(fineness) and recombine() operations
preserve geometry but may change backend-specific representation state.
size() counts enabled leaves returned by the backend's current iteration;
it is not a geometric invariant. Trees can retain artificially split
enabled leaves, whereas canonical ROBDDs compact these away. A live child
view may only modify representation in that region; a canonical ROBDD
view has no local depth metadata, so its mince/recombine are no-ops.
Fineness is relative to the zero-level grid cell.

Virtual equals(SubPavingInterface) now performs representation-independent
geometric equality for same-backend operands. The tree implementation
uses two geometric subset checks instead of structural operator==.
The concrete BinaryTreeSubpaving::operator== deliberately stays structural.
Cross-backend paving operands remain forbidden.

The new polymorphic refinement regression covers full 1D and 2D roots,
explicit tree mincing versus ROBDD logical mincing, differing leaf counts,
same-backend geometric equality without recombination, and the
preservation of sibling coverage after local view mince/recombine.
It also checks comparisons against independent same-backend pavings
with differing root cells and destroys borrowed tree views before
owner-wide recombination. No assertions are removed or weakened.

Keep the legacy virtual operations available for source compatibility;
adding a compiler deprecation attribute could fail warning-as-error
builds. Portable fixed-fineness enumeration must eventually be a
separate, explicit, lazily evaluated API, rather than a side effect of
mince(). Audit existing callers before introducing or migrating that API.
No BDD manager or concrete tree mincing changes are made here.
CI validation of this increment is required.


## 2026-11-06 - Non-mutating grid-relative fineness enumeration

Introduced for_each_enabled_cell_at_fineness(SubPavingInterface const&,
Nat, Visitor&&) in paving_cell_operations.hpp. This free function
streams enabled cells using current backend iteration as input, then
expands any enabled leaf with word length below
(root_extent + fineness) * dimension, respecting the common grid-relative
fineness convention. Existing deeper leaves remain unchanged (this is a
minimum, not a demand to coarsen). Split order is left before right.
It keeps only the pending depth-first search frontier, rather than
building a new paving or changing the ROBDD root. Polymorphic child-view
source iterators may independently snapshot their compact leaf sequence.
Depth/index overflow is checked before iteration; consumers must not
mutate the source while visiting cells.

Added two regression fixtures in test_paving_backends.cpp. The first
compares independently constructed tree and ROBDD pavings to an
explicitly minced tree oracle across 1, 2 and 3 dimensions, two primary
extents, multiple masks and fineness levels, checking exact visit order,
coverage, and no mutation of either owner. The second covers nested
live SubPavingInterface views of an enabled root at two extents; it
tests root-relative clipping, preservation of already finer leaves,
and sibling geometry. The source tree must not be recombined while its
borrowed child views are alive.

Audited repository consumers:
- Legacy tree, paving and geometric approximation tests intentionally
  exercise physical mincing, and continue to use the concrete APIs.
- Calls to recombine() in approximation code, C++ and Python tutorials,
  and tree projection compact a concrete representation; they do not
  request enumeration and are unchanged.
- outer_skew_product() previously const_cast a const input BinaryTreePaving
  merely to call mince() before iteration. It now uses the new streaming
  enumerator for both inputs, removing the const mutation and avoiding
  transient physical leaf expansion. An existing product regression is
  retained and extended to assert structural input immutability.
- External call sites (including downstream reasoning integration) are
  not rewritten in this increment.

No mixed-backend paving operands are enabled. No changes to BDD manager,
ROBDD canonicalization, Python API, or virtual method signatures.
The CI matrix must validate this change.


## 2026-11-06 - Uniform-cell paving is a geometric interface capability

After introducing a minimum-fineness enumerator, clarified the distinct
need to resume continuous evolution from a spatial discretization whose
enabled cells have identical resolution. That is an interface-level
algorithmic capability, rather than an obligation for every backend to
store one physical node per cell.

Added SubPavingInterface::for_each_enabled_cell_at_exact_fineness(f,visitor),
which visits only cells with word length
(root_extent + f) * grid.dimension(). Canonical ROBDDs retain their
reduced DAG and explicit-tree input nodes are untouched. If a tree is
minced more finely than requested, the enumerator can coalesce fully
covered target-level cells using the virtual superset(GridCell) test.
Incompatible partial target cells raise a precondition failure before
visiting anything; no inner/outer rounding is implicit. A nonempty
non-primary view whose root lies deeper than the target cannot be
represented by complete cells at that depth, and is rejected.
Iteration order is left-to-right as in the existing backend iterators.

SubPavingInterface::uniform_cell_paving(f) materializes a detached
PavingHandle carrying an explicit BinaryTreePaving with exactly the same
geometric region and uniformly sized enabled leaves; it can therefore
be consumed by algorithms requiring a regular-cell partition, including
restarting continuous evolution from a fixed spatial discretization.
Its root is the primary cell of the same extent as the input root;
for a child view this can be larger than the view root, although the
enabled geometry is preserved exactly. The materialization can require
exponential storage, in contrast with streaming and a compact ROBDD.
Legacy mince/recombine remain representation-management operations.

Added same-backend isolated tests for exact tree/ROBDD cell sequences,
uniform detached paving leaf counts and shape, coalescing redundant
tree refinements, rejecting geometry-losing coarsening before any
callback, valid exact fine-grid conversion, and non-primary child views.
All tests preserve the source paving.


## 2026-11-06 - Forward exact-fineness cell traversal through handles

The exact-fineness visiting operation added to SubPavingInterface must
also be directly available through PavingHandle and SubPavingHandle:
both already exposed uniform_cell_paving(), but initially required
explicit interface conversion for the visitor. Added matching forwarding
member templates without introducing new virtual methods or changing
backend storage. The full paving and a borrowed child view are now
covered by a dedicated polymorphic handle test for both backends in
1D and 2D, including equal output, correct depth, uniform detached
results and unchanged sibling geometry. These tests do not change
existing geometric assertions or legacy mince/recombine behavior.


## 2026-11-06 - Independent exact-uniformity regression oracle

Added test_exact_uniform_oracle_exhaustive_and_nested_views to the
comparative paving suite. Earlier tests exercised selected positive and
negative examples; this regression computes expected exact-depth cells
from an independent three-bit occupancy truth table. It exhausts all
256 tables for one-dimensional extent-zero roots and covers a diverse
deterministic subset for the other combinations of 1D/2D and primary
extent 0/1. It tests four root regions: the primary root, left child,
nested left-right child, and nested right-left-right child.

For each root and grid-relative fineness 0..3, the oracle independently
checks if every target cell is fully occupied or fully vacant. If
partial occupancy is present, exact traversal and materialization must
throw before emitting any output. A nonempty child view deeper than the
target resolution must also reject coarsening, while an empty child
view is legal and produces an empty result. For valid target depths,
both backends must return exactly the oracle's ordered cells and
materialized leaf counts. The detached results are probed against every
original depth-three occupancy bit, including cells outside child views.
After removing one cell from a detached result, both original sources
must remain unchanged. Every case also checks source tree structure
and ROBDD root/depth stability around enumeration, with the separate
known caveat that the tree's branch() may materialize mutable nodes
before these checks begin. No implementation behavior was changed.


## 2026-11-06 - Native backend benchmark and numeric CLI conventions

The paving benchmark intentionally follows the established
ariadne-cps/numeric/benchmarks CLI and reporting conventions:
ThreadingCommandLineInterface, CommandLineContribution, validated unsigned
named options with optional short aliases, Ariadne logger initialization
via ThreadManager's real thread registry, LOGGING_PRINTLN for progress/results,
one unmeasured warm-up sample, odd measured sample counts, median sample
time per invocation, aligned detailed tables and geometric-mean speed
ratios. The executable is benchmark_paving_backends in benchmarks/.
It is an explicit CMake benchmark target, with only a tiny functional
smoke invocation registered for CI. Normal performance experiments
must run as Release builds on a quiet host, not under instrumented CI.

The initial compact-set families are an axis-aligned rectangle,
its orthogonally rotated counterpart, aligned/rotated ellipsoids with
matching principal radii, and an isotropic Euclidean ball, in d>=2
dimensions. They all use the identical EffectiveConstrainedImageSet
objects for tree and ROBDD approximation; in higher dimensions axes
2...d-1 use a common radius/half-width. Rotated-box bounds use finite
decimal approximations to sqrt(2), so equal volume is approximate
rather than exact; the other rotated quadratic pairs use exact rational
quadratic forms. Domain [0,1]^d contains all five compact families.

CLI options: --dimensions/-d (list), --finenesses/-f (list),
--shapes/-g (list), --iterations/-i, --samples/-n, --max-depth-bits/-m,
--summary-only, plus shared threading/logging flags (--concurrency,
verbosity, theme, scheduler) and --help. ThreadingCommandLineInterface
configures the runtime now but the current BDD kernels still execute
sequentially. Nonzero concurrency is not evidence of parallel BDD speedup. A conservative (d+f)*d bit budget skips impractical
cartesian-product combinations and reports the skip count. Results
include shape, space dimension, fineness, backend, operation, median
milliseconds, compact leaf count and node count, followed by
paired ROBDD/tree time ratios and group geometric means. ROBDD's
node count is explicitly a manager-level total, not per-paving memory.

Initial operations: fresh geometric outer approximation + recombination,
copy, compact enabled-leaf iteration, nonmutating minimum-fineness
cell iteration, a deterministic 16-query superset batch, and copy
followed by in-place adjoin of one grid cell and recombination.
The source geometry is checked for exact equality between independent
backend approximations outside timing, failing rather than timing
incomparable geometric sets. Each case reuses one ROBDD manager for
timed constructions, so reported construction figures represent a
warm-manager regime: manager creation and cold-cache effects are
deliberately NOT measured in this increment. No unsupported claim of
exclusive ROBDD memory usage is made; future cold-manager and resident
memory measurements are separate work.


## 2026-11-06 - Indicative physical node memory for backend benchmark

Added per-scenario node-memory table to benchmark_paving_backends, separate
from per-operation timing rows to avoid duplicating identical memory stats.
The tree uses a physical node count including disabled and internal nodes
times the actual sizeof(BinaryTreeNode). The ROBDD uses *distinct physical
nodes reachable from its root*, not merely manager.node_count(): traversal
is based on the public top_variable/low/high API and interns a canonical
(triple of variable, normalized low child, normalized high child).
Complemented edges are normalized, so f and !f share one physical node.
Zero/one terminals do not consume physical decision nodes. Self-checks
exercise constant nodes, single variables, XOR and complemented roots.
The size estimate mirrors the private RobddManager::Node layout using
{BddVariable,SizeType,SizeType} and sizeof of that mirror; no change to
the bdd dependency is needed.

The report gives Tree N, BDD N reachable, manager-wide allocated nodes,
Tree B~, BDD B~ and Mgr B~ and BDD/tree indicative node-byte ratio. All
counts are captured directly after both source approximations and before
the geometry checker, which may itself allocate nodes. These are lower-
scope node payload estimates only: neither backend's dynamically allocated
container metadata, allocator overhead, vector spare capacity nor the
ROBDD manager's unique and computed tables are included. BDD B~ counts
all nodes reachable from one root even if those nodes could be shared
with other pavings; Mgr B~ captures all allocated canonical manager nodes
including those no longer reachable, and cannot be interpreted as per-root
memory or full manager RSS. These estimates have no memory-ownership
implications and must not be used as replacement for a resident-memory
measurement.

## 2026-11-06 - Exact benchmark shapes, adaptive sampling and warm/cold

A strict ExactDouble precondition exposed the earlier benchmark's
invalid non-dyadic exact literals (0.2_x, 0.35_x, irrational rotation
approximations). Replaced with dyadic exact half-widths 1/4 and 1/8,
and an exact 3-4-5 rational orthogonal rotation of the first two
coordinates. The rotated box uses the scaled constraints 3*x+4*y,
-4*x+3*y with exact dyadic boundaries. Aligned ellipsoids use
16*x^2+64*y^2<=1; rotated ellipsoids use the equivalent rotated
16*u^2+64*v^2<=25, where u=3*x+4*y and v=-4*x+3*y.
For d>2 the rotated quadratic uses 400*(x_i-0.5)^2 so that the
extra axes have the same radius 1/4 as the aligned counterpart.
The centered unit ball uses 16*sum(x_i-0.5)^2<=1.
All variants lie inside [0,1]^d. Orientation changes, not volume or
principal radii.

Benchmark sampling now calibrates independently for each backend and
operation, doubling repetitions until a target --min-sample-ms
elapsed per batch or --max-iterations is reached, with an initial
unmeasured run. An odd number of samples is sorted for the median
milliseconds per invocation; the calibrated iteration count is
reported. Defaults: samples=7, min-sample-ms=12, max-iterations=32768,
iterations=1. Construction remains a complete operation, never an
unexplained subtraction of an empty baseline.

The matrix defaults to d={2,3,4}, fineness={2,3},
max-depth-bits=26, min-leaves=2. A case with fewer than min-leaves
enabled leaves is classified as geometric degeneration and excluded
from timing and speedup aggregates. Explicit --min-leaves 0 preserves
coarse-grid diagnostics, including the trivial fineness=0 smoke test.
Dimensions and fineness are subject to the prior conservative budget.

New outer_cold times complete construction, including a new ROBDD
manager for each invocation. The existing construction benchmark is
renamed outer_warm, explicitly reusing the manager. Both receive the
same prebuilt compact geometry, excluding its creation time. The
memory table reports the fresh-manager allocated node count after
source construction, and a separate manager node count after warm
timings (which can grow because canonical nodes and caches persist).
Neither number is per-root ownership or RSS. The rooted physical DAG
node count and indicative payload bytes continue to be reported
separately. Cold/warm comparisons are not claimed to isolate only
manager startup: they also measure allocator/cache effects, as desired.

## 2026-11-06 - Manager allocation diagnostics, isolated mutations and retention

The BDD dependency was updated to the new main commit exposing
RobddManager::statistics(), an immutable under-lock snapshot with:
physical node vector count/capacity, sizeof internal node, unique table
size and sizeof value_type, ITE computed cache size and sizeof value_type.
The sizes are exact ABI measurements for these C++ container elements;
even their summed payload is only a conservative estimate, not full
memory accounting (std::map tree links, allocators and other allocations
are excluded). No garbage collection or canonicalization behavior was
changed in BDD.

The paving benchmark captures baseline and after-timing statistics for
every geometry, and separately reports the root-reachable DAG node
count. Its detailed manager table includes vector capacity, unique
entries, cache entries and a lower-bound payload byte estimate.
Timing output now uses reachable ROBDD node count rather than manager-
global count in its per-operation Nodes column; the manager totals are
reported only in memory tables.

The copy workload no longer invokes size() on the copied object.
The former mixed copy_adjoin_cell timing was replaced by
adjoin_recombine: independent paving copies are prepared outside
each timed chunk, and only adjoin() and recombine() are measured.
A 256-live-copy limit applies per chunk, not per sample. A sample
accumulates measured chunks until --min-sample-ms or --max-iterations
is reached; all copy/destruction time is excluded. This removes
the formerly misleading 256-iteration calibration limit.

Two separate sequential retention experiments (disable both using
--no-retention) compare SAME and VARIED on 2D boxes. SAME repeatedly
rebuilds the same exact compact input; VARIED translates the box on
a deterministic dyadic 8x8 grid. The managers are independent, with
the same configurable --retention-steps (1..64, default 12) and
--retention-fineness (1..6, default 3). No paving root remains live
when manager statistics are captured. Different exact inputs may
discretize to the same paving at coarse resolutions.
The observation is not a proof of an unintended memory leak:
RobddManager has no root-tracked reclamation/GC by design.
The experiment is not included in operation-time speedup aggregates.

## 2026-11-06 - Two retention trajectories and geometric-work isolation

A same-vs-varied controlled experiment now compares two entirely
independent 2D managers at the same fineness/number of constructions.
One manager rebuilds the same exact dyadic compact box repeatedly;
the other receives distinct exact translated compact boxes on an
8x8 dyadic grid (at most 64 settings). All temporary paving roots
are destroyed before their manager's statistics are recorded. Each
row reports the temporary reachable DAG node count separately from
post-destruction node, vector-capacity, unique-table and ITE-cache
totals. A plateau on repeated input and growth on varied input can
therefore be observed without conflating existing live roots or
different numbers of operations. Distinct geometric shapes may share
the same coarse outer approximation, so the experiment reports
rather than assumes nonidentical discretizations.

A replay_cells benchmark operation collects enabled cells of the
source tree before timing and independently checks that the same
cells reconstruct identical tree and ROBDD geometry. Timed operations
then construct an empty representation, adjoin this exact same cell
sequence, and recombine. This excludes geometric predicate and
constraint-solving cost, whereas outer_warm includes it. Precomputed
input cell materialization and equivalence validation are both
outside the timed region; cases exceeding --max-replay-cells
(default 16384) are explicitly skipped. outer_warm now returns
the paving directly instead of computing size(), bringing its
returned-object timing convention in line with replay_cells.
outer_cold retains a size() sink because its manager cannot outlive
the local construction scope.

Mutation timing now accumulates measured intervals over sequential
chunks of at most 256 prebuilt independent pavings. Copy/allocator
and destructor work remains outside timed intervals. The number of
mutations per sample may grow toward --max-iterations, allowing
submicrosecond operations to approach --min-sample-ms without
allocating thousands of copies simultaneously. No BDD library or
backend semantics were altered by this benchmark iteration.

## 2026-11-06 - Direct ROBDD batch compilation from grid-cell prefixes

RobddPaving::from_cells(manager,root_cell,vector<GridCell>) is a native
construction primitive. Each input is normalized with relative_cell_path,
requiring the same grid and containment within the explicit root cell.
A temporary prefix trie canonicalizes overlapping, duplicate and
ancestor/descendant cells: any fully covered trie node collapses all
descendants. A single post-order walk turns occupied branches into
manager.make_node(BddVariable(level),low,high), and covered/empty
branches into constant(true/false). Reduced DAG nodes and complemented
edges are interned by RobddManager; no explicit BinaryTreePaving is
allocated, and no per-cell singleton union is performed. The returned
logical depth equals the longest normalized input path, including
skipped variables and completely covered subtrees; recombine may
subsequently shrink depth following the existing semantics.

RobddPaving::adjoin_cells_bulk(vector<GridCell>) constructs the common
geometric ancestor of the current root and every input cell, builds
one batch operand, and merges it once into a nonempty paving. A
genuinely empty depth-zero paving adopts the batch root directly,
avoiding a superfluous Boolean union. A previously minced empty paving
does NOT lose its declared depth: it uses the ordinary union path.
All cell grid and relative-path checks occur before the existing
paving root is changed. Empty batches are no-ops.

Both outer/inner approximation methods keep their existing certified
recursive geometry traversal and selected-cell order; only the final
incremental cell-adjoin loop is replaced with adjoin_cells_bulk.
These two methods' logical geometry must remain unchanged. Existing
single-cell adjoin and same-backend algebra are retained.

The regression suite includes an exhaustive 256-mask three-variable
oracle and tests covering duplicated cells, out-of-order ancestors,
different primary-cell extents, root enlargement, invalid grids and
preservation of the original paving on rejected input. The benchmark
retains replay_cells (incremental reference) and adds replay_bulk
using the identical precomputed cell sequence, including geometric
equivalence verification before timing. A separate fresh-manager
table reports both strategies' physical node counts, reachable nodes,
ITE-cache occupancy and indicative payload bytes. This is an
architecture/performance change, not a memory reclaiming collector.

## 2026-11-06 - Read-only ROBDD GridCell superset and empty-root shortcut

Previously RobddPaving::superset(GridCell) built a singleton
RobddPaving and called subset(), which lifts/aligns roots and computes
a Boolean difference through the manager. Although semantically correct,
this creates canonical intermediate nodes and ITE cache entries on a
query intended to be read-only; the benchmark revealed this amplification
during outer approximation's recursive subdivision.

The new query first verifies grid compatibility and immediately returns
false for a false root. It computes common_root_cell(query,pavingRoot)
and checks that the paving's geometric root itself is the common
ancestor: otherwise the queried cell cannot be completely contained.
This detects disjoint queries and those extending outside the root,
while accepting alternate primary extents that describe the same
region. relative_cell_path then gives the query's normalized prefix.
The walker descends through low()/high() for each queried decision
index, leaves the BDD untouched for skipped variables and terminates
early when the cofactor is constant. Complete coverage requires the
remaining function to be exactly the true terminal. Complemented edges
are transparently decoded by the existing manager cofactors. The
entire query uses only public read-only manager accessors, and does
not populate the unique table or the ITE cache.

Outer/inner geometric traversals previously checked superset(cell)
even when the paving was empty, a case that can never certify a
contained cell. The new check_existing flag is computed once before
the traversal and suppresses this pointless check only when the
existing root is empty. The paving is modified only after the selected
cells have been collected, so the flag remains valid throughout
traversal; nonempty paving behavior and all geometric predicates
retain their existing semantics.

Regression tests compare direct containment to an independent
BinaryTreePaving oracle for all 256 three-level occupancy tables and
many ancestor, sibling, deeper and different-primary-extent queries.
They verify no changes to physical manager node count/capacity or
unique/ITE entry counts across each entire batch of direct queries.
A separate complemented-root query check compares with the previous
reference subset implementation, followed by a no-allocation check.
No changes to the BDD manager or its threading contracts are required.

## 2026-11-06 - Polymorphic adjoin(vector<GridCell>) and explicit-import batching

The batch overload is now part of PavingInterface, not only
RobddPaving. The virtual default `adjoin(std::vector<GridCell> const&)` loops
through `adjoin(GridCell const&)`, so the existing binary-tree backend
continues to implement the same semantics without new storage code.
BinaryTreePaving re-exposes the inherited overload using a using
declaration; PavingHandle forwards it virtually. RobddPaving
explicitly overrides the new virtual to invoke its bottom-up trie
compiler, preserving its amortized efficient batch insertion.
The generic design allows clients to request one batch through
PavingInterface independently of the actual backend.

The existing `RobddPaving::adjoin(SubPavingInterface const&)` remains
a direct Boolean union when the operand is already a ROBDD or an
attached ROBDD view. Implicit migration from a tree-backed source is
still forbidden by design. Separately, the explicit conversion
constructor `RobddPaving(manager, SubPavingInterface const&)` now collects
source enabled cells into one vector and bulk-adjoins them, rather
than executing one singleton union per source leaf. The specialized
BinaryTreeSubpaving constructor already encodes its tree directly
and is unchanged.

Regression tests exercise direct and virtual batch dispatch on the
tree, virtual dispatch on the ROBDD, PavingHandle forwarding, and
explicit generic-subpaving conversion. They also check that the
ROBDD's ITE cache stays empty for fresh batch construction and
explicit imports.

## 2026-11-06 - Dedicated superset microbenchmark and allocation-free prefix alignment

The separate executable benchmark_paving_superset constructs a
deterministic, partially occupied ROBDD and the corresponding binary
tree, then generates four query groups before timing: internal enabled
cells, external/disjoint or disabled cells, ancestors containing mixed
enabled/disabled regions, and enabled descendants including different
primary-cell extents. For every query, the tree oracle, the former
common_root_cell/relative_cell_path alignment implementation (retained
only as an executable-local reference), and the current optimized
RobddPaving::superset must produce exactly the same result. The manager
snapshot before and after all timed queries must be identical.

Each backend and category has independently calibrated timing batches,
an odd-sample median and query-normalized nanosecond output, with
--probes-per-class, --dimension, --iterations, --samples,
--min-sample-ms and --max-iterations controls. The CLI uses the
existing threading/logging conventions; CTest executes a short
smoke scenario in every build platform.

The production superset(GridCell) no longer builds a common GridCell
or either temporary concatenated relative BinaryWord. Instead it
compares the two normalized root/query bit sequences directly.
When primary extents agree, no prefix buffer is constructed at all.
When extents differ, exactly the primary-cell alignment prefix is
generated, then referenced together with each cell's existing word.
Any query that is shorter than the root prefix or differs at any
position returns false; for a descendant, the remaining bits select
ROBDD cofactors and the remaining function must be the true terminal.
This preserves root equivalence, mixed extents, skipped variables and
complemented edges, without mutating the BDD manager or any stored root.

The old alignment path is *not* exposed as a production option or a
second set implementation. It appears only as a local timing and
correctness reference in the microbenchmark. Full structural correctness
remains tested by the exhaustive 256-mask tree oracle; its query set
now includes more mixed-primary-extent descendants.

The benchmark is intended to be measured in Release mode on the
same hardware; the resulting relative speedups are not claimed
until measured. It does not compare against an external process or
claim precise heap allocation counts.

## 2026-11-06 - Concurrency policy decision: common safe paving, then ROBDD parallelism

**Status: design decision and implementation roadmap, NOT implemented.**
The public PavingInterface contract must eventually be safe for concurrent
operations on the same paving object for both BinaryTreePaving (BinaryTreePaving)
and RobddPaving. Once this contract passes concurrent stress tests and TSan,
optimize RobddManager for parallel internal BDD operations, using the existing
Ariadne ThreadManager and work stealing. Do not introduce a separate scheduler
or claim that the present manager-level mutex protects paving metadata.

### Common correctness contract

- Mutating paving operations (adjoin(single cell), adjoin(vector of cells),
  same-backend adjoin, remove, restrict, set_root_cell, mince, recombine,
  geometric adjoin approximations) must be linearizable with respect to
  each other and to coherent read operations on the same paving.
- A bulk adjoin is a single atomic change, not a sequence of separately
  visible per-cell updates. Concurrent unions may appear in any serial
  order, but no update may be lost. Noncommuting remove/restrict operations
  must have a well-defined serial order.
- Readers such as superset, size, root_cell, geometry checks and snapshot
  construction must never observe partially updated geometry/root metadata.
- Iterators and read-only snapshots must remain valid while an unrelated
  worker changes the owning paving; especially the tree backend cannot
  expose raw mutable node traversal without an ownership/snapshot policy.
  An iterator pair must refer to one consistent logical traversal state.
- Live child views must retain their documented live semantics; their
  operations have to synchronize with the owner. They must not silently
  become detached snapshots. Lifetime of the owning paving remains a
  separate API contract; the current borrowed-view ownership cannot be
  made safe against owner destruction merely with a mutex.
- Returning internal references, notably RobddPaving::robdd() const&,
  is unsafe under concurrent root replacement. Replace such exposure
  with a safe immutable value/snapshot or an explicitly scoped read view
  before advertising the full contract. Grid references are immutable
  but require documented owner lifetime.
- Multi-paving operations must avoid lock-order deadlocks. When feasible,
  snapshot each source independently, then synchronize the destination
  only for the atomic update; aliasing and self-operations require tests.
- Strong exception safety: failed validation and failed transactions must
  not leave a half-updated paving. Existing value semantics of copied
  pavings must remain intact.

### Backend implementations

PavingInterface specifies the semantic guarantee but should not carry
a universal mutex, since the synchronization/ownership strategies differ.
BinaryTreePaving initially uses synchronized mutable state and detached,
stable iterator snapshots; copying a tree may be costly and should be
measured. RobddPaving initially uses synchronized coherent triples
(root cell, canonical ROBDD handle, logical depth) while manager-owned
decision nodes stay immutable; a later atomic publication strategy can
replace the per-paving mutex if contention justifies it. Avoid holding
a paving write lock during slow independent geometry classification,
nested work scheduling, or waits for workers.

RobddManager's mutexes remain independently necessary for concurrent
shared-manager operations. A paving's mutex cannot guard other pavings
using the same manager, nor public manager operations called directly.

### Verification and sequencing

1. Specify lifetime, iterator/view and concurrent-operation semantics
   precisely, including existing live PavingCellViewAdapter behavior.
2. Implement the common contract on BinaryTreePaving and RobddPaving.
   Do not label PavingInterface thread-safe while some public methods
   still return unguarded references to mutable state.
3. Add the *same* multiworker correctness workload for both backends:
   disjoint/overlapping single-cell and batch adjoins; reader/writer
   snapshots; iterator validity during updates; mixed operations with
   explicit linearizable histories; exception and copy semantics.
   Verify under ThreadSanitizer on all supported configurations and
   check no incorrect results or lost updates.
4. Measure end-to-end update throughput, snapshot costs and lock
   contention with 1, 2, 4 and 8 workers, using realistic geometry
   computations followed by writes to one origin paving.
5. Only then optimize parallel ROBDD manager operations (concurrent
   unique table, computed table and bounded fork/join through
   ThreadManager). Keep the existing correct manager synchronized
   until its replacement is validated under the same stress suite.

Do not confuse thread safety with representation-independent
performance: ROBDD snapshots can share immutable canonical nodes;
tree snapshots currently imply node copying. Parallel work should
accumulate local batches when algorithmically allowed, while
single-cell adjoin must remain independently safe.

## 2026-11-07 - First implementation milestone: serialized concurrent adjoin

Implementation introduced per-paving mutexes for BinaryTreePaving and RobddPaving, with an explicitly limited concurrency contract. Both backends serialize adjoin(single GridCell), adjoin(vector<GridCell>) and native-backend adjoin(SubPavingInterface); the vector overload holds one lock for the whole batch, rather than taking a lock for each member. Backend-internal unlocked primitives avoid repeated locking. Tree-to-tree unions of separately owned pavings lock source and destination together using std::scoped_lock (self-union is a no-op); ROBDD unions snapshot the source under its own mutex before locking the destination, and publish the newly computed root metadata coherently. Copies and assignments of both owning pavings synchronize with concurrent adjoin. The manager's mutex is unaffected.

The common test_paving_concurrent_adjoin runs 8 std::thread workers on the same PavingInterface destination for each backend, mixing singleton cells, vector batches and full-paving sources; the test validates the final union and copy/assignment after the workers join.

**Not a completed full-PavingInterface thread-safety guarantee:** read-only methods, iterators, borrowed views, and non-adjoin mutations such as remove/restrict/mince/recombine remain outside this milestone. In particular, do not invoke those APIs concurrently with adjoin on the same paving; borrowed source views must remain stable. Enforcing the broader public-interface contract requires an additional systematic pass. Independent subpaving computations may run in workers, whose finished results are adjoined to one destination. Parallel internal binary-tree and ROBDD operations remain deferred.

## 2026-11-07 - Coherent basic readers during concurrent adjoin

Second incremental milestone: BinaryTreePaving protects superset(GridCell), size and root_cell with the same mutex used by adjoin. Tree cell superset evaluates through a borrowed base view under the lock (no expensive deep copy and no recursive lock). Private tree alignment and union bypass the synchronized public root_cell accessor under the owning lock. RobddPaving protects superset, root_cell, depth, emptiness, root-handle access and count/membership, and creates compact iterators and enumerations from coherent snapshots. Its robdd() accessor returns a canonical handle *by value*, replacing an unsafe reference to a mutable field. Geometric queries and detached subpaving creation take an initial snapshot; visitors do not hold the origin mutex. The regression suite now overlaps eight concurrent writers with repeated read/count/root/copy probes for both backends.

**Explicitly incomplete:** tree iterators, live child views, cross-paving operations and references returned by grid(), plus remove/restrict/mince/recombine and other non-adjoin mutations, remain outside the supported concurrency contract. No internal BDD or binary-tree parallelism is introduced in this change.

## 2026-11-07 - Separate benchmarks from tests and consolidate tree regressions

The `benchmark_paving_backends_smoke` and `benchmark_paving_superset_smoke` CTest registrations and the benchmark dependencies of the `tests` target have been removed. Both benchmark executables remain available through the dedicated `benchmarks` target for explicit performance runs; neither is part of the unit-test suite.

The eight additional grid/tree paving regression methods from `test_grid_paving_extensions.cpp` have been moved into the `TestBinaryTreePaving` fixture in `test_binary_tree_paving.cpp`, preserving their assertions and CTest coverage under that test executable. The separate source and target have been removed.

The concurrent-read snapshot change accidentally wrote `box.snapshot.dimension()` in the preconditions for ROBDD `inside`, `separated`, and `overlaps`. These now compare `box.dimension()` to `snapshot.dimension()` as intended. This is a build fix, not an expansion of the currently supported concurrent-operation contract.


## 2026-11-07 - Concurrent root mutations, bounded increment

The owning BinaryTreePaving and RobddPaving now synchronize `set_root_cell`,
`mince`, and `recombine` with the same mutex as `adjoin` and basic readers.
ROBDD `mince` changes depth under one lock, avoiding nested acquisition
of the nonrecursive mutex via `mince_to_depth`. Tree paving overrides
invoke their inherited base implementations under the owner lock.
A concurrent regression overlaps these mutations and insertion with
snapshot copies and `size` and `root_cell` readers. This incremental
contract does NOT cover inherited mutable views, iterators, arbitrary
cross-paving operations, or other unsynchronized mutation paths.


## 2026-11-07 - Atomic restrict/remove against concurrent adjoin

The two owning paving backends now serialize `restrict` and `remove` with
concurrent destination mutations. The explicit-tree backend acquires both
owner mutexes through `std::scoped_lock` when the source is another owning
`BinaryTreePaving`, with self-intersection and self-difference handled
separately. The helpers use qualified base accessors while the owner is
already locked, preventing recursive mutex acquisition. Removing an
individual tree cell and restricting by root extent also acquire the
owner mutex. Borrowed `BinaryTreeSubpaving` source views still require
external stability; they are not made safe against invalidation.

The ROBDD backend copies a stable source before acquiring the destination
mutex, then computes the entire intersection/difference and publishes its
root, depth and geometric metadata under that single destination lock.
This follows the existing source-snapshot policy of `adjoin`: the source
snapshot and destination publication are not one globally atomic operation
across independently changing owners. Both backend paths handle self-
intersection (identity) and self-difference (empty result) explicitly,
including calls through `SubPavingInterface`.

Two stress cases mix polymorphic remove/restrict, concurrent insertion and
snapshot reads; they also check both self-operations. This phase does not
claim thread-safety for non-owning live views, iterators, inherited
`BinaryTreeSubpaving` APIs, or all geometric approximation operations.


## 2026-11-07 - Eliminate nested source-owner locking in set algebra

The first owner-locking implementation of `BinaryTreePaving::restrict` and
`remove` acquired both owner mutexes through `std::scoped_lock`, then
called `BinaryTreePaving::root_cell()` on the source indirectly via virtual
dispatch inside `restrict_to_lower()` / `remove_from_lower()`. The
non-recursive source mutex was therefore acquired twice by the same thread,
deadlocking even a single-threaded set operation. All four calls now
explicitly target `BinaryTreeSubpaving::root_cell()`, which accesses the
already-protected geometry without re-entering the owner's mutex.

A separate single-threaded regression executable verifies native and
polymorphic restrict/remove with two distinct owners, as well as self-set
operations. CTest limits that test to 20 seconds and the pre-existing
concurrent-paving stress suite to 60 seconds to fail promptly on future
lock-order regressions instead of indefinitely blocking CI.


## 2026-11-07 - Owning tree snapshot iterators

The owning `BinaryTreePaving` now returns deep-copy snapshot iterators via
both concrete `begin()/end()` and the polymorphic `PavingInterface`
iterator API. The copy is made under the owner's existing mutex, and
`BinaryTreePavingConstIterator` retains the copied `BinaryTreePaving` through
`shared_ptr`. The stack of node pointers therefore remains valid after
concurrent owner mutations and even after the owner is destroyed.
Copies of iterators share the same snapshot. Two fresh `begin()`
iterators from the same source compare their logical `GridCell` positions,
not allocation addresses; comparing historical snapshots from different
versions is not a supported ordering relation. An `end()` iterator is
stateless.

`BinaryTreeSubpaving` still represents a borrowed mutable view. Its
`BinaryTreePavingCursor` and direct `begin/end` do not keep an owner alive;
callers must stabilize the owner through any traversal and must not
retain these views across subtree mutations. Owner `_branch()` now
serializes initial splitting with other owner operations, but the
returned live branch remains borrowed and is not safe to retain across
concurrent mutations. Use a deep copy of the owning paving for
independent traversal.


## 2026-11-07 - Geometric reads of mutable owning tree pavings

`BinaryTreePaving` now captures an independent, owner-locked snapshot for
`bounding_box`, `inside`, `separated`, `overlaps`, `measure` and
polymorphic `equals`, `subset` and `intersects` calls. The snapshot
invokes the qualified `BinaryTreeSubpaving` implementation to avoid a
recursive lock when the base algorithm reads root-cell geometry or
traverses the binary tree. `is_empty` and `dimension` instead use the
owner mutex directly. If the right operand is another owning tree paving,
it is also copied; each copy is coherent individually, but the two
copies are not an atomic cross-owner transaction. Borrowed subpaving
views must remain stable during comparisons.

Concurrent geometric-read regressions overlap these APIs, including
polymorphic calls, with insertion and restriction. Neither the
reference-returning `grid()` accessor nor raw binary-tree pointers or
borrowed `BinaryTreeSubpaving` views gain lifetime guarantees from this
change. Those remain outside the concurrent contract and must not race
with mutations or owner assignment.


## 2026-11-07 - Fix reentrant emptiness query after geometric snapshot rollout

The first owner-side `BinaryTreePaving::is_empty()` implementation acquired
`_adjoin_mutex` and then called `BinaryTreeSubpaving::is_empty()`. The
base implementation tests `this->size()`, whose virtual dispatch reaches
`BinaryTreePaving::size()` and attempts to acquire the same non-recursive
mutex again. This caused `std::system_error` (resource deadlock would
occur) on MSVC and hung CTest on some other platforms. The owning query
now calls qualified `BinaryTreeSubpaving::size()` directly while holding
the mutex, avoiding the second lock acquisition without weakening the
snapshot and data-race guarantees.

The `test_paving_lock_reentrancy` executable now tests owning and base
emptiness queries on empty, populated and cleared trees, plus snapshot
bounding boxes. Its 20-second CTest timeout provides an explicit regression
signal if a recursive lock is reintroduced.


## 2026-11-07 - Fix reentrant dimension access during extent restriction

`BinaryTreePaving::restrict_to_extent()` acquired the owner's
non-recursive mutex, then called `this->dimension()`. After the
geometric-reader refactoring, that virtual owner method also acquired
`_adjoin_mutex`, triggering a recursive-lock `std::system_error` on
Windows and blocking some other platforms. The extent-restriction
algorithm now calls qualified `BinaryTreeSubpaving::dimension()` under the
existing owner lock, and the owning `dimension()` method reads its
protected geometry directly rather than calling any virtual accessor.
A dedicated, time-bounded sequential regression exercises both the no-op
and mutating restriction paths in one, two, and four dimensions.
The previous `is_empty()` correction remains unchanged.


## 2026-11-07 - Scoped mutable subpaving access

The owning `BinaryTreePaving` now exposes `with_locked_subpaving(callback)`
and `with_locked_branch(right,callback)` for operations on live borrowed
subtrees that must be synchronized with concurrent owner updates. Both
helpers acquire the same non-recursive owner mutex used by `adjoin`,
`remove`, `restrict`, `mince`, `recombine` and snapshot copying, and
hold it until the callback returns. A callback receives a
`BinaryTreeSubpaving&` and must return `void`. The root callback creates
a temporary borrowed base view; the branch callback also performs the
necessary semantics-preserving split while holding the lock.

The callback must not call another operation of the owning
`BinaryTreePaving` that attempts to acquire its mutex (non-recursive).
It also must not retain/capture the view, an iterator, or a node pointer
past the scope: although the void return constraint prevents accidentally
returning the view, C++ cannot prevent clients from storing borrowed
references elsewhere. The existing `BinaryTreeSubpaving::branch()` and
`SubPavingInterface::branch()` remain live, non-owning APIs with no
cross-call lifetime guarantee. Changing them to snapshots would silently
break mutability semantics and is deliberately avoided.

Regressions cover scoped mutation of root/child views and a concurrent
owner update overlapping a locked child callback. Snapshot iterators
remain the recommended interface for independent read-only traversal.


## 2026-11-07 - Atomic explicit-tree geometric approximations and clear

An audit of the direct tree mutators identified four public geometric
approximation entry points that traversed and modified owner nodes without
holding `_adjoin_mutex`: the effective/validated compact outer algorithms,
the overt-set lower approximation by extent, and the open-set inner
approximation by extent. Each now acquires the owner mutex for its entire
direct mutation, including tree alignment and recursive updates. Delegating
box/located/regular overloads retain their existing call graph and enter
one of these guarded terminal algorithms, avoiding nested acquisition of
the non-recursive mutex. Owner `root_cell()` access inside the four locked
functions is now explicitly qualified to the base `BinaryTreeSubpaving`
accessor, avoiding the previous virtual-dispatch deadlock pattern.

`BinaryTreePaving::clear()` now performs both its root-geometry reset to
extent zero and the disabling of the binary-tree root under a single owner
mutex. Previously, clear constructed a replacement using an unprotected
reference to the owner's grid before a separately synchronized assignment.
The new implementation preserves the existing default-root semantics.

A concurrent stress regression exercises compact outer, overt lower and
open inner approximations alternating with clear while another thread
snapshots and iterates the owner; it also verifies root extent and
membership after clear. A caller must not make an approximation's set
predicate callback re-enter a locking method of the same owner while the
mutex is held. Approximation operations remain potentially expensive and
hold the owner lock for their entire calculation. The existing borrowed
view lifetime restrictions remain unchanged.


## 2026-11-07 - Guard remaining concrete tree refinements and output

The public owning `BinaryTreePaving` now hides and synchronizes the
borrowed-base methods `mince_to_depth`, `mince_to_tree_depth`,
`subdivide` and `tree_depth`. The original owning `mince` remains
synchronized; its qualified base implementation calls a nonvirtual base
helper, so no recursive lock is introduced. The three new mutators hold
the same mutex as `adjoin`, `recombine`, `clear` and snapshot copying,
preventing concurrent changes from invalidating an active tree traversal.

A concrete `BinaryTreePaving::branch` now serializes the split required to
construct a borrowed child view; it does NOT extend the lock to the use
of that view after return. Use `with_locked_branch` for this. The owning
stream insertion operator and the virtual `_write` now print from a
deep snapshot instead of traversing mutable owner nodes without a lock.

A concurrency regression overlaps the direct refinements with the owning
`mince/recombine` operations and with snapshot/stream/depth readers,
then verifies the sequential borrowed-branch mutability behavior.

The `BinaryTreeSubpaving` base class still exposes unsynchronized methods
and raw borrowed node pointers. Explicitly invoking them on an owning
object through a base reference bypasses the above concrete wrappers.
Concurrent callers must not do this, retain mutable views beyond their
protected scope, or dereference binary-tree pointers during a mutation.
The `grid()` reference accessor likewise has no cross-assignment
lifetime guarantee.


## 2026-11-07 - Stable ROBDD value algebra under concurrent owner writes

The public native `RobddPaving::join`, `intersection`, `difference`,
`subset`, and `intersects` methods previously accessed the current
root, manager and depth without synchronizing with owner updates.
These value operations now capture an independent snapshot of each
owning operand before performing Boolean algebra. Self-union and
self-intersection capture one version, while self-difference returns an
empty Boolean root retaining the source snapshot's geometric metadata.
The polymorphic comparisons use compatible backend snapshots, avoiding
unsynchronized `grid()` reference reads before validation; self-equality
and self-subset are identities. Cross-owner snapshots are individually
coherent but are not captured as a single global transaction.

The existing mutation paths already hold the destination's
non-recursive owner mutex; they now invoke explicitly named private
`_join_unlocked`, `_intersection_unlocked`, and `_difference_unlocked`
implementations rather than entering the public snapshotting wrappers.
This is required to avoid deadlocks and preserves the existing atomic
publication of root, depth and geometry. The manager continues to
synchronize the canonical node operations independently.

A threaded regression alternates a persistent left cell with an optional
right cell while value algebra and polymorphic/self-comparisons run,
asserting semantic properties valid for either snapshot. Borrowed view
lifetimes and self-mutating predicate callbacks remain outside this
concurrency contract.


## 2026-11-07 - Manager and geometry safety during ROBDD assignment

`RobddPaving::adjoin(GridCell)` and `remove(GridCell)` previously read
`_manager` without synchronizing with copy assignment, which can change
that pointer alongside the root. Both methods now hold the owner mutex
while constructing the singleton and applying the corresponding private
unlocked union/difference. Manager selection and destination publication
therefore happen in one critical section, without reentrant locks.

The previously inline `dimension()` is now locked. The `mince` and
`superset` paths already hold that same mutex, so their dimension queries
are explicitly based on protected `_root_cell.grid()` rather than
recursively acquiring the owner lock. Geometric enclosing-cell and
approximation entry points now capture a copied root/grid geometry before
using it. Candidate enumeration remains outside the owner lock; the
final adjoin continues to validate grid compatibility atomically.

The virtual `grid() const -> Grid const&` API still returns a borrowed
reference and does not offer cross-assignment lifetime safety. Callers
requiring an independent geometric value should copy `root_cell().grid()`
from the synchronized `root_cell()` accessor instead.

A new threaded regression alternates assignment from two compatible
pavings managed by distinct live `RobddManager` instances, while another
thread repeatedly adjoins/removes a cell and checks the synchronized
dimension and enclosing geometry.


## 2026-11-07 - Enforce lifetime-invariant grid in copy assignment

The grid of an owning paving is immutable after construction. Previously,
`BinaryTreePaving::operator=` and `RobddPaving::operator=` could replace it
with an incompatible grid while changing the root. Both operators now
reject cross-grid assignment before changing the destination: tree
assignment checks inside the joint owner lock, while ROBDD assignment
checks the snapshotted source against the destination under the latter's
mutex. Assignments within one grid remain supported and may change root
extent, tree subdivision, or the ROBDD manager and logical depth.

This removes the incorrect assumption that the grid may legitimately
change during a paving's lifetime. It does not guarantee that a reference
returned by `grid()` remains safe to retain while an equivalent-grid
assignment replaces the embedded `GridCell`. Value-returning
`root_cell()` is the safe cross-call geometric snapshot. The regression
covers rejected cross-grid assignment without changing state and accepted
same-grid assignment across different extents and ROBDD managers.


## 2026-11-07 - Coherent projections under immutable-grid assignment

The grid remains fixed for an owning paving's lifetime; nevertheless,
copy assignment can replace the embedded `GridCell`, so a reference
obtained from `grid()` is not protected across concurrent calls. The
concrete tree's `smallest_enclosing_primary_cell` now obtains one
synchronized `root_cell()` value before checking dimensions and locating
the primary cell. It does not allow any grid changes.

`image(BinaryTreePaving, Projection)` now takes a single owner-locked
snapshot at the start, and computes projected geometry and enumerates
enabled cells from that one version. It explicitly traverses the borrowed
base view of the local snapshot to avoid constructing a second deep copy
inside the owning `begin()` method. Similarly,
`outer_skew_product(BinaryTreePaving, ...)` captures its entire input
source once before reading its root extent, depth, grid and enabled cells.
Neither operation holds the original owner's lock during long geometric
computation.

A stress regression overlaps changing root shapes and same-grid copy
assignment with `image` and enclosing-primary-cell queries, checking
that projection is either empty or the correct single projected cell.
This does not protect legacy free operations on directly borrowed
`BinaryTreeSubpaving` views: their owner must remain stable for the entire
operation, or the caller must use an independent owner snapshot.


## 2026-11-07 - Opt-in measurements of tree snapshot iterator costs

The existing `benchmark_paving_backends` reports `copy` and
`iterate_leaves`, but does not isolate the cost of acquiring the
iterator's deep snapshot from the cost of traversing enabled leaves.
The new optional `--tree-iterator-cost` flag adds per-shape/dimension/
fineness diagnostics on the exact same immutable tree:

- creation of an owning `BinaryTreePaving::begin()` iterator, including its
  thread-safe deep snapshot;
- creation of a borrowed `BinaryTreeSubpaving::begin()` iterator, which is
  legal only while the owner remains unmodified;
- full owning traversal, including snapshot construction;
- full borrowed traversal, excluding snapshot construction.

Both traversal variants must yield the same checksum before timing.
Each sample uses the benchmark's existing independently calibrated
median timing routine; results include tree node/leaf counts, four
per-operation milliseconds and safe/borrowed ratios. These extra
measurements run only when explicitly requested and do not enter the
paired tree/ROBDD aggregate speed ratios. There are deliberately no
performance pass/fail thresholds, and no claim that a borrowed iterator
is safe during owner mutation. Actual timing results must be obtained
from a built benchmark on the target platform before choosing a
performance optimization.

## 2026-11-07 - Prebuilt binary-tree cursor profiling (diagnostic only)

Added an opt-in `benchmark_binary_tree_cursor` executable and a small
Ubuntu Release compile/sequence-verification check. Existing production
`BinaryTreePavingCursor` and `BinaryTreePavingConstIterator` remain
unchanged. The benchmark constructs full unrecombined binary trees
outside timing, with selectable depth, dimension and enabled-leaf
density. For every case it checks exact ordered-path equality between
the production borrowed iterator, raw DFS, a diagnostic copy of the
recursive enabled-leaf search without geometry, and the same diagnostic
search with eager `GridCell` reconstruction on each movement.

Reported medians are comparative rather than additive CPU attribution:
the raw DFS has a different control flow, and the diagnostic cursor is
not the production machine code. In particular, compare the two
diagnostic search variants to estimate the overhead of eager geometry.
Before optimizing the cursor, obtain measurements from compiled Release
executables and preserve the owning snapshot semantics and borrowed
view lifetime limitations. There are no performance pass/fail thresholds.


## 2026-11-07 - Lazy GridCell materialization in BinaryTreePavingCursor

The dedicated prebuilt-tree benchmark isolated eager `GridCell` rebuilding
as the dominant cost of traversal (geometry/search ratios 131x..245x on
511..131071-node full trees, 2D and 4D). In the production cursor,
`move()` now changes the binary path and marks the cached geometric box
dirty instead of rebuilding it on every left/right/up step. `cell() const`
reconstructs the complete cell only on first demand after a movement;
the cursor stream operator also goes through this accessor. The existing
enabled-leaf search order and physical tree representation are unchanged.

The cache state is copied with the cursor so copies and assignments from
dirty cursors remain correct. The returned `cell()` reference is valid only
until the next movement, as with references into an advancing iterator.
Independent cursor objects are not shared between threads; concurrent
access to one iterator object requires external synchronization. Owning
iterators retain their existing detached snapshot semantics.

The regression covers dirty copies, assignments, non-primary subviews,
up/down moves, materialized boxes, and matching full cells between borrowed
and owning iterators. Run `benchmark_binary_tree_cursor` again to measure
the actual improvement against the supplied eager-production baseline;
diagnostic Geometry remains deliberately eager as a stable reference.


## 2026-11-07 - Measured lazy-cursor results (d4/f3 aligned-box)

The 2026-11-07 Release benchmark reported by the operator, on the same
aligned-box case (`dimension=4`, `fineness=3`, 416 enabled leaves, 2031
physical tree nodes), compared the eager and lazy production cursor.
These are measured times from two separate executions, not a controlled
multi-run cross-version distribution. Per-run medians used seven samples.

| Workload | Before (ms) | After (ms) | Improvement |
| --- | ---: | ---: | ---: |
| tree `iterate_leaves` | 5.4924 | 0.6592 | 8.33x |
| tree `iterate_min` | 6.8310 | 2.0259 | 3.37x |
| tree borrowed full visit | 5.44010425 | 0.605229156 | 8.99x |
| tree borrowed begin | 0.0549741211 | 0.000416769684 | 131.9x |
| tree owner begin | 0.0991162109 | 0.0439348965 | 2.26x |

The *after* paired backend measurements were `iterate_leaves`:
tree 0.6592 ms vs ROBDD 0.6561 ms, and `iterate_min`:
tree 2.0259 ms vs ROBDD 2.0212 ms. Differences are under 0.5%
in this case and should not be interpreted as a significant advantage
for either backend without repeated cross-run measurements. The tree
still has 2031 physical nodes versus 108 root-reachable nonterminal
ROBDD nodes (estimated node payload 48744 versus 2592 bytes, not
whole-object heap usage). ROBDD copying remains significantly cheaper;
tree `replay_bulk` remains significantly faster (0.0724 versus
0.6974 ms). The outer-approximation pipeline takes about 570 ms
and is not materially improved by the cursor change.

The measured case supports closing the original eager-geometric-cursor
bottleneck as addressed, **not** general performance equivalence of
both backends. Before further cursor changes, repeat the broader
geometric workload matrix and check workload-specific scaling. The
remaining per-emitted-cell geometry cost is a separate prospective
optimization, not an established regression. Owning snapshot,
borrowed-lifetime and iterator equality contracts remain unchanged.

The lazy-cursor commit `bcadd1345ce5eb5991f4da29ddfd66ab34235437`
passed Ubuntu, macOS, Windows, Sanitizers and Coverage workflows.


## 2026-11-07 - Five-shape local 4D/f3 benchmark after lazy cursor

The operator ran the Release `benchmark_paving_backends` matrix locally
with dimensions=4, fineness=3, max-depth-bits=28, seven timing samples,
`--tree-iterator-cost`, no retention and concurrency=0. All five
geometries completed without a budget skip. These measurements are
single-run per-operation medians on the user's machine (hardware details
not recorded); do not infer significance from differences of a few percent.

| Shape | Leaves | Tree physical N | ROBDD reachable N | Tree iterate leaves (ms) | ROBDD iterate leaves (ms) | Tree iterate min (ms) | ROBDD iterate min (ms) |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| aligned-box | 416 | 2031 | 108 | 0.6604 | 0.6528 | 2.0077 | 1.9883 |
| rotated-box | 360 | 1871 | 201 | 0.5785 | 0.5738 | 1.7529 | 1.7192 |
| aligned-ellipsoid | 96 | 879 | 181 | 0.1659 | 0.1635 | 0.4541 | 0.4565 |
| rotated-ellipsoid | 104 | 671 | 136 | 0.1769 | 0.1792 | 0.5730 | 0.5749 |
| ball | 80 | 911 | 156 | 0.1443 | 0.1402 | 0.8493 | 0.8449 |

Geometric means of paired ROBDD/tree operation ratios across the five
cases (ratio >1 means the binary tree is faster): `iterate_leaves`
0.990x; `iterate_min` 0.995x; `outer_warm` 1.004x;
`outer_cold` 0.997x; `replay_bulk` 8.288x;
`superset_16` 0.486x; `adjoin_recombine` 1.014x;
`copy` 0.004x (ROBDD's near-0.0001 ms copies have
limited timer precision). `adjoin_recombine` reverses relative
advantage across shapes: ROBDD wins on the two boxes, whereas
the tree wins on the three non-box sets. The outer construction
time still ranges from roughly 500 to 800 ms and dominates the
full pipeline.

Conclusion for these five cases: the severe eager-geometric cursor
iteration penalty was addressed, and the remaining tree/ROBDD
iteration difference is within about 3%; no backend superiority
can be asserted from a single run at this margin. ROBDD reachable
node counts are 4.9x..18.8x smaller than physical tree counts,
but this is *not* a whole-memory footprint comparison. Retain
the borrowed-vs-owning lifetime and snapshot distinctions.

Future development should be driven by application profiles
and operation-specific bottlenecks (particularly bulk insertion,
queries, copying and memory), not more cursor micro-optimizations
without new evidence. Benchmark experiments belong on the local
machine and results in this design journal. Do not use the official
README as a running experimental notebook or add GitHub Actions
performance workflows for this interactive investigation.


## 2026-11-07 - Opt-in outer-approximation phase profiling

The local five-shape 4D/fineness-3 measurements put the tree and ROBDD
at approximate parity for leaf iteration and for minimum-fineness
enumeration, while both backends still take 0.5-0.8 s for
`outer_warm`/ `outer_cold`. The dedicated `replay_bulk` timings
are roughly three orders of magnitude smaller, suggesting a common
classification cost; this is a strong hypothesis, not a proven
cycle-level attribution.

Added an opt-in `--outer-phases` switch to the existing
`benchmark_paving_backends` executable. The diagnostic enumerates
the same enclosing root and conservative separated/covers decisions
used by the empty ROBDD construction path. It checks the resulting
cell set for semantic equivalence with both production backends
before timing. Three independently calibrated workloads are logged
as `OUTER_PHASES`: complete geometric candidate selection; replay
of exactly the recorded set predicates over precomputed boxes; and
traversal plus GridCell creation of the same recorded branch topology
without predicate evaluation. Counts of visited cells, separation
calls, cover calls, subdivisions, and selected cells are also emitted.

The timing columns **overlap** and must not be summed or subtracted
to claim exact attribution. The diagnostic explores cells through
`GridCell::split` as the ROBDD path does; it does not reproduce
the explicit tree's path/structural update implementation. All
profiling is opt-in and outside CTest, with no production changes
and no new workflows or README modifications.

Local Release invocation (start with one bounded geometric case):

```bash
cmake --build build-release --target benchmark_paving_backends
./build-release/benchmarks/benchmark_paving_backends \
  --dimensions 4 --finenesses 3 --shapes aligned-box \
  --max-depth-bits 28 --no-retention --outer-phases \
  --samples 7 --min-sample-ms 12 --concurrency 0
```

Once this runs correctly, pass the five comma-separated shape names
to obtain the full comparison on the user's machine.


## 2026-11-07 - Measured outer-classification phase cost (4D/f3 aligned-box)

The operator compiled the new opt-in `--outer-phases` diagnostic locally
with Clang and observed `-Wmissing-field-initializers` at the
`OuterSelection result{root}` aggregate. All fields are now explicitly
initialized in this diagnostic (`root, {}, {}, 0u, 0u, 0u, 0u`), with
no production changes.

Local Release measurement, aligned-box/d4/f3, seven median samples:
- Candidate selection: 597.601666 ms.
- Replaying recorded geometric predicates over precomputed boxes:
  592.243875 ms, 99.1% of the candidate-selection elapsed time.
- Replaying the recorded tree of GridCell subdivisions without predicates:
  4.128146 ms (the output rounded between line wraps).
- 2927 visited candidate cells, 2927 calls to `separated`,
  **zero calls to `covers`**, 1463 subdivisions, 864 candidate cells
  selected before simplification.
- The actual recombined geometric paving contains 416 enabled leaves,
  2031 physical binary-tree nodes, or 108 ROBDD decision nodes.

The 864 selected candidates and 416 final leaves are different
representation stages: the benchmark validated semantic equivalence,
not equal leaf counts before and after recombination. The predicate
replay covers the same recorded inputs, but its timing overlaps the
complete selection workload; the ratio is not an exact additive
CPU attribution. A direct consequence is that changes limited to
paving tree traversal/ROBDD reduction cannot greatly accelerate
this workload while the set's `separated()` dominates.

The next optimization investigation should profile the
`EffectiveConstrainedImageSet` separation predicate itself and its
constraint solver on the candidate boxes, without weakening certified
set-separation semantics. Repeat the new `--outer-phases` diagnostic
on the remaining four geometric shapes to see whether this pattern
generalizes; no new workflow or README changes are needed.


## 2026-11-07 - Multi-shape separation predicate profile (d4/f3)

The operator repeated `benchmark_paving_backends --outer-phases`
locally on rotated-box, aligned-ellipsoid, rotated-ellipsoid and ball
in 4D/fineness 3. Combined below with the preceding aligned-box run.
Each reported phase time is a calibrated seven-sample median; the
measurements are comparative and overlapping, NOT additive CPU budgets.

| Shape | Candidate cells visited | Selected candidates | Separated calls | Covers calls | Selection (ms) | Predicate replay (ms) | Geometry-only (ms) | Predicate time / separated call (us) |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| aligned-box | 2927 | 864 | 2927 | 0 | 597.602 | 592.244 | 4.128 | 202.3 |
| rotated-box | 2591 | 720 | 2591 | 0 | 716.180 | 686.692 | 3.634 | 265.0 |
| aligned-ellipsoid | 1127 | 192 | 1127 | 0 | 527.708 | 528.513 | 1.697 | 469.0 |
| rotated-ellipsoid | 1055 | 232 | 1055 | 0 | 810.761 | 810.844 | 1.395 | 768.6 |
| ball | 1467 | 320 | 1467 | 0 | 649.950 | 646.710 | 1.968 | 440.8 |

The predicate replay takes 95.9-100.2% of full selection time.
Ratios slightly above 100% are expected from independent noisy timing
samples; they are not evidence of negative overhead. Geometry-only
path replay costs under 1% of selection in every listed case. Per-call
cost differs by a factor of approximately 3.8, and the rotated
ellipsoid has the highest per-call cost despite the fewest visited
cells. There were no `covers()` calls for any of the five shapes,
so these runs isolate `separated()`.

Code inspection in the connected `ariadne-cps/set` repository shows
`ConstrainedImageSet::separated(const ExactBoxType&,Effort)`
initializes an upper subdomain, joins the image/constraint
functions, builds the codomain and instantiates `ConstraintSolver`,
then calls `solver.reduce(subdomain,function,codomain)`.
The non-effort overload creates a lazy lower-logical expression.
The *relative* time spent in function assembly, solver setup and
`ConstraintSolver::reduce` has NOT been measured; it would be
incorrect to claim all of the 500-800 ms lies inside the solver
algorithm. Next profiling should isolate those subphases in the
`set` module before modifying any algorithm. Certified
separation/conservativeness must remain unchanged.

No paving production algorithm or official README change is
warranted by this result. Performance benchmark runs remain local;
experimental findings belong in this design journal.


## 2026-11-07 - ConstraintSolver phase benchmark: 32 fixed 4D queries

The local Release benchmark in `ariadne-cps/constraint-solving`
(commit `6b4c87d1de0851714a5b22fee978452d36a75f13`) compiled
and passed `--verify-only` on all five geometries before timing.
Its exact 4D formulas agree with the corresponding paving benchmark
(`make_shape`), but it uses **32 fixed dyadic query boxes** per
shape, rather than the adaptively visited boxes in paving. This is
a solver microbenchmark, **not** a direct breakdown of paving's
`separated()` end-to-end wall time.

Measured median milliseconds per 32-query sweep; seven samples,
minimum calibration 12 ms, concurrency 0:

| Shape | `reduce_full` | `hull_vector_fresh` | `hull_scalar_fresh` | `hull_scalar_cached` | `box_sweep_post_hull` | Scalar fresh / cached |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| aligned-box | 3.72976 | 0.23933 | 1.16003 | 0.39596 | 1.66671 | 2.930x |
| rotated-box | 6.33023 | 0.58877 | 1.50631 | 0.55969 | 3.28296 | 2.691x |
| aligned-ellipsoid | 7.24679 | 0.38788 | 0.70123 | 0.33585 | 2.11379 | 2.088x |
| rotated-ellipsoid | 6.85729 | 0.49853 | 0.84975 | 0.35596 | 2.68927 | 2.387x |
| ball | 10.00283 | 0.38840 | 0.73911 | 0.33608 | 2.55528 | 2.199x |

The geometric mean scalar fresh/cached ratio is **2.440x**.
The geometric means of phase-to-full ratios (five shapes) are:
`hull_vector_fresh` 0.062x; `hull_scalar_fresh` 0.146x;
`hull_scalar_cached` 0.060x; and `box_sweep_post_hull`
0.368x. These figures are for overlapping and *non-equivalent*
units of work; they **must not be summed** to estimate full reduce
CPU shares. In particular, comparing fresh/cached *scalar* hull
passes is valid, whereas `hull_vector_fresh` is a different
contractor path.

The verification data (empty domains after full/hull/one-box-sweep)
were respectively 16/16/16, 16/0/16, 20/0/20, 21/0/19 and
14/0/14. Rotated ellipsoid confirms that full reduction can
establish emptiness beyond the first post-hull box sweep; equality
of the counts for other shapes does not establish equality of all
contracted boxes.

Caution: the mean full-reduction costs per synthetic query are
116.6, 197.8, 226.5, 214.3 and 312.6 microseconds, respectively.
The prior full paving predicate replay per adaptive query reported
202.3, 265.0, 469.0, 768.6 and 440.8 microseconds. The markedly
different shape ordering and costs indicate that input selection
affects performance and that the fixed-box benchmark cannot rank
the true paving bottlenecks. The paving replay additionally includes
logical evaluation and set-side construction not represented by
the prebuilt solver problem.

Next useful diagnosis: gather actual solver call counts / rounds,
hull vs box time, and optionally procedure-compilation costs on
*the same adaptive query distribution*, without weakening certified
separation. Avoid optimizing caching in the production solver
solely from the observed scalar microbenchmark speedup.
No official README, algorithm, or workflow change is implied.


## 2026-11-07 - Adaptive paving candidate trace: reducer hull/box breakdown

The operator ran the optional \`--outer-trace-prefix\` export in
\`ariadne-cps/paving\` (commit \`dd93ecae4345b823e55de6fc1cb7978be29d331a\`)
at dimension 4 and fineness 3 and replayed all five traces with
\`ariadne-cps/constraint-solving\` (commit
\`cfcbeb0b01e9866dd4060dc8aeee2c05207d980b\`).
Every query reproduced the original separation bit from paving.
On each query, the diagnostic \`traced_reduce\` produced exactly the
same final contracted upper domain as the unmodified production
\`ConstraintSolver::reduce\`; no mismatches were reported.

The traced routine mirrors the production reduction loop and times
hull and box contractor calls in place, accumulating the measured
durations over all adaptive input boxes. The duration is therefore
**instrumented replay time**, not a seven-sample median and not the
end-to-end time of \`paving::separated()\`. The paired replay wall clock
includes a separate uninstrumented oracle call for each input; it
must not be interpreted as pure solver overhead or added to the
instrumented totals.

| Shape | Adaptive queries | Empty proofs | Reducer rounds | Box calls | Hull (ms) | Box (ms) | Other (ms) | Instrumented (ms) | Box share | Rounds/query |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| aligned-box | 2927 | 600 | 5254 | 223392 | 45.1824 | 472.3325 | 10.4374 | 527.9523 | 89.5% | 1.80 |
| rotated-box | 2591 | 576 | 4733 | 203174 | 91.6618 | 511.8064 | 9.0344 | 612.5027 | 83.6% | 1.83 |
| aligned-ellipsoid | 1127 | 372 | 4679 | 92145 | 63.6710 | 429.5840 | 5.7011 | 498.9561 | 86.1% | 4.15 |
| rotated-ellipsoid | 1055 | 296 | 5841 | 115414 | 103.8222 | 670.9843 | 6.9988 | 781.8052 | 85.8% | 5.54 |
| ball | 1467 | 414 | 5795 | 114344 | 78.9988 | 528.5662 | 7.0099 | 614.5749 | 86.0% | 3.95 |

Aggregated across the five separate runs: 9167 adaptive
queries, 2258 proven empty, 26302 reducer rounds, and 748469
\`box_reduce\` calls. The aggregate instrumented time is 3035.7912
ms, of which box calls account for 2613.2734 ms (**86.1%**),
hull calls for 383.3362 ms (**12.6%**) and the rest
for approximately 39.18 ms (**1.3%**). These proportions
describe the instrumented solver path, including timer overhead.

Reasons for termination, respectively **empty after hull**,
**empty after box**, **insufficient further shrink**:

| Shape | Hull-empty | Box-empty | Threshold |
| --- | ---: | ---: | ---: |
| aligned-box | 600 | 0 | 2327 |
| rotated-box | 420 | 156 | 2015 |
| aligned-ellipsoid | 21 | 351 | 755 |
| rotated-ellipsoid | 36 | 260 | 759 |
| ball | 22 | 392 | 1053 |

The quadratic geometries typically require substantially more
reducer rounds per query than the axis-aligned boxes. Rotated
ellipsoid is particularly expensive: ~109.4 box calls/query and
~5.81 microseconds per box call, compared with ~76.3 box
calls/query and ~2.11 microseconds per box call for aligned box.
The pair of effects explains its unusually expensive replay.

Inspection of the production \`ConstraintSolver::box_reduce\`
implementation shows an initial singleton/zero-width check and
otherwise *up to eight lower and up to seven upper* interval
slice evaluations in each invocation (with early exits). The
current statistics do not count actual slice/function evaluations,
productive reductions, unchanged calls, per-constraint/function
or variable contributions, and therefore cannot yet tell whether
time is due to unnecessary box calls, repeated expression evaluation,
or intrinsically expensive nonlinear evaluations. These should be
the next diagnostic measurements. Never skip contractor work or
alter its stopping rule to increase speed without separately
proving and regression-testing the certified outcomes.

These are direct measurements on the genuine adaptive paving
candidate sequence, unlike the preceding fixed-32-query benchmark;
the differing timing methodology and the omitted set-side
assembly still preclude a literal subtractive interpretation of
solver and paving time. No README/workflow changes are warranted.


## 2026-11-08 - Actual box-contractor activity on adaptive traces

The operator reran `constraint-solving/benchmarks/benchmark_constraint_reduce`
(commit `993dc3e4aed5573e18f3288d73db195c9e23d022`) on the five
exported adaptive paving traces (4D/fineness 3), using the original
`ConstraintSolver::reduce` as an exact contracted-domain oracle for
every query. All five traces passed equivalence and paving separation
checks. The diagnostic calls the existing five-argument
`box_reduce(...,function_evaluations)` overload: it counts actual
interval-function evaluations, compares the targeted interval before
and after each call, and tabulates activity by output and variable.

| Shape | Box calls | Function evaluations | Changed | Unchanged | Zero evaluations | Changed (%) |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| aligned-box | 223392 | 278592 | 0 | 223392 | 84096 | 0.00 |
| rotated-box | 203174 | 315092 | 3540 | 199634 | 52284 | 1.74 |
| aligned-ellipsoid | 92145 | 214460 | 11351 | 80794 | 16370 | 12.32 |
| rotated-ellipsoid | 115414 | 291702 | 19082 | 96332 | 938 | 16.53 |
| ball | 114344 | 263612 | 13528 | 100816 | 19834 | 11.83 |
| **All** | **748469** | **1363458** | **47501** | **700968** | **173522** | **6.35** |

Overall **93.65%** of box calls leave their targeted interval
unchanged; 23.18% execute zero function evaluations, often
because a coordinate is already degenerate. A no-change call
is **not automatically safe to skip**: it may be required to
establish certified impossibility or confirm the absence of
further contraction. These are counts from an instrumented
single replay, not repeated-sample speed measurements.

**Important per-output pattern:** every one of the first four
outputs (the image identity coordinates) has exactly zero
box-based contractions in all five workloads. Those outputs
account for 403248/748469 = **53.88%** of box calls and
about 779/2560 = **30.43%** of measured box-contractor time
in these replays; their call share reaches ~80.5-80.9%
for the three ellipsoidal/ball examples. On the axis-aligned
box workload, *all* box calls are unchanged, whereas its
600 proofs of separation occur during hull contraction.

For aligned/rotated ellipsoid and ball, every productive box
contraction occurs at the single nonlinear constraint output
(index 4), with 11351, 19082, and 13528 changed calls
respectively. For rotated-box, the only productive box outputs
are indices 4-7 (the rotated-coordinate constraints);
the image outputs 0-3 and axis-aligned constraint outputs
8-11 are unchanged throughout these traces.

**Next hypothesis to test, NOT yet a production change:**
before calling all box contractors for a particular output in
a given round, compute a certified interval image of the
current domain. If the image is *provably contained* in that
output's allowed codomain, the constraint is satisfied
throughout the entire domain, so shaving for that output
cannot soundly eliminate any portion; all its coordinate
calls can be skipped for that round. As an alternative,
consider static per-function variable dependencies using the
existing `propagation_procedure_dependencies` machinery
already employed by the separate `ConstraintSolver::propagate`
path, but do not assume it has been tested for `reduce`.
Compare both proposed diagnostic strategies with the unchanged
`reduce` on **every** adaptive input, requiring identical final
contracted domains and certified separation bits. Only then
benchmark end-to-end speed and consider a production change.

No README, CI benchmark workflow, or production solver algorithm
change follows from these measurements alone.

## 2026-11-08 - Benchmark-only preparation of outer separation predicates

**Motivation and baseline.** An independent five-shape d4/f3
`--outer-phases` replay reported approximately
95.9%–100.8% as much measured time in predicate-only sweeps
as in independent full outer-selection sweeps, while pure cell
traversal took only 1.4–4.2 ms per shape. Those workloads
overlap and are *not* additive, but they establish that
optimization of BinaryTree/ROBDD traversal alone is unlikely
to materially accelerate outer approximation here.
The existing `ConstrainedImageSet::separated(bx,Effort)`
reconstructs the joined image/constraint function, the
constraint bounds and a solver for each cell query.

**Diagnostic implementation.** Extend the **existing**
`benchmark_paving_backends` with opt-in
`--compare-outer-preparation` and `--verify-only`
modes (the latter only valid with the former). They require
`--dimensions 4 --finenesses 3 --max-depth-bits 28`
and run over all five geometry families by default. The
experiment uses the same recorded adaptive outer-selection
boxes and order as `--outer-phases`, while keeping
the actual paving library, its backends and the `set`
library **unchanged**.

The three measured predicate workloads are:

1. **Baseline:** the actual virtual `set.separated(box)`
   path, with its normal effort dispatch and per-query
   construction of combined function/bounds/solver.
2. **Joined:** create one immutable joined validated
   image/constraint function and static constraint bounds
   per full replay, then call the unchanged production
   `ConstraintSolver::reduce()` on each query.
3. **Compiled:** additionally create a specialized
   `Vector<ValidatedProcedure>` once per full replay;
   use a benchmark-only reproduction of the P6 hull/guard/box
   loop with that procedure and per-query certified-containment
   state. This variant is not a new solver API.

**Timing and correctness.** Each candidate constructs its
preparation inside **every calibrated timed full-trace
sweep**, so setup is included. The trace generation and
cell-box construction are equally excluded from all three
sweeps; comparisons are on whole predicate replays,
not on an isolated contractor microbenchmark. A separate
pre-timing check compares all three separation decisions
with the recorded paving bits and checks exact equality
of the *full contracted boxes* between the Joined solver
and Compiled diagnostic on every query. The original
selected cell trace was already checked against the two
concrete backends by the existing benchmark mechanism.
The new reports use `LOGGING_PRINTLN` and Numeric-style
aligned tables: `OUTER_PREPARATION_VERIFIED`,
`OUTER_PREPARATION_COMPARE`, and
`OUTER_PREPARATION_AGGREGATE`. The aggregate
reports both geometric-mean P1-relative speedups
(equal shape weight) and ratio of independently
sampled shape medians (workload weight).

```sh
git pull --ff-only
cmake --build build-release --target benchmark_paving_backends

./build-release/benchmarks/benchmark_paving_backends \
  --dimensions 4 --finenesses 3 --max-depth-bits 28 \
  --shapes aligned-box,rotated-box,aligned-ellipsoid,rotated-ellipsoid,ball \
  --compare-outer-preparation --verify-only \
  --no-retention --concurrency 0

./build-release/benchmarks/benchmark_paving_backends \
  --dimensions 4 --finenesses 3 --max-depth-bits 28 \
  --shapes aligned-box,rotated-box,aligned-ellipsoid,rotated-ellipsoid,ball \
  --compare-outer-preparation \
  --samples 7 --min-sample-ms 12 \
  --no-retention --concurrency 0
```

**Build coverage:** Ubuntu and macOS Release CI explicitly
build the standalone `benchmark_paving_backends`
target, which ordinary library builds omit. This protects
against specialized-vector API and compiler-warning failures,
but does not run the expensive experimental five-shape trace
on CI.

**Validation status:** code and benchmark registration changed,
but no complete CI/local C++ build or operator replay of
this revision has yet been observed. All claimed speedups
are *hypotheses*, not measurements. A change to production
`set` or paving APIs is prohibited until equivalence
and performance are demonstrated across all five shapes.
The compiled variant is a diagnostic that must remain
synchronized with P6's exact contractor semantics; it is not
a proof of general equivalence for arbitrary functions or
precision/lifetime settings.


## 2026-11-08 - Production preparation of effective-set outer predicates

**Architecture.** The P6 contractor loop is now centralized in
constraint-solving, shared by ordinary reduce() and the new
reduce(domain, PreparedConstraintSolverFunction, codomain).
The immutable prepared object owns its function and specialized
vector procedure. Reduction certificates, domains, and numerical
scratch remain per call; no global cache exists.

ConstrainedImageSet::prepare_separation() provides a caller-owned
snapshot of the joined image/constraint function, fixed bounds and
prepared solver. The snapshot does not follow later mutations to
the originating ConstrainedImageSet, preventing stale-cache use.
The unchanged direct separated() method remains available.

The effective-set outer paths of BinaryTreePaving and RobddPaving
prepare exactly once per operation **only for concrete
ConstrainedImageSet instances**. Other effective compact sets
continue using their prior virtual separated() methods. Validated
constrained-set paving algorithms remain entirely unchanged.

**Benchmark integration.** The opt-in --compare-outer-preparation
path now invokes the actual set-level prepared API instead of
duplicating the P1 contractor loop. Preparation is included inside
each timed sweep. Verification compares the complete final
BinaryTree and ROBDD geometries against an oracle built using
the original separated() decisions, as well as checking exact
P1-contracted box equality on each recorded query.

**Historical result, not a measured production result:** the
previous diagnostic Compiled implementation saved 13.78% of
predicate replay time over the original baseline on the five
d4/f3 geometries, but the new integrated implementation must
be rebuilt and benchmarked before claiming any production gain.

Validation commands:

~~~sh
git pull --ff-only
git submodule update --init --recursive
cmake --build build-release --target benchmark_paving_backends test_paving_backends
ctest --test-dir build-release -R '^test_paving_backends$' --output-on-failure
./build-release/benchmarks/benchmark_paving_backends \
  --dimensions 4 --finenesses 3 --max-depth-bits 28 \
  --compare-outer-preparation --verify-only --concurrency 0
./build-release/benchmarks/benchmark_paving_backends \
  --dimensions 4 --finenesses 3 --max-depth-bits 28 \
  --compare-outer-preparation --samples 7 --min-sample-ms 12 \
  --concurrency 0
~~~

**Release gate:** new solver/set/paving builds and tests must pass
cross-platform CI. The integrated 9,167-query correctness replay,
including complete geometry checks, and the new seven-sample
timings are pending. Do not treat earlier standalone results as
measurements of the integrated production implementation.


## 2026-11-08 - Avoid reclassifying already-covered outer candidates

**Optimization scope.** For the explicit tree's effective and validated
outer-approximation visitors, an already-enabled leaf is a no-op for
union. Both visitors now return before constructing the candidate
GridCell or invoking `separated()`/`covers()`, while retaining the
existing one-level path unwind. This applies to repeated and partially
overlapping approximations, without changing the first-visit geometric
classification or the ownership/locking model.

The ROBDD effective outer visitor now checks `superset(cell)`
before `separated(cell.box())` when the destination was nonempty at
the start. An initially empty ROBDD still avoids containment queries,
as before. The new order can skip certified predicates for covered
prefixes; it trades an inexpensive read-only DAG membership query for
potentially fewer expensive geometric calls on nonempty destinations.
It need not improve workloads with little existing overlap.

**Regression.** The backend test uses a derived
`ConstrainedImageSet` that counts calls to its virtual
`separated()` override (therefore bypassing exact-type prepared
dispatch). It checks that re-adjoining an approximation makes fewer
predicate calls than its first insertion, that both backend unions
are set-idempotent and agree geometrically, and that an ROBDD already
covering the enclosing primary root needs zero separation calls.

**Verification status.** This increment has not been built or
benchmark-run locally. Cross-platform CI and the regression are
required to validate it. No speedup is claimed: the existing
five-shape fresh-construction benchmark cannot measure a repeated
or overlapping-adjoin win. A future isolated benchmark should compare
fresh and prepopulated destinations, including disjoint and high-overlap
cases, before generalizing the performance benefit.


## 2026-11-08 - Controlled outer-approximation coverage benchmark

The existing backend benchmark now accepts opt-in
`--compare-outer-overlap`. It isolates the performance consequences
of reusing previously populated paving destinations without inventing
a different geometric set. For each selected shape/dimension/fineness,
a fully independent reference is built from the original virtual
`separated()`/`covers()` candidate classification. Compact enabled
reference leaves provide a deterministic half-leaf prepopulation. A
third destination begins with the complete enclosing search root
enabled. The empty destination is the first control.

For each of the three cases, tree and ROBDD inputs represent the
same geometric set; after applying the actual production outer
approximation, their geometries are checked in both directions
against the independently constructed expected union and against
one another. The verification-only mode executes these oracles but
does not sample timings. Cases with fewer than two reference leaves
are skipped, rather than claiming meaningful partial coverage.

Timed runs clone each prepared destination outside their measured
interval, run only one `adjoin_outer_approximation` per clone, and
exclude clone/destruction and recombination. The production predicate
preparation, when applicable, remains inside the operation. Samples
are independently calibrated for each backend and initial coverage;
the ROBDD manager is reused (warm/shared). The existing
`--max-depth-bits` budget applies to this experiment.

A short two-geometry 2D/f2 verification-only smoke is added to the
Ubuntu Release workflow, after the benchmarks are built. It does
not enforce machine-specific performance thresholds.

**Interpretation:** the reported empty/partial/full ratios are
state-dependent operation timings, *not an A/B old-versus-new
implementation speedup*. The presence of root-alignment, manager
and predicate preparation overhead limits what can be inferred from
full-coverage cases. No performance result is recorded until a
compiled Release run returns actual measurements. Cross-platform
compilation and Ubuntu verification are pending CI.

See README for commands including a bounded 4D/f3 five-shape run.


## 2026-11-08 - Operator d4/f3 overlap results and calibration limits

An operator supplied one Release run of the full five-shape
--compare-outer-overlap workload, dimension 4, fineness 3, 7 samples,
min-sample-ms 12, max-iterations 128, concurrency 0. All fifteen
scenario/backend pairings reported geometry_matches=true.
These figures compare different initial destinations, **not the
predicate-first code versus the early-pruning optimization**.

| Shape | Reference leaves | Tree empty ms | Tree partial ms | Tree saved | ROBDD empty ms | ROBDD partial ms | ROBDD saved |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| aligned-box | 416 | 33.381 | 25.072 | 24.9% | 34.733 | 26.490 | 23.7% |
| rotated-box | 360 | 156.871 | 119.909 | 23.6% | 158.085 | 121.176 | 23.3% |
| aligned-ellipsoid | 96 | 323.464 | 297.648 | 8.0% | 326.503 | 300.213 | 8.1% |
| rotated-ellipsoid | 104 | 540.443 | 503.763 | 6.8% | 553.028 | 505.120 | 8.7% |
| ball | 80 | 399.695 | 347.452 | 13.1% | 401.593 | 351.722 | 12.4% |

Partial initial state uses every second reference compact leaf (half
the leaf count, not half the geometric volume). Measured reductions
are 6.8%-24.9% for tree, 8.1%-23.7% for ROBDD. Tree and ROBDD
are close on these examples: the ROBDD/tree median ratio ranges
approximately 1.003-1.057 for empty/partial configurations. Without
between-run variance, this does not establish a reliable relative
backend advantage.

Full initial coverage produced 0.018-0.030 ms operations, versus
tens to hundreds of milliseconds for initially empty pavings.
However the 128-iteration cap was insufficient to meet the requested
12 ms minimum sample duration: 128 such operations occupy only
about 2.2-3.8 ms. The very large full/empty ratios thus describe
state-dependent avoided work, not a measured software speedup;
microsecond-level backend ratios also warrant better calibration.

The benchmark now records whether mutation timing reached its
minimum calibration duration and emits explicit diagnostics on a
shortfall. The README raises the recommended maximum to 2048 to
calibrate fast full-coverage cases without unnecessarily repeating
already-slow empty/partial calls. No production algorithm changed.
CI/compilation of this reporting improvement and any new measured
timings remain pending.


## 2026-11-08 - Recalibrated overlap measurements and deferred preparation

**Measured baseline.** The operator repeated the five-shape 4D/f3
Release overlap experiment with 7 samples, `--min-sample-ms 12` and
`--max-iterations 2048`. Every one of the 15 scenario checks reported
`geometry_matches=true`. All timing rows reported
`tree_calibration_met=true` and `robdd_calibration_met=true`;
the fast full scenario calibrated to 512 or 1024 iterations.

| Shape | Tree empty ms | Tree partial ms | Tree saved | ROBDD empty ms | ROBDD partial ms | ROBDD saved | Full tree/ROBDD us | Full iterations |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| aligned-box | 33.905 | 25.192 | 25.7% | 35.128 | 26.670 | 24.1% | 23.6 / 23.8 | 512 |
| rotated-box | 157.478 | 120.447 | 23.5% | 158.790 | 121.401 | 23.5% | 29.6 / 30.0 | 512 |
| aligned-ellipsoid | 325.008 | 289.601 | 10.9% | 326.259 | 291.619 | 10.6% | 17.1 / 17.4 | 1024 |
| rotated-ellipsoid | 541.792 | 504.762 | 6.8% | 546.222 | 503.565 | 7.8% | 20.8 / 20.9 | 1024 |
| ball | 393.522 | 353.831 | 10.1% | 407.880 | 350.063 | 14.2% | 17.2 / 17.4 | 1024 |

The reductions for partial coverage are 6.8%-25.7% for tree and
7.8%-24.1% for ROBDD. Full coverage costs approximately 17-30 us.
These are comparisons between **different preexisting paving states**,
not measurements of the early-pruning optimization versus its previous
version. Even with sufficiently long batches, backend differences near
unity lack cross-run variance estimates.

**New optimization hypothesis.** A fully covered search root still
constructed `PreparedConstrainedImageSetSeparation`, despite never
executing `separated()`. The exact-concrete-type effective outer path
now avoids this unnecessary solver snapshot:

- Tree checks `superset(enclosing_root)` through a borrowed base view
  under the existing owning mutex, before preparing the contractor
  and before mutating tree alignment. The preparation remains ahead
  of alignment for uncovered regions. Derived virtual set overrides
  and validated set paths are unchanged.
- ROBDD allocates the prepared snapshot lazily, only upon encountering
  the first non-covered candidate. The existing earlier read-only
  `superset` short circuit remains authoritative. No global cache.

An exact-constrained-set test now checks both fully covered backends
retain their geometric roots, inclusion and ROBDD depth. These timing
measurements are **pre-optimization baseline numbers**, not claimed
speedups for deferred preparation. CI plus a new Release run are
required to establish correctness and performance of this change.
