/***************************************************************************
 *            test_robdd_paving.cpp
 *
 *  Copyright  2026  Luca Geretti
 ****************************************************************************/

/*
 *  This file is part of Ariadne Paving.
 *
 *  Ariadne Paving is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 */

#include <type_traits>
#include <vector>

#include "paving/binary_tree_paving.hpp"
#include "paving/grid_paving.hpp"
#include "paving/robdd_paving.hpp"
#include "ariadne-testing.hpp"

using namespace Ariadne;

static_assert(std::is_same_v<BinaryTreePaving,GridTreePaving>);


class TestRobddPaving : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

    Void test_import_and_explicit_tree_oracle() {
        RobddManager manager;
        Grid grid(1u);
        BinaryWord prefix = make_binary_word("01");
        BinaryTreeNode tree(false);
        tree.add_enabled(make_binary_word("100"));
        tree.add_enabled(make_binary_word("110"));
        tree.recombine();

        GridTreeSubpaving source(grid,2u,prefix,&tree);
        RobddPaving snapshot(manager,source);
        ARIADNE_TESTING_EXPECT_EQUAL(snapshot.depth(),3u);
        ARIADNE_TESTING_PROVE(snapshot.root_cell() == source.root_cell());
        ARIADNE_TESTING_PROVE(not snapshot.is_empty());
        ARIADNE_TESTING_EXPECT_EQUAL(snapshot.enabled_cell_count(),2u);
        ARIADNE_TESTING_EXPECT_EQUAL(manager.support(snapshot.robdd()).size(),2u);

        std::vector<BinaryWord> visited;
        snapshot.for_each_enabled_cell([&](GridCell const& cell) {
            ARIADNE_TESTING_PROVE(cell.grid() == grid);
            ARIADNE_TESTING_EXPECT_EQUAL(cell.root_extent(),2u);
            visited.push_back(cell.word());
        });
        ARIADNE_TESTING_EXPECT_EQUAL(visited.size(),2u);
        ARIADNE_TESTING_PROVE(visited[0u] == make_binary_word("01100"));
        ARIADNE_TESTING_PROVE(visited[1u] == make_binary_word("01110"));

        auto restored = snapshot.to_binary_tree();
        for (unsigned mask=0u; mask!=8u; ++mask) {
            BinaryWord path;
            for (unsigned bit=0u; bit!=3u; ++bit) {
                path.append((mask & (1u << bit)) != 0u);
            }
            ARIADNE_TESTING_EXPECT_EQUAL(
                snapshot.is_enabled(path),tree.is_enabled(path));
            ARIADNE_TESTING_EXPECT_EQUAL(
                restored->is_enabled(path),tree.is_enabled(path));
        }
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            snapshot.is_enabled(make_binary_word("10")));

        snapshot.mince_to_depth(5u);
        ARIADNE_TESTING_EXPECT_EQUAL(snapshot.depth(),5u);
        ARIADNE_TESTING_EXPECT_EQUAL(snapshot.enabled_cell_count(),8u);
        auto minced_tree = snapshot.to_binary_tree();
        ARIADNE_TESTING_PROVE(minced_tree->is_enabled(make_binary_word("10000")));
        ARIADNE_TESTING_PROVE(not minced_tree->is_enabled(make_binary_word("10100")));
        snapshot.recombine();
        ARIADNE_TESTING_EXPECT_EQUAL(snapshot.depth(),3u);
        ARIADNE_TESTING_EXPECT_EQUAL(snapshot.enabled_cell_count(),2u);
    }

    Void test_terminals_and_complemented_roots() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        auto full = RobddPaving(manager,root,manager.constant(true),0u);
        auto empty = RobddPaving(manager,root,manager.constant(false),0u);
        ARIADNE_TESTING_PROVE(full.is_enabled(BinaryWord{}));
        ARIADNE_TESTING_PROVE(not empty.is_enabled(BinaryWord{}));
        ARIADNE_TESTING_EXPECT_EQUAL(full.enabled_cell_count(),1u);
        ARIADNE_TESTING_EXPECT_EQUAL(empty.enabled_cell_count(),0u);

        full.mince_to_depth(4u);
        empty.mince_to_depth(4u);
        ARIADNE_TESTING_EXPECT_EQUAL(full.enabled_cell_count(),16u);
        ARIADNE_TESTING_EXPECT_EQUAL(empty.enabled_cell_count(),0u);
        SizeType before = manager.node_count();
        SizeType visits = 0u;
        full.for_each_enabled_cell([&](GridCell const& cell) {
            ARIADNE_TESTING_EXPECT_EQUAL(cell.word().size(),4u);
            ++visits;
        });
        ARIADNE_TESTING_EXPECT_EQUAL(visits,16u);
        ARIADNE_TESTING_EXPECT_EQUAL(manager.node_count(),before);
        full.recombine();
        empty.recombine();
        ARIADNE_TESTING_EXPECT_EQUAL(full.depth(),0u);
        ARIADNE_TESTING_EXPECT_EQUAL(empty.depth(),0u);

        BinaryTreeNode enabled(true);
        GridTreeSubpaving source(grid,0u,BinaryWord{},&enabled);
        RobddPaving imported(manager,source);
        ARIADNE_TESTING_PROVE(imported.robdd().is_true());
        ARIADNE_TESTING_PROVE(imported.to_binary_tree()->is_enabled());
    }

    Void test_set_operations_and_preconditions() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        BinaryTreeNode left_tree(false);
        left_tree.add_enabled(make_binary_word("100"));
        left_tree.add_enabled(make_binary_word("110"));
        left_tree.recombine();
        BinaryTreeNode right_tree(false);
        right_tree.add_enabled(make_binary_word("101"));
        right_tree.add_enabled(make_binary_word("111"));
        right_tree.recombine();

        GridTreeSubpaving left_explicit(grid,0u,BinaryWord{},&left_tree);
        GridTreeSubpaving right_explicit(grid,0u,BinaryWord{},&right_tree);
        RobddPaving lhs(manager,left_explicit);
        RobddPaving rhs(manager,right_explicit);
        ARIADNE_TESTING_PROVE(not lhs.intersects(rhs));
        ARIADNE_TESTING_PROVE(not lhs.subset(rhs));
        ARIADNE_TESTING_PROVE(lhs.subset(lhs));
        ARIADNE_TESTING_EXPECT_EQUAL(lhs.join(rhs).enabled_cell_count(),4u);
        ARIADNE_TESTING_PROVE(lhs.intersection(rhs).is_empty());
        ARIADNE_TESTING_PROVE(lhs.difference(rhs).robdd() == lhs.robdd());

        auto combined = lhs.join(rhs);
        for (unsigned mask=0u; mask!=8u; ++mask) {
            BinaryWord path;
            for (unsigned bit=0u; bit!=3u; ++bit) {
                path.append((mask & (1u << bit)) != 0u);
            }
            ARIADNE_TESTING_EXPECT_EQUAL(
                combined.is_enabled(path),
                left_tree.is_enabled(path) or right_tree.is_enabled(path));
            ARIADNE_TESTING_EXPECT_EQUAL(
                lhs.intersection(rhs).is_enabled(path),
                left_tree.is_enabled(path) and right_tree.is_enabled(path));
        }
        // Restoring at the declared depth expands true terminal subtrees.
        ARIADNE_TESTING_EXPECT_EQUAL(
            combined.to_binary_tree()->depth(),3u);
        ARIADNE_TESTING_PROVE(combined.to_binary_tree()->is_enabled(
            make_binary_word("100")));
        combined.recombine();
        ARIADNE_TESTING_EXPECT_EQUAL(combined.depth(),1u);
        ARIADNE_TESTING_EXPECT_EQUAL(combined.to_binary_tree()->depth(),1u);

        RobddManager foreign;
        auto foreign_value = foreign.variable(0u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            RobddPaving(manager,root,foreign_value,1u));
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            RobddPaving(manager,root,manager.variable(2u),2u));
        RobddPaving other_manager(foreign,root,foreign.variable(0u),1u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(lhs.join(other_manager));
        ARIADNE_TESTING_EXPECT_EXCEPTION(lhs.subset(other_manager));
        GridCell different_root(grid,0u,make_binary_word("0"));
        RobddPaving wrong_cell(manager,different_root,manager.variable(0u),1u);
        // Different geometric roots are aligned rather than rejected.
        ARIADNE_TESTING_PROVE(not lhs.intersects(wrong_cell));
        ARIADNE_TESTING_EXPECT_EXCEPTION(lhs.for_each_enabled_cell(
            std::function<void(GridCell const&)>{}));
    }
    Void test_distinct_roots_and_extents() {
        RobddManager manager;
        Grid grid(1u);
        auto enabled = manager.constant(true);
        auto disabled = manager.constant(false);

        GridCell left_cell(grid,0u,make_binary_word("0"));
        GridCell right_cell(grid,0u,make_binary_word("1"));
        RobddPaving lhs(manager,left_cell,enabled,0u);
        RobddPaving rhs(manager,right_cell,enabled,0u);

        auto united = lhs.join(rhs);
        ARIADNE_TESTING_PROVE(united.root_cell() == GridCell(grid,0u,BinaryWord{}));
        ARIADNE_TESTING_EXPECT_EQUAL(united.depth(),1u);
        ARIADNE_TESTING_EXPECT_EQUAL(united.enabled_cell_count(),2u);
        ARIADNE_TESTING_PROVE(united.is_enabled(make_binary_word("0")));
        ARIADNE_TESTING_PROVE(united.is_enabled(make_binary_word("1")));
        ARIADNE_TESTING_PROVE(not lhs.intersects(rhs));
        ARIADNE_TESTING_PROVE(lhs.intersection(rhs).is_empty());
        ARIADNE_TESTING_PROVE(not lhs.subset(rhs));
        ARIADNE_TESTING_PROVE(lhs.subset(united));
        ARIADNE_TESTING_EXPECT_EQUAL(lhs.difference(rhs).enabled_cell_count(),1u);

        GridCell ancestor(grid,0u,BinaryWord{});
        auto parent = RobddPaving(manager,ancestor,enabled,0u);
        ARIADNE_TESTING_PROVE(lhs.subset(parent));
        ARIADNE_TESTING_PROVE(parent.intersects(lhs));
        ARIADNE_TESTING_PROVE(parent.join(lhs).robdd().is_true());
        auto parent_minus_left = parent.difference(lhs);
        ARIADNE_TESTING_EXPECT_EQUAL(parent_minus_left.enabled_cell_count(),1u);
        ARIADNE_TESTING_PROVE(parent_minus_left.is_enabled(make_binary_word("1")));

        GridCell deep_cell(grid,0u,make_binary_word("010"));
        RobddPaving deep(manager,deep_cell,enabled,0u);
        auto shared = lhs.join(deep);
        ARIADNE_TESTING_PROVE(shared.root_cell() == left_cell);
        ARIADNE_TESTING_PROVE(shared.robdd().is_true());
        ARIADNE_TESTING_PROVE(deep.subset(lhs));
        ARIADNE_TESTING_EXPECT_EQUAL(
            deep.intersection(lhs).depth(),2u);

        // Extent 1 has primary path '1' to extent 0 in dimension 1.
        GridCell raised_cell(grid,1u,BinaryWord{});
        auto raised = RobddPaving(manager,raised_cell,enabled,0u);
        auto across_extent = raised.intersection(lhs);
        ARIADNE_TESTING_PROVE(across_extent.root_cell() == raised_cell);
        ARIADNE_TESTING_EXPECT_EQUAL(across_extent.depth(),2u);
        ARIADNE_TESTING_EXPECT_EQUAL(across_extent.enabled_cell_count(),1u);
        ARIADNE_TESTING_PROVE(
            across_extent.is_enabled(make_binary_word("10")));
        ARIADNE_TESTING_PROVE(lhs.subset(raised));
        ARIADNE_TESTING_PROVE(not raised.subset(lhs));

        // Disjoint paving roots at different extents are still legal.
        GridCell distant_cell(grid,1u,make_binary_word("0"));
        RobddPaving distant(manager,distant_cell,enabled,0u);
        ARIADNE_TESTING_PROVE(not distant.intersects(lhs));
        auto separated_union = distant.join(lhs);
        ARIADNE_TESTING_PROVE(
            separated_union.root_cell() == GridCell(grid,1u,BinaryWord{}));
        ARIADNE_TESTING_EXPECT_EQUAL(separated_union.depth(),2u);

        // The coarser extent-1 cell covers both "00" and "01",
        // whereas the extent-0 cell contributes "10".
        BinaryTreeNode explicit_oracle(false);
        explicit_oracle.add_enabled(make_binary_word("0"));
        explicit_oracle.add_enabled(make_binary_word("10"));
        auto reconstructed = separated_union.to_binary_tree();
        SizeType satisfying = 0u;
        for (unsigned mask=0u; mask!=4u; ++mask) {
            BinaryWord path;
            path.push_back((mask & 2u) != 0u);
            path.push_back((mask & 1u) != 0u);
            Bool expected = explicit_oracle.is_enabled(path);
            ARIADNE_TESTING_EXPECT_EQUAL(
                separated_union.is_enabled(path),expected);
            ARIADNE_TESTING_EXPECT_EQUAL(
                reconstructed->is_enabled(path),expected);
            if (expected) { ++satisfying; }
        }
        ARIADNE_TESTING_EXPECT_EQUAL(satisfying,3u);
        ARIADNE_TESTING_EXPECT_EQUAL(
            separated_union.enabled_cell_count(),satisfying);
        std::vector<BinaryWord> enumerated;
        separated_union.for_each_enabled_cell([&](GridCell const& cell) {
            ARIADNE_TESTING_EXPECT_EQUAL(cell.root_extent(),1u);
            enumerated.push_back(cell.word());
        });
        ARIADNE_TESTING_EXPECT_EQUAL(enumerated.size(),3u);
        ARIADNE_TESTING_PROVE(
            enumerated[0u] == make_binary_word("00"));
        ARIADNE_TESTING_PROVE(
            enumerated[1u] == make_binary_word("01"));
        ARIADNE_TESTING_PROVE(
            enumerated[2u] == make_binary_word("10"));

        // An independent grid is never silently aligned.
        Grid other_grid(2u);
        RobddPaving incompatible(
            manager,GridCell(other_grid,0u,BinaryWord{}),disabled,0u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(lhs.join(incompatible));
        ARIADNE_TESTING_EXPECT_EXCEPTION(lhs.subset(incompatible));
    }

    Void test_compact_leaf_iteration() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});

        RobddPaving full(manager,root,manager.constant(true),4u);
        RobddPaving empty(manager,root,manager.constant(false),4u);
        ARIADNE_TESTING_EXPECT_EQUAL(full.enabled_cell_count(),16u);
        ARIADNE_TESTING_EXPECT_EQUAL(full.enabled_leaf_count(),1u);
        ARIADNE_TESTING_EXPECT_EQUAL(full.size(),1u);
        ARIADNE_TESTING_EXPECT_EQUAL(full.dimension(),1u);
        ARIADNE_TESTING_PROVE(full.grid() == grid);
        ARIADNE_TESTING_EXPECT_EQUAL(empty.enabled_leaf_count(),0u);
        ARIADNE_TESTING_EXPECT_EQUAL(empty.size(),0u);
        ARIADNE_TESTING_PROVE(empty.begin() == empty.end());
        ARIADNE_TESTING_PROVE(full.begin() != full.end());
        ARIADNE_TESTING_PROVE(full.begin()->word() == BinaryWord{});
        auto full_iter = full.begin();
        auto full_copy = full_iter;
        ARIADNE_TESTING_PROVE(full_copy == full_iter);
        auto full_old = full_iter++;
        ARIADNE_TESTING_PROVE(full_old == full_copy);
        ARIADNE_TESTING_PROVE(full_iter == full.end());
        ++full_copy;
        ARIADNE_TESTING_PROVE(full_copy == full_iter);
        SizeType empty_visits=0u;
        empty.for_each_enabled_leaf([&](GridCell const&) { ++empty_visits; });
        ARIADNE_TESTING_EXPECT_EQUAL(empty_visits,0u);
        std::vector<BinaryWord> full_leaves;
        full.for_each_enabled_leaf([&](GridCell const& cell) {
            full_leaves.push_back(cell.word());
        });
        ARIADNE_TESTING_EXPECT_EQUAL(full_leaves.size(),1u);
        ARIADNE_TESTING_PROVE(full_leaves[0u] == BinaryWord{});
        ARIADNE_TESTING_EXPECT_EXCEPTION(full.for_each_enabled_leaf(
            std::function<void(GridCell const&)>{}));

        // x0 OR x1: the true branch at x0 yields a single prefix leaf.
        auto x0 = manager.variable(0u);
        auto x1 = manager.variable(1u);
        RobddPaving either(
            manager,root,manager.disjunction(x0,x1),3u);
        ARIADNE_TESTING_EXPECT_EQUAL(either.enabled_cell_count(),6u);
        ARIADNE_TESTING_EXPECT_EQUAL(either.enabled_leaf_count(),2u);
        std::vector<BinaryWord> either_leaves;
        either.for_each_enabled_leaf([&](GridCell const& cell) {
            either_leaves.push_back(cell.word());
        });
        ARIADNE_TESTING_EXPECT_EQUAL(either_leaves.size(),2u);
        std::vector<BinaryWord> either_from_iterator;
        for (auto it=either.begin(), stop=either.end(); it!=stop; ++it) {
            either_from_iterator.push_back(it->word());
        }
        ARIADNE_TESTING_PROVE(either_from_iterator == either_leaves);
        ARIADNE_TESTING_PROVE(either_leaves[0u] == make_binary_word("01"));
        ARIADNE_TESTING_PROVE(either_leaves[1u] == make_binary_word("1"));

        // x1 ignores x0, so its true leaf occurs under both x0 branches.
        RobddPaving skipped(manager,root,x1,3u);
        ARIADNE_TESTING_EXPECT_EQUAL(skipped.enabled_cell_count(),4u);
        ARIADNE_TESTING_EXPECT_EQUAL(skipped.enabled_leaf_count(),2u);
        std::vector<BinaryWord> skipped_leaves;
        skipped.for_each_enabled_leaf([&](GridCell const& cell) {
            skipped_leaves.push_back(cell.word());
        });
        ARIADNE_TESTING_EXPECT_EQUAL(skipped_leaves.size(),2u);
        ARIADNE_TESTING_PROVE(skipped_leaves[0u] == make_binary_word("01"));
        ARIADNE_TESTING_PROVE(skipped_leaves[1u] == make_binary_word("11"));

        // A complemented reference has the same short-leaf semantics.
        auto conjunction = manager.conjunction(x0,x1);
        RobddPaving complemented(
            manager,root,manager.negation(conjunction),3u);
        ARIADNE_TESTING_EXPECT_EQUAL(complemented.enabled_cell_count(),6u);
        ARIADNE_TESTING_EXPECT_EQUAL(complemented.enabled_leaf_count(),2u);
        std::vector<BinaryWord> complemented_leaves;
        complemented.for_each_enabled_leaf([&](GridCell const& cell) {
            complemented_leaves.push_back(cell.word());
        });
        ARIADNE_TESTING_EXPECT_EQUAL(complemented_leaves.size(),2u);
        ARIADNE_TESTING_PROVE(complemented_leaves[0u] == make_binary_word("0"));
        ARIADNE_TESTING_PROVE(complemented_leaves[1u] == make_binary_word("10"));

        SizeType before=manager.node_count();
        complemented.for_each_enabled_leaf([&](GridCell const&) {});
        ARIADNE_TESTING_EXPECT_EQUAL(manager.node_count(),before);
        full.mince_to_depth(6u);
        ARIADNE_TESTING_EXPECT_EQUAL(full.enabled_leaf_count(),1u);
        ARIADNE_TESTING_EXPECT_EQUAL(full.enabled_cell_count(),64u);
    }

    Void test_compact_leaves_exhaustive_truth_tables() {
        RobddManager manager;
        Grid grid(1u);
        BinaryWord prefix=make_binary_word("01");
        // All 256 truth tables of three subdivision variables.
        for (unsigned table=0u; table<256u; ++table) {
            BinaryTreeNode explicit_tree(false);
            for (unsigned mask=0u; mask<8u; ++mask) {
                if ((table & (1u<<mask)) == 0u) { continue; }
                BinaryWord path;
                for (unsigned bit=0u; bit<3u; ++bit) {
                    path.push_back(((mask>>(2u-bit)) & 1u) != 0u);
                }
                explicit_tree.add_enabled(path);
            }
            explicit_tree.recombine();
            GridTreeSubpaving source(grid,2u,prefix,&explicit_tree);
            RobddPaving snapshot(manager,source);
            // A fully uniform truth table can recombine to a terminal.
            // Restore the declared three-variable universe for evaluation.
            snapshot.mince_to_depth(3u);

            std::vector<BinaryWord> expected;
            for (auto it=source.begin(), end=source.end(); it!=end; ++it) {
                expected.push_back(it->word());
            }
            std::vector<BinaryWord> actual;
            snapshot.for_each_enabled_leaf([&](GridCell const& cell) {
                actual.push_back(cell.word());
            });
            ARIADNE_TESTING_EXPECT_EQUAL(actual.size(),expected.size());
            ARIADNE_TESTING_EXPECT_EQUAL(snapshot.enabled_leaf_count(),expected.size());
            ARIADNE_TESTING_EXPECT_EQUAL(snapshot.size(),expected.size());
            std::vector<BinaryWord> incrementally_visited;
            for (auto it=snapshot.begin(), end=snapshot.end(); it!=end; ++it) {
                incrementally_visited.push_back(it->word());
            }
            ARIADNE_TESTING_PROVE(incrementally_visited == actual);
            // Copying the iterator copies its pending search frontier,
            // and post-increment retains the old position.
            if (snapshot.begin() != snapshot.end()) {
                auto iterator=snapshot.begin();
                auto copy=iterator;
                ARIADNE_TESTING_PROVE(iterator == copy);
                auto old=iterator++;
                ARIADNE_TESTING_PROVE(old == copy);
                ++copy;
                ARIADNE_TESTING_PROVE(copy == iterator);
            }
            for (SizeType i=0u; i<expected.size(); ++i) {
                ARIADNE_TESTING_PROVE(actual[i] == expected[i]);
            }
            auto reconstructed=snapshot.to_binary_tree();
            reconstructed->recombine();
            ARIADNE_TESTING_EXPECT_EQUAL(
                snapshot.enabled_leaf_count(),
                BinaryTreeNode::count_enabled_leaf_nodes(reconstructed.get()));
            SizeType expected_satisfying=0u;
            for (unsigned mask=0u; mask<8u; ++mask) {
                BinaryWord path;
                for (unsigned bit=0u; bit<3u; ++bit) {
                    path.push_back(((mask>>(2u-bit)) & 1u) != 0u);
                }
                Bool enabled=(table & (1u<<mask)) != 0u;
                ARIADNE_TESTING_EXPECT_EQUAL(snapshot.is_enabled(path),enabled);
                if (enabled) { ++expected_satisfying; }
            }
            ARIADNE_TESTING_EXPECT_EQUAL(
                snapshot.enabled_cell_count(),expected_satisfying);
        }
    }

    Void test_in_place_mutation_and_value_semantics() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        GridCell left(grid,0u,make_binary_word("0"));
        GridCell right(grid,0u,make_binary_word("1"));
        RobddPaving paving(manager,root,manager.constant(false),0u);
        BinaryTreePaving explicit_tree(grid);

        paving.adjoin(left);
        explicit_tree.adjoin(left);
        explicit_tree.recombine();
        ARIADNE_TESTING_PROVE(paving.root_cell() == root);
        ARIADNE_TESTING_EXPECT_EQUAL(paving.depth(),1u);
        ARIADNE_TESTING_EXPECT_EQUAL(paving.size(),explicit_tree.size());
        ARIADNE_TESTING_PROVE(paving.is_enabled(make_binary_word("0")));
        ARIADNE_TESTING_PROVE(not paving.is_enabled(make_binary_word("1")));

        // The snapshot and an iterator must survive subsequent root updates.
        RobddPaving snapshot(paving);
        auto previous=paving.begin();
        auto previous_end=paving.end();
        ARIADNE_TESTING_PROVE(previous->word() == left.word());

        paving.adjoin(right);
        explicit_tree.adjoin(right);
        explicit_tree.recombine();
        ARIADNE_TESTING_PROVE(paving.robdd().is_true());
        ARIADNE_TESTING_EXPECT_EQUAL(paving.size(),explicit_tree.size());
        ARIADNE_TESTING_EXPECT_EQUAL(snapshot.size(),1u);
        ARIADNE_TESTING_PROVE(snapshot.is_enabled(make_binary_word("0")));
        ARIADNE_TESTING_PROVE(not snapshot.is_enabled(make_binary_word("1")));
        ARIADNE_TESTING_PROVE(previous->word() == left.word());
        ++previous;
        ARIADNE_TESTING_PROVE(previous == previous_end);

        paving.remove(left);
        explicit_tree.remove(left);
        explicit_tree.recombine();
        ARIADNE_TESTING_PROVE(not paving.is_enabled(make_binary_word("0")));
        ARIADNE_TESTING_PROVE(paving.is_enabled(make_binary_word("1")));
        ARIADNE_TESTING_EXPECT_EQUAL(paving.size(),explicit_tree.size());

        RobddPaving other(manager,root,manager.constant(false),0u);
        other.adjoin(left);
        paving.adjoin(other);
        ARIADNE_TESTING_PROVE(paving.robdd().is_true());
        paving.restrict(other);
        ARIADNE_TESTING_PROVE(paving.is_enabled(make_binary_word("0")));
        ARIADNE_TESTING_PROVE(not paving.is_enabled(make_binary_word("1")));
        ARIADNE_TESTING_PROVE(other.is_enabled(make_binary_word("0")));
        paving.adjoin(paving);
        paving.restrict(paving);
        ARIADNE_TESTING_PROVE(paving.robdd() == other.robdd());
        paving.remove(paving);
        ARIADNE_TESTING_PROVE(paving.is_empty());

        paving = snapshot;
        ARIADNE_TESTING_PROVE(paving.robdd() == snapshot.robdd());
        paving.clear();
        ARIADNE_TESTING_PROVE(paving.is_empty());
        ARIADNE_TESTING_PROVE(snapshot.is_enabled(make_binary_word("0")));

        paving.mince_to_depth(4u);
        paving.set_root_cell(true);
        ARIADNE_TESTING_PROVE(paving.robdd().is_true());
        ARIADNE_TESTING_EXPECT_EQUAL(paving.depth(),0u);
        ARIADNE_TESTING_EXPECT_EQUAL(paving.size(),1u);
        paving.mince_to_depth(3u);
        paving.set_root_cell(false);
        ARIADNE_TESTING_PROVE(paving.is_empty());
        ARIADNE_TESTING_EXPECT_EQUAL(paving.depth(),0u);
        ARIADNE_TESTING_PROVE(paving.root_cell() == root);
    }

    Void test_in_place_mutation_across_extents() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,1u,BinaryWord{});
        GridCell coarse(grid,1u,make_binary_word("0"));
        GridCell fine(grid,0u,make_binary_word("0"));
        RobddPaving paving(manager,root,manager.constant(false),0u);
        BinaryTreePaving oracle(grid);

        paving.adjoin(coarse);
        paving.adjoin(fine);
        oracle.adjoin(coarse);
        oracle.adjoin(fine);
        oracle.recombine();
        ARIADNE_TESTING_PROVE(paving.root_cell() == root);
        ARIADNE_TESTING_EXPECT_EQUAL(paving.depth(),2u);
        ARIADNE_TESTING_EXPECT_EQUAL(paving.enabled_cell_count(),3u);
        ARIADNE_TESTING_EXPECT_EQUAL(paving.size(),oracle.size());
        for (unsigned mask=0u; mask!=4u; ++mask) {
            BinaryWord path;
            path.push_back((mask & 2u) != 0u);
            path.push_back((mask & 1u) != 0u);
            GridCell cell(grid,1u,path);
            ARIADNE_TESTING_EXPECT_EQUAL(
                paving.is_enabled(path),oracle.superset(cell));
        }

        paving.remove(coarse);
        oracle.remove(coarse);
        oracle.recombine();
        ARIADNE_TESTING_EXPECT_EQUAL(paving.size(),oracle.size());
        ARIADNE_TESTING_PROVE(not paving.is_enabled(make_binary_word("00")));
        ARIADNE_TESTING_PROVE(not paving.is_enabled(make_binary_word("01")));
        ARIADNE_TESTING_PROVE(paving.is_enabled(make_binary_word("10")));

        RobddPaving saved(paving);
        RobddManager foreign;
        RobddPaving foreign_paving(
            foreign,root,foreign.constant(true),0u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(paving.adjoin(foreign_paving));
        ARIADNE_TESTING_EXPECT_EXCEPTION(paving.restrict(foreign_paving));
        ARIADNE_TESTING_EXPECT_EXCEPTION(paving.remove(foreign_paving));
        ARIADNE_TESTING_PROVE(paving.robdd() == saved.robdd());
        ARIADNE_TESTING_PROVE(paving.root_cell() == saved.root_cell());
        ARIADNE_TESTING_EXPECT_EQUAL(paving.depth(),saved.depth());

        Grid other_grid(2u);
        GridCell incompatible(other_grid,0u,BinaryWord{});
        ARIADNE_TESTING_EXPECT_EXCEPTION(paving.adjoin(incompatible));
        ARIADNE_TESTING_EXPECT_EXCEPTION(paving.remove(incompatible));
        ARIADNE_TESTING_PROVE(paving.robdd() == saved.robdd());
    }

};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_robdd_paving");
    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_import_and_explicit_tree_oracle);
    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_terminals_and_complemented_roots);
    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_set_operations_and_preconditions);
    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_distinct_roots_and_extents);
    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_compact_leaf_iteration);
    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_compact_leaves_exhaustive_truth_tables);
    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_in_place_mutation_and_value_semantics);
    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_in_place_mutation_across_extents);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc,argv);
}
