/***************************************************************************
 *            test_grid_cell_alignment.cpp
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

#include "paving/grid_cell_alignment.hpp"
#include "ariadne-testing.hpp"

using namespace Ariadne;

class TestGridCellAlignment : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

    Void test_common_ancestor() {
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        GridCell left(grid,0u,make_binary_word("010"));
        GridCell right(grid,0u,make_binary_word("0111"));
        GridCell common(grid,0u,make_binary_word("01"));
        ARIADNE_TESTING_PROVE(common_root_cell(left,right) == common);
        ARIADNE_TESTING_PROVE(common_root_cell(right,left) == common);
        ARIADNE_TESTING_PROVE(common_root_cell(left,left) == left);
        ARIADNE_TESTING_PROVE(common_root_cell(root,left) == root);
        ARIADNE_TESTING_EXPECT_EQUAL(
            relative_cell_path(common,left),make_binary_word("0"));
        ARIADNE_TESTING_EXPECT_EQUAL(
            relative_cell_path(common,right),make_binary_word("11"));
        ARIADNE_TESTING_EXPECT_EQUAL(
            relative_cell_path(root,left),make_binary_word("010"));
        ARIADNE_TESTING_EXPECT_EQUAL(
            relative_cell_path(left,left),BinaryWord{});
        ARIADNE_TESTING_EXPECT_EQUAL(
            cell_path_at_extent(left,0u),left.word());
    }

    Void test_cross_extent() {
        Grid grid(1u);
        GridCell ancestor(grid,1u,BinaryWord{});
        GridCell fine(grid,0u,make_binary_word("0"));
        GridCell equivalent(grid,1u,make_binary_word("10"));
        ARIADNE_TESTING_EXPECT_EQUAL(
            cell_path_at_extent(fine,1u),make_binary_word("10"));
        ARIADNE_TESTING_PROVE(common_root_cell(fine,ancestor) == ancestor);
        ARIADNE_TESTING_PROVE(common_root_cell(ancestor,fine) == ancestor);
        ARIADNE_TESTING_PROVE(common_root_cell(fine,equivalent) == equivalent);
        ARIADNE_TESTING_EXPECT_EQUAL(
            relative_cell_path(ancestor,fine),make_binary_word("10"));
        ARIADNE_TESTING_EXPECT_EQUAL(
            relative_cell_path(equivalent,fine),BinaryWord{});

        GridCell coarse(grid,1u,make_binary_word("0"));
        GridCell root(grid,1u,BinaryWord{});
        ARIADNE_TESTING_PROVE(common_root_cell(coarse,fine) == root);
        ARIADNE_TESTING_EXPECT_EQUAL(
            relative_cell_path(root,coarse),make_binary_word("0"));
        ARIADNE_TESTING_EXPECT_EQUAL(
            relative_cell_path(root,fine),make_binary_word("10"));
    }

    Void test_multiple_dimensions_and_preconditions() {
        Grid grid(2u);
        GridCell ancestor(grid,2u,make_binary_word("0"));
        GridCell descendant(grid,0u,make_binary_word("10"));
        GridCell other_branch(grid,2u,make_binary_word("1"));
        // Primary-cell path from extent 2 to extent 0 is "0011".
        ARIADNE_TESTING_EXPECT_EQUAL(
            cell_path_at_extent(descendant,2u),make_binary_word("001110"));
        ARIADNE_TESTING_PROVE(
            common_root_cell(ancestor,descendant) == ancestor);
        ARIADNE_TESTING_EXPECT_EQUAL(
            relative_cell_path(ancestor,descendant),make_binary_word("01110"));
        ARIADNE_TESTING_PROVE(
            common_root_cell(ancestor,other_branch) ==
            GridCell(grid,2u,BinaryWord{}));

        ARIADNE_TESTING_EXPECT_EXCEPTION(cell_path_at_extent(ancestor,1u));
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            relative_cell_path(descendant,ancestor));
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            relative_cell_path(ancestor,other_branch));

        Grid different_grid(1u);
        GridCell incompatible(different_grid,0u,BinaryWord{});
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            common_root_cell(ancestor,incompatible));
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            relative_cell_path(ancestor,incompatible));
    }
};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_grid_cell_alignment");
    ARIADNE_TESTING_ADD_TEST(TestGridCellAlignment,test_common_ancestor);
    ARIADNE_TESTING_ADD_TEST(TestGridCellAlignment,test_cross_extent);
    ARIADNE_TESTING_ADD_TEST(
        TestGridCellAlignment,test_multiple_dimensions_and_preconditions);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc,argv);
}
