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
