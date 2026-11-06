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
