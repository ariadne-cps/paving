/***************************************************************************
 *            test_robdd_grid_paving.cpp
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

#include "paving/binary_tree_grid_paving.hpp"
#include "paving/grid_paving.hpp"
#include "paving/robdd_grid_paving.hpp"
#include "ariadne-testing.hpp"

using namespace Ariadne;

static_assert(std::is_same_v<BinaryTreeGridPaving,GridTreePaving>);


class TestRobddGridPaving : public Ariadne::Testing::ContextFixture {
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
        RobddGridPaving snapshot(manager,source);
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
        auto full = RobddGridPaving(manager,root,manager.constant(true),0u);
        auto empty = RobddGridPaving(manager,root,manager.constant(false),0u);
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
        RobddGridPaving imported(manager,source);
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
        RobddGridPaving lhs(manager,left_explicit);
        RobddGridPaving rhs(manager,right_explicit);
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
            RobddGridPaving(manager,root,foreign_value,1u));
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            RobddGridPaving(manager,root,manager.variable(2u),2u));
        RobddGridPaving other_manager(foreign,root,foreign.variable(0u),1u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(lhs.join(other_manager));
        ARIADNE_TESTING_EXPECT_EXCEPTION(lhs.subset(other_manager));
        GridCell different_root(grid,0u,make_binary_word("0"));
        RobddGridPaving wrong_cell(manager,different_root,manager.variable(0u),1u);
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
        RobddGridPaving lhs(manager,left_cell,enabled,0u);
        RobddGridPaving rhs(manager,right_cell,enabled,0u);

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
        auto parent = RobddGridPaving(manager,ancestor,enabled,0u);
        ARIADNE_TESTING_PROVE(lhs.subset(parent));
        ARIADNE_TESTING_PROVE(parent.intersects(lhs));
        ARIADNE_TESTING_PROVE(parent.join(lhs).robdd().is_true());
        auto parent_minus_left = parent.difference(lhs);
        ARIADNE_TESTING_EXPECT_EQUAL(parent_minus_left.enabled_cell_count(),1u);
        ARIADNE_TESTING_PROVE(parent_minus_left.is_enabled(make_binary_word("1")));

        GridCell deep_cell(grid,0u,make_binary_word("010"));
        RobddGridPaving deep(manager,deep_cell,enabled,0u);
        auto shared = lhs.join(deep);
        ARIADNE_TESTING_PROVE(shared.root_cell() == left_cell);
        ARIADNE_TESTING_PROVE(shared.robdd().is_true());
        ARIADNE_TESTING_PROVE(deep.subset(lhs));
        ARIADNE_TESTING_EXPECT_EQUAL(
            deep.intersection(lhs).depth(),2u);

        // Extent 1 has primary path '1' to extent 0 in dimension 1.
        GridCell raised_cell(grid,1u,BinaryWord{});
        auto raised = RobddGridPaving(manager,raised_cell,enabled,0u);
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
        RobddGridPaving distant(manager,distant_cell,enabled,0u);
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
        RobddGridPaving incompatible(
            manager,GridCell(other_grid,0u,BinaryWord{}),disabled,0u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(lhs.join(incompatible));
        ARIADNE_TESTING_EXPECT_EXCEPTION(lhs.subset(incompatible));
    }

};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_robdd_grid_paving");
    ARIADNE_TESTING_ADD_TEST(TestRobddGridPaving, test_import_and_explicit_tree_oracle);
    ARIADNE_TESTING_ADD_TEST(TestRobddGridPaving, test_terminals_and_complemented_roots);
    ARIADNE_TESTING_ADD_TEST(TestRobddGridPaving, test_set_operations_and_preconditions);
    ARIADNE_TESTING_ADD_TEST(TestRobddGridPaving, test_distinct_roots_and_extents);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc,argv);
}
