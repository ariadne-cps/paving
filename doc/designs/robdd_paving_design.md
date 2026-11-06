# ROBDD-backed paving: M5 integration journal

## Scope and invariants

This is an additive, paving-local experiment. `ariadne-cps/bdd` continues to
provide only general-purpose Boolean functions, canonical ROBDD nodes,
model counting and satisfying-assignment enumeration. The geometric
interpretation is entirely owned by `ariadne-cps/paving`.

The new `RobddGridTreePaving` snapshot stores a `GridCell` root, a
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
