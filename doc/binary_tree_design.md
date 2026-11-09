# BinaryTreeNode design and extraction contract

## Purpose and name

`BinaryTreeNode` means a node **and the subtree rooted at that node**.
The class is not named after the paving domain: neither its representation
nor its operations require geometric cells, grids or paving interfaces.
Keep this public name during extraction to `ariadne-cps/tree`.
The future n-ary tree for trajectory genealogy is a separate abstraction.

## Abstract Boolean interpretation

Paths are finite words over `{0,1}`, represented by `BinaryWord`.
The left child appends 0 and the right child appends 1. An enabled leaf
covers every extension of its path; a disabled leaf excludes all extensions.
Every internal node has both children and an indeterminate
`ValidatedKleenean` value. Leaf membership is Boolean for the path-set
operations. Equivalently, the tree denotes a Boolean function of infinite
bit sequences, constant on the cylinders represented by its leaves.

A tree consisting of a single enabled leaf has the same represented set
as a tree whose two children are enabled leaves. Nonetheless, `operator==`
compares the *representation*, including node state and structure.
For roots of the same abstract domain, semantic equality can be checked
as `subset(a,b) && subset(b,a)`.

## Structural invariants and operations

- Each node is either a leaf with no children or an internal node with
  exactly two ordered children; it exclusively owns its descendants.
- `split()` replaces a leaf with two children of its former state.
  `recombine()` collapses identical sibling leaves; both preserve
  the represented set for Boolean-valued leaves.
- `mince(depth)` refines enabled leaves to the requested depth; disabled
  leaves need not be expanded. `depth()` concerns stored tree depth.
- `add_enabled(path)` inserts a covered prefix; adding a descendant of
  an already enabled ancestor does not change the represented set.
- `restrict(a,b)` and `remove(a,b)` update `a` by intersection and
  difference. `intersect(a,b)` and `subset(a,b)` are read-only tests.
  Both roots must refer to the same abstract binary domain.
- `set()` and `make_leaf()` discard an existing subtree.
  `tree_to_binary_words()` serializes structure and Boolean leaf flags
  in depth-first, left-before-right order; it assumes determinate leaves.
- `prepend_tree(path,root)` transfers ownership of `root` to a newly
  allocated ancestor. It currently assumes the path is nonempty; callers
  must meet that precondition.

## Lifetime, ownership and concurrency

Copy construction and assignment make deep copies. The destructor
recursively destroys descendants. `left_node()`, `right_node()`, and
`child_node()` return **borrowed raw pointers**; changing or recombining
the tree can invalidate them. No persistent handles or shared subtree
ownership are provided.

`BinaryTreeNode` is a sequential building block; simultaneous reads
and mutations of overlapping nodes are not guaranteed safe. Locking
a root only during writes does not protect a borrowed child pointer
used after unlocking. A later owning `BinaryTree` abstraction (or a
domain-specific owner such as `BinaryTreePaving`) can provide locking,
snapshots and lifetime-safe operations. Algorithms may parallelize
independent subtrees, but concurrency and parallelism do not require
a mutex on every node. None of those new guarantees is implemented here.

## Minimal dependency boundary

`foundation/logical.hpp` supplies `ValidatedKleenean` and associated
logical operations. `utility/binary_word.hpp` supplies path words;
Utility also supplies macros and basic types. The node has no direct
dependencies on `function/functional.hpp`, `set/list_set.hpp` or paving
types. `tree` can therefore depend on Foundation (and transitively Utility).

## Extraction plan and verification

Keep the node and its tests in `paving` until independently validated.
Transfer its source, specification and representation-focused tests into
`tree`; update paving includes and CMake/submodule edges separately.
Do not alter its public name, raw-pointer contract, or membership
semantics incidentally during the move. The regression suite
`tests/test_binary_tree.cpp` includes grid-independent path tests;
other paving consumers remain tested in their existing suites.

Cross-platform compilation and sanitizers remain necessary to validate
the include cleanup and eventual migration; documentation is not a
substitute for a successful build.


## 2026-11-09 - Paving Tree dependency baseline and updated pin

Paving upgrades `ss/tre` from
`7166244bc2cd22fec9e2c7aec1db38903c08b206` to
`ba055ee7cf5663c916a54635565fb97513b690bb`,
together with a BDD upgrade `3b6c918` → `378d16e`.
The public `BinaryTreeNode::recombine_parallel()` in the
new Tree module uses a bounded TaskGroup fork/join, but
**Paving still calls the existing sequential `recombine()`**.
This pin update alone therefore does not activate
tree recombination parallelism.

A manual Paving-level factorial benchmark will evaluate
old/new Tree with either old/new BDD, preserving identical
Paving source and geometry inputs. Differences in Tree
operation timings may reflect unrelated Tree changes or
compiler/runtime noise: the opt-in parallel recombination
must not be credited for them without an explicit call.
The existing single-owner/different-subtree concurrency
contract remains unchanged. No performance benchmark
runs in CI.
