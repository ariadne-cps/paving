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

- Import a `GridTreeSubpaving` without retaining its mutable tree pointers.
- Query membership at exact logical depth and count/visit enabled cells.
- Adjust logical depth (`mince_to_depth`) and reduce it to the
  maximum supported variable index (`recombine`) without BDD mutation.
- Convert back to an explicit `BinaryTreeNode`.
- Boolean union, intersection, difference, subset and overlap tests.
  Operands must use the same BDD manager and **identical root cells**.
  Binary functions are evaluated at a logical depth equal to the maximum
  of the operand depths. The results are immutable snapshots.

The existing `GridTreePaving`, `GridTreeSubpaving`, iterators and
`GridTreeCursor` remain unchanged. This prototype does not yet implement
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

Next milestone: real `GridTreePaving` parity under root-cell
alignment/height changes, projection and efficient path-based snapshot
iteration. Do not replace pointer-based cursors with raw pointers into
an immutable ROBDD DAG.

## 2026-11-06 - Explicit backend naming

Standardized the public representation-specific C++ names:
`BinaryTreeGridPaving` for the existing explicit binary tree backend,
and `RobddGridPaving` for the new ROBDD prototype.

`BinaryTreeGridPaving` is currently a type alias for the longstanding
`GridTreePaving`, declared in
`include/paving/binary_tree_grid_paving.hpp`. The old name remains valid,
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

The explicit `GridTreeSubpaving::begin/end` iterator visits enabled
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
`GridTreeSubpaving`'s leaf iterator after explicit-tree
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
`BinaryTreeGridPaving` / `RobddGridPaving` and their headers are
**deleted outright**. There are no transition compatibility headers or
aliases for these two names.

`BinaryTreePaving` is a public type alias of the long-established
`GridTreePaving` concrete class. That original class is still used
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
spelling: `Storage::_state_set` owns `GridTreePaving` by value,
`Storage::state_set()` returns a concrete reference, and
`Storage::state_auxiliary_set()` uses `outer_skew_product`.
`Storage` also calls functions not currently exposed by
`PavingInterface`, such as `clear`, `restrict_to_extent`,
`adjoin_lower_approximation`, and concrete drawing support.
`LabelledStorage`, `LabelledGridTreePaving`,
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

The legacy `GridTreePaving::size()` can count redundant explicitly
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
the exact box semantics of `GridTreeSubpaving`:
`inside` compares every enabled cell with the narrowed query
box; `separated` requires that no enabled cell's interior
overlaps the widened query box; and `overlaps` succeeds when
an enabled cell overlaps the narrowed query box. The
conservative widen/narrow steps intentionally preserve the
tree backend's handling of boundary contacts. All methods
check dimension compatibility and leave the paving unchanged.

A reference-oracle regression enumerates all 256 Boolean
functions over three subdivision bits and compares the four
queries against a recombined `GridTreeSubpaving` using six
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
constructed a `GridTreeSubpaving` from a `BinaryTreeNode` and then
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

The existing `GridTreeSubpaving` cases remain in the suite
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
  used by `GridTreeSubpaving::mince`:
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
`GridTreeSubpaving`; verifies the computed logical depth and stable
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
`RobddSubpaving` type. Existing `GridTreeSubpaving` borrows the
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

As in `GridTreeSubpaving`, inclusion uses the conservatively
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
`GridTreePaving` backend, and visits cells recursively. Cells proved
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
`GridTreeSubpaving::equals`/`subset`/`intersects` was
reverted: those methods retain the same-backend
`GridTreeSubpaving` dynamic cast. The ROBDD virtual
`adjoin`, `restrict`, `remove`, `subset`, `intersects`
and `equals` now accept only `RobddPaving` operands or
`PavingCellViewAdapter<RobddPaving>` child views. Different
backends are rejected before mutation, rather than implicitly
converted. The same-ROBDD-manager compatibility restriction
also remains in force.

Geometric cross-backend validation belongs only in external
test/oracle code, where each backend is constructed and queried
independently. The existing explicit constructors from
`GridTreeSubpaving` or `SubPavingInterface` remain available
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

- `tests/test_grid_paving.cpp` was renamed to
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
`GridTreePaving` class and its public API retain their existing
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
a real `GridTreeSubpaving` bug: after a uniform enabled region is
recombined to a leaf, the tree's virtual `_branch()` unconditionally
returned `child_node(side)`, which is null for any leaf.
`SubPavingHandle` consequently exposed a live view with a null
`BinaryTreeNode*`; mutating its root dereferenced the null pointer.
The reported crash followed the check
`robdd_snapshot.superset(left)`, immediately before taking and
mutating the nested child views. The ROBDD snapshot was not the
origin of this failure.

Fixed both `GridTreeSubpaving::_branch(Bool)` and the concrete
`GridTreeSubpaving::branch(Bool)`: each splits the current node
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
The concrete GridTreeSubpaving::operator== deliberately stays structural.
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
- outer_skew_product() previously const_cast a const input GridTreePaving
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
PavingHandle carrying an explicit GridTreePaving with exactly the same
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
adjoin_recombine: for each timed sample, independently copied
pavings are prepared outside the timed portion, and only adjoin()
and recombine() are measured. A maximum batch length of 256 caps
temporary working memory; larger --iterations requests fail rather
than silently violating input semantics. Calibration and medians
otherwise follow the numeric benchmark conventions.

A separate sequential retention experiment (disable using
--no-retention) creates the selected geometries in one 2D manager
at fineness 2. Each paving object is destroyed before its manager
snapshot, so the report demonstrates growth/retention of nodes,
capacity and tables in the absence of surviving paving roots.
The observation is not a proof of an unintended memory leak:
RobddManager has no root-tracked reclamation/GC by design.
The experiment is not included in operation-time speedup aggregates.
