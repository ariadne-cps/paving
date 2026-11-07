/***************************************************************************
 *            test_binary_tree_paving.cpp
 *
 *  Copyright  2008-20  Ivan S. Zapreev, Pieter Collins
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne.
 *
 *  Ariadne is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <memory>
#include <utility>
#include <vector>
#include "utility/stlio.hpp"

#include "utility/macros.hpp"
#include "foundation/logical.hpp"
#include "numeric/numeric.hpp"

#include "tree/binary_tree.hpp"
#include "set/function_set.hpp"
#include "paving/binary_tree_paving.hpp"
#include "geometry/set_interface.hpp"
#include "geometry/set_wrapper.hpp"
#include "function/projection.hpp"

#include "ariadne-testing.hpp"

using namespace Ariadne;
using namespace std;

static const Nat extentZero = 0;
static const Nat extentOne = 1;
static const Nat extentTwo = 2;
static const Nat extentThree = 3;
static const Nat extentFour = 4;

namespace {


class TestBinaryTreePaving : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

    template<class ExpectedCells>
    Void check_grid_tree_subpaving_iterator(
        ExpectedCells const& expected_result,
        BinaryTreeSubpaving const& paving,
        SizeType expected_number_elements) {
        SizeType elements_count=0u;
        for (BinaryTreeSubpaving::ConstIterator it=paving.begin(), end=paving.end();
             it!=end; it++, elements_count++) {
            if (elements_count < expected_number_elements) {
                ARIADNE_TESTING_NOTE("The next Iterator node is:");
                ARIADNE_TESTING_EXPECT_EQUAL(*expected_result[elements_count], *it);
            }
        }
        ARIADNE_TESTING_NOTE("Test that we iterated through the right number of nodes");
        ARIADNE_TESTING_EXPECT_EQUAL(elements_count,expected_number_elements);
    }

    template<class Item>
    Void clean_test_vector(std::vector<Item*>& values) {
        for (Item*& value : values) { delete value; value=nullptr; }
    }

    Void test_grid_paving_cursor(){

    //Allocate the Grid
    Grid theGrid( {-0.25, 0.25, 1.5}, {0.25, 0.25, 0.25} );

    //Define the higth of the primary root cell.
    const Nat theExtent = 2;

    //Create the binary tree;
    BinaryTreeNode * pRootTreeNode = new BinaryTreeNode(true);

    //Mince the tree to the certain higth
    pRootTreeNode->mince(3);

    //Mark some nodes as disabled
    pRootTreeNode->left_node()->right_node()->left_node()->set_disabled();
    pRootTreeNode->left_node()->left_node()->right_node()->set_disabled();
    pRootTreeNode->right_node()->right_node()->right_node()->set_disabled();

    //Create the path to the root node of the subpaving, which
    //corresponds to pRootTreeNode->left_node()->right_node()
    BinaryWord thePathToSubPavingRoot;
    thePathToSubPavingRoot.push_back(false);
    thePathToSubPavingRoot.push_back(true);

    //Create the BinaryTreeSubpaving
    BinaryTreeSubpaving theGridSubPavingSmall( theGrid, theExtent, thePathToSubPavingRoot, pRootTreeNode->left_node()->right_node() );
    //Create the Cursor for the BinaryTreeSubpaving
    BinaryTreePavingCursor theGSPCursorSmall( &theGridSubPavingSmall );

    //Create the BinaryTreeSubpaving
    BinaryTreeSubpaving theGridSubPavingLarge( theGrid, theExtent, BinaryWord(), pRootTreeNode );
    //Create the Cursor for the BinaryTreeSubpaving
    BinaryTreePavingCursor theGSPCursorLarge( &theGridSubPavingLarge );

    // !!!
    ARIADNE_TESTING_NOTE("Check the created Cursor data on a small subpaving");
    ExactBoxType expected_box = make_box("[-0.5,0]x[0.5,1]x[1.25,2.25]");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_box, theGSPCursorSmall.cell().box() );

    // !!!
    ARIADNE_TESTING_NOTE("Check the created Cursor data on a large subpaving");
    expected_box = make_box("[-0.5,0.5]x[0,1]x[1.25,2.25]");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_box, theGSPCursorLarge.cell().box() );

    // !!!
    ARIADNE_TESTING_NOTE("Moving up from the Cursor's root node should cause the NotAllowedMoveException exception");
    ARIADNE_TESTING_EXPECT_THROWS(NotAllowedMoveException, theGSPCursorSmall.move_up());

    // !!!
    ARIADNE_TESTING_NOTE("Let's move the cursor of theGSPCursorLarge: left, right, we should be in the root of the smaller subpaving then");
    theGSPCursorLarge.move_left();
    theGSPCursorLarge.move_right();
    BinaryTreeSubpaving tmpSubPaving = *theGSPCursorLarge;
    BinaryTreeNode expected_subset_root(*theGridSubPavingSmall.binary_tree());
    BinaryTreeSubpaving expected_tree_subset( theGrid, theExtent, make_binary_word("01"), &expected_subset_root );
    ARIADNE_TESTING_NOTE("The cursor has moved and we have the following subpaving: ");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_tree_subset, tmpSubPaving);

    expected_box = make_box("[-0.5,0]x[0.5,1]x[1.25,2.25]");
    ARIADNE_TESTING_NOTE("The box pointed by the cursor is: ");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_box, theGSPCursorLarge.cell().box() );

    // !!!
    ARIADNE_TESTING_NOTE("Check the is_enabled/disabled/leaf/root functions, while moving the cursor in the large and small subpavings");
    ARIADNE_TESTING_EXPECT_EQUAL(theGSPCursorSmall.is_root(), true);
    ARIADNE_TESTING_EXPECT_EQUAL(theGSPCursorSmall.is_leaf(), false);
    ARIADNE_TESTING_EXPECT_EQUAL(theGSPCursorLarge.is_root(), false);
    ARIADNE_TESTING_EXPECT_EQUAL(theGSPCursorLarge.is_leaf(), false);
    ARIADNE_TESTING_EXPECT_EQUAL(theGSPCursorLarge.is_enabled(), false);
    ARIADNE_TESTING_EXPECT_EQUAL(theGSPCursorLarge.is_disabled(), false);
    ARIADNE_TESTING_NOTE("Moving to the left, to a disabled node");
    theGSPCursorLarge.move_left();
    ARIADNE_TESTING_EXPECT_EQUAL(theGSPCursorLarge.is_leaf(), true);
    ARIADNE_TESTING_EXPECT_EQUAL(theGSPCursorLarge.is_root(), false);
    ARIADNE_TESTING_EXPECT_EQUAL(theGSPCursorLarge.is_enabled(), false);
    ARIADNE_TESTING_EXPECT_EQUAL(theGSPCursorLarge.is_disabled(), true);
    ARIADNE_TESTING_NOTE("Moving up and to the right, to an enabled node");
    theGSPCursorLarge.move_up();
    theGSPCursorLarge.move_right();
    ARIADNE_TESTING_EXPECT_EQUAL(theGSPCursorLarge.is_leaf(), true);
    ARIADNE_TESTING_EXPECT_EQUAL(theGSPCursorLarge.is_root(), false);
    ARIADNE_TESTING_EXPECT_EQUAL(theGSPCursorLarge.is_enabled(), true);
    ARIADNE_TESTING_EXPECT_EQUAL(theGSPCursorLarge.is_disabled(), false);

    // !!!
    ARIADNE_TESTING_NOTE("Test that moving to the left and right with the leaf nodes causes the NotAllowedMoveException exception");
    ARIADNE_TESTING_NOTE("Get to the leaf node on the left");
    theGSPCursorSmall.move_left();

    ARIADNE_TESTING_NOTE("Test the present state of theGSPCursorSmall");
    expected_box = make_box("[-0.5,0]x[0.5,1]x[1.25,1.75]");
    ARIADNE_TESTING_NOTE("The box pointed by the cursor is: ");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_box, theGSPCursorSmall.cell().box() );

    ARIADNE_TESTING_NOTE("Try to get to the node on the left");
    ARIADNE_TESTING_EXPECT_THROWS(NotAllowedMoveException, theGSPCursorSmall.move_left());

    ARIADNE_TESTING_NOTE("Try to get to the node on the right");
    ARIADNE_TESTING_EXPECT_THROWS(NotAllowedMoveException, theGSPCursorSmall.move_right());

    ARIADNE_TESTING_NOTE("The state of theGSPCursorSmall is supposed to remain unchanged");
    ARIADNE_TESTING_NOTE("The box pointed by the cursor is: ");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_box, theGSPCursorSmall.cell().box() );

    // !!!
    ARIADNE_TESTING_NOTE("Test the set_enabled/set_disabled methods of the cursor");
    delete pRootTreeNode;
    pRootTreeNode = new BinaryTreeNode(true);
    ARIADNE_TESTING_NOTE("Mince the enabled binary tree node to level 2");
    pRootTreeNode->mince(2);
    BinaryTreePaving theBinaryTreePaving( theGrid, theExtent, pRootTreeNode );
    //Create the Cursor for the BinaryTreeSubpaving
    BinaryTreePavingCursor theGPCursor( &theBinaryTreePaving );
    //Move to the leaf node
    ARIADNE_TESTING_NOTE("Move the cursor to the right most leaf and check it");
    theGPCursor.move_right();
    theGPCursor.move_right();
    ARIADNE_TESTING_EXPECT_EQUAL(theGPCursor.is_leaf(), true);
    ARIADNE_TESTING_EXPECT_EQUAL(theGPCursor.is_root(), false);
    ARIADNE_TESTING_EXPECT_EQUAL(theGPCursor.is_enabled(), true);
    ARIADNE_TESTING_EXPECT_EQUAL(theGPCursor.is_disabled(), false);
    ARIADNE_TESTING_NOTE("Set the leaf as disabled");
    theGPCursor.set_disabled();
    ARIADNE_TESTING_EXPECT_EQUAL(theGPCursor.is_disabled(), true);
    ARIADNE_TESTING_NOTE("Set the leaf (back) as enabled");
    theGPCursor.set_enabled();
    ARIADNE_TESTING_EXPECT_EQUAL(theGPCursor.is_enabled(), true);
    ARIADNE_TESTING_NOTE("Move one node up, to a non-leaf node and try to enable it, the NotALeafNodeException should be thrown");
    theGPCursor.move_up();
    ARIADNE_TESTING_EXPECT_THROWS(NotALeafNodeException, theGPCursor.set_disabled());
}


Void test_cursor_lazy_cell_materialization() {
    Grid grid(2u);
    BinaryTreeNode root(true);
    root.mince(3u);
    // Non-primary subview: its root already has a nonempty binary path.
    BinaryTreeSubpaving view(grid,1u,make_binary_word("01"),&root);
    BinaryTreePavingCursor cursor(&view);
    cursor.move_right();
    cursor.move_left();
    cursor.move_up();
    cursor.move_right();
    GridCell expected(grid,1u,make_binary_word("0111"));
    ARIADNE_TESTING_EXPECT_EQUAL(cursor.cell().word(),expected.word());
    ARIADNE_TESTING_EXPECT_EQUAL(cursor.cell().box(),expected.box());
    ARIADNE_TESTING_EXPECT_EQUAL((*cursor).root_cell().box(),expected.box());

    // Copy and assign a cursor while its cached geometry is stale.
    cursor.move_up();
    BinaryTreePavingCursor copied(cursor);
    BinaryTreePavingCursor assigned;
    assigned=cursor;
    cursor.move_left();
    GridCell parent(grid,1u,make_binary_word("011"));
    GridCell left(grid,1u,make_binary_word("0110"));
    ARIADNE_TESTING_EXPECT_EQUAL(copied.cell().box(),parent.box());
    ARIADNE_TESTING_EXPECT_EQUAL(assigned.cell().box(),parent.box());
    ARIADNE_TESTING_EXPECT_EQUAL(cursor.cell().box(),left.box());
    ARIADNE_TESTING_EXPECT_EQUAL(cursor.cell().box(),left.box());
    cursor.move_up();
    ARIADNE_TESTING_EXPECT_EQUAL(cursor.cell().box(),parent.box());
    cursor.move_up();
    ARIADNE_TESTING_EXPECT_EQUAL(cursor.cell().box(),
        GridCell(grid,1u,make_binary_word("01")).box());

    // Both iterator modes must return the same geometrically valid cells.
    BinaryTreePaving owning(grid,1u,new BinaryTreeNode(root));
    BinaryTreeSubpaving const& borrowed=owning;
    auto bi=borrowed.begin(),be=borrowed.end();
    auto oi=owning.begin(),oe=owning.end();
    SizeType count=0u;
    for(;bi!=be && oi!=oe;++bi,++oi) {
        ARIADNE_TESTING_EXPECT_EQUAL(bi->word(),oi->word());
        ARIADNE_TESTING_EXPECT_EQUAL(bi->box(),oi->box());
        GridCell canonical(grid,1u,bi->word());
        ARIADNE_TESTING_EXPECT_EQUAL(bi->box(),canonical.box());
        ++count;
    }
    ARIADNE_TESTING_EXPECT_EQUAL(bi==be,true);
    ARIADNE_TESTING_EXPECT_EQUAL(oi==oe,true);
    ARIADNE_TESTING_EXPECT_EQUAL(count,8u);
}

Void test_grid_paving_const_iterator(){
    std::vector< GridCell *> expected_result( 8 );

    //Allocate the Grid
    Grid theGrid( {-0.25, 0.25}, {0.25, 0.25} );

    //Define the higth of the primary root cell.
    const Nat theExtent = 2;

    //Create the binary tree;
    BinaryTreeNode rootTreeNode(true);
    BinaryTreeNode * pRootTreeNode = &rootTreeNode;

    //Create the BinaryTreeSubpaving
    BinaryTreeSubpaving theGridSubPavingLarge( theGrid, theExtent, BinaryWord(), pRootTreeNode );

    // !!!
    ARIADNE_TESTING_NOTE("Test the sequence in which GridPavingIterator goes through the tree leafs ");
    ARIADNE_TESTING_NOTE("The tree depth is 0 and all leaf nodes are enabled");

    expected_result[0] = new GridCell( theGrid, 2, make_binary_word("") );

    check_grid_tree_subpaving_iterator( expected_result, theGridSubPavingLarge, 1 );
    clean_test_vector( expected_result );

    // !!!
    ARIADNE_TESTING_NOTE("Test the sequence in which GridPavingIterator goes through the tree leafs ");
    ARIADNE_TESTING_NOTE("The tree depth is 1 and all leaf nodes are enabled");

    //Mince the tree to the certain higth
    pRootTreeNode->mince(1);

    expected_result[0] =  new GridCell( theGrid, 2, make_binary_word("0") );
    expected_result[1] =  new GridCell( theGrid, 2, make_binary_word("1") );

    check_grid_tree_subpaving_iterator( expected_result, theGridSubPavingLarge, 2 );
    clean_test_vector( expected_result );

    // !!!
    ARIADNE_TESTING_NOTE("Test the sequence in which GridPavingIterator goes through the tree leafs ");
    ARIADNE_TESTING_NOTE("The tree depth is 2 and all leaf nodes are enabled");

    //Mince the tree to the certain higth
    pRootTreeNode->mince(2);

    expected_result[0] =  new GridCell( theGrid, 2, make_binary_word("00") );
    expected_result[1] =  new GridCell( theGrid, 2, make_binary_word("01") );
    expected_result[2] =  new GridCell( theGrid, 2, make_binary_word("10") );
    expected_result[3] =  new GridCell( theGrid, 2, make_binary_word("11") );

    check_grid_tree_subpaving_iterator( expected_result, theGridSubPavingLarge, 4 );
    clean_test_vector( expected_result );

    // !!!
    ARIADNE_TESTING_NOTE("Test the sequence in which GridPavingIterator goes through the tree leafs ");
    ARIADNE_TESTING_NOTE("The tree depth is 3 and all leaf nodes are enabled");

    //Mince the tree to the certain higth
    pRootTreeNode->mince(3);

    expected_result[0] = new GridCell( theGrid, 2, make_binary_word("000") );
    expected_result[1] = new GridCell( theGrid, 2, make_binary_word("001") );
    expected_result[2] = new GridCell( theGrid, 2, make_binary_word("010") );
    expected_result[3] = new GridCell( theGrid, 2, make_binary_word("011") );
    expected_result[4] = new GridCell( theGrid, 2, make_binary_word("100") );
    expected_result[5] = new GridCell( theGrid, 2, make_binary_word("101") );
    expected_result[6] = new GridCell( theGrid, 2, make_binary_word("110") );
    expected_result[7] = new GridCell( theGrid, 2, make_binary_word("111") );

    check_grid_tree_subpaving_iterator( expected_result, theGridSubPavingLarge, 8 );
    clean_test_vector( expected_result );

    // !!!
    ARIADNE_TESTING_NOTE("Disable some of the leaf nodes and test GridPavingIterator");
    ARIADNE_TESTING_NOTE("The tree depth is 3 and some tree nodes are disabled");

    //Mark some nodes as disabled
    pRootTreeNode->left_node()->left_node()->left_node()->set_disabled();
    pRootTreeNode->left_node()->right_node()->right_node()->set_disabled();
    pRootTreeNode->right_node()->left_node()->left_node()->set_disabled();
    pRootTreeNode->right_node()->left_node()->right_node()->set_disabled();

    //Reuse the previous results
    std::vector< GridCell *>  expected_result_tmp(4);
    expected_result_tmp[0] = new GridCell( theGrid, 2, make_binary_word("001") );
    expected_result_tmp[1] = new GridCell( theGrid, 2, make_binary_word("010") );
    expected_result_tmp[2] = new GridCell( theGrid, 2, make_binary_word("110") );
    expected_result_tmp[3] = new GridCell( theGrid, 2, make_binary_word("111") );

    check_grid_tree_subpaving_iterator( expected_result_tmp, theGridSubPavingLarge, 4 );
    clean_test_vector( expected_result_tmp );

    // !!!
    ARIADNE_TESTING_NOTE("Recombine the tree and test GridPavingIterator");
    ARIADNE_TESTING_NOTE("The tree depth is 3 but some nodes are at depth 2");

    //Recombine the paving
    theGridSubPavingLarge.recombine();

    //Reuse some of the previous results
    expected_result_tmp[0] = new GridCell( theGrid, 2, make_binary_word("001") );
    expected_result_tmp[1] = new GridCell( theGrid, 2, make_binary_word("010") );
    expected_result_tmp[2] = new GridCell( theGrid, 2, make_binary_word("11") );

    check_grid_tree_subpaving_iterator( expected_result_tmp, theGridSubPavingLarge, 3 );
    clean_test_vector( expected_result_tmp );

    // !!!
    ARIADNE_TESTING_NOTE("Mince the tree back to level 3, enable/disable some nodes, recombine and and test GridPavingIterator");
    ARIADNE_TESTING_NOTE("The tree depth is 3 but some nodes are at depth 2");

    //Mince the tree to the certain higth, (only for enabled nodes)
    pRootTreeNode->mince(3);
    //Split the disabled node
    pRootTreeNode->right_node()->left_node()->split();

    //Mark some nodes as disabled/enabled
    pRootTreeNode->right_node()->right_node()->left_node()->set_disabled();
    pRootTreeNode->right_node()->left_node()->left_node()->set_enabled();
    pRootTreeNode->right_node()->left_node()->right_node()->set_enabled();

    //Recombine the paving
    theGridSubPavingLarge.recombine();

    //Reuse some of the previous result strings
    expected_result_tmp[0] = new GridCell( theGrid, 2, make_binary_word("001") );
    expected_result_tmp[1] = new GridCell( theGrid, 2, make_binary_word("010") );
    expected_result_tmp[2] = new GridCell( theGrid, 2, make_binary_word("10") );
    expected_result_tmp[3] = new GridCell( theGrid, 2, make_binary_word("111") );

    check_grid_tree_subpaving_iterator( expected_result_tmp, theGridSubPavingLarge, 4 );
    clean_test_vector( expected_result_tmp );

    // !!!
    ARIADNE_TESTING_NOTE("Test how the constant Cursor can be retrieved from the Constant Iterator");
    BinaryTreeSubpaving::ConstIterator it = theGridSubPavingLarge.begin();
    const BinaryTreePavingCursor theGPCursor = it.cursor();
    ARIADNE_TESTING_EXPECT_EQUAL(theGPCursor.is_leaf(), true);
    ARIADNE_TESTING_EXPECT_EQUAL(theGPCursor.is_root(), false);
    ARIADNE_TESTING_EXPECT_EQUAL(theGPCursor.is_enabled(), true);
    ARIADNE_TESTING_EXPECT_EQUAL(theGPCursor.is_disabled(), false);
    ARIADNE_TESTING_NOTE("Set the leaf as disabled");
    theGPCursor.set_disabled();
    ARIADNE_TESTING_EXPECT_EQUAL(theGPCursor.is_disabled(), true);
    ARIADNE_TESTING_NOTE("Set the leaf (back) as enabled");
    theGPCursor.set_enabled();
    ARIADNE_TESTING_EXPECT_EQUAL(theGPCursor.is_enabled(), true);
}

Void test_grid_sub_paving_one(){

    //Allocate the Grid, one Dimension
    Vector<ApproximateDouble> originOne(1, 0.0_x);
    Vector<ApproximateDouble> lengthsOne(1, 0.5_x );
    const Grid theOneDimGrid( originOne, lengthsOne );

    //Define the higth of the primary root cell.
    const Nat theRootExtent = 2;

    //Create the binary tree;
    BinaryTreeNode rootTreeNode(true);
    BinaryTreeNode * pRootTreeNode = &rootTreeNode;

    //Create the path to the root node of the subpaving, which
    //corresponds to pRootTreeNode->left_node()->right_node()
    BinaryWord thePathToSubPavingRoot;
    thePathToSubPavingRoot.push_back(false);
    thePathToSubPavingRoot.push_back(true);

    //Create the BinaryTreeSubpaving
    BinaryTreeSubpaving theGridSPOneDim( theOneDimGrid, theRootExtent, thePathToSubPavingRoot, pRootTreeNode );

    //
    ARIADNE_TESTING_NOTE("Test Mincing operations of BinaryTreeSubpaving on the one dimensional Grid");
    ARIADNE_TESTING_NOTE("The initial paving: ");
    ARIADNE_TESTING_EXPECT_EQUAL( theGridSPOneDim.grid() , theOneDimGrid );
    ARIADNE_TESTING_EXPECT_EQUAL( theGridSPOneDim.root_cell().root_extent() , theRootExtent );
    ARIADNE_TESTING_EXPECT_EQUAL( theGridSPOneDim.root_cell().word() , thePathToSubPavingRoot );
    ARIADNE_TESTING_EXPECT_EQUAL( (*theGridSPOneDim.binary_tree()) , (*pRootTreeNode) );
    ARIADNE_TESTING_EXPECT_EQUAL( theGridSPOneDim.root_cell().box() , make_box("[0,0.5]") );

    //Mince to the level two from the root of the paving, this should give
    //us 4 leaf nodes with intervals of width 0.125 (in the original space)
    theGridSPOneDim.mince_to_tree_depth(2);

    ARIADNE_TESTING_NOTE("Minced the sub-paving to depth 2");
    std::vector< GridCell* > expected_result_arr(4);

    expected_result_arr[0] = new GridCell( theOneDimGrid, theRootExtent, make_binary_word("0100") );
    expected_result_arr[1] = new GridCell( theOneDimGrid, theRootExtent, make_binary_word("0101") );
    expected_result_arr[2] = new GridCell( theOneDimGrid, theRootExtent, make_binary_word("0110") );
    expected_result_arr[3] = new GridCell( theOneDimGrid, theRootExtent, make_binary_word("0111") );

    check_grid_tree_subpaving_iterator( expected_result_arr, theGridSPOneDim, 4 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("Recombine and subdivide the sub-paving to cell width 1.1");
    theGridSPOneDim.recombine();
    theGridSPOneDim.subdivide(1.1);
    ARIADNE_TESTING_NOTE( "We should have a single node as in the initial sub paving: ");
    ARIADNE_TESTING_EXPECT_EQUAL( theGridSPOneDim.binary_tree()->is_leaf(), true );
    ARIADNE_TESTING_EXPECT_EQUAL( theGridSPOneDim.binary_tree()->is_enabled(), true );

    ARIADNE_TESTING_NOTE("Subdivide the sub-paving to cell width 0.4, this should give us two sub cells");
    theGridSPOneDim.subdivide(0.4);

    expected_result_arr[0] = new GridCell( theOneDimGrid, theRootExtent, make_binary_word("010") );
    expected_result_arr[1] = new GridCell( theOneDimGrid, theRootExtent, make_binary_word("011") );

    check_grid_tree_subpaving_iterator( expected_result_arr, theGridSPOneDim, 2 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("Subdivide the sub-paving to cell width 0.126, this should give us four sub cells");
    theGridSPOneDim.subdivide(0.126);

    expected_result_arr[0] = new GridCell( theOneDimGrid, theRootExtent, make_binary_word("0100") );
    expected_result_arr[1] = new GridCell( theOneDimGrid, theRootExtent, make_binary_word("0101") );
    expected_result_arr[2] = new GridCell( theOneDimGrid, theRootExtent, make_binary_word("0110") );
    expected_result_arr[3] = new GridCell( theOneDimGrid, theRootExtent, make_binary_word("0111") );

    check_grid_tree_subpaving_iterator( expected_result_arr, theGridSPOneDim, 4 );
    //NOTE: To not clear the vector, the expected result is reused in the next test;

    ARIADNE_TESTING_NOTE("Recombine and Subdivide the sub-paving to cell width 0.126, this should give us four sub cells");
    theGridSPOneDim.recombine();
    theGridSPOneDim.subdivide(0.126);

    check_grid_tree_subpaving_iterator( expected_result_arr, theGridSPOneDim, 4 );
    clean_test_vector( expected_result_arr );

    // !!!
    ARIADNE_TESTING_NOTE("Test Mincing operations of BinaryTreeSubpaving on the two dimensional Grid");
    //Allocate the Grid, one Dimension
    const Grid theTwoDimGrid( {-0.25, 0.5}, {0.25, 0.5} );

    //Create the BinaryTreeSubpaving
    BinaryTreeSubpaving theGridSPTwoDim( theTwoDimGrid, theRootExtent, thePathToSubPavingRoot, pRootTreeNode );

    ARIADNE_TESTING_NOTE("Recombine and Mince the sub-paving to depth 2, this should give us four sub cells");
    theGridSPTwoDim.recombine();
    theGridSPTwoDim.mince_to_tree_depth(2);

    expected_result_arr[0] =  new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("0100") );
    expected_result_arr[1] =  new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("0101") );
    expected_result_arr[2] =  new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("0110") );
    expected_result_arr[3] =  new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("0111") );

    check_grid_tree_subpaving_iterator( expected_result_arr, theGridSPTwoDim, 4 );
    //Do not clean the Array yet, the result well be reused

    ARIADNE_TESTING_NOTE("Recombine and Subdivide the sub-paving to cell width 0.5, this should give us four sub cells");
    //At this moment the coordinate cell widths are: for x -- 0.25 and for y -- 0.5
    theGridSPTwoDim.recombine();
    theGridSPTwoDim.subdivide(0.51);

    check_grid_tree_subpaving_iterator( expected_result_arr, theGridSPTwoDim, 4 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("Subdivide the sub-paving to cell width 0.4, this should give us sixteen sub cells");
    //At this moment the coordinate cell widths are: for x -- 0.25 and for y -- 0.5
    theGridSPTwoDim.subdivide(0.4);

    BinaryTreeNode binaryTree( make_binary_word("1111001001100100111001001100100"), make_binary_word("1111111111111111") );
    BinaryTreeSubpaving expected_result( theTwoDimGrid, theRootExtent, make_binary_word("01"), &binaryTree );
    ARIADNE_TESTING_NOTE("The sub paving with 16 leaf nodes: ");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_result, theGridSPTwoDim );

    ARIADNE_TESTING_NOTE("The depth of the sub-paving's tree should be 4");
    ARIADNE_TESTING_EXPECT_EQUAL( theGridSPTwoDim.tree_depth(), 4u );
}

Void test_grid_sub_paving_two() {
    //Allocate a trivial Grid two dimensional grid
    Grid theTwoDimGrid(2, 1.0);

    //Define the higth of the primary root cell.
    const Nat theRootExtent = 2;

    //Create the binary tree;
    BinaryTreeNode rootTreeNode(true);
    BinaryTreeNode * pRootTreeNode = &rootTreeNode;

    //Create the path to the root node of the subpaving, which
    //corresponds to pRootTreeNode->left_node()->left_node()
    BinaryWord thePathToSubPavingRoot;
    thePathToSubPavingRoot.push_back(false);
    thePathToSubPavingRoot.push_back(false);

    //Create the BinaryTreeSubpaving
    BinaryTreeSubpaving theGridSPOneDim( theTwoDimGrid, theRootExtent, thePathToSubPavingRoot, pRootTreeNode );
    std::vector< GridCell* > expected_result_arr( 16 );

    ARIADNE_TESTING_NOTE("Take the [-1,1]x[-1,1] box and do mince(0), i.e. down to the lovel of zero cells");
    expected_result_arr[0]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("0000") );
    expected_result_arr[1]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("0001") );
    expected_result_arr[2]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("0010") );
    expected_result_arr[3]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("0011") );
    theGridSPOneDim.mince(0);
    check_grid_tree_subpaving_iterator( expected_result_arr, theGridSPOneDim, 4 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("Take the [-1,1]x[-1,1] box and do mince(1), i.e. one time in each dimension from the zero cell level");
    expected_result_arr[0]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("000000") );
    expected_result_arr[1]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("000001") );
    expected_result_arr[2]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("000010") );
    expected_result_arr[3]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("000011") );
    expected_result_arr[4]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("000100") );
    expected_result_arr[5]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("000101") );
    expected_result_arr[6]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("000110") );
    expected_result_arr[7]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("000111") );
    expected_result_arr[8]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("001000") );
    expected_result_arr[9]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("001001") );
    expected_result_arr[10] = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("001010") );
    expected_result_arr[11] = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("001011") );
    expected_result_arr[12] = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("001100") );
    expected_result_arr[13] = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("001101") );
    expected_result_arr[14] = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("001110") );
    expected_result_arr[15] = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("001111") );
    theGridSPOneDim.mince(1);
    check_grid_tree_subpaving_iterator( expected_result_arr, theGridSPOneDim, 16 );
    clean_test_vector( expected_result_arr );
}

Void test_grid_sub_paving_three() {
    //Allocate a trivial Grid two dimensional grid
    Grid theTwoDimGrid(2, 1.0);

    //Define the higth of the primary root cell.
    const Nat theRootExtent = 1;

    //Create the binary tree;
    BinaryTreeNode rootTreeNode(true);
    BinaryTreeNode * pRootTreeNode = &rootTreeNode;

    //Create the path to the root node of the subpaving, which corresponds
    //to pRootTreeNode->left_node()->left_node()->left_node()->left_node()
    BinaryWord thePathToSubPavingRoot = make_binary_word("0000");

    //Create the BinaryTreeSubpaving
    BinaryTreeSubpaving theGridSPOneDim( theTwoDimGrid, theRootExtent, thePathToSubPavingRoot, pRootTreeNode );
    std::vector< GridCell* > expected_result_arr( 4 );

    ARIADNE_TESTING_NOTE("Take the [-1,-0.5]x[-1,-0.5] box and do mince(0), i.e. nothing should get minced");
    theGridSPOneDim.mince(0);
    expected_result_arr[0]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("0000") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theGridSPOneDim, 1 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("Take the [-1,-0.5]x[-1,-0.5] box and do mince(1), i.e. nothing should get minced");
    theGridSPOneDim.mince(1);
    expected_result_arr[0]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("0000") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theGridSPOneDim, 1 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("Take the [-1,-0.5]x[-1,-0.5] box and do mince(2), i.e. we should get four subcells");
    expected_result_arr[0]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("000000") );
    expected_result_arr[1]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("000001") );
    expected_result_arr[2]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("000010") );
    expected_result_arr[3]  = new GridCell( theTwoDimGrid, theRootExtent, make_binary_word("000011") );
    theGridSPOneDim.mince(2);
    check_grid_tree_subpaving_iterator( expected_result_arr, theGridSPOneDim, 4 );
    clean_test_vector( expected_result_arr );
}

Void test_grid_paving(){
    //Create a trivial 4-dimensional Grid
    Grid trivialFourDimGrid( {0.0,0.0,0.0,0.0}, {1.0,1.0,1.0,1.0} );

    // !!!
    ARIADNE_TESTING_NOTE("Test allocation of a trivial BinaryTreePaving");
    BinaryTreePaving trivialPaving(4, true);
    BinaryTreePaving * pTrivialPaving = &trivialPaving;
    BinaryTreePaving expected_result1( trivialFourDimGrid , 0, make_binary_word("0"), make_binary_word("1") );
    ARIADNE_TESTING_NOTE("A trivial paving for [0,1]x[0,1]x[0,1]x[0,1], enabled cell: ");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_result1, (*pTrivialPaving) );

    // !!!
    ARIADNE_TESTING_NOTE("Test BinaryTreePaving copy constructor");
    BinaryTreePaving theTrivialPaving( ( *pTrivialPaving ) );
    pTrivialPaving->mince_to_tree_depth(1);

    ARIADNE_TESTING_NOTE("Minced trivial paving for [0,1]x[0,1]x[0,1]x[0,1], enabled cell: ");
    BinaryTreePaving expected_result_minced(trivialFourDimGrid , 0, make_binary_word("100"), make_binary_word("11") );
    ARIADNE_TESTING_EXPECT_EQUAL( expected_result_minced, (*pTrivialPaving) );

    ARIADNE_TESTING_NOTE("A copy of the original paving, should stay unchanged: ");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_result1, theTrivialPaving );

    // !!!
    ARIADNE_TESTING_NOTE("Test BinaryTreePaving (Grid, ExactBoxType) constructor");
    //Allocate the Grid, one Dimension
    const Grid theTwoDimGrid( {-0.25,0.5}, {0.25,0.5} );
    //Note: the box is related to the grid, but not to the original space
    BinaryTreePaving theTwoDimPaving( theTwoDimGrid, make_box("[0,1.5]x[-1.5,3.5]") );
    ARIADNE_TESTING_NOTE("The resulting BinaryTreePaving: ");
    BinaryTreePaving expected_result2( theTwoDimGrid, 4, make_binary_word("0"), make_binary_word("0") );
    ARIADNE_TESTING_EXPECT_EQUAL( expected_result2, theTwoDimPaving );

    // !!!
    ARIADNE_TESTING_NOTE("Test BinaryTreePaving (Grid, Height, BooleanArray, BooleanArray ) constructor");
    BooleanArray theTree(9), theEnabledLeafs(5);
    theTree[0] = true;
    theTree[1] = true;
    theTree[2] = false;  theEnabledLeafs[0] = true;
    theTree[3] = false;  theEnabledLeafs[1] = false;
    theTree[4] = true;
    theTree[5] = true;
    theTree[6] = false;  theEnabledLeafs[2] = true;
    theTree[7] = false;  theEnabledLeafs[3] = false;
    theTree[8] = false;  theEnabledLeafs[4] = true;
    BinaryTreePaving theTwoDimTreePaving( theTwoDimGrid, 2, theTree, theEnabledLeafs );
    ARIADNE_TESTING_NOTE("The resulting BinaryTreePaving: ");
    BinaryTreeNode * pRootTreeNode = new BinaryTreeNode(false);
    pRootTreeNode->split();
    pRootTreeNode->left_node()->split();
    pRootTreeNode->left_node()->left_node()->set_enabled();
    pRootTreeNode->right_node()->split();
    pRootTreeNode->right_node()->right_node()->set_enabled();
    pRootTreeNode->right_node()->left_node()->split();
    pRootTreeNode->right_node()->left_node()->left_node()->set_enabled();
    BinaryTreePaving expected_result3( theTwoDimGrid, 2, pRootTreeNode );
    ARIADNE_TESTING_EXPECT_EQUAL( expected_result3, theTwoDimTreePaving );

    // !!!
    //Create a trivial 2-dimensional Grid
    Grid trivialTwoDimGrid( {0.0,0.0}, {1.0,1.0} );
    ARIADNE_TESTING_NOTE("Test BinaryTreePaving::restrict_to_extent( const Nat theExtent )");
    BinaryTreePaving initialTreeSetOne( trivialTwoDimGrid, 2, make_binary_word("1111001000100"), make_binary_word("10010001") );
    BinaryTreePaving initialTreeSetOneCopyOne( initialTreeSetOne );
    BinaryTreePaving initialTreeSetOneCopyTwo( initialTreeSetOne );

    ARIADNE_TESTING_NOTE("The initialTreeSetOne.root_cell().root_extent() == 2, theExtent == 3, nothing should change");
    initialTreeSetOne.restrict_to_extent( 3 );
    ARIADNE_TESTING_EXPECT_EQUAL( initialTreeSetOneCopyOne, initialTreeSetOne );

    ARIADNE_TESTING_NOTE("The initialTreeSetOneCopyOne.root_cell().root_extent() == 2, theExtent == 1");
    initialTreeSetOneCopyOne.restrict_to_extent( 1 );
    BinaryTreePaving expectedTreeSetOne( trivialTwoDimGrid, 2, make_binary_word("11110010000"), make_binary_word("100100") );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedTreeSetOne, initialTreeSetOneCopyOne );

    ARIADNE_TESTING_NOTE("The initialTreeSetOneCopyTwo.root_cell().root_extent() == 2, theExtent == 0");
    initialTreeSetOneCopyTwo.restrict_to_extent( 0 );
    BinaryTreePaving expectedTreeSetTwo( trivialTwoDimGrid, 2, make_binary_word("111010000"), make_binary_word("00100") );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedTreeSetTwo, initialTreeSetOneCopyTwo );

    ARIADNE_TESTING_NOTE("The initialTreeSetTwo.root_cell().root_extent() == 1, theExtent == 0");
    BinaryTreePaving initialTreeSetTwo( trivialTwoDimGrid, 1, make_binary_word("11000"), make_binary_word("100") );
    initialTreeSetTwo.restrict_to_extent( 0 );
    BinaryTreePaving expectedTreeSetThree( trivialTwoDimGrid, 1, make_binary_word("100"), make_binary_word("00") );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedTreeSetThree, initialTreeSetTwo );
}

Void test_grid_cell(){
    BinaryWord expected_result;

    // !!!
    ARIADNE_TESTING_NOTE("Test the static methods of the GridCell");
    BinaryWord theBinaryPath;

    theBinaryPath = GridCell::primary_cell_path( 1, 0, 0 );
    ARIADNE_TESTING_NOTE( "Dimension: 1, topCellExtent: 0, bottomCellExtent: 0"  );
    ARIADNE_TESTING_EXPECT_EQUAL( expected_result , theBinaryPath );

    expected_result = make_binary_word( "1" ) ;
    theBinaryPath = GridCell::primary_cell_path( 1, 1, 0 );
    ARIADNE_TESTING_NOTE( "Dimension: 1, topCellExtent: 1, bottomCellExtent: 0" );
    ARIADNE_TESTING_EXPECT_EQUAL( expected_result , theBinaryPath );

    expected_result = make_binary_word( "01" );
    theBinaryPath = GridCell::primary_cell_path( 1, 2, 0 );
    ARIADNE_TESTING_NOTE( "Dimension: 1, topCellExtent: 2, bottomCellExtent: 0" );
    ARIADNE_TESTING_EXPECT_EQUAL( expected_result , theBinaryPath );

    expected_result = make_binary_word( "0" );
    theBinaryPath = GridCell::primary_cell_path( 1, 2, 1 );
    ARIADNE_TESTING_NOTE( "Dimension: 1, topCellExtent: 2, bottomCellExtent: 1" );
    ARIADNE_TESTING_EXPECT_EQUAL( expected_result , theBinaryPath );

    expected_result.clear();
    theBinaryPath = GridCell::primary_cell_path( 1, 2, 2 );
    ARIADNE_TESTING_NOTE( "Dimension: 1, topCellExtent: 2, bottomCellExtent: 2" );
    ARIADNE_TESTING_EXPECT_EQUAL( expected_result , theBinaryPath );

    Grid theGrid( {0.0,0.0}, {1.0,1.0} );
    //pFirstCell_01 == pSecondCell_01
    GridCell firstCell01( theGrid, 0, make_binary_word("01") );
    GridCell * pFirstCell_01 = &firstCell01;
    GridCell secondCell01( theGrid, 1, make_binary_word("1101") );
    GridCell * pSecondCell_01 = &secondCell01;
    //pFirstCell_01 != pThirdCell_01 (pFirstCell_01 is left to pThirdCell_01)
    GridCell thirdCell01( theGrid, 2, make_binary_word("001111") );
    GridCell * pThirdCell_01 = &thirdCell01;
    //pFirstCell_01 < pFourthCell_01
    GridCell fourthCell01( theGrid, 1, make_binary_word("11011") );
    GridCell * pFourthCell_01 = &fourthCell01;

    //!!!
    ARIADNE_TESTING_NOTE( "pFirstCell_01 == pFirstCell_01, check for operators < and ==" );
    ARIADNE_TESTING_EXPECT_EQUAL( false , ( (*pFirstCell_01) < (*pFirstCell_01) )  );
    ARIADNE_TESTING_EXPECT_EQUAL( true , ( (*pFirstCell_01) == (*pFirstCell_01) )  );

    //!!!
    ARIADNE_TESTING_NOTE( "pFirstCell_01 == pSecondCell_01, check for operators < and ==" );
    ARIADNE_TESTING_EXPECT_EQUAL( false , ( (*pFirstCell_01) < (*pSecondCell_01) )  );
    ARIADNE_TESTING_EXPECT_EQUAL( true , ( (*pFirstCell_01) == (*pSecondCell_01) )  );
    //!!!
    ARIADNE_TESTING_NOTE( "pFirstCell_01 == pSecondCell_01, check for operators < and ==" );
    ARIADNE_TESTING_EXPECT_EQUAL( false , ( (*pSecondCell_01) < (*pFirstCell_01) )  );
    ARIADNE_TESTING_EXPECT_EQUAL( true , ( (*pSecondCell_01) == (*pFirstCell_01) )  );
    //!!!
    ARIADNE_TESTING_NOTE( "pFirstCell_01 != pThirdCell_01 (pFirstCell_01 is left to pThirdCell_01), check for operators < and ==" );
    ARIADNE_TESTING_EXPECT_EQUAL( true , ( (*pFirstCell_01) < (*pThirdCell_01) )  );
    ARIADNE_TESTING_EXPECT_EQUAL( false , ( (*pFirstCell_01) == (*pThirdCell_01) )  );
    //!!!
    ARIADNE_TESTING_NOTE( "pFirstCell_01 != pThirdCell_01 (pFirstCell_01 is left to pThirdCell_01), check for operators < and ==" );
    ARIADNE_TESTING_EXPECT_EQUAL( false , ( (*pThirdCell_01) < (*pFirstCell_01) )  );
    ARIADNE_TESTING_EXPECT_EQUAL( false , ( (*pThirdCell_01) == (*pFirstCell_01) )  );
    //!!!
    ARIADNE_TESTING_NOTE( "pFirstCell_01 < pFourthCell_01, check for operators < and ==" );
    ARIADNE_TESTING_EXPECT_EQUAL( true , ( (*pFirstCell_01) < (*pFourthCell_01) )  );
    ARIADNE_TESTING_EXPECT_EQUAL( false , ( (*pFirstCell_01) == (*pFourthCell_01) )  );
    //!!!
    ARIADNE_TESTING_NOTE( "pFirstCell_01 < pFourthCell_01, check for operators < and ==" );
    ARIADNE_TESTING_EXPECT_EQUAL( false , ( (*pFourthCell_01) < (*pFirstCell_01) )  );
    ARIADNE_TESTING_EXPECT_EQUAL( false , ( (*pFourthCell_01) == (*pFirstCell_01) )  );
    //!!!
    ARIADNE_TESTING_NOTE( "pFirstCell_01 < pFourthCell_01, check for an exception in the compare_grid_cells method" );
    ARIADNE_TESTING_EXPECT_THROWS(InvalidInput, GridCell::compare_grid_cells( pFourthCell_01, (*pFirstCell_01), 1000));

    //!!!
    ARIADNE_TESTING_NOTE("Test Cell splitting, dimension: 2");
    string word = "001111";
    GridCell thirdCellToSplit( theGrid, 2, make_binary_word( word ) );
    GridCell * pThirdCell_To_Split = &thirdCellToSplit;
    ExactBoxType expected_cell_box = make_box("[0.5,1.0]x[0.5,1.0]");
    ARIADNE_TESTING_NOTE("The initial GridCell, as given by it's box: ");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_cell_box, pThirdCell_To_Split->box() );
    ARIADNE_TESTING_NOTE("The left sub-box of the initial GridCell, as given by it's box: ");
    ExactBoxType expected_left_sub_cell_box = make_box("[0.5,0.75]x[0.5,1.0]");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_left_sub_cell_box, pThirdCell_To_Split->split(false).box() );
    ARIADNE_TESTING_NOTE("The right sub-box of the initial GridCell, as given by it's box: ");
    ExactBoxType expected_right_sub_cell_box = make_box("[0.75,1.0]x[0.5,1.0]");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_right_sub_cell_box, pThirdCell_To_Split->split(true).box() );
    ARIADNE_TESTING_NOTE("The word of the initial GridCell: ");
    BinaryWord expected_cell_word = make_binary_word( word );
    ARIADNE_TESTING_EXPECT_EQUAL( expected_cell_word, pThirdCell_To_Split->word() );

    //!!!
    ARIADNE_TESTING_NOTE("Test taking the cell's interior, dimension: 2");
    ARIADNE_TESTING_NOTE("The interior of the grid cell given by the box of the corresponding open grid cell: ");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_cell_box, pThirdCell_To_Split->interior().box() );
}

Void test_grid_open_cell_two(){
    //Allocate a trivial Grid two dimensional grid
    Grid theTrivialGrid(2, 1.0);

    //!!!
    ARIADNE_TESTING_NOTE("Construct a trivial open cell and split it, dimension: 2");
    GridOpenCell trivialOpenCell = GridOpenCell( theTrivialGrid, 0, BinaryWord() );
    ExactBoxType expected_trivial_open_cell_box = make_box("[0.0,2.0]x[0.0,2.0]");
    ARIADNE_TESTING_NOTE("The trivial open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_trivial_open_cell_box, trivialOpenCell.box() );
    ARIADNE_TESTING_NOTE("The left open sub cell, as given by its box:");
    ExactBoxType expected_left_open_cell_box = make_box("[0.0,1.0]x[0.0,2.0]");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_left_open_cell_box, trivialOpenCell.split( TernaryChild::LEFT ).box() );
    ARIADNE_TESTING_NOTE("The middle open sub cell, as given by its box:");
    ExactBoxType expected_middle_open_cell_box = make_box("[0.5,1.5]x[0.0,2.0]");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_middle_open_cell_box, trivialOpenCell.split( TernaryChild::MIDDLE ).box() );
    ARIADNE_TESTING_NOTE("The right open sub cell, as given by its box:");
    ExactBoxType expected_right_open_cell_box = make_box("[1.0,2.0]x[0.0,2.0]");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_right_open_cell_box, trivialOpenCell.split( TernaryChild::RIGHT ).box() );

    //!!!
    ARIADNE_TESTING_NOTE("Construct an open cell 01 extent=2 and split it, dimension: 2");
    GridOpenCell openCell01 = GridOpenCell( theTrivialGrid, 2, make_binary_word( "0011111" ) );
    ExactBoxType expected_open_cell_box01 = make_box("[0.75,1.25]x[0.5,1.5]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box01, openCell01.box() );
    ARIADNE_TESTING_NOTE("The left open sub cell, as given by its box:");
    ExactBoxType expected_left_open_cell_box01 = make_box("[0.75,1.25]x[0.5,1.0]");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_left_open_cell_box01, openCell01.split( TernaryChild::LEFT ).box() );
    ARIADNE_TESTING_NOTE("The middle open sub cell, as given by its box:");
    ExactBoxType expected_middle_open_cell_box01 = make_box("[0.75,1.25]x[0.75,1.25]");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_middle_open_cell_box01, openCell01.split( TernaryChild::MIDDLE ).box() );
    ARIADNE_TESTING_NOTE("The right open sub cell, as given by its box:");
    ExactBoxType expected_right_open_cell_box01 = make_box("[0.75,1.25]x[1.0,1.5]");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_right_open_cell_box01, openCell01.split( TernaryChild::RIGHT ).box() );
}

Void test_grid_open_cell_three(){
    //Allocate a trivial Grid two dimensional grid
    Grid theGrid(2, 1.0);

    //!!!
    ARIADNE_TESTING_NOTE("Construct a [0.0,1.0]x[0.0,1.0] box and make and open over approximation, dimension: 2");
    ExactBoxType theBox = make_box("[0.0,1.0]x[0.0,1.0]");
    ExactBoxType expectedOpenCellBoxType = make_box("[-1.0,3.0]x[-1.0,3.0]");
    BinaryWord expectedOpenCellWord = make_binary_word("00");
    Nat expectedOpenCellRootExtent = 2;
    GridOpenCell actualOpenCell = GridOpenCell::outer_approximation( theBox, theGrid );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellBoxType, actualOpenCell.box() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellWord, actualOpenCell.word() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellRootExtent, actualOpenCell.root_extent() );

    //!!!
    ARIADNE_TESTING_NOTE("Construct a [-0.5,0.5]x[-0.5,0.5] box and make and open over approximation, dimension: 2");
    theBox = make_box("[-0.5,0.5]x[-0.5,0.5]");
    expectedOpenCellBoxType = make_box("[-1.0,1.0]x[-1.0,1.0]");
    expectedOpenCellWord = make_binary_word("00");
    expectedOpenCellRootExtent = 1;
    actualOpenCell = GridOpenCell::outer_approximation( theBox, theGrid );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellBoxType, actualOpenCell.box() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellWord, actualOpenCell.word() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellRootExtent, actualOpenCell.root_extent() );

    //!!!
    ARIADNE_TESTING_NOTE("Construct a [0.1,0.3]x[0.1,0.3] box and make and open over approximation, dimension: 2");
    theBox = make_box("[0.1,0.3]x[0.1,0.3]");
    expectedOpenCellBoxType = make_box("[0.0,0.5]x[0.0,0.5]");
    expectedOpenCellWord = make_binary_word("0000");
    expectedOpenCellRootExtent = 0;
    actualOpenCell = GridOpenCell::outer_approximation( theBox, theGrid );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellBoxType, actualOpenCell.box() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellWord, actualOpenCell.word() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellRootExtent, actualOpenCell.root_extent() );

    //!!!
    ARIADNE_TESTING_NOTE("Construct a [0.1,0.3]x[0.1,0.6] box and make and open over approximation, dimension: 2");
    theBox = make_box("[0.1,0.3]x[0.1,0.6]");
    expectedOpenCellBoxType = make_box("[0.0,0.5]x[0.0,1.0]");
    expectedOpenCellWord = make_binary_word("000");
    expectedOpenCellRootExtent = 0;
    actualOpenCell = GridOpenCell::outer_approximation( theBox, theGrid );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellBoxType, actualOpenCell.box() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellWord, actualOpenCell.word() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellRootExtent, actualOpenCell.root_extent() );

    //!!!
    ARIADNE_TESTING_NOTE("Construct a [0.5,0.6]x[0.1,0.6] box and make and open over approximation, dimension: 2");
    theBox = make_box("[0.5,0.6]x[0.1,0.6]");
    expectedOpenCellBoxType = make_box("[0.25,0.75]x[0.0,1.0]");
    expectedOpenCellWord = make_binary_word("001");
    expectedOpenCellRootExtent = 0;
    actualOpenCell = GridOpenCell::outer_approximation( theBox, theGrid );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellBoxType, actualOpenCell.box() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellWord, actualOpenCell.word() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellRootExtent, actualOpenCell.root_extent() );

    //!!!
    ARIADNE_TESTING_NOTE("Construct a [0.6,0.7]x[0.1,0.6] box and make and open over approximation, dimension: 2");
    theBox = make_box("[0.6,0.7]x[0.1,0.6]");
    expectedOpenCellBoxType = make_box("[0.25,0.75]x[0.0,1.0]");
    expectedOpenCellWord = make_binary_word("001");
    expectedOpenCellRootExtent = 0;
    actualOpenCell = GridOpenCell::outer_approximation( theBox, theGrid );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellBoxType, actualOpenCell.box() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellWord, actualOpenCell.word() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellRootExtent, actualOpenCell.root_extent() );

    //!!!
    ARIADNE_TESTING_NOTE("Construct a [0.9,1.1]x[0.9,1.1] box and make and open over approximation, dimension: 2");
    theBox = make_box("[0.9,1.1]x[0.9,1.1]");
    expectedOpenCellBoxType = make_box("[0.875,1.125]x[0.875,1.125]");
    expectedOpenCellWord = make_binary_word("0011111111");
    expectedOpenCellRootExtent = 2;
    actualOpenCell = GridOpenCell::outer_approximation( theBox, theGrid );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellBoxType, actualOpenCell.box() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellWord, actualOpenCell.word() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellRootExtent, actualOpenCell.root_extent() );

    //!!!
    ARIADNE_TESTING_NOTE("Construct a [0.2,0.3]x[0.7,0.8] box and make and open over approximation, dimension: 2");
    theBox = make_box("[0.2,0.3]x[0.7,0.8]");
    expectedOpenCellBoxType = make_box("[0.1875,0.3125]x[0.6875,0.8125]");
    expectedOpenCellWord = make_binary_word("01001111");
    expectedOpenCellRootExtent = 0;
    actualOpenCell = GridOpenCell::outer_approximation( theBox, theGrid );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellBoxType, actualOpenCell.box() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellWord, actualOpenCell.word() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedOpenCellRootExtent, actualOpenCell.root_extent() );
}

Void test_grid_open_cell_four(){
    //Allocate a trivial Grid two dimensional grid
    Grid theGrid(2, 1.0);

    //!!!
    ARIADNE_TESTING_NOTE("Test that the open cell intersects with itself, dimension: 2");
    GridOpenCell leftOpenCell = GridOpenCell( theGrid, 2, make_binary_word( "0011111" ) );
    GridOpenCell rightOpenCell = GridOpenCell( theGrid, 2, make_binary_word( "0011111" ) );
    ARIADNE_TESTING_EXPECT_EQUAL( GridOpenCell::intersect( leftOpenCell, rightOpenCell ), true );

    //!!!
    ARIADNE_TESTING_NOTE("Test that the open cells [0.0,1.0]x[0.0,1.0] and [-0.25,0.25]x[0.0,1.0] intersect, dimension: 2");
    ExactBoxType expectedLeftOpenCellBoxType = make_box("[0.0,1.0]x[0.0,1.0]");
    leftOpenCell = GridOpenCell( theGrid, 0, make_binary_word( "00" ) );
    ExactBoxType expectedRightOpenCellBoxType = make_box("[-0.25,0.25]x[0.0,1.0]");
    rightOpenCell = GridOpenCell( theGrid, 1, make_binary_word( "01101" ) );
    //First check that the cells result in correct boxes
    ARIADNE_TESTING_EXPECT_EQUAL( expectedLeftOpenCellBoxType, leftOpenCell.box() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedRightOpenCellBoxType, rightOpenCell.box() );
    //Test the intersect
    ARIADNE_TESTING_EXPECT_EQUAL( GridOpenCell::intersect( leftOpenCell, rightOpenCell ), true );

    //!!!
    ARIADNE_TESTING_NOTE("Test that the open cells [0.0,0.5]x[0.5,1.5] and [0.5,1.0]x[0.5,1.5] intersect, dimension: 2");
    expectedLeftOpenCellBoxType = make_box("[0.0,0.5]x[0.5,1.5]");
    leftOpenCell = GridOpenCell( theGrid, 0, make_binary_word( "010" ) );
    expectedRightOpenCellBoxType = make_box("[0.5,1.0]x[0.5,1.5]");
    rightOpenCell = GridOpenCell( theGrid, 0, make_binary_word( "110" ) );
    //First check that the cells result in correct boxes
    ARIADNE_TESTING_EXPECT_EQUAL( expectedLeftOpenCellBoxType, leftOpenCell.box() );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedRightOpenCellBoxType, rightOpenCell.box() );
    //Test the intersect
    ARIADNE_TESTING_EXPECT_EQUAL( GridOpenCell::intersect( leftOpenCell, rightOpenCell ), false );
}

Void test_grid_open_cell_five() {
    //The vector for storing the results
    std::vector< GridCell* > expected_result_arr(4);

    //Allocate a trivial Grid two dimensional grid
    Grid theTrivialGrid(2, 1.0);

    //!!! 00
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 00 extent=0, dimension: 2");
    GridOpenCell openCell = GridOpenCell( theTrivialGrid, 0, BinaryWord() );

    Nat theRootExtent = 2;
    expected_result_arr[0] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("0011") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("0110") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("1001") );
    expected_result_arr[3] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("1100") );

    BinaryTreePaving closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );
    clean_test_vector( expected_result_arr );

    //!!! 01
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 01 extent=2, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 2, make_binary_word( "0011111" ) );

    theRootExtent = 2;
    expected_result_arr[0] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("0011111") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("0110101") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("1001010") );
    expected_result_arr[3] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("1100000") );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    //!!! 02
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 01 extent=1, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 1, make_binary_word( "11111" ) );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    //!!! 03
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 01 extent=1, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 0, make_binary_word( "111" ) );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    clean_test_vector( expected_result_arr );

    //!!! 04
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 02 extent=2, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 2, make_binary_word( "00110101" ) );

    theRootExtent = 2;
    expected_result_arr[0] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00110101") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00110111") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("01100000") );
    expected_result_arr[3] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("01100010") );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    //!!! 05
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 02 extent=1, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 1, make_binary_word( "110101" ) );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    //!!! 06
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 02 extent=0, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 0, make_binary_word( "0101" ) );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    clean_test_vector( expected_result_arr );

    //!!! 07
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 03 extent=2, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 2, make_binary_word( "000111111" ) );

    theRootExtent = 2;
    expected_result_arr[0] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("000111111") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("001101010") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("010010101") );
    expected_result_arr[3] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("011000000") );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    //!!! 08
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 03 extent=1, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 1, make_binary_word( "0111111" ) );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    clean_test_vector( expected_result_arr );

    //!!! 09
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 04 extent=2, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 2, make_binary_word( "00000001" ) );

    theRootExtent = 2;
    expected_result_arr[0] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00000001") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00000011") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00000100") );
    expected_result_arr[3] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00000110") );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    //!!! 10
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 04 extent=1, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 1, make_binary_word( "000001" ) );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    clean_test_vector( expected_result_arr );

    //!!! 11
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 05 extent=2, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 2, make_binary_word( "00000011" ) );

    theRootExtent = 2;
    expected_result_arr[0] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00000011") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00000110") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00001001") );
    expected_result_arr[3] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00001100") );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    //!!! 12
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 05 extent=1, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 1, make_binary_word( "000011" ) );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    clean_test_vector( expected_result_arr );

    //!!! 13
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 06 extent=2, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 2, make_binary_word( "00000010" ) );

    theRootExtent = 2;
    expected_result_arr[0] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00000010") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00000011") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00001000") );
    expected_result_arr[3] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00001001") );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    //!!! 14
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 06 extent=1, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 1, make_binary_word( "000010" ) );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    clean_test_vector( expected_result_arr );

    //!!! 15
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 07 extent=2, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 2, make_binary_word( "00000000" ) );

    theRootExtent = 2;
    expected_result_arr[0] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00000000") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00000001") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00000010") );
    expected_result_arr[3] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("00000011") );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    //!!! 16
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 07 extent=1, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 1, make_binary_word( "000000" ) );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    clean_test_vector( expected_result_arr );

    //!!! 17
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 08 extent=2, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 2, make_binary_word( "0000001" ) );

    theRootExtent = 2;
    expected_result_arr[0] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("0000001") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("0000011") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("0000100") );
    expected_result_arr[3] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("0000110") );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    //!!! 18
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 08 extent=1, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 1, make_binary_word( "00001" ) );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    clean_test_vector( expected_result_arr );

    //!!! 19
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 09 extent=2, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 2, make_binary_word( "0000" ) );

    theRootExtent = 2;
    expected_result_arr[0] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("0000") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("0001") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("0010") );
    expected_result_arr[3] = new GridCell( theTrivialGrid, theRootExtent, make_binary_word("0011") );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    //!!! 20
    ARIADNE_TESTING_NOTE("Construct closure of an open cell 09 extent=1, dimension: 2");
    openCell = GridOpenCell( theTrivialGrid, 1, make_binary_word( "00" ) );

    closureSet = openCell.closure();
    check_grid_tree_subpaving_iterator( expected_result_arr, closureSet, 4 );

    clean_test_vector( expected_result_arr );
}

Void test_grid_open_cell_six() {
    std::vector<GridOpenCell> expectedResult;
    std::vector<GridOpenCell> actualResult;
    Nat theRootExtent;

    //Allocate a trivial Grid two dimensional grid
    Grid theTrivialGrid(2, 1.0);

    //!!! 00
    ARIADNE_TESTING_NOTE("Intersect the open cell with itself, dimension: 2");
    GridOpenCell openCellOne = GridOpenCell( theTrivialGrid, 0, BinaryWord() );
    GridOpenCell openCellTwo = GridOpenCell( theTrivialGrid, 0, BinaryWord() );

    theRootExtent = 2;
    expectedResult.push_back( GridOpenCell( theTrivialGrid, theRootExtent, make_binary_word( "001100" ) ) );
    expectedResult.push_back( GridOpenCell( theTrivialGrid, theRootExtent, make_binary_word( "001101" ) ) );
    expectedResult.push_back( GridOpenCell( theTrivialGrid, theRootExtent, make_binary_word( "001110" ) ) );
    expectedResult.push_back( GridOpenCell( theTrivialGrid, theRootExtent, make_binary_word( "001111" ) ) );
    expectedResult.push_back( GridOpenCell( theTrivialGrid, theRootExtent, make_binary_word( "011000" ) ) );
    expectedResult.push_back( GridOpenCell( theTrivialGrid, theRootExtent, make_binary_word( "011010" ) ) );
    expectedResult.push_back( GridOpenCell( theTrivialGrid, theRootExtent, make_binary_word( "100100" ) ) );
    expectedResult.push_back( GridOpenCell( theTrivialGrid, theRootExtent, make_binary_word( "100101" ) ) );
    expectedResult.push_back( GridOpenCell( theTrivialGrid, theRootExtent, make_binary_word( "110000" ) ) );

    actualResult = GridOpenCell::intersection( openCellOne, openCellTwo );

    ARIADNE_TESTING_EXPECT_EQUAL( expectedResult, actualResult );
    expectedResult.erase( expectedResult.begin(), expectedResult.end() );

    //!!! 01
    ARIADNE_TESTING_NOTE("Intersect an open cell with an open cell that is a robust subset, dimension: 2");
    openCellOne = GridOpenCell( theTrivialGrid, 0, make_binary_word( "00" ) );
    openCellTwo = GridOpenCell( theTrivialGrid, 0, make_binary_word( "0011" ) );

    theRootExtent = 0;
    expectedResult.push_back( GridOpenCell( theTrivialGrid, theRootExtent, make_binary_word( "0011" ) ) );

    actualResult = GridOpenCell::intersection( openCellOne, openCellTwo );

    ARIADNE_TESTING_EXPECT_EQUAL( expectedResult, actualResult );
    expectedResult.erase( expectedResult.begin(), expectedResult.end() );

    //!!! 02
    ARIADNE_TESTING_NOTE("Intersect two intersectping open cells, dimension: 2");
    openCellOne = GridOpenCell( theTrivialGrid, 0, make_binary_word( "00" ) );
    openCellTwo = GridOpenCell( theTrivialGrid, 0, make_binary_word( "1011" ) );

    theRootExtent = 0;
    expectedResult.push_back( GridOpenCell( theTrivialGrid, theRootExtent, make_binary_word( "101100" ) ) );
    expectedResult.push_back( GridOpenCell( theTrivialGrid, theRootExtent, make_binary_word( "101101" ) ) );
    expectedResult.push_back( GridOpenCell( theTrivialGrid, theRootExtent, make_binary_word( "111000" ) ) );

    actualResult = GridOpenCell::intersection( openCellOne, openCellTwo );

    ARIADNE_TESTING_EXPECT_EQUAL( expectedResult, actualResult );
    expectedResult.erase( expectedResult.begin(), expectedResult.end() );

    //!!! 03
    ARIADNE_TESTING_NOTE("Intersect two non-intersectping open cells, dimension: 2");
    openCellOne = GridOpenCell( theTrivialGrid, 0, make_binary_word( "0000" ) );
    openCellTwo = GridOpenCell( theTrivialGrid, 0, make_binary_word( "1000" ) );

    actualResult = GridOpenCell::intersection( openCellOne, openCellTwo );

    ARIADNE_TESTING_EXPECT_EQUAL( expectedResult, actualResult );
    expectedResult.erase( expectedResult.begin(), expectedResult.end() );

    //!!! 04
    ARIADNE_TESTING_NOTE("Intersect two intersectping open cells whoes intersection is just one GridCell, dimension: 2");
    openCellOne = GridOpenCell( theTrivialGrid, 0, make_binary_word( "0000" ) );
    openCellTwo = GridOpenCell( theTrivialGrid, 2, make_binary_word( "0001101111" ) );

    theRootExtent = 0;
    expectedResult.push_back( GridOpenCell( theTrivialGrid, theRootExtent, make_binary_word( "00010100" ) ) );

    actualResult = GridOpenCell::intersection( openCellOne, openCellTwo );

    ARIADNE_TESTING_EXPECT_EQUAL( expectedResult, actualResult );
    expectedResult.erase( expectedResult.begin(), expectedResult.end() );

    //!!! 05
    ARIADNE_TESTING_NOTE("Intersect two intersectping open cells based on x-splitted primary cells, dimension: 2");
    openCellOne = GridOpenCell( theTrivialGrid, 0, make_binary_word( "0" ) );
    openCellTwo = GridOpenCell( theTrivialGrid, 0, make_binary_word( "1" ) );

    theRootExtent = 2;
    expectedResult.push_back( GridOpenCell( theTrivialGrid, theRootExtent, make_binary_word( "0011100" ) ) );
    expectedResult.push_back( GridOpenCell( theTrivialGrid, theRootExtent, make_binary_word( "0011110" ) ) );
    expectedResult.push_back( GridOpenCell( theTrivialGrid, theRootExtent, make_binary_word( "0110100" ) ) );

    actualResult = GridOpenCell::intersection( openCellOne, openCellTwo );

    ARIADNE_TESTING_EXPECT_EQUAL( expectedResult, actualResult );
    expectedResult.erase( expectedResult.begin(), expectedResult.end() );
}

Void test_grid_open_cell_one(){
    //Allocate a trivial Grid two dimensional grid
    Grid theTrivialGrid(2, 1.0);

    //!!! 00
    ARIADNE_TESTING_NOTE("Constructing a trivial open cell, dimension: 2");
    GridOpenCell trivialOpenCell = GridOpenCell( theTrivialGrid, 0, BinaryWord() );
    ExactBoxType expected_trivial_open_cell_box = make_box("[0.0,2.0]x[0.0,2.0]");
    ARIADNE_TESTING_NOTE("The trivial open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_trivial_open_cell_box, trivialOpenCell.box() );

    //!!! 01
    ARIADNE_TESTING_NOTE("Constructing an open cell 01 extent=2, dimension: 2");
    GridOpenCell openCell01 = GridOpenCell( theTrivialGrid, 2, make_binary_word( "0011111" ) );
    ExactBoxType expected_open_cell_box01 = make_box("[0.75,1.25]x[0.5,1.5]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box01, openCell01.box() );

    //!!! 02
    ARIADNE_TESTING_NOTE("Constructing an open cell 01 extent=1, dimension: 2");
    GridOpenCell openCell02 = GridOpenCell( theTrivialGrid, 1, make_binary_word( "11111" ) );
    ExactBoxType expected_open_cell_box02 = make_box("[0.75,1.25]x[0.5,1.5]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box02, openCell02.box() );

    //!!! 03
    ARIADNE_TESTING_NOTE("Constructing an open cell 01 extent=0, dimension: 2");
    GridOpenCell openCell03 = GridOpenCell( theTrivialGrid, 0, make_binary_word( "111" ) );
    ExactBoxType expected_open_cell_box03 = make_box("[0.75,1.25]x[0.5,1.5]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box03, openCell03.box() );

    //!!! 04
    ARIADNE_TESTING_NOTE("Constructing an open cell 02 extent=2, dimension: 2");
    GridOpenCell openCell04 = GridOpenCell( theTrivialGrid, 2, make_binary_word( "00110101" ) );
    ExactBoxType expected_open_cell_box04 = make_box("[0.0,0.5]x[0.75,1.25]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box04, openCell04.box() );

    //!!! 05
    ARIADNE_TESTING_NOTE("Constructing an open cell 02 extent=1, dimension: 2");
    GridOpenCell openCell05 = GridOpenCell( theTrivialGrid, 1, make_binary_word( "110101" ) );
    ExactBoxType expected_open_cell_box05 = make_box("[0.0,0.5]x[0.75,1.25]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box05, openCell05.box() );

    //!!! 06
    ARIADNE_TESTING_NOTE("Constructing an open cell 02 extent=0, dimension: 2");
    GridOpenCell openCell06 = GridOpenCell( theTrivialGrid, 0, make_binary_word( "0101" ) );
    ExactBoxType expected_open_cell_box06 = make_box("[0.0,0.5]x[0.75,1.25]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box06, openCell06.box() );

    //!!! 07
    ARIADNE_TESTING_NOTE("Constructing an open cell 03 extent=2, dimension: 2");
    GridOpenCell openCell07 = GridOpenCell( theTrivialGrid, 2, make_binary_word( "000111111" ) );
    ExactBoxType expected_open_cell_box07 = make_box("[-0.125,0.125]x[0.75,1.25]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box07, openCell07.box() );

    //!!! 08
    ARIADNE_TESTING_NOTE("Constructing an open cell 03 extent=1, dimension: 2");
    GridOpenCell openCell08 = GridOpenCell( theTrivialGrid, 1, make_binary_word( "0111111" ) );
    ExactBoxType expected_open_cell_box08 = make_box("[-0.125,0.125]x[0.75,1.25]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box08, openCell08.box() );

    //!!! 09
    ARIADNE_TESTING_NOTE("Constructing an open cell 04 extent=2, dimension: 2");
    GridOpenCell openCell09 = GridOpenCell( theTrivialGrid, 2, make_binary_word( "00000001" ) );
    ExactBoxType expected_open_cell_box09 = make_box("[-1.0,-0.5]x[-0.75,-0.25]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box09, openCell09.box() );

    //!!! 10
    ARIADNE_TESTING_NOTE("Constructing an open cell 04 extent=1, dimension: 2");
    GridOpenCell openCell10 = GridOpenCell( theTrivialGrid, 1, make_binary_word( "000001" ) );
    ExactBoxType expected_open_cell_box10 = make_box("[-1.0,-0.5]x[-0.75,-0.25]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box10, openCell10.box() );

    //!!! 11
    ARIADNE_TESTING_NOTE("Constructing an open cell 05 extent=2, dimension: 2");
    GridOpenCell openCell11 = GridOpenCell( theTrivialGrid, 2, make_binary_word( "00000011" ) );
    ExactBoxType expected_open_cell_box11 = make_box("[-0.75,-0.25]x[-0.75,-0.25]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box11, openCell11.box() );

    //!!! 12
    ARIADNE_TESTING_NOTE("Constructing an open cell 05 extent=1, dimension: 2");
    GridOpenCell openCell12 = GridOpenCell( theTrivialGrid, 1, make_binary_word( "000011" ) );
    ExactBoxType expected_open_cell_box12 = make_box("[-0.75,-0.25]x[-0.75,-0.25]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box12, openCell12.box() );

    //!!! 13
    ARIADNE_TESTING_NOTE("Constructing an open cell 06 extent=2, dimension: 2");
    GridOpenCell openCell13 = GridOpenCell( theTrivialGrid, 2, make_binary_word( "00000010" ) );
    ExactBoxType expected_open_cell_box13 = make_box("[-0.75,-0.25]x[-1.0,-0.5]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box13, openCell13.box() );

    //!!! 14
    ARIADNE_TESTING_NOTE("Constructing an open cell 06 extent=1, dimension: 2");
    GridOpenCell openCell14 = GridOpenCell( theTrivialGrid, 1, make_binary_word( "000010" ) );
    ExactBoxType expected_open_cell_box14 = make_box("[-0.75,-0.25]x[-1.0,-0.5]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box14, openCell14.box() );

    //!!! 15
    ARIADNE_TESTING_NOTE("Constructing an open cell 07 extent=2, dimension: 2");
    GridOpenCell openCell15 = GridOpenCell( theTrivialGrid, 2, make_binary_word( "00000000" ) );
    ExactBoxType expected_open_cell_box15 = make_box("[-1.0,-0.5]x[-1.0,-0.5]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box15, openCell15.box() );

    //!!! 16
    ARIADNE_TESTING_NOTE("Constructing an open cell 07 extent=1, dimension: 2");
    GridOpenCell openCell16 = GridOpenCell( theTrivialGrid, 1, make_binary_word( "000000" ) );
    ExactBoxType expected_open_cell_box16 = make_box("[-1.0,-0.5]x[-1.0,-0.5]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box16, openCell16.box() );

    //!!! 17
    ARIADNE_TESTING_NOTE("Constructing an open cell 08 extent=2, dimension: 2");
    GridOpenCell openCell17 = GridOpenCell( theTrivialGrid, 2, make_binary_word( "0000001" ) );
    ExactBoxType expected_open_cell_box17 = make_box("[-0.75,-0.25]x[-1.0,0.0]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box17, openCell17.box() );

    //!!! 18
    ARIADNE_TESTING_NOTE("Constructing an open cell 08 extent=1, dimension: 2");
    GridOpenCell openCell18 = GridOpenCell( theTrivialGrid, 1, make_binary_word( "00001" ) );
    ExactBoxType expected_open_cell_box18 = make_box("[-0.75,-0.25]x[-1.0,0.0]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box18, openCell18.box() );

    //!!! 19
    ARIADNE_TESTING_NOTE("Constructing an open cell 09 extent=2, dimension: 2");
    GridOpenCell openCell19 = GridOpenCell( theTrivialGrid, 2, make_binary_word( "0000" ) );
    ExactBoxType expected_open_cell_box19 = make_box("[-1.0,1.0]x[-1.0,1.0]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box19, openCell19.box() );

    //!!! 20
    ARIADNE_TESTING_NOTE("Constructing an open cell 09 extent=1, dimension: 2");
    GridOpenCell openCell20 = GridOpenCell( theTrivialGrid, 1, make_binary_word( "00" ) );
    ExactBoxType expected_open_cell_box20 = make_box("[-1.0,1.0]x[-1.0,1.0]");
    ARIADNE_TESTING_NOTE("The open cell, as given by its box:");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_open_cell_box20, openCell20.box() );
}

Void test_adjoin_operation_one(){
    //Allocate a trivial Grid
    Grid theGrid(2, 1.0);

    // !!!
    ARIADNE_TESTING_NOTE("Test adjoining a GridCell to the BinaryTreePaving");
    //Define the GridCell that is rooted to a high primary cell
    const Int theHigherCellExtent = 2;
    BinaryWord theHigherCellPath;
    theHigherCellPath.push_back(true);
    theHigherCellPath.push_back(false);
    theHigherCellPath.push_back(true);
    theHigherCellPath.push_back(false);
    GridCell theHigherLevelCell( theGrid, theHigherCellExtent, theHigherCellPath );

    ARIADNE_TESTING_NOTE("The GridCell with the primary root cell extent = 2");
    ExactBoxType expected_box = make_box("[2,3]x[-1,0]");
    ARIADNE_TESTING_NOTE("The initial GridCell, as given by it's box: ");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_box, theHigherLevelCell.box() );

    //Define the higth of the primary root cell.
    //Create the BinaryTreePaving with the box related to the grid, but not to the original space
    BinaryTreePaving theOneCellPaving( theGrid, true );
    ARIADNE_TESTING_NOTE(theOneCellPaving);
    ARIADNE_TESTING_NOTE("The BinaryTreePaving with the primary root cell extent = 0");
    BinaryTreePaving expected_one_cell_paving( theGrid, 0, make_binary_word("0"), make_binary_word("1") );
    ARIADNE_TESTING_EXPECT_EQUAL( expected_one_cell_paving, theOneCellPaving );

    ARIADNE_TESTING_NOTE("The BinaryTreePaving after adding the cell: ");
    theOneCellPaving.adjoin( theHigherLevelCell );
    BinaryTreePaving expected_two_cell_paving( theGrid, 2, make_binary_word("111010001101000"), make_binary_word("00100100") );
    ARIADNE_TESTING_EXPECT_EQUAL( expected_two_cell_paving, theOneCellPaving );
}

Void test_adjoin_operation_two(){

    //Allocate a trivial Grid
    Grid theGrid(2, 1.0);

    // !!!
    ARIADNE_TESTING_NOTE("Test adjoining a GridCell to the BinaryTreePaving");
    //Define the GridCell that is rooted to the lower primary cell
    const Int theLowerCellExtent = 1;
    BinaryWord theLowerCellPath;
    theLowerCellPath.push_back(true);
    theLowerCellPath.push_back(true);
    GridCell theLowerLevelCell( theGrid, theLowerCellExtent, theLowerCellPath );

    ARIADNE_TESTING_NOTE("The GridCell with the primary root cell extent = 1");
    ExactBoxType expected_box = make_box("[0,1]x[0,1]");
    ARIADNE_TESTING_NOTE("The box of the initial GridCell: ");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_box, theLowerLevelCell.box() );

    //Define the higth of the primary root cell.
    const Nat theExtent = 2;
    //Create the binary tree;
    BinaryTreeNode * pRootTreeNode = new BinaryTreeNode(false);
    pRootTreeNode->split();
    pRootTreeNode->right_node()->split();
    pRootTreeNode->right_node()->left_node()->split();
    pRootTreeNode->right_node()->left_node()->right_node()->split();
    pRootTreeNode->right_node()->left_node()->right_node()->left_node()->set_enabled();

    //Create the BinaryTreePaving with the box is related to the grid, but not to the original space
    BinaryTreePaving theOneCellPaving( theGrid, theExtent, pRootTreeNode );
    ARIADNE_TESTING_NOTE("The BinaryTreePaving with the primary root cell extent = 2");
    BinaryTreePaving expected_tree_set( theGrid, theExtent, make_binary_word("101101000"), make_binary_word("00100") );
    ARIADNE_TESTING_EXPECT_EQUAL( expected_tree_set, theOneCellPaving );

    ARIADNE_TESTING_NOTE("The BinaryTreePaving after adding the cell: ");
    theOneCellPaving.adjoin( theLowerLevelCell );
    BinaryTreePaving expected_tree_set_result( theGrid, theExtent, make_binary_word("111010001101000"), make_binary_word("00100100") );
    ARIADNE_TESTING_EXPECT_EQUAL( expected_tree_set_result, theOneCellPaving );
}

Void test_adjoin_operation_three(){
    string expected_result;

    //Allocate a trivial Grid
    Grid theGrid(2, 1.0);

    // !!!
    ARIADNE_TESTING_NOTE("Test adjoining a GridCell to the BinaryTreePaving");
    //Define the GridCell that is rooted to the same primary cell
    const Int theLowerCellExtent = 2;
    BinaryWord theLowerCellPath;
    theLowerCellPath.push_back(false);
    theLowerCellPath.push_back(false);
    theLowerCellPath.push_back(true);
    theLowerCellPath.push_back(true);
    GridCell theLowerLevelCell( theGrid, theLowerCellExtent, theLowerCellPath );

    ARIADNE_TESTING_NOTE("The GridCell with the primary root cell extent = 2");
    ExactBoxType expected_box = make_box("[0,1]x[0,1]");
    ARIADNE_TESTING_NOTE("The initial GridCell: ");
    ARIADNE_TESTING_EXPECT_EQUAL( expected_box, theLowerLevelCell.box() );

    //Define the higth of the primary root cell.
    const Nat theExtent = 2;
    //Create the binary tree;
    BinaryTreeNode * pRootTreeNode = new BinaryTreeNode(false);
    pRootTreeNode->split();
    pRootTreeNode->right_node()->split();
    pRootTreeNode->right_node()->left_node()->split();
    pRootTreeNode->right_node()->left_node()->right_node()->split();
    pRootTreeNode->right_node()->left_node()->right_node()->left_node()->set_enabled();

    //Create the BinaryTreePaving with the box is related to the grid, but not to the original space
    ARIADNE_TESTING_NOTE("The BinaryTreePaving with the primary root cell extent = 2");
    BinaryTreePaving theOneCellPaving( theGrid, theExtent, pRootTreeNode );
    BinaryTreePaving expected_tree_set( theGrid, theExtent, make_binary_word("101101000"), make_binary_word("00100") );
    ARIADNE_TESTING_EXPECT_EQUAL( expected_tree_set, theOneCellPaving );

    ARIADNE_TESTING_NOTE("The BinaryTreePaving after adding the cell: ");
    theOneCellPaving.adjoin( theLowerLevelCell );
    BinaryTreePaving expected_tree_set_result( theGrid, theExtent, make_binary_word("111010001101000"), make_binary_word("00100100") );
    ARIADNE_TESTING_EXPECT_EQUAL( expected_tree_set_result, theOneCellPaving );
}

Void test_adjoin_outer_approximation_operation(){
    //Allocate a trivial Grid
    Grid theTrivialGrid(2, 1.0);

    // !!!
    ARIADNE_TESTING_NOTE("Test adjoining_outer_approximation a SetInterface to the BinaryTreePaving");
    ExactBoxType initialRectangle( make_box("[-0.5,1.5]x[-0.3,1.0]") );

    //Define the higth of the primary root cell.
    const Nat theExtent = 2;
    //Create the binary tree;
    BinaryTreeNode * pRootTreeNode = new BinaryTreeNode(false);
    pRootTreeNode->split();
    pRootTreeNode->right_node()->split();
    pRootTreeNode->right_node()->left_node()->split();
    pRootTreeNode->right_node()->left_node()->right_node()->split();
    pRootTreeNode->right_node()->left_node()->right_node()->left_node()->set_enabled();

    //Create the BinaryTreePaving with the box related to the grid, but not to the original space
    ARIADNE_TESTING_NOTE("The initial BinaryTreePaving with the primary root cell extent = 2");
    BinaryTreePaving theOneCellPaving( theTrivialGrid, theExtent, pRootTreeNode );
    BinaryWord tree = make_binary_word("101101000");
    BinaryWord leaves = make_binary_word("00100");
    BinaryTreePaving expected_grid_tree_set1( theTrivialGrid, theExtent, tree, leaves );
    ARIADNE_TESTING_EXPECT_EQUAL( expected_grid_tree_set1, theOneCellPaving );

    ARIADNE_TESTING_NOTE("The BinaryTreePaving after adding the cell: ");
    theOneCellPaving.adjoin_outer_approximation( initialRectangle, 1 );
    tree = make_binary_word("1110101111110010011001001110010011001001111001000111001000111110010011001001001111001000000");
    leaves = make_binary_word("0001011111010111111010010100010111111010100000");
    BinaryTreePaving expected_grid_tree_set2( theTrivialGrid, 4, tree, leaves );
    ARIADNE_TESTING_EXPECT_EQUAL( expected_grid_tree_set2, theOneCellPaving );

    ARIADNE_TESTING_NOTE("Recombined BinaryTreePaving after adding the cell: ");
    std::vector< GridCell* > expected_result_arr(16);
    expected_result_arr[0] = new GridCell( theTrivialGrid, 4, make_binary_word("[0,0,1,1,0,0,0,0,0,1]") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, 4, make_binary_word("[0,0,1,1,0,0,0,0,1,1]") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, 4, make_binary_word("[0,0,1,1,0,0,0,1]") );
    expected_result_arr[3] = new GridCell( theTrivialGrid, 4, make_binary_word("[0,0,1,1,0,0,1,0,0,1]") );
    expected_result_arr[4] = new GridCell( theTrivialGrid, 4, make_binary_word("[0,0,1,1,0,0,1,0,1,1]") );
    expected_result_arr[5] = new GridCell( theTrivialGrid, 4, make_binary_word("[0,0,1,1,0,0,1,1]") );
    expected_result_arr[6] = new GridCell( theTrivialGrid, 4, make_binary_word("[0,0,1,1,0,1,0,0,0,0]") );
    expected_result_arr[7] = new GridCell( theTrivialGrid, 4, make_binary_word("[0,0,1,1,0,1,0,0,1,0]") );
    expected_result_arr[8] = new GridCell( theTrivialGrid, 4, make_binary_word("[0,0,1,1,0,1,1,0,0,0]") );
    expected_result_arr[9] = new GridCell( theTrivialGrid, 4, make_binary_word("[0,0,1,1,0,1,1,0,1,0]") );
    expected_result_arr[10] = new GridCell( theTrivialGrid, 4, make_binary_word("[0,0,1,1,1,0,0,0,0,1]") );
    expected_result_arr[11] = new GridCell( theTrivialGrid, 4, make_binary_word("[0,0,1,1,1,0,0,0,1,1]") );
    expected_result_arr[12] = new GridCell( theTrivialGrid, 4, make_binary_word("[0,0,1,1,1,0,0,1]") );
    expected_result_arr[13] = new GridCell( theTrivialGrid, 4, make_binary_word("[0,0,1,1,1,0,1,0]") );
    expected_result_arr[14] = new GridCell( theTrivialGrid, 4, make_binary_word("[0,0,1,1,1,1,0,0,0,0]") );
    expected_result_arr[15] = new GridCell( theTrivialGrid, 4, make_binary_word("[0,0,1,1,1,1,0,0,1,0]") );
    theOneCellPaving.recombine();
    check_grid_tree_subpaving_iterator( expected_result_arr, theOneCellPaving, 16 );
    clean_test_vector( expected_result_arr );

    // !!!
    ARIADNE_TESTING_NOTE("Create an outer_approximation of the rectangle on the scaling grid and get the BinaryTreePaving");
    Grid theScalingGrid(2, 2.0);
    BinaryTreePaving theOuterApproxBinaryTreePaving = outer_approximation( initialRectangle, theScalingGrid, 1 );
    //IVAN S. ZAPREEV
    //NOTE: The recombination is needed because in the scaling Grid doing
    //    outer_approximation( theScalingGrid, initialRectangle, 2 )
    //will result in subdivisions that are equal to unit cell in the original
    //space in this case we will get much more elements, e.g. the cells
    //  [-1,0]x[0,2], [0,2]x[0,2] will be subdivided as well
    theOuterApproxBinaryTreePaving.recombine();
    expected_result_arr[0] = new GridCell( theScalingGrid, 3, make_binary_word("[1,1,0,0,0,0,1,1]") );
    expected_result_arr[1] = new GridCell( theScalingGrid, 3, make_binary_word("[1,1,0,0,0,1,1]") );
    expected_result_arr[2] = new GridCell( theScalingGrid, 3, make_binary_word("[1,1,0,0,1,0,0,1]") );
    expected_result_arr[3] = new GridCell( theScalingGrid, 3, make_binary_word("[1,1,0,0,1,0,1,1]") );
    expected_result_arr[4] = new GridCell( theScalingGrid, 3, make_binary_word("[1,1,0,0,1,1]") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theOuterApproxBinaryTreePaving, 5 );
    clean_test_vector( expected_result_arr );
}

Void test_adjoin_inner_approximation_operation_one(){
    //Allocate a trivial Grid
    Grid theTrivialGrid(2, 1.0);

    //Create an empty set to which we will be adding inner approximations
    //theSetTwo = empty, with the bounding box [0,1]x[0,1]
    BinaryTreePaving theSetZero( theTrivialGrid, extentZero, new BinaryTreeNode( make_binary_word("0"), make_binary_word("0") ) );
    BinaryTreePaving theSetZeroCopy( theSetZero );
    ExactBoxSetType theBoxZeroOne = make_box("[-0.9,-0.1]x[0.1,0.9]");
    ExactBoxType theBoundingBoxZeroOne = make_box("[0.01,0.99]x[0.01,0.99]");

    // !!!
    ARIADNE_TESTING_NOTE(" theSetZero.adjoin_inner_approximation( theBoxZeroOne, 0, 4) ");
    ARIADNE_TESTING_NOTE("theSetZero");
    ARIADNE_TESTING_NOTE(theSetZero);
    ARIADNE_TESTING_NOTE("theBoxZeroOne");
    ARIADNE_TESTING_NOTE(theBoxZeroOne);
    ARIADNE_TESTING_NOTE("Nothing should be added since nothing of theBoxZeroOne intersects with the primary cell of height zero.");
    theSetZero.adjoin_inner_approximation( theBoxZeroOne, extentZero, extentFour);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetZero , theSetZeroCopy );

    // !!!
    ARIADNE_TESTING_NOTE(" theSetZero.adjoin_inner_approximation( theBoxZeroOne, theBoundingBoxZeroOne, 4) ");
    ARIADNE_TESTING_NOTE("theBoundingBoxZeroOne");
    ARIADNE_TESTING_NOTE(theBoundingBoxZeroOne);
    ARIADNE_TESTING_NOTE("Nothing should be added since theBoundingBoxZeroOne is rooted to the primary cell of height zero.");
    theSetZero.adjoin_inner_approximation( theBoxZeroOne, theBoundingBoxZeroOne, 4);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetZero, theSetZeroCopy );

    // !!!
    ARIADNE_TESTING_NOTE(" theSetZero.adjoin_inner_approximation( theBoxZeroOne, 1, 2) ");
    ARIADNE_TESTING_NOTE("The complete inner approximation of theBoxZeroOne should be added sine it is enclosed in the primary cell of height one.");
    theSetZero.adjoin_inner_approximation( theBoxZeroOne, extentOne, 2);
    std::vector< GridCell* > expected_result_arr( 4 );
    expected_result_arr[0] = new GridCell( theTrivialGrid, extentOne, make_binary_word("010011") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, extentOne, make_binary_word("010110") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, extentOne, make_binary_word("011001") );
    expected_result_arr[3] = new GridCell( theTrivialGrid, extentOne, make_binary_word("011100") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theSetZero, 4 );
    clean_test_vector( expected_result_arr );
}

Void test_adjoin_inner_approximation_operation_two(){
    //Allocate a trivial Grid
    Grid theTrivialGrid(2, 1.0);

    //Create an empty set to which we will be adding inner approximations
    //theSetTwo = empty, with the bounding box [-1,1]x[-1,1]
    BinaryTreePaving theSetOne( theTrivialGrid, extentOne, new BinaryTreeNode( make_binary_word("0"), make_binary_word("0") ) );
    BinaryTreePaving theSetOneCopy( theSetOne );
    ExactBoxSetType theBoxOneOne = make_box("[-1.9,-0.1]x[0.1,1.75]");
    ExactBoxType theBoundingBoxOneOne = make_box("[-0.99,0.99]x[-0.99,1.99]");

    // !!!
    ARIADNE_TESTING_NOTE(" theSetOne.adjoin_inner_approximation( theBoxOneOne, 1, 2) ");
    ARIADNE_TESTING_NOTE("theSetOne");
    ARIADNE_TESTING_NOTE(theSetOne);
    ARIADNE_TESTING_NOTE("theBoxOneOne");
    ARIADNE_TESTING_NOTE(theBoxOneOne);
    theSetOne.adjoin_inner_approximation( theBoxOneOne, extentOne, 2);
    std::vector< GridCell* > expected_result_arr( 7 );
    expected_result_arr[0] = new GridCell( theTrivialGrid, extentOne, make_binary_word("010001") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, extentOne, make_binary_word("010011") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, extentOne, make_binary_word("0101") );
    expected_result_arr[3] = new GridCell( theTrivialGrid, extentOne, make_binary_word("011001") );
    expected_result_arr[4] = new GridCell( theTrivialGrid, extentOne, make_binary_word("01110") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theSetOne, 5 );
    clean_test_vector( expected_result_arr );

    // !!!
    ARIADNE_TESTING_NOTE(" theSetOneCopy.adjoin_inner_approximation( theBoxOneOne, theBoundingBoxOneOne, 2) ");
    ARIADNE_TESTING_NOTE("theBoundingBoxZeroOne");
    ARIADNE_TESTING_NOTE(theBoundingBoxOneOne);
    ARIADNE_TESTING_NOTE("Nothing should be added since theBoundingBoxOneOne is rooted to the primary cell of height two.");
    theSetOneCopy.adjoin_inner_approximation( theBoxOneOne, theBoundingBoxOneOne, 2);
    expected_result_arr[0]  = new GridCell( theTrivialGrid, extentTwo, make_binary_word("00010001") );
    expected_result_arr[1]  = new GridCell( theTrivialGrid, extentTwo, make_binary_word("00010011") );
    expected_result_arr[2]  = new GridCell( theTrivialGrid, extentTwo, make_binary_word("000101") );
    expected_result_arr[3]  = new GridCell( theTrivialGrid, extentTwo, make_binary_word("00011001") );
    expected_result_arr[4]  = new GridCell( theTrivialGrid, extentTwo, make_binary_word("0001110") );
    expected_result_arr[5]  = new GridCell( theTrivialGrid, extentTwo, make_binary_word("010000") );
    expected_result_arr[6] = new GridCell( theTrivialGrid, extentTwo, make_binary_word("0100100") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theSetOneCopy, 7 );
    clean_test_vector( expected_result_arr );
}

Void test_adjoin_inner_approximation_operation_three(){
    //Allocate a trivial Grid
    Grid theTrivialGrid(2, 1.0);

    //Create a more complex set set to which we will be adding inner approximations
    //theSetTwo = [-1,0]x[-1,0] U [0,1]x[0,1] U [1,3]x[1,3]
    //The set's bounding box is [-1,3]x[-1,3]
    BinaryTreePaving theSetTwo( theTrivialGrid, extentTwo, new BinaryTreeNode( make_binary_word("1111001000100"), make_binary_word("1001001") ) );
    BinaryTreePaving theSetTwoCopy( theSetTwo );
    ExactBoxSetType theBoxTwoOne = make_box("[0.49,1.51]x[0.49,1.51]");

    // !!!
    ARIADNE_TESTING_NOTE(" theSetTwo.adjoin_inner_approximation( theBoxTwoOne, 1, 2) ");
    ARIADNE_TESTING_NOTE("theSetTwo");
    ARIADNE_TESTING_NOTE(theSetTwo);
    ARIADNE_TESTING_NOTE("theBoxTwoOne");
    ARIADNE_TESTING_NOTE(theBoxTwoOne);
    theSetTwo.adjoin_inner_approximation( theBoxTwoOne, extentOne, 2);
    std::vector< GridCell* > expected_result_arr( 5 );
    expected_result_arr[0] = new GridCell( theTrivialGrid, extentTwo, make_binary_word("0000") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, extentTwo, make_binary_word("0011") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, extentTwo, make_binary_word("11") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theSetTwo, 3 );
    clean_test_vector( expected_result_arr );

    // !!!
    ARIADNE_TESTING_NOTE(" theSetTwoCopy.adjoin_inner_approximation( theBoxTwoOne, 2, 2) ");
    ARIADNE_TESTING_NOTE("theSetTwoCopy");
    ARIADNE_TESTING_NOTE(theSetTwoCopy);
    theSetTwoCopy.adjoin_inner_approximation( theBoxTwoOne, extentTwo, 2);
    expected_result_arr[0] = new GridCell( theTrivialGrid, extentTwo, make_binary_word("0000") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, extentTwo, make_binary_word("0011") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, extentTwo, make_binary_word("011010") );
    expected_result_arr[3] = new GridCell( theTrivialGrid, extentTwo, make_binary_word("100101") );
    expected_result_arr[4] = new GridCell( theTrivialGrid, extentTwo, make_binary_word("11") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theSetTwoCopy, 5 );
    clean_test_vector( expected_result_arr );
}

Void test_restrict() {
    std::vector< GridCell* > expected_result_arr(3);

    //Allocate a trivial Grid
    Grid theTrivialGrid(2, 1.0);

    //Create the binary tree with two enabled nodes;
    BinaryTreeNode * pTwoEnabledNodeTreeH2 = new BinaryTreeNode(false);
    pTwoEnabledNodeTreeH2->split();
    pTwoEnabledNodeTreeH2->right_node()->split();
    pTwoEnabledNodeTreeH2->right_node()->left_node()->split();
    pTwoEnabledNodeTreeH2->right_node()->left_node()->left_node()->set_enabled();
    pTwoEnabledNodeTreeH2->right_node()->left_node()->right_node()->split();
    pTwoEnabledNodeTreeH2->right_node()->left_node()->right_node()->left_node()->set_enabled();
    //Create the BinaryTreePaving
    BinaryTreePaving theTwoCellPavingH2( theTrivialGrid, extentTwo, pTwoEnabledNodeTreeH2 );

    //Create another binary tree
    BinaryTreeNode * pThreeEnabledNodeTreeH2 = new BinaryTreeNode( *pTwoEnabledNodeTreeH2 );
    pThreeEnabledNodeTreeH2->left_node()->split();
    pThreeEnabledNodeTreeH2->left_node()->left_node()->set_enabled();
    pThreeEnabledNodeTreeH2->right_node()->left_node()->right_node()->right_node()->set_enabled();
    pThreeEnabledNodeTreeH2->right_node()->left_node()->right_node()->left_node()->set_disabled();
    pThreeEnabledNodeTreeH2->right_node()->left_node()->left_node()->split();
    pThreeEnabledNodeTreeH2->right_node()->left_node()->left_node()->right_node()->set_disabled();

    //Create another BinaryTreePaving
    BinaryTreePaving theThreeCellPavingH2( theTrivialGrid, extentTwo, pThreeEnabledNodeTreeH2 );

    //Create another binary tree
    BinaryTreeNode ownedThreeEnabledNodeTreeH3(false);
    BinaryTreeNode * pThreeEnabledNodeTreeH3 = &ownedThreeEnabledNodeTreeH3;
    pThreeEnabledNodeTreeH3->split();
    pThreeEnabledNodeTreeH3->right_node()->split();
    pThreeEnabledNodeTreeH3->right_node()->right_node()->copy_from( pThreeEnabledNodeTreeH2 );
    BinaryWord thePathToSubPavingRoot;
    thePathToSubPavingRoot.push_back(true);
    thePathToSubPavingRoot.push_back(true);
    //Create the BinaryTreeSubpaving
    BinaryTreeSubpaving theThreeCellSubPavingH3( theTrivialGrid, extentThree , thePathToSubPavingRoot, pThreeEnabledNodeTreeH3->right_node()->right_node() );

    // !!!
    ARIADNE_TESTING_NOTE("Test restrict operation: BinaryTreePaving1.restrict( BinaryTreePaving2 )");
    ARIADNE_TESTING_NOTE("The initial BinaryTreePaving1: ");
    expected_result_arr[0] = new GridCell( theTrivialGrid, 2, make_binary_word("[0,0]") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, 2, make_binary_word("[1,0,0,0]") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, 2, make_binary_word("[1,0,1,1]") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theThreeCellPavingH2, 3 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("The initial BinaryTreePaving2: ");
    expected_result_arr[0] = new GridCell( theTrivialGrid, 2, make_binary_word("[1,0,0]") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, 2, make_binary_word("[1,0,1,0]") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theTwoCellPavingH2, 2 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("The result after restrict: ");
    theThreeCellPavingH2.restrict( theTwoCellPavingH2 );
    expected_result_arr[0] = new GridCell( theTrivialGrid, 2, make_binary_word("[1,0,0,0]") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theThreeCellPavingH2, 1 );
    clean_test_vector( expected_result_arr );

    // !!!
    ARIADNE_TESTING_NOTE("Test restrict operation: BinaryTreePaving.restrict( BinaryTreeSubpaving )");
    ARIADNE_TESTING_NOTE("The initial BinaryTreePaving: ");
    expected_result_arr[0] = new GridCell( theTrivialGrid, 2, make_binary_word("[1,0,0]") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, 2, make_binary_word("[1,0,1,0]") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theTwoCellPavingH2, 2 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("The initial BinaryTreeSubpaving: ");
    expected_result_arr[0] = new GridCell( theTrivialGrid, 3, make_binary_word("[1,1,0,0]") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, 3, make_binary_word("[1,1,1,0,0,0]") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, 3, make_binary_word("[1,1,1,0,1,1]") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theThreeCellSubPavingH3, 3 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("The result after restrict: ");
    theTwoCellPavingH2.restrict( theThreeCellSubPavingH3 );
    expected_result_arr[0] = new GridCell( theTrivialGrid, 3, make_binary_word("[1,1,1,0,0,0]") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theTwoCellPavingH2, 1 );
    clean_test_vector( expected_result_arr );

    //TODO: Test the case when the BinaryTreePaving has primary cell of the level 3
    //    The BinaryTreeSubpaving is at level 1 and it's primary cell is at level 2
}

Void test_remove_one() {
    std::vector< GridCell* > expected_result_arr(3);

    //Allocate a trivial Grid
    Grid theTrivialGrid(2, 1.0);

    //Create the binary tree with two enabled nodes;
    BinaryTreeNode * pTwoEnabledNodeTreeH2 = new BinaryTreeNode(false);
    pTwoEnabledNodeTreeH2->split();
    pTwoEnabledNodeTreeH2->right_node()->split();
    pTwoEnabledNodeTreeH2->right_node()->left_node()->split();
    pTwoEnabledNodeTreeH2->right_node()->left_node()->left_node()->set_enabled();
    pTwoEnabledNodeTreeH2->right_node()->left_node()->right_node()->split();
    pTwoEnabledNodeTreeH2->right_node()->left_node()->right_node()->left_node()->set_enabled();
    //Create the BinaryTreePaving
    BinaryTreePaving theTwoCellPavingH2( theTrivialGrid, extentTwo, pTwoEnabledNodeTreeH2 );

    //Create another binary tree
    BinaryTreeNode * pThreeEnabledNodeTreeH2 = new BinaryTreeNode( *pTwoEnabledNodeTreeH2 );
    pThreeEnabledNodeTreeH2->left_node()->split();
    pThreeEnabledNodeTreeH2->left_node()->left_node()->set_enabled();
    pThreeEnabledNodeTreeH2->right_node()->left_node()->right_node()->right_node()->set_enabled();
    pThreeEnabledNodeTreeH2->right_node()->left_node()->right_node()->left_node()->set_disabled();
    pThreeEnabledNodeTreeH2->right_node()->left_node()->left_node()->split();
    pThreeEnabledNodeTreeH2->right_node()->left_node()->left_node()->right_node()->set_disabled();

    //Create another BinaryTreePaving
    BinaryTreePaving theThreeCellPavingH2( theTrivialGrid, extentTwo, pThreeEnabledNodeTreeH2 );

    //Create another binary tree
    BinaryTreeNode ownedThreeEnabledNodeTreeH3(false);
    BinaryTreeNode * pThreeEnabledNodeTreeH3 = &ownedThreeEnabledNodeTreeH3;
    pThreeEnabledNodeTreeH3->split();
    pThreeEnabledNodeTreeH3->right_node()->split();
    pThreeEnabledNodeTreeH3->right_node()->right_node()->copy_from( pThreeEnabledNodeTreeH2 );
    BinaryWord thePathToSubPavingRoot;
    thePathToSubPavingRoot.push_back(true);
    thePathToSubPavingRoot.push_back(true);
    //Create the BinaryTreeSubpaving
    BinaryTreeSubpaving theThreeCellSubPavingH3( theTrivialGrid, extentThree , thePathToSubPavingRoot, pThreeEnabledNodeTreeH3->right_node()->right_node() );

    // !!!
    ARIADNE_TESTING_NOTE("Test remove operation: BinaryTreePaving1.remove( BinaryTreePaving2 )");
    ARIADNE_TESTING_NOTE("The initial BinaryTreePaving1: ");
    expected_result_arr[0] = new GridCell( theTrivialGrid, 2, make_binary_word("[0,0]") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, 2, make_binary_word("[1,0,0,0]") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, 2, make_binary_word("[1,0,1,1]") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theThreeCellPavingH2, 3 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("The initial BinaryTreePaving2: ");
    expected_result_arr[0] = new GridCell( theTrivialGrid, 2, make_binary_word("[1,0,0]") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, 2, make_binary_word("[1,0,1,0]") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theTwoCellPavingH2, 2 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("The result after removal: ");
    theThreeCellPavingH2.remove( theTwoCellPavingH2 );
    expected_result_arr[0] = new GridCell( theTrivialGrid, 2, make_binary_word("[0,0]") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, 2, make_binary_word("[1,0,1,1]") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theThreeCellPavingH2, 2 );
    clean_test_vector( expected_result_arr );

    // !!!
    ARIADNE_TESTING_NOTE("Test remove operation: BinaryTreePaving.remove( BinaryTreeSubpaving )");
    ARIADNE_TESTING_NOTE("The initial BinaryTreePaving: ");
    expected_result_arr[0] = new GridCell( theTrivialGrid, 2, make_binary_word("[1,0,0]") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, 2, make_binary_word("[1,0,1,0]") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theTwoCellPavingH2, 2 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("The initial BinaryTreeSubpaving: ");
    expected_result_arr[0] = new GridCell( theTrivialGrid, 3, make_binary_word("[1,1,0,0]") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, 3, make_binary_word("[1,1,1,0,0,0]") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, 3, make_binary_word("[1,1,1,0,1,1]") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theThreeCellSubPavingH3, 3 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("The result after remove: ");
    theTwoCellPavingH2.remove( theThreeCellSubPavingH3 );
    expected_result_arr[0] = new GridCell( theTrivialGrid, 3, make_binary_word("[1,1,1,0,0,1]") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, 3, make_binary_word("[1,1,1,0,1,0]") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theTwoCellPavingH2, 2 );
    clean_test_vector( expected_result_arr );

    //TODO: Test the case when the BinaryTreePaving has primary cell of the level 3
    //    The BinaryTreeSubpaving is at level 1 and it's primary cell is at level 2
}

Void test_remove_two() {
    std::vector< GridCell* > expected_result_arr(4);

    //Allocate a trivial Grid
    Grid theTrivialGrid(2, 1.0);

    //Create a BinaryTreePaving and its copies
    BinaryTreePaving theSet01( theTrivialGrid, extentOne, make_binary_word("110010100"), make_binary_word("10001") );
    BinaryTreePaving theSet02( theSet01 );
    BinaryTreePaving theSet03( theSet01 );
    BinaryTreePaving theSet04( theSet01 );
    BinaryTreePaving theSet05( theSet01 );
    BinaryTreePaving theSet06( theSet01 );

    //Create the cells we will be removing and test removals on the copies of the set

    ARIADNE_TESTING_NOTE("Remove a GridCell (p.c. extent=0) form a BinaryTreePaving(p.c. extent=1): The cell is a subset.");
    GridCell lowerPrimaryCellSubset( theTrivialGrid, extentZero, make_binary_word("11") );
    theSet01.remove( lowerPrimaryCellSubset );
    expected_result_arr[0] = new GridCell( theTrivialGrid, extentOne, make_binary_word("00") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, extentOne, make_binary_word("1110") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theSet01, 2 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("Remove a GridCell (p.c. extent=0) form a BinaryTreePaving(p.c. extent=1): The cell does not intersect the set.");
    GridCell lowerPrimaryCellNoIntersection( theTrivialGrid, extentZero, make_binary_word("00") );
    theSet02.remove( lowerPrimaryCellNoIntersection );
    expected_result_arr[0] = new GridCell( theTrivialGrid, extentOne, make_binary_word("00") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, extentOne, make_binary_word("111") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theSet02, 2 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("Remove a GridCell (p.c. extent=0) form a BinaryTreePaving(p.c. extent=1): The cell intersects the set.");
    GridCell lowerPrimaryCellIntersection( theTrivialGrid, extentZero, BinaryWord() );
    theSet03.remove( lowerPrimaryCellIntersection );
    expected_result_arr[0] = new GridCell( theTrivialGrid, extentOne, make_binary_word("00") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theSet03, 1 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("Remove a GridCell (p.c. extent=2) form a BinaryTreePaving(p.c. extent=1): The cell is a subset.");
    GridCell higherPrimaryCellSubset( theTrivialGrid, extentTwo, make_binary_word("000011") );
    theSet04.remove( higherPrimaryCellSubset );
    expected_result_arr[0] = new GridCell( theTrivialGrid, extentTwo, make_binary_word("00000") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, extentTwo, make_binary_word("000010") );
    expected_result_arr[2] = new GridCell( theTrivialGrid, extentTwo, make_binary_word("00111") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theSet04, 3 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("Remove a GridCell (p.c. extent=2) form a BinaryTreePaving(p.c. extent=1): The cell does not intersect the set.");
    GridCell higherPrimaryCellNoIntersection( theTrivialGrid, extentTwo, make_binary_word("010") );
    theSet05.remove( higherPrimaryCellNoIntersection );
    expected_result_arr[0] = new GridCell( theTrivialGrid, extentTwo, make_binary_word("0000") );
    expected_result_arr[1] = new GridCell( theTrivialGrid, extentTwo, make_binary_word("00111") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theSet05, 2 );
    clean_test_vector( expected_result_arr );

    ARIADNE_TESTING_NOTE("Remove a GridCell (p.c. extent=2) form a BinaryTreePaving(p.c. extent=1): The cell intersects the set.");
    GridCell higherPrimaryCellIntersection( theTrivialGrid, extentTwo, make_binary_word("0011") );
    theSet06.remove( higherPrimaryCellIntersection );
    expected_result_arr[0] = new GridCell( theTrivialGrid, extentTwo, make_binary_word("0000") );
    check_grid_tree_subpaving_iterator( expected_result_arr, theSet06, 1 );
    clean_test_vector( expected_result_arr );
}

Void test_cell_subset_subset() {

    //Allocate a trivial Grid two dimensional grid
    Grid theTrivialGrid(2, 1.0);

    const Nat smallExtent = 0;
    const Nat mediumExtent = 1;
    const Nat bigExtent = 2;

    //Create the cell, will be rooted to the primary cell mediumExtent
    GridCell theCell( theTrivialGrid, mediumExtent, make_binary_word("110") );

    //Create the binary tree, will be rooted to the primary cell of height bigExtent
    BinaryTreeNode ownedBinaryTreeRoot(true);
    BinaryTreeNode * pBinaryTreeRoot = &ownedBinaryTreeRoot;
    pBinaryTreeRoot->split();
    pBinaryTreeRoot->left_node()->split();
    pBinaryTreeRoot->right_node()->split();
    pBinaryTreeRoot->left_node()->right_node()->set_disabled();
    pBinaryTreeRoot->right_node()->left_node()->set_disabled();

    pBinaryTreeRoot->left_node()->left_node()->split();
    pBinaryTreeRoot->left_node()->left_node()->right_node()->split();
    pBinaryTreeRoot->left_node()->left_node()->left_node()->split();
    pBinaryTreeRoot->left_node()->left_node()->right_node()->left_node()->set_disabled();
    pBinaryTreeRoot->left_node()->left_node()->left_node()->right_node()->set_disabled();

    //Create the BinaryTreeSubpaving, will be rooted to the primary cell of height smallExtent
    BinaryTreeSubpaving theSmallSubPaving( theTrivialGrid, smallExtent, make_binary_word("0011"),
                                      pBinaryTreeRoot->left_node()->left_node()->right_node()->right_node() );

    // !!!
    ARIADNE_TESTING_NOTE("Test subset operation GridCell (extent=1), BinaryTreeSubpaving.mince_to_tree_depth(2) (extent=0)");
    theSmallSubPaving.mince_to_tree_depth(2);
    ARIADNE_TESTING_NOTE("theCell");
    ARIADNE_TESTING_NOTE(theCell);
    ARIADNE_TESTING_NOTE("theSmallSubPaving");
    ARIADNE_TESTING_NOTE(theSmallSubPaving);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theCell, theSmallSubPaving), false );

    // !!!
    ARIADNE_TESTING_NOTE("Test subset operation GridCell (extent=1), BinaryTreeSubpaving.recombine() (extent=0)");
    theSmallSubPaving.recombine();
    ARIADNE_TESTING_NOTE("theCell");
    ARIADNE_TESTING_NOTE(theCell);
    ARIADNE_TESTING_NOTE("theSmallSubPaving");
    ARIADNE_TESTING_NOTE(theSmallSubPaving);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theCell, theSmallSubPaving), false );

    //Create the BinaryTreeSubpaving, will be rooted to the primary cell of height bigExtent
    BinaryTreeSubpaving theBigSubPaving( theTrivialGrid, bigExtent, make_binary_word("0011"),
                                    pBinaryTreeRoot->left_node()->left_node()->right_node()->right_node() );

    // !!!
    ARIADNE_TESTING_NOTE("Test subset operation GridCell (extent=1), BinaryTreeSubpaving.mince_to_tree_depth(2) (extent=2)");
    theSmallSubPaving.mince_to_tree_depth(2);
    ARIADNE_TESTING_NOTE("theCell");
    ARIADNE_TESTING_NOTE(theCell);
    ARIADNE_TESTING_NOTE("theBigSubPaving");
    ARIADNE_TESTING_NOTE(theBigSubPaving);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theCell, theBigSubPaving), true );

    // !!!
    ARIADNE_TESTING_NOTE("Test subset operation GridCell (extent=1), BinaryTreeSubpaving.recombine() (extent=2)");
    theSmallSubPaving.recombine();
    ARIADNE_TESTING_NOTE("theCell");
    ARIADNE_TESTING_NOTE(theCell);
    ARIADNE_TESTING_NOTE("theBigSubPaving");
    ARIADNE_TESTING_NOTE(theBigSubPaving);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theCell, theBigSubPaving), true );

    // !!!
    ARIADNE_TESTING_NOTE("Test subset operation GridCell (extent=1), BinaryTreeSubpaving.mince_to_tree_depth(2), left_node()->left_node()->set_disabled() (extent=2)");
    theSmallSubPaving.mince_to_tree_depth(2);
    theSmallSubPaving.binary_tree()->left_node()->left_node()->set_disabled();
    ARIADNE_TESTING_NOTE("theCell");
    ARIADNE_TESTING_NOTE(theCell);
    ARIADNE_TESTING_NOTE("theBigSubPaving");
    ARIADNE_TESTING_NOTE(theBigSubPaving);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theCell, theBigSubPaving), false );

    // !!!
    ARIADNE_TESTING_NOTE("Test subset operation GridCell (extent=1), BinaryTreeSubpaving.mince_to_tree_depth(2), left_node()->left_node()->set_enabled(), right_node()->make_leaf(false) (extent=2)");
    theSmallSubPaving.mince_to_tree_depth(2);
    theSmallSubPaving.binary_tree()->left_node()->left_node()->set_enabled();
    theSmallSubPaving.binary_tree()->right_node()->make_leaf(false);
    ARIADNE_TESTING_NOTE("theCell");
    ARIADNE_TESTING_NOTE(theCell);
    ARIADNE_TESTING_NOTE("theBigSubPaving");
    ARIADNE_TESTING_NOTE(theBigSubPaving);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theCell, theBigSubPaving), true );


    //Create the BinaryTreeSub, will be rooted to the primary cell of height smallExtent
    // BinaryTreePaving owns its root: do not transfer a subtree owned by pBinaryTreeRoot.
    BinaryTreePaving theSmallPaving( theTrivialGrid, smallExtent,
                                   new BinaryTreeNode(*pBinaryTreeRoot->left_node()->left_node()->right_node()->right_node()) );
    //Restore the binary tree
    theSmallPaving.binary_tree()->right_node()->set_enabled();
    theSmallPaving.binary_tree()->right_node()->split();

    // !!!
    ARIADNE_TESTING_NOTE("Test subset operation GridCell (extent=1), BinaryTreePaving.mince_to_tree_depth(2) (after restoring the binary tree) (extent=2)");
    theSmallPaving.mince_to_tree_depth(2);
    ARIADNE_TESTING_NOTE("theCell");
    ARIADNE_TESTING_NOTE(theCell);
    ARIADNE_TESTING_NOTE("theSmallPaving");
    ARIADNE_TESTING_NOTE(theSmallPaving);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theCell, theSmallPaving), true );

    // !!!
    ARIADNE_TESTING_NOTE("Test subset operation GridCell (extent=1), BinaryTreePaving.recombine() (extent=2)");
    theSmallPaving.recombine();
    ARIADNE_TESTING_NOTE("theCell");
    ARIADNE_TESTING_NOTE(theCell);
    ARIADNE_TESTING_NOTE("theSmallPaving");
    ARIADNE_TESTING_NOTE(theSmallPaving);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theCell, theSmallPaving), true );

    // !!!
    ARIADNE_TESTING_NOTE("Test subset operation GridCell (extent=1), BinaryTreePaving.mince_to_tree_depth(2), left_node()->left_node()->set_disabled() (extent=2)");
    theSmallPaving.mince_to_tree_depth(2);
    theSmallPaving.binary_tree()->left_node()->left_node()->set_disabled();
    ARIADNE_TESTING_NOTE("theCell");
    ARIADNE_TESTING_NOTE(theCell);
    ARIADNE_TESTING_NOTE("theSmallPaving");
    ARIADNE_TESTING_NOTE(theSmallPaving);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theCell, theSmallPaving), false );

    // !!!
    ARIADNE_TESTING_NOTE("Test subset operation GridCell (extent=1), BinaryTreePaving.mince_to_tree_depth(2), left_node()->left_node()->set_enabled(), ...right_node()->make_leaf(false) (extent=2)");
    theSmallPaving.mince_to_tree_depth(2);
    theSmallPaving.binary_tree()->left_node()->left_node()->set_enabled();
    theSmallPaving.binary_tree()->right_node()->make_leaf(false);
    ARIADNE_TESTING_NOTE("theCell");
    ARIADNE_TESTING_NOTE(theCell);
    ARIADNE_TESTING_NOTE("theSmallPaving");
    ARIADNE_TESTING_NOTE(theSmallPaving);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theCell, theSmallPaving), true );


    //Create the BinaryTreeSub, will be rooted to the primary cell of height bigExtent
    // Preserve the shared-subtree state expected by the subsequent big-paving tests.
    pBinaryTreeRoot->left_node()->left_node()->right_node()->right_node()->copy_from(theSmallPaving.binary_tree());
    BinaryTreeNode * pBinaryTreeRootCopy = new BinaryTreeNode( * pBinaryTreeRoot );
    BinaryTreePaving theBigPaving( theTrivialGrid, bigExtent, pBinaryTreeRootCopy );
    //Restore the binary tree
    pBinaryTreeRootCopy->left_node()->left_node()->right_node()->right_node()->right_node()->set_enabled();
    pBinaryTreeRootCopy->left_node()->left_node()->right_node()->right_node()->right_node()->split();

    // !!!
    ARIADNE_TESTING_NOTE("Test subset operation GridCell (extent=1), BinaryTreePaving.mince_to_tree_depth(6) (after restoring the binary tree) (extent=2)");
    theBigPaving.mince_to_tree_depth(6);
    ARIADNE_TESTING_NOTE("theCell");
    ARIADNE_TESTING_NOTE(theCell);
    ARIADNE_TESTING_NOTE("theBigPaving");
    ARIADNE_TESTING_NOTE(theBigPaving);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theCell, theBigPaving), true );

    // !!!
    ARIADNE_TESTING_NOTE("Test subset operation GridCell (extent=1), BinaryTreePaving.recombine() (extent=2)");
    theBigPaving.recombine();
    ARIADNE_TESTING_NOTE("theCell");
    ARIADNE_TESTING_NOTE(theCell);
    ARIADNE_TESTING_NOTE("theBigPaving");
    ARIADNE_TESTING_NOTE(theBigPaving);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theCell, theBigPaving), true );

    // !!!
    ARIADNE_TESTING_NOTE("Test subset operation GridCell (extent=1), BinaryTreePaving.mince_to_tree_depth(6), ...left_node()->left_node()->set_disabled() (extent=2)");
    theBigPaving.mince_to_tree_depth(6);
    theBigPaving.binary_tree()->left_node()->left_node()->right_node()->right_node()->left_node()->left_node()->set_disabled();
    ARIADNE_TESTING_NOTE("theCell");
    ARIADNE_TESTING_NOTE(theCell);
    ARIADNE_TESTING_NOTE("theBigPaving");
    ARIADNE_TESTING_NOTE(theBigPaving);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theCell, theBigPaving), false );

    // !!!
    ARIADNE_TESTING_NOTE("Test subset operation GridCell (extent=1), BinaryTreePaving.mince_to_tree_depth(6), ...left_node()->left_node()->set_enabled(), ...right_node()->make_leaf(false) (extent=2)");
    theBigPaving.mince_to_tree_depth(6);
    theBigPaving.binary_tree()->left_node()->left_node()->right_node()->right_node()->left_node()->left_node()->set_enabled();
    theBigPaving.binary_tree()->left_node()->left_node()->right_node()->right_node()->right_node()->make_leaf(false);
    ARIADNE_TESTING_NOTE("theCell");
    ARIADNE_TESTING_NOTE(theCell);
    ARIADNE_TESTING_NOTE("theBigPaving");
    ARIADNE_TESTING_NOTE(theBigPaving);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theCell, theBigPaving), true );
}

Void test_subsets_join() {

    //Allocate a trivial Grid two dimensional grid
    Grid theTrivialGrid(2, 1.0);

    const Nat smallExtent = 1;
    const Nat bigExtent = 2;

    //Make set one
    BinaryWord tree = make_binary_word("1100100");
    BinaryWord leaves = make_binary_word("1010");
    BinaryTreeNode binaryTreeRootOne( tree, leaves );
    BinaryTreeSubpaving theSet1( theTrivialGrid, smallExtent, BinaryWord(), &binaryTreeRootOne );

    //Make set two
    tree = make_binary_word("1100100");
    leaves = make_binary_word("0101");
    BinaryTreeNode binaryTreeRootTwo( tree, leaves );
    BinaryTreeSubpaving theSet2( theTrivialGrid, bigExtent, make_binary_word("00"), &binaryTreeRootTwo );

    // !!!
    ARIADNE_TESTING_NOTE("Join theSet1 and theSet2, when recombined they should give us the primary cell of height 1 rooted to the primary cell of height 2");
    ARIADNE_TESTING_NOTE("theSet1");
    ARIADNE_TESTING_NOTE(theSet1);
    ARIADNE_TESTING_NOTE("theSet2");
    ARIADNE_TESTING_NOTE(theSet2);
    BinaryTreePaving resultSet = join( theSet1, theSet2 );
    resultSet.recombine();
    BinaryTreePaving expectedResultSet( theTrivialGrid, bigExtent, make_binary_word("11000"), make_binary_word("1000") );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedResultSet, resultSet);
}

Void test_subsets_intersection() {

    //Allocate a trivial Grid two dimensional grid
    Grid theTrivialGrid(2, 1.0);

    const Nat smallExtent = 1;
    const Nat bigExtent = 2;

    //Make set one
    BinaryWord tree = make_binary_word("1100100");
    BinaryWord leaves = make_binary_word("1010");
    BinaryTreeNode binaryTreeRootOne( tree, leaves );
    BinaryTreeSubpaving theSet1( theTrivialGrid, smallExtent, BinaryWord(), &binaryTreeRootOne );

    //Make set two
    tree = make_binary_word("1100100");
    leaves = make_binary_word("0111");
    BinaryTreeNode binaryTreeRootTwo( tree, leaves );
    BinaryTreeSubpaving theSet2( theTrivialGrid, bigExtent, make_binary_word("00"), &binaryTreeRootTwo );

    // !!!
    ARIADNE_TESTING_NOTE("Intersect theSet1 and theSet2, should give us a set rooted to the primary cell of height 2");
    ARIADNE_TESTING_NOTE("theSet1");
    ARIADNE_TESTING_NOTE(theSet1);
    ARIADNE_TESTING_NOTE("theSet2");
    ARIADNE_TESTING_NOTE(theSet2);
    BinaryTreePaving resultSet = intersection( theSet1, theSet2 );
    BinaryTreePaving expectedResultSet( theTrivialGrid, bigExtent, make_binary_word("11110010000"), make_binary_word("001000") );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedResultSet, resultSet);
}

Void test_subsets_difference() {

    //Allocate a trivial Grid two dimensional grid
    Grid theTrivialGrid(2, 1.0);

    const Nat smallExtent = 1;
    const Nat bigExtent = 2;

    //Make set one
    BinaryWord tree = make_binary_word("1100100");
    BinaryWord leaves = make_binary_word("1010");
    BinaryTreeNode binaryTreeRootOne( tree, leaves );
    BinaryTreeSubpaving theSet1( theTrivialGrid, smallExtent, BinaryWord(), &binaryTreeRootOne );

    //Make set two
    tree = make_binary_word("1100100");
    leaves = make_binary_word("0111");
    BinaryTreeNode binaryTreeRootTwo( tree, leaves );
    BinaryTreeSubpaving theSet2( theTrivialGrid, bigExtent, make_binary_word("00"), &binaryTreeRootTwo );

    // !!!
    ARIADNE_TESTING_NOTE("Removing theSet1 from theSet2, should give us a set rooted to the primary cell of height 2");
    ARIADNE_TESTING_NOTE("theSet1");
    ARIADNE_TESTING_NOTE(theSet1);
    ARIADNE_TESTING_NOTE("theSet2");
    ARIADNE_TESTING_NOTE(theSet2);
    BinaryTreePaving resultSet = difference( theSet1, theSet2 );
    BinaryTreePaving expectedResultSet( theTrivialGrid, bigExtent, make_binary_word("11110010000"), make_binary_word("100000") );
    ARIADNE_TESTING_EXPECT_EQUAL( expectedResultSet, resultSet);
}

Void test_cell_intersect_subset() {

    //Allocate a trivial Grid two dimensional grid
    Grid theTrivialGrid(2, 1.0);

    BinaryWord tree = make_binary_word("1111001000100");
    BinaryWord leaves = make_binary_word("1001001");
    //Create the set and the subset of this set, they are both rooted to the same primary node of extentTwo
    BinaryTreePaving theSet( theTrivialGrid, extentTwo, new BinaryTreeNode( tree, leaves ) );
    //The subset is basically the zero level primary cell
    BinaryWord path = make_binary_word("11");
    BinaryTreeSubpaving theSubset( theTrivialGrid, extentOne, path, theSet.binary_tree()->left_node()->left_node()->right_node()->right_node() );

    GridCell theLowCell( theTrivialGrid, extentZero, make_binary_word("111") );        // does intersect with the set and the subset
    GridCell theMediumCellOne( theTrivialGrid, extentOne, make_binary_word("1000") );    // does not intersect with the set and the subset
    GridCell theMediumCellTwo( theTrivialGrid, extentOne, make_binary_word("0000") );    // does intersect with the set but not the subset
    GridCell theHighCellOne( theTrivialGrid, extentTwo, make_binary_word("11") );        // does intersect with the set but not the subset
    GridCell theHighCellTwo( theTrivialGrid, extentTwo, make_binary_word("01") );        // does not intersect with the set and the subset
    GridCell theHighCellThree( theTrivialGrid, extentTwo, make_binary_word("00") );        // does intersect with the set and the subset

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const GridCell& , const BinaryTreeSubpaving& )");
    ARIADNE_TESTING_NOTE("theSet");
    ARIADNE_TESTING_NOTE(theSet);
    ARIADNE_TESTING_NOTE("theLowCell");
    ARIADNE_TESTING_NOTE(theLowCell);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theLowCell, theSet ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const GridCell& , const BinaryTreeSubpaving& )");
    ARIADNE_TESTING_NOTE("theSet");
    ARIADNE_TESTING_NOTE(theSet);
    ARIADNE_TESTING_NOTE("theMediumCellOne");
    ARIADNE_TESTING_NOTE(theMediumCellOne);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theMediumCellOne, theSet ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const GridCell& , const BinaryTreeSubpaving& )");
    ARIADNE_TESTING_NOTE("theSet");
    ARIADNE_TESTING_NOTE(theSet);
    ARIADNE_TESTING_NOTE("theMediumCellTwo");
    ARIADNE_TESTING_NOTE(theMediumCellTwo);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theMediumCellTwo, theSet ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const GridCell& , const BinaryTreeSubpaving& )");
    ARIADNE_TESTING_NOTE("theSet");
    ARIADNE_TESTING_NOTE(theSet);
    ARIADNE_TESTING_NOTE("theHighCellOne");
    ARIADNE_TESTING_NOTE(theHighCellOne);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theHighCellOne, theSet ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const GridCell& , const BinaryTreeSubpaving& )");
    ARIADNE_TESTING_NOTE("theSet");
    ARIADNE_TESTING_NOTE(theSet);
    ARIADNE_TESTING_NOTE("");
    ARIADNE_TESTING_NOTE(theHighCellTwo);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theHighCellTwo, theSet ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const GridCell& , const BinaryTreeSubpaving& )");
    ARIADNE_TESTING_NOTE("theSet");
    ARIADNE_TESTING_NOTE(theSet);
    ARIADNE_TESTING_NOTE("theHighCellThree");
    ARIADNE_TESTING_NOTE(theHighCellThree);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theHighCellThree, theSet ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const GridCell& , const BinaryTreePaving& )");
    ARIADNE_TESTING_NOTE("theSubset");
    ARIADNE_TESTING_NOTE(theSubset);
    ARIADNE_TESTING_NOTE("theLowCell");
    ARIADNE_TESTING_NOTE(theLowCell);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theLowCell, theSubset ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const GridCell& , const BinaryTreePaving& )");
    ARIADNE_TESTING_NOTE("theSubset");
    ARIADNE_TESTING_NOTE(theSubset);
    ARIADNE_TESTING_NOTE("theMediumCellOne");
    ARIADNE_TESTING_NOTE(theMediumCellOne);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theMediumCellOne, theSubset ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const GridCell& , const BinaryTreePaving& )");
    ARIADNE_TESTING_NOTE("theSubset");
    ARIADNE_TESTING_NOTE(theSubset);
    ARIADNE_TESTING_NOTE("theMediumCellTwo");
    ARIADNE_TESTING_NOTE(theMediumCellTwo);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theMediumCellTwo, theSubset ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const GridCell& , const BinaryTreePaving& )");
    ARIADNE_TESTING_NOTE("theSubset");
    ARIADNE_TESTING_NOTE(theSubset);
    ARIADNE_TESTING_NOTE("theHighCellOne");
    ARIADNE_TESTING_NOTE(theHighCellOne);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theHighCellOne, theSubset ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const GridCell& , const BinaryTreePaving& )");
    ARIADNE_TESTING_NOTE("theSubset");
    ARIADNE_TESTING_NOTE(theSubset);
    ARIADNE_TESTING_NOTE("theHighCellTwo");
    ARIADNE_TESTING_NOTE(theHighCellTwo);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theHighCellTwo, theSubset ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const GridCell& , const BinaryTreePaving& )");
    ARIADNE_TESTING_NOTE("theSubset");
    ARIADNE_TESTING_NOTE(theSubset);
    ARIADNE_TESTING_NOTE("theHighCellThree");
    ARIADNE_TESTING_NOTE(theHighCellThree);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theHighCellThree, theSubset ), true );

}


Void test_subset_intersect_subset() {

    //Allocate a trivial Grid two dimensional grid
    Grid theTrivialGrid(2, 1.0);

    BinaryWord tree = make_binary_word("1111001000100");
    BinaryWord leaves = make_binary_word("1001001");
    //Create the set and the subset of this set, they are both rooted to the same primary node of extentTwo
    BinaryTreePaving theSet( theTrivialGrid, extentTwo, new BinaryTreeNode( tree, leaves ) );
    //The subset is basically the zero level primary cell
    BinaryWord path = make_binary_word("11");
    BinaryTreeSubpaving theSubset( theTrivialGrid, extentOne, path, theSet.binary_tree()->left_node()->left_node()->right_node()->right_node() );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const BinaryTreeSubpaving& theSet, const BinaryTreeSubpaving& theSubset )");
    ARIADNE_TESTING_NOTE("theSet");
    ARIADNE_TESTING_NOTE(theSet);
    ARIADNE_TESTING_NOTE("theSubset");
    ARIADNE_TESTING_NOTE(theSubset);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theSet, theSubset ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const BinaryTreeSubpaving& theSubset, const BinaryTreeSubpaving& theSet )");
    ARIADNE_TESTING_NOTE("theSet");
    ARIADNE_TESTING_NOTE(theSet);
    ARIADNE_TESTING_NOTE("theSubset");
    ARIADNE_TESTING_NOTE(theSubset);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theSubset, theSet ), true );

    //Make the subset one
    BinaryWord treeOne = make_binary_word("11110100001010100");
    BinaryWord leavesOne = make_binary_word("010010001");
    BinaryTreeNode binaryTreeRootOne( treeOne, leavesOne );
    //Create a set that intersects with theSet but not the theSubset
    BinaryTreeSubpaving theSubSetOne( theTrivialGrid, extentThree, make_binary_word("11"), &binaryTreeRootOne );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const BinaryTreeSubpaving& theSet, const BinaryTreeSubpaving& theSubSetOne )");
    ARIADNE_TESTING_NOTE("theSet");
    ARIADNE_TESTING_NOTE(theSet);
    ARIADNE_TESTING_NOTE("theSubSetOne");
    ARIADNE_TESTING_NOTE(theSubSetOne);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theSet, theSubSetOne ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const BinaryTreeSubpaving& theSubset, const BinaryTreeSubpaving& theSubSetOne )");
    ARIADNE_TESTING_NOTE("theSubset");
    ARIADNE_TESTING_NOTE(theSubset);
    ARIADNE_TESTING_NOTE("theSubSetOne");
    ARIADNE_TESTING_NOTE(theSubSetOne);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theSubset, theSubSetOne ), false );

    //Make the subset two: a subset that does not intersect with theSubSetOne
    //This is done simply changing the prefix in such a way that they do not intersect
    BinaryTreeSubpaving theSubSetTwo( theTrivialGrid, extentThree, make_binary_word("10"), &binaryTreeRootOne );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const BinaryTreeSubpaving& theSubSetOne, const BinaryTreeSubpaving& theSubSetTwo )");
    ARIADNE_TESTING_NOTE("theSubSetOne");
    ARIADNE_TESTING_NOTE(theSubSetOne);
    ARIADNE_TESTING_NOTE("theSubSetTwo");
    ARIADNE_TESTING_NOTE(theSubSetTwo);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theSubSetOne, theSubSetTwo ), false );

    //Make a set that intersects with theSet: Just root it to height zero and
    //reuse the tree make a copy of the tree to avoid double deallocations.
    BinaryTreePaving theSetOne( theTrivialGrid, extentZero, new BinaryTreeNode( binaryTreeRootOne ) );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const BinaryTreeSubpaving& theSet, const BinaryTreeSubpaving& theSetOne )");
    ARIADNE_TESTING_NOTE("theSet");
    ARIADNE_TESTING_NOTE(theSet);
    ARIADNE_TESTING_NOTE("theSetOne");
    ARIADNE_TESTING_NOTE(theSetOne);
    ARIADNE_TESTING_NOTE("Mince theSet and theSetOne to depth 10, just to make things more complex");
    theSet.mince_to_tree_depth(10);
    theSetOne.mince_to_tree_depth(10);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theSet, theSetOne ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool intersect( const BinaryTreeSubpaving& theSetOne, const BinaryTreeSubpaving& theSet )");
    ARIADNE_TESTING_NOTE("Mince theSet to depth 20, just to make things more complex");
    theSet.mince_to_tree_depth(20);
    ARIADNE_TESTING_EXPECT_EQUAL( intersect( theSetOne, theSet ), true );

}

Void test_subset_subset_subset() {

    //Allocate a trivial Grid two dimensional grid
    Grid theTrivialGrid(2, 1.0);

    //Create the set and the subset of this set, they are both rooted to the same primary node of extentTwo
    //theSetOne = [-1,0]x[-1,0] U [0,1]x[0,1] U [1,3]x[1,3]
    BinaryTreePaving theSetOne( theTrivialGrid, extentTwo, new BinaryTreeNode( make_binary_word("1111001000100"), make_binary_word("1001001") ) );
    //theSubsetOne = [-1,0]x[-1,0] U [0,1]x[0,1]
    BinaryTreeSubpaving theSubsetOne( theTrivialGrid, extentTwo, make_binary_word("0"), theSetOne.binary_tree()->left_node() );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool subset( const BinaryTreeSubpaving& theSetOne, const BinaryTreeSubpaving& theSubsetOne )");
    ARIADNE_TESTING_NOTE("theSetOne");
    ARIADNE_TESTING_NOTE(theSetOne);
    ARIADNE_TESTING_NOTE("theSubsetOne");
    ARIADNE_TESTING_NOTE(theSubsetOne);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theSetOne, theSubsetOne ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool subset( const BinaryTreeSubpaving& theSubsetOne, const BinaryTreeSubpaving& theSetOne )");
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theSubsetOne, theSetOne ), true );

    //Make two subsets such that the tree of the first one is a super tree of the second one,
    //but the second one contains the first one as a set
    BinaryTreeNode binaryTreeRootTwo( make_binary_word("1111001000100"), make_binary_word("1001000") );
    //theSubSetTwo = [-1,0]x[-1,0] U [0,1]x[0,1]
    BinaryTreeSubpaving theSubSetTwo( theTrivialGrid, extentTwo, BinaryWord(), &binaryTreeRootTwo );
    BinaryTreeNode binaryTreeRootThree( make_binary_word("100"), make_binary_word("10") );
    //theSubSetThree = [-1,1]x[-1,3]
    BinaryTreeSubpaving theSubSetThree( theTrivialGrid, extentThree, make_binary_word("110"), &binaryTreeRootThree );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool subset( const BinaryTreeSubpaving& theSubSetTwo, const BinaryTreeSubpaving& theSubSetThree )");
    ARIADNE_TESTING_NOTE("theSubSetTwo");
    ARIADNE_TESTING_NOTE(theSubSetTwo);
    ARIADNE_TESTING_NOTE("theSubSetThree");
    ARIADNE_TESTING_NOTE(theSubSetThree);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theSubSetTwo, theSubSetThree ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool subset( const BinaryTreeSubpaving& theSubSetThree, const BinaryTreeSubpaving& theSubSetTwo )");
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theSubSetThree, theSubSetTwo ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool subset( const BinaryTreeSubpaving& theSubSetTwo, const BinaryTreeSubpaving& theSubSetThree )");
    ARIADNE_TESTING_NOTE("Mince theSubSetThree to depth 10, just to make things more complex");
    theSubSetThree.mince_to_tree_depth(10);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theSubSetTwo, theSubSetThree ), true );

    //Create a new set which is a subset of theSubSetThree but is not a superset of theSubSetTwo
    BinaryTreeNode binaryTreeRootFour( binaryTreeRootThree );
    binaryTreeRootFour.recombine();
    binaryTreeRootFour.mince(5);
    binaryTreeRootFour.left_node()->left_node()->left_node()->left_node()->left_node()->set_disabled();
    //theSubSetThree = [-1,1]x[-1,3] \ [-1,-1]x[-0.5,-0.5]
    BinaryTreeSubpaving theSubSetFour( theTrivialGrid, extentThree, make_binary_word("110"), &binaryTreeRootFour );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool subset( const BinaryTreeSubpaving& theSubSetFour, const BinaryTreeSubpaving& theSubSetThree )");
    ARIADNE_TESTING_NOTE("theSubSetFour");
    ARIADNE_TESTING_NOTE(theSubSetFour);
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theSubSetFour, theSubSetThree ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing Bool subset( const BinaryTreeSubpaving& theSubSetTwo, const BinaryTreeSubpaving& theSubSetFour )");
    ARIADNE_TESTING_EXPECT_EQUAL( subset( theSubSetTwo, theSubSetFour ), false );

}

Void test_subset_intersects_box() {

    //Allocate a trivial Grid two dimensional grid
    Grid theTrivialGrid(2, 1.0);

    //Create the set and the subset of this set, they are both rooted to the same primary node of extentTwo
    //theSetOne = [-1,0]x[-1,0] U [0,1]x[0,1] U [1,3]x[1,3]
    BinaryTreePaving theSetOne( theTrivialGrid, extentTwo, new BinaryTreeNode( make_binary_word("1111001000100"), make_binary_word("1001001") ) );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::intersects( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that does not intersects with theSetOne");
    ExactBoxType box = make_box("[-2.0,-1.5]x[10,20]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.intersects( box ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::intersects( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that encloses theSetOne as a strict subset");
    box = make_box("[-2.0,4.0]x[-2.0,4.0]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.intersects( box ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::intersects( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that coincides with one cell of theSetOne");
    box = make_box("[-1.0,0.0]x[-1.0,0.0]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.intersects( box ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::intersects( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that is a subset of one cell of theSetOne");
    box = make_box("[1.5,2.5]x[1.5,2.5]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.intersects( box ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::intersects( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that intersects two out of three enabled cells of theSetOne");
    box = make_box("[0.3,1.7]x[0.6,1.2]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.intersects( box ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::intersects( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that is located within the bounding box of theSetOne but does not intersect any enabled cells");
    box = make_box("[-0.6,-0.3]x[1.5,3.0]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.intersects( box ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::intersects( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that shares a border with some of the enabled cells of theSetOne");
    box = make_box("[-1.0,1.0]x[1.0,3.0]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    //NOTE: The common border makes ExactBoxType:intersects(ExactBoxType) return false so we have no intersection here
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.intersects( box ), false );

    //TODO: I do not know how to test indeterminate result of the intersection here
    //Somehow I need two boxes for which we can not determine if they intersect or not.
}

Void test_subset_subset_box(){

    //Allocate a trivial Grid two dimensional grid
    Grid theTrivialGrid(2, 1.0);

    //Create the set rooted to the primary node of extentTwo
    //theSetOne = [-1,0]x[-1,0] U [0,1]x[0,1] U [1,3]x[1,3]
    BinaryTreePaving theSetOne( theTrivialGrid, extentTwo, new BinaryTreeNode( make_binary_word("1111001000100"), make_binary_word("1001001") ) );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::subset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that does not intersect with theSetOne");
    ExactBoxType box = make_box("[-2.0,-1.5]x[10,20]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.subset( box ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::subset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that encloses theSetOne as a strict subset");
    box = make_box("[-2.0,4.0]x[-2.0,4.0]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.subset( box ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::subset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that coincides with one cell of theSetOne");
    box = make_box("[-1.0,0.0]x[-1.0,0.0]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.subset( box ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::subset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that is a subset of one cell of theSetOne");
    box = make_box("[1.5,2.5]x[1.5,2.5]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.subset( box ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::subset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that intersects two out of three enabled cells of theSetOne");
    box = make_box("[0.3,1.7]x[0.6,1.2]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.subset( box ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::subset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that is located within the bounding box of theSetOne but does not intersect any enabled cells");
    box = make_box("[-0.6,-0.3]x[1.5,3.0]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.subset( box ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::subset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that shares a border with some of the enabled cells of theSetOne");
    box = make_box("[-1.0,1.0]x[1.0,3.0]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.subset( box ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::subset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that is contains the set but is located strictly subset the bounding cell of the set");
    //theSetTwo = [0,1]x[0,1]
    BinaryTreePaving theSetTwo( theTrivialGrid, extentTwo, new BinaryTreeNode( make_binary_word("1111001000100"), make_binary_word("0001000") ) );
    box = make_box("[-0.5,1.5]x[-0.5, 1.5]");
    ARIADNE_TESTING_NOTE("theSetTwo: " << theSetTwo);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetTwo.subset( box ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::subset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that contains the set but is located subset the bounding cell of the set, sharing a border with it");
    //theSetThree = [-1,0]x[-1,0] U [0,1]x[0,1]
    BinaryTreePaving theSetThree( theTrivialGrid, extentTwo, new BinaryTreeNode( make_binary_word("1111001000100"), make_binary_word("1001000") ) );
    box = make_box("[-1.0,1.0]x[-1.0, 1.0]");
    ARIADNE_TESTING_NOTE("theSetThree: " << theSetThree);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetThree.subset( box ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::subset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that contains the set and is partially located subset the bounding cell of the set, sharing a border with it");
    box = make_box("[-1.0,1.5]x[-1.0, 4.0]");
    ARIADNE_TESTING_NOTE("theSetThree: " << theSetThree);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetThree.subset( box ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::subset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that contains the set and is partially located subset the bounding cell of the set");
    //theSetFour = [0,1]x[0,1]
    BinaryTreePaving theSetFour( theTrivialGrid, extentTwo, new BinaryTreeNode( make_binary_word("1111001000100"), make_binary_word("0001000") ) );
    box = make_box("[-0.5,1.5]x[-0.5, 4.0]");
    ARIADNE_TESTING_NOTE("theSetFour: " << theSetFour);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetFour.subset( box ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::subset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that coincides with the set");
    box = make_box("[0.0,1.0]x[0.0,1.0]");
    ARIADNE_TESTING_NOTE("theSetFour: " << theSetFour);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetFour.subset( box ), true );

    //TODO: I do not know how to test indeterminate result of the subset relation here.
    //I need two boxes for which we can not determine if they one is a subset of another.

}

Void test_subset_superset_box(){
    //Allocate a trivial Grid two dimensional grid
    Grid theTrivialGrid(2, 1.0);

    //Create the set and the subset of this set, they are both rooted to the same primary node of extentTwo
    //theSetOne = [-1,0]x[-1,0] U [0,1]x[0,1] U [1,2]x[1,2] U [1,2]x[2,3] U [2,3]x[1,2] U [2,3]x[2,3]
    BinaryTreePaving theSetOne( theTrivialGrid, extentTwo, new BinaryTreeNode( make_binary_word("1111001000101100100"), make_binary_word("1001001111") ) );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::superset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that is disjoint from the set and is placed outside the set's box");
    ExactBoxType box = make_box("[5.0,6.0]x[7.0,8.0]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.superset( box ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::superset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that is disjoint from the set and is placed within the set's box");
    box = make_box("[-0.5,0.5]x[1.5,2.5]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.superset( box ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::superset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that is disjoint from the set but shares a borders with the set's box");
    box = make_box("[-2.0,-1.0]x[-1.0,2.0]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.superset( box ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::superset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that only intersects the set and is partially covered by it's cells");
    box = make_box("[1.5,2.5]x[1.5,3.5]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.superset( box ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::superset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that coincides with a cell of the set");
    box = make_box("[-1.0,-1.0]x[0.0,0.0]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.superset( box ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::superset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that is within the set and shares a border with an enabled cell");
    box = make_box("[-1.0,1.0]x[1.0,2.0]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.superset( box ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::superset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that is partially covered by two enabled cells of the set");
    box = make_box("[-0.5,0.5]x[-0.5,0.5]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.superset( box ), false );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::superset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that is a subset of an enabled cell");
    box = make_box("[1.5,1.8]x[1.3,1.9]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.superset( box ), true );

    // !!!
    ARIADNE_TESTING_NOTE("Testing ValidatedKleenean BinaryTreeSubpaving::superset( const ExactBoxType& box ) ");
    ARIADNE_TESTING_NOTE("A box that is covered by several enabled cells of the set");
    box = make_box("[1.5,2.5]x[1.5,2.5]");
    ARIADNE_TESTING_NOTE("theSetOne: " << theSetOne);
    ARIADNE_TESTING_NOTE("ExactBoxType: " << box);
    ARIADNE_TESTING_EXPECT_EQUAL( theSetOne.superset( box ), true );

    //TODO: I do not know how to test indeterminate result of the superset relation here.
    //I need two boxes for which we can not determine if they one is a superset of another.

}

void test_products() {
    Grid g1(2);
    Grid g2(1);
    GridCell gc1(g1,3,BinaryWord("0100110001"));
    GridCell gc2(g2,2,BinaryWord("1001"));

    Grid g3(3);
    GridCell gc3(g3,3,BinaryWord("011001110000011"));
    ARIADNE_TESTING_EXPECT_EQUAL(product(gc1,gc2),gc3);


    BinaryTreePaving gtp1(g1);
    gtp1.adjoin(GridCell(g1,3,BinaryWord("0100110001")));
    gtp1.adjoin(GridCell(g1,3,BinaryWord("0100110011")));

    BinaryTreePaving gtp2(g2);
    gtp2.adjoin(GridCell(g2,2,BinaryWord("1001")));

    BinaryTreePaving gtp3(g3);
    gtp3.adjoin(GridCell(g3,3,BinaryWord("011001110000011")));
    gtp3.adjoin(GridCell(g3,3,BinaryWord("011001110000111")));

    ARIADNE_TESTING_EXPECT_EQUAL(product(gtp1,gtp2),gtp3);

    EffectiveVectorMultivariateFunction x = EffectiveVectorMultivariateFunction::identity(2);
    EffectiveVectorMultivariateFunction f({x[0]*x[0]-exp(x[1])-11});

    // outer_skew_product takes a const paving: refinement must not mutate
    // its input tree, even as an implementation detail.
    BinaryTreePaving unchanged(gtp1);
    BinaryTreePaving const& const_gtp1=gtp1;
    ARIADNE_TESTING_EXPECT_EQUAL(outer_skew_product(const_gtp1,g2,f).size(),11);
    ARIADNE_TESTING_PROVE(
        static_cast<BinaryTreeSubpaving const&>(gtp1) ==
        static_cast<BinaryTreeSubpaving const&>(unchanged));


}

    Void test_branch_from_uniform_leaf() {
        Grid grid(1u);
        BinaryTreePaving enabled(grid,true);
        GridCell root=enabled.root_cell();
        GridCell left=root.split(false);
        GridCell right=root.split(true);
        GridCell left_left=left.split(false);
        GridCell left_right=left.split(true);
        PavingInterface& as_interface=enabled;

        // The root starts as one enabled leaf. Virtual branch() must
        // materialize a mutable child without changing its geometry.
        ARIADNE_TESTING_PROVE(enabled.superset(root));
        {
            SubPavingHandle left_view=as_interface.branch(false);
            ARIADNE_TESTING_PROVE(left_view.root_cell()==left);
            ARIADNE_TESTING_PROVE(left_view.superset(left));
            ARIADNE_TESTING_PROVE(enabled.superset(root));

            // A second branch must also split an enabled leaf.
            {
                SubPavingHandle grandchild=
                    static_cast<SubPavingInterface const&>(left_view).branch(true);
                ARIADNE_TESTING_PROVE(grandchild.root_cell()==left_right);
                ARIADNE_TESTING_PROVE(grandchild.superset(left_right));
                grandchild.set_root_cell(false);
            }
            ARIADNE_TESTING_PROVE(not enabled.superset(left_right));
            ARIADNE_TESTING_PROVE(enabled.superset(left_left));
            ARIADNE_TESTING_PROVE(enabled.superset(right));
        }
        enabled.recombine();
        ARIADNE_TESTING_PROVE(not enabled.superset(left_right));
        ARIADNE_TESTING_PROVE(enabled.superset(right));

        // A disabled leaf also needs real children so it can be enabled
        // locally without changing its sibling.
        BinaryTreePaving disabled(grid,false);
        {
            SubPavingHandle right_view=
                static_cast<SubPavingInterface const&>(disabled).branch(true);
            ARIADNE_TESTING_PROVE(not right_view.superset(right));
            right_view.set_root_cell(true);
            ARIADNE_TESTING_PROVE(disabled.superset(right));
            ARIADNE_TESTING_PROVE(not disabled.superset(left));
        }
        disabled.recombine();
        ARIADNE_TESTING_PROVE(disabled.superset(right));
        ARIADNE_TESTING_PROVE(not disabled.superset(left));

        // The concrete BinaryTreeSubpaving::branch() has the same contract.
        BinaryTreePaving concrete(grid,true);
        {
            BinaryTreeSubpaving child=concrete.branch(false);
            ARIADNE_TESTING_PROVE(child.root_cell()==left);
            ARIADNE_TESTING_PROVE(child.superset(left));
            child.set_root_cell(false);
        }
        concrete.recombine();
        ARIADNE_TESTING_PROVE(not concrete.superset(left));
        ARIADNE_TESTING_PROVE(concrete.superset(right));
    }


    // Additional binary-tree paving regressions (formerly grid_paving_extensions).

    Void test_projection_levels() {
    Grid grid({1.0_x,-2.0_x},{2.0_x,4.0_x});
    GridCell cell(grid,0u,BinaryWord("0110"));
    BinaryTreePaving source(cell);
    auto projected=image(source,Projection(2u,{1u}));
    auto expected=ExactBoxType{cell.box()[1]};
    ARIADNE_TESTING_PROVE(same(cast_exact_box(projected.bounding_box()),expected));
    ARIADNE_TESTING_EXPECT_EQUAL(projected.grid().origin()[0],grid.origin()[1]);
    ARIADNE_TESTING_EXPECT_EQUAL(projected.grid().lengths()[0],grid.lengths()[1]);
    for(const auto& word: {String("0"),String("011"),String("01101"),String("011010")}) {
        BinaryTreePaving partial(GridCell(grid,1u,BinaryWord(word)));
        for(const auto& indices: {Array<SizeType>{0u},Array<SizeType>{1u},Array<SizeType>{1u,0u}}) {
            Projection projection(2u,indices);
            auto image_set=image(partial,projection);
            ExactBoxType box(indices.size());
            // A paving's primary root differs from the single stored cell.
            const auto original=*partial.begin();
            for(SizeType i=0;i!=indices.size();++i) { box[i]=original.box()[indices[i]]; }
            ARIADNE_TESTING_PROVE(same(cast_exact_box(image_set.bounding_box()),box));
        }
    }
    ARIADNE_TESTING_PROVE(!image(source,Projection(2u,{})).is_empty());
    ARIADNE_TESTING_PROVE(image(BinaryTreePaving(grid),Projection(2u,{})).is_empty());
    ARIADNE_TESTING_EXPECT_EXCEPTION(image(source,Projection(1u,{0u})));
}
BinaryTreePaving mask_paving(Nat mask,Nat extent) {
    Grid grid(1u);
    BinaryTreePaving paving(grid,extent,new BinaryTreeNode(false));
    for(Nat i=0;i!=4u;++i) {
        if(mask&(1u<<i)) {
            BinaryWord word=GridCell::primary_cell_path(1u,extent,0u);
            word.append(static_cast<Bool>(i&2u)); word.append(static_cast<Bool>(i&1u));
            paving.adjoin(GridCell(grid,extent,word));
        }
    }
    return paving;
}

Void test_paving_set_algebra() {
    for(Nat a=0;a!=16u;++a) {
        for(Nat b=0;b!=16u;++b) {
            auto lhs=mask_paving(a,a%3u),rhs=mask_paving(b,b%3u);
            ARIADNE_TESTING_EXPECT_EQUAL(subset(lhs,rhs),(a&~b)==0u);
            ARIADNE_TESTING_EXPECT_EQUAL(intersect(lhs,rhs),(a&b)!=0u);
            auto united=join(lhs,rhs),common=intersection(lhs,rhs),remaining=difference(lhs,rhs);
            for(Nat i=0;i!=4u;++i) {
                BinaryWord word;word.append(static_cast<Bool>(i&2u));word.append(static_cast<Bool>(i&1u));
                GridCell cell(Grid(1u),0u,word);
                ARIADNE_TESTING_EXPECT_EQUAL(united.superset(cell),static_cast<Bool>((a|b)&(1u<<i)));
                ARIADNE_TESTING_EXPECT_EQUAL(common.superset(cell),static_cast<Bool>((a&b)&(1u<<i)));
                ARIADNE_TESTING_EXPECT_EQUAL(remaining.superset(cell),static_cast<Bool>((a&~b)&(1u<<i)));
            }
        }
    }
}

Void test_queries_cursors_and_constructors() {
    Grid grid(1u);
    ARIADNE_TESTING_PROVE(Grid(1u)!=Grid(2u));
    ARIADNE_TESTING_PROVE(Grid(2u)!=Grid(1u));
    BinaryTreePaving default_paving;
    ARIADNE_TESTING_PROVE(default_paving.is_empty());
    BinaryTreePaving enabled(grid,true),disabled(1u,false);
    ARIADNE_TESTING_PROVE(disabled.is_empty());
    BinaryTreePaving enclosing(grid,ExactBoxType{{-1.0_x,2.0_x}});
    ARIADNE_TESTING_PROVE(enclosing.is_empty());
    BinaryTreePaving encoded(grid,0u,BooleanArray{true,false,false},BooleanArray{true,false});
    ARIADNE_TESTING_EXPECT_EQUAL(encoded.size(),1u);
    BinaryTreePaving copied(encoded);
    ARIADNE_TESTING_PROVE(copied==encoded);
    const BinaryTreePaving& alias=copied; copied=alias;
    ARIADNE_TESTING_PROVE(copied==encoded);
    const auto empty_bounds=disabled.bounding_box();
    ARIADNE_TESTING_PROVE(definitely(empty_bounds.is_empty()));
    ARIADNE_TESTING_EXPECT_EQUAL(disabled.measure().raw(),FloatDP(0,dp));
    ARIADNE_TESTING_EXPECT_EQUAL(enabled.measure().raw(),FloatDP(1,dp));
    auto boxes=static_cast<ListSet<ExactBoxType>>(encoded);
    ARIADNE_TESTING_EXPECT_EQUAL(boxes.size(),1u);
    ARIADNE_TESTING_PROVE(static_cast<ListSet<ExactBoxType>>(disabled).empty());
    BinaryTreeSubpaving view(encoded);
    ARIADNE_TESTING_PROVE(view==encoded);
    auto full=mask_paving(15u,0u),half=mask_paving(3u,0u);
    for(auto* paving:{&full,&half,&disabled}) {
        ARIADNE_TESTING_NOTE("*paving = " << (*paving));
        for(const auto& query:{ExactBoxType{{-1.0_x,2.0_x}},ExactBoxType{{0.125_x,0.375_x}},ExactBoxType{{0.625_x,0.875_x}},ExactBoxType{{1.0_x,2.0_x}}}) {
            ARIADNE_TESTING_EXPECT_EQUAL(paving->disjoint(query),!paving->intersects(query));
            ARIADNE_TESTING_EXPECT_EQUAL(paving->subset(query),definitely(subset(paving->bounding_box(),query)));
            ARIADNE_TESTING_PROVE(!definitely(paving->covers(query)) || paving->superset(query));
        }
    }
    // Boundary-only contact gives the same result before and after subdivision.
    ARIADNE_TESTING_PROVE(enabled.disjoint(ExactBoxType{{1.0_x,2.0_x}}));
    enabled.mince_to_depth(2u);
    ARIADNE_TESTING_PROVE(enabled.disjoint(ExactBoxType{{1.0_x,2.0_x}}));
    enabled.mince_to_depth(0u);
    auto branch=enabled.branch(false);
    branch.mince_to_depth(0u);
    branch.subdivide(ApproximateDouble(0.125));
    BinaryTreePavingCursor singular,other_singular;
    ARIADNE_TESTING_PROVE(singular==other_singular);
    BinaryTreePavingCursor cursor(&encoded);
    ARIADNE_TESTING_PROVE(!(singular==cursor));
    ARIADNE_TESTING_PROVE(!(cursor==singular));
    BinaryTreePavingCursor assigned;assigned=cursor;
    ARIADNE_TESTING_PROVE(assigned==cursor);
    ARIADNE_TESTING_PROVE(!cursor.is_right_child());
    const auto& const_cursor=cursor;
    ARIADNE_TESTING_PROVE((*const_cursor)==encoded);
    std::ostringstream text;text<<singular;
    ARIADNE_TESTING_PROVE(!text.str().empty());
    BinaryTreePavingConstIterator invalid;
    ARIADNE_TESTING_PROVE(invalid==BinaryTreePavingConstIterator());
    auto end=encoded.end();++end;
    ARIADNE_TESTING_PROVE(end==encoded.end());
    BinaryTreePavingConstIterator last(&encoded,false),none(&encoded,indeterminate);
    ARIADNE_TESTING_PROVE(last!=none);
    ARIADNE_TESTING_PROVE(none!=last);
    ARIADNE_TESTING_PROVE(last->box().subset(ExactBoxType{{0.0_x,0.5_x}}));
    BinaryTreePaving only_right=mask_paving(8u,0u);
    BinaryTreePavingConstIterator right_last(&only_right,false);
    ARIADNE_TESTING_PROVE(same(right_last->box(),ExactBoxType{{0.75_x,1.0_x}}));
    GridCell zero;GridOpenCell open_zero;
    ARIADNE_TESTING_EXPECT_EQUAL(zero.dimension(),0u);
    ARIADNE_TESTING_EXPECT_EQUAL(open_zero.box().dimension(),0u);
    GridCell cell(grid,1u,BinaryWord("10"));
    const GridAbstractCell& abstract=cell;
    ARIADNE_TESTING_EXPECT_EQUAL(abstract.dimension(),1u);
    ARIADNE_TESTING_EXPECT_EQUAL(abstract.root_height(),1u);
    ARIADNE_TESTING_PROVE(same(abstract[0u],cell.box()[0u]));
    auto small=cell.interior(),large=GridCell(grid,1u,BinaryWord("1")).interior();
    ARIADNE_TESTING_PROVE(small<large || large<small);
    std::ostringstream open_text;open_text<<small;
    ARIADNE_TESTING_PROVE(!open_text.str().empty());
    ARIADNE_TESTING_PROVE(!GridOpenCell::intersection(small,large).empty());
    BinaryWord first_path,second_path;Nat extent=0u;
    ARIADNE_TESTING_PROVE(subset(cell,cell,nullptr,nullptr,nullptr));
    ARIADNE_TESTING_PROVE(subset(cell,cell,&first_path,&second_path,&extent));
    ARIADNE_TESTING_EXPECT_EQUAL(extent,1u);
    ARIADNE_TESTING_EXPECT_EXCEPTION(subset(cell,cell,&first_path,nullptr,nullptr));
    ARIADNE_TESTING_EXPECT_EXCEPTION(subset(cell,cell,nullptr,&second_path,nullptr));
}

Void test_approximation_apis() {
    Grid grid(1u);
    ExactBoxType box{{0.125_x,0.875_x}};
    ExactBoxSetType shape(box);
    const EffectiveEuclideanCompactSetInterface& compact_shape=shape;
    const EffectiveEuclideanSetInterface& regular_shape=shape;
    CompactSet<EffectiveTag,RealVector> compact(box);
    OpenSet<EffectiveTag,RealVector> open(box);
    OvertSet<EffectiveTag,RealVector> overt(box);
    BinaryTreePaving lower(grid),open_lower(grid),bounded_lower(grid);
    lower.adjoin_lower_approximation(overt.reference(),0u,3u);
    lower.adjoin_lower_approximation(overt.reference(),0u,3u);
    open_lower.adjoin_lower_approximation(shape,3u);
    bounded_lower.adjoin_lower_approximation(overt.reference(),box,3u);
    bounded_lower.adjoin_lower_approximation(open.reference(),box,3u);
    ARIADNE_TESTING_PROVE(!lower.is_empty());
    ARIADNE_TESTING_PROVE(!open_lower.is_empty());
    ARIADNE_TESTING_PROVE(!bounded_lower.is_empty());
    ARIADNE_TESTING_PROVE(lower.subset(ExactBoxType{{0.0_x,1.0_x}}));
    auto inner_by_extent=inner_approximation(open.reference(),grid,0u,3u);
    auto inner_by_box=inner_approximation(open.reference(),grid,box,3u);
    auto inner_regular=inner_approximation(regular_shape,grid,3u);
    ARIADNE_TESTING_PROVE(!inner_by_extent.is_empty());
    ARIADNE_TESTING_PROVE(inner_by_extent.subset(box));
    ARIADNE_TESTING_PROVE(inner_by_box.subset(box));
    ARIADNE_TESTING_PROVE(inner_regular.subset(box));
    BinaryTreePaving inner_lower(grid);
    inner_lower.adjoin_inner_approximation(LowerBoxType(box),3u);
    ARIADNE_TESTING_PROVE(inner_lower.subset(box));
    auto outer_box=outer_approximation(box,2u);
    auto outer_compact=outer_approximation(compact_shape,2u);
    ValidatedConstrainedImageSet validated(box);
    const ValidatedEuclideanCompactSetInterface& validated_compact=validated;
    auto outer_validated=outer_approximation(validated_compact,2u);
    ARIADNE_TESTING_PROVE(outer_box.superset(box));
    ARIADNE_TESTING_PROVE(outer_compact.superset(box));
    ARIADNE_TESTING_PROVE(outer_validated.superset(box));
    BinaryTreePaving over(grid);
    over.adjoin_over_approximation(box,2u);
    ARIADNE_TESTING_PROVE(over.superset(box));
    ARIADNE_TESTING_EXPECT_EXCEPTION(over.adjoin_over_approximation(ExactBoxType{{0.0_x,0.0_x}},2u));
    BinaryTreePaving compact_outer(grid);
    compact_outer.adjoin_outer_approximation(compact.reference(),2u);
    compact_outer.adjoin_outer_approximation(shape,2u);
    ARIADNE_TESTING_PROVE(compact_outer.superset(box));
    BinaryTreePaving full(grid,true);
    full.adjoin_outer_approximation(shape,2u);
    full.adjoin_lower_approximation(overt.reference(),0u,2u);
    full.adjoin_inner_approximation(open.reference(),0u,2u);
    ARIADNE_TESTING_PROVE(full.superset(GridCell(grid,0u,BinaryWord())));
    ExactBoxType wrong{{0.0_x,1.0_x},{0.0_x,1.0_x}};
    ARIADNE_TESTING_EXPECT_EXCEPTION(over.subset(wrong));
    ARIADNE_TESTING_EXPECT_EXCEPTION(over.superset(wrong));
    ARIADNE_TESTING_EXPECT_EXCEPTION(over.disjoint(wrong));
    ARIADNE_TESTING_EXPECT_EXCEPTION(over.intersects(wrong));
    ARIADNE_TESTING_EXPECT_EXCEPTION(over.covers(wrong));
    ARIADNE_TESTING_EXPECT_EXCEPTION(over.inside(wrong));
    ARIADNE_TESTING_EXPECT_EXCEPTION(over.separated(wrong));
    ARIADNE_TESTING_EXPECT_EXCEPTION(over.overlaps(wrong));
    ARIADNE_TESTING_EXPECT_EXCEPTION(over.smallest_enclosing_primary_cell(wrong));
    BinaryTreePaving wrong_grid(Grid(2u));
    ARIADNE_TESTING_EXPECT_EXCEPTION(over.adjoin(wrong_grid.root_cell()));
    ARIADNE_TESTING_EXPECT_EXCEPTION(over.adjoin(wrong_grid));
    ARIADNE_TESTING_EXPECT_EXCEPTION(subset(over,wrong_grid));
    ARIADNE_TESTING_EXPECT_EXCEPTION(intersect(over,wrong_grid));
    ARIADNE_TESTING_EXPECT_EXCEPTION(join(over,wrong_grid));
    ARIADNE_TESTING_EXPECT_EXCEPTION(intersection(over,wrong_grid));
    ARIADNE_TESTING_EXPECT_EXCEPTION(difference(over,wrong_grid));
}

Void test_products_and_drawing() {
    GridCell cell(Grid(1u),0u,BinaryWord("01"));
    BinaryTreePaving set(cell);
    auto product_set=product(cell,set);
    ARIADNE_TESTING_PROVE(same(cast_exact_box(product_set.bounding_box()),product(cell.box(),cell.box())));
    ARIADNE_TESTING_PROVE(product(cell,BinaryTreePaving(Grid(1u))).is_empty());
}

class VectorCellIterator : public ForwardConstantIteratorInterface<GridCell> {
    std::vector<GridCell> _cells;
    SizeType _position=0u;
  public:
    explicit VectorCellIterator(std::vector<GridCell> cells) : _cells(std::move(cells)) { }
    VectorCellIterator* clone() const override { return new VectorCellIterator(*this); }
    Void increment() override { ++_position; }
    GridCell const& dereference() const override { return _cells[_position]; }
    Bool equals(ForwardConstantIteratorInterface<GridCell> const& other) const override {
        auto pointer=dynamic_cast<VectorCellIterator const*>(&other);
        return pointer && _position==pointer->_position && _cells==pointer->_cells;
    }
    OutputStream& _write(OutputStream& os) const override { return os<<"VectorCellIterator("<<_position<<")"; }
};

Void test_snapshot_iterator_identity_and_borrowed_union() {
    Grid grid(1u);
    BinaryTreePaving owner(grid,true);
    owner.mince(1u);
    BinaryTreePaving other(owner);
    auto first=owner.begin(), same=owner.begin(), next=owner.begin();
    ++next;
    ARIADNE_TESTING_PROVE(first==same);
    ARIADNE_TESTING_PROVE(first!=next);
    ARIADNE_TESTING_PROVE(first!=other.begin());
    BinaryTreeSubpaving borrowed(owner);
    auto live=borrowed.begin();
    ARIADNE_TESTING_PROVE(live==borrowed.begin());
    ARIADNE_TESTING_PROVE(first!=live);
    ARIADNE_TESTING_PROVE(live!=first);
    auto snapshot=std::make_shared<BinaryTreePaving>(owner);
    BinaryTreePavingConstIterator end(snapshot,&owner,indeterminate);
    ARIADNE_TESTING_PROVE(end==owner.end());

    BinaryTreeNode foreign_node(true);
    BinaryTreeSubpaving foreign(Grid(2u),0u,BinaryWord{},&foreign_node);
    ARIADNE_TESTING_EXPECT_EXCEPTION(owner.adjoin(foreign));
    ARIADNE_TESTING_PROVE(owner.superset(owner.root_cell()));

    // Union accepts a stable, borrowed subtree as well as an owning tree.
    auto left=owner.branch(false);
    BinaryTreePaving destination(grid);
    destination.adjoin(left);
    ARIADNE_TESTING_PROVE(destination.superset(left.root_cell()));
    ARIADNE_TESTING_PROVE(!destination.superset(owner.branch(true).root_cell()));
    destination.adjoin(destination);
    destination.set_root_cell(true);
    destination.adjoin(left);
    ARIADNE_TESTING_PROVE(destination.superset(owner.root_cell()));
    ARIADNE_TESTING_PROVE(destination.superset(left.root_cell()));
    // The first inclusion is true and the reverse inclusion is false.
    SubPavingInterface const& left_interface=left;
    ARIADNE_TESTING_PROVE(!left_interface.equals(borrowed));
}

Void test_partial_tree_operations() {
    Grid grid(1u);
    BinaryTreePaving owner(grid,true);
    owner.mince(2u);
    auto left=owner.branch(false),right=owner.branch(true);
    BinaryTreeSubpaving assigned(left);assigned=right;
    ARIADNE_TESTING_PROVE(assigned==right);
    ARIADNE_TESTING_PROVE(!(left==right));
    auto disabled=mask_paving(0u,0u);
    ARIADNE_TESTING_PROVE(!(owner==disabled));
    ARIADNE_TESTING_PROVE(!subset(left,right));
    ARIADNE_TESTING_PROVE(!subset(right,left));
    ARIADNE_TESTING_PROVE(subset(left,owner));
    ARIADNE_TESTING_PROVE(!subset(owner,left));
    auto quarter=left.branch(false);
    ARIADNE_TESTING_PROVE(!subset(owner,quarter));
    BinaryTreePaving whole_cell(grid,true);
    ARIADNE_TESTING_PROVE(subset(whole_cell,whole_cell));
    ARIADNE_TESTING_PROVE(subset(disabled,left));
    ARIADNE_TESTING_PROVE(!subset(left,disabled));
    BinaryTreeSubpaving whole(owner);
    std::unique_ptr<BinaryTreeSubpaving> cloned(whole.clone());
    ARIADNE_TESTING_PROVE(cloned->superset(owner.root_cell()));
    BinaryTreePaving larger(grid,2u,new BinaryTreeNode(true));
    ARIADNE_TESTING_PROVE(!subset(larger,left));
    auto first_half=mask_paving(3u,0u);
    ARIADNE_TESTING_PROVE(subset(first_half,left));
    auto last_half=mask_paving(12u,0u);
    ARIADNE_TESTING_PROVE(subset(last_half,right));
    BinaryTreePaving empty(grid);
    ARIADNE_TESTING_PROVE(subset(empty,left));
    BinaryTreePavingCursor root_cursor(&owner);
    ARIADNE_TESTING_PROVE(!root_cursor.is_right_child());
    root_cursor.move_left();
    ARIADNE_TESTING_PROVE(!root_cursor.is_right_child());
    BinaryTreePavingConstIterator grid_iterator(&owner,true);
    VectorCellIterator vector_iterator(std::vector<GridCell>{*grid_iterator});
    ForwardConstantIteratorHandle<GridCell> grid_handle(grid_iterator),vector_handle(vector_iterator);
    ARIADNE_TESTING_PROVE(grid_handle!=vector_handle);
    BinaryTreePaving false_tree(grid,false);
    BinaryTreePavingConstIterator last_disabled(&false_tree,false);
    ARIADNE_TESTING_PROVE(last_disabled==false_tree.end());
    BinaryTreePaving left_only=mask_paving(1u,0u);
    BinaryTreePavingConstIterator last_left(&left_only,false);
    ARIADNE_TESTING_PROVE(same(last_left->box(),ExactBoxType{{0.0_x,0.25_x}}));
    auto small=GridCell(grid,0u,BinaryWord("01")).interior();
    auto large=GridCell(grid,0u,BinaryWord()).interior();
    ARIADNE_TESTING_PROVE(!GridOpenCell::intersection(small,large).empty());
    Grid skew({0.0_x,0.0_x},{8.0_x,1.0_x});
    BinaryTreePaving skew_set(GridCell(skew,0u,BinaryWord("00")));
    auto skew_branch=skew_set.branch(false).branch(false);
    skew_branch.subdivide(ApproximateDouble(0.125));
    ARIADNE_TESTING_EXPECT_EQUAL(skew_branch.tree_depth(),9u);
    for(const auto& cell:skew_branch) {
        ARIADNE_TESTING_PROVE(cell.box()[0].width().upper_raw()<=FloatDP(0.125_x,dp));
        ARIADNE_TESTING_PROVE(cell.box()[1].width().upper_raw()<=FloatDP(0.125_x,dp));
    }
    BinaryTreePaving another(Grid(2u),true);
    another.subdivide(ApproximateDouble(0.5));
    ARIADNE_TESTING_EXPECT_EQUAL(another.tree_depth(),2u);
}

Void test_ancestor_early_stops() {
    Grid grid(1u);
    GridCell cell(grid,0u,BinaryWord());
    ExactBoxType box{{0.125_x,0.875_x}};
    ExactBoxSetType shape(box);
    CompactSet<EffectiveTag,RealVector> compact(box);
    OpenSet<EffectiveTag,RealVector> open(box);
    OvertSet<EffectiveTag,RealVector> overt(box);
    ValidatedConstrainedImageSet validated(box);
    BinaryTreePaving full(grid,2u,new BinaryTreeNode(true));
    full.adjoin(cell);
    full.adjoin(BinaryTreePaving(cell));
    full.adjoin_outer_approximation(compact.reference(),2u);
    full.adjoin_outer_approximation(validated,2u);
    full.adjoin_lower_approximation(overt.reference(),0u,2u);
    full.adjoin_inner_approximation(open.reference(),0u,2u);
    ARIADNE_TESTING_PROVE(full.superset(GridCell(grid,2u,BinaryWord())));
    full.remove(cell);
    ARIADNE_TESTING_PROVE(!full.superset(cell));
    BinaryTreePaving empty(grid,2u,new BinaryTreeNode(false));
    empty.remove(cell);
    ARIADNE_TESTING_PROVE(empty.is_empty());
    auto previous_lower=mask_paving(1u,0u);
    previous_lower.adjoin_lower_approximation(shape,0u);
    ARIADNE_TESTING_PROVE(previous_lower.superset(GridCell(grid,0u,BinaryWord("00"))));
    BinaryTreePaving deep_removal(grid,true);
    deep_removal.remove(GridCell(grid,0u,BinaryWord("000")));
    ARIADNE_TESTING_PROVE(!deep_removal.superset(GridCell(grid,0u,BinaryWord("000"))));
    BinaryTreePaving small(grid,true);
    small.mince(2u);
    small.remove(GridCell(grid,0u,BinaryWord("01")));
    ARIADNE_TESTING_PROVE(!small.superset(GridCell(grid,0u,BinaryWord("01"))));
    auto partial=mask_paving(1u,0u);
    auto rhs=partial.branch(false);
    BinaryTreePaving root_enabled(grid,true);
    root_enabled.restrict(rhs);
    ARIADNE_TESTING_PROVE(root_enabled==partial);
    BinaryTreePaving root_empty(grid,false);
    root_empty.restrict(rhs);root_empty.remove(rhs);
    ARIADNE_TESTING_PROVE(root_empty.is_empty());
    BinaryTreePaving root_full(grid,true);
    root_full.remove(rhs);
    ARIADNE_TESTING_PROVE(!root_full.intersects(ExactBoxType{{0.0_x,0.25_x}}));
    BinaryTreePaving restricted(grid,2u,new BinaryTreeNode(true));
    restricted.restrict_to_extent(0u);
    ARIADNE_TESTING_PROVE(restricted.superset(cell));
    auto effective=mask_paving(1u,0u);
    effective.adjoin_outer_approximation(shape,0u);
    ARIADNE_TESTING_PROVE(effective.superset(box));
    auto validated_paving=mask_paving(1u,0u);
    RegularLocatedSet<ValidatedTag,RealVector> validated_regular(box);
    validated_paving.adjoin_outer_approximation(validated_regular.reference(),0u);
    ARIADNE_TESTING_PROVE(validated_paving.superset(box));
    validated_paving.adjoin_outer_approximation(validated_regular.reference(),2u);
    BinaryTreePaving validated_fresh(grid);
    validated_fresh.adjoin_outer_approximation(validated_regular.reference(),3u);
    ARIADNE_TESTING_PROVE(validated_fresh.superset(box));
    BinaryTreePaving validated_full(grid,true);
    validated_full.adjoin_outer_approximation(validated,2u);
    ARIADNE_TESTING_PROVE(validated_full.superset(cell));
}

Void test_cell_range_contracts() {
    Grid grid(1u);
    GridCell large=GridCell::smallest_enclosing_primary_cell(ExactBoxType{{1099511627776.0_x,1099511627777.0_x}},grid);
    ARIADNE_TESTING_PROVE(large.box().covers(ExactBoxType{{1099511627776.0_x,1099511627777.0_x}}));
    GridCell largest(grid,53u,BinaryWord());
    ARIADNE_TESTING_EXPECT_EQUAL(largest.root_height(),53u);
    ARIADNE_TESTING_EXPECT_EXCEPTION(GridCell::primary_cell_lattice_box(54u,1u));
    ARIADNE_TESTING_EXPECT_EXCEPTION(GridCell::smallest_enclosing_primary_cell(ExactBoxType{{100000000000000000.0_x,100000000000000016.0_x}},grid));
    ARIADNE_TESTING_EXPECT_EXCEPTION(GridCell::compute_lattice_box(65536u,0u,BinaryWord()));
    ARIADNE_TESTING_EXPECT_EXCEPTION(GridCell::compute_lattice_box(0u,0u,BinaryWord("0")));
    BinaryWord overlong;
    overlong.resize(static_cast<SizeType>(std::numeric_limits<Int>::max())+1u);
    ARIADNE_TESTING_EXPECT_EXCEPTION(GridCell::compute_lattice_box(1u,0u,overlong));
    Grid wrong(2u);
    GridCell cell(grid,0u,BinaryWord()),other(wrong,0u,BinaryWord());
    ARIADNE_TESTING_EXPECT_EXCEPTION(static_cast<Void>(cell==other));
    ARIADNE_TESTING_EXPECT_EXCEPTION(subset(cell,other,nullptr,nullptr,nullptr));
    BinaryTreePaving paving(grid),wrong_paving(wrong);
    ARIADNE_TESTING_EXPECT_EXCEPTION(subset(cell,wrong_paving));
    ARIADNE_TESTING_EXPECT_EXCEPTION(intersect(cell,wrong_paving));
    ARIADNE_TESTING_EXPECT_EXCEPTION(paving.remove(other));
    ARIADNE_TESTING_EXPECT_EXCEPTION(paving.remove(wrong_paving));
    ARIADNE_TESTING_EXPECT_EXCEPTION(paving.restrict(wrong_paving));
    ARIADNE_TESTING_EXPECT_EXCEPTION(product(cell,GridCell(grid,0u,BinaryWord("0"))));
    ExactBoxType wrong_box{{0.0_x,1.0_x},{0.0_x,1.0_x}};
    ExactBoxSetType wrong_shape(wrong_box);
    ValidatedConstrainedImageSet wrong_validated(wrong_box);
    ARIADNE_TESTING_EXPECT_EXCEPTION(paving.adjoin_outer_approximation(wrong_shape,1u));
    ARIADNE_TESTING_EXPECT_EXCEPTION(paving.adjoin_outer_approximation(wrong_validated,1u));
    const EffectiveEuclideanOvertSetInterface& wrong_overt=wrong_shape;
    const EffectiveEuclideanOpenSetInterface& wrong_open=wrong_shape;
    ExactBoxSetType shape(ExactBoxType{{0.0_x,1.0_x}});
    const EffectiveEuclideanOvertSetInterface& overt=shape;
    const EffectiveEuclideanOpenSetInterface& open=shape;
    ARIADNE_TESTING_EXPECT_EXCEPTION(paving.adjoin_lower_approximation(wrong_overt,0u,1u));
    ARIADNE_TESTING_EXPECT_EXCEPTION(paving.adjoin_lower_approximation(wrong_overt,wrong_box,1u));
    ARIADNE_TESTING_EXPECT_EXCEPTION(paving.adjoin_lower_approximation(overt,wrong_box,1u));
    ARIADNE_TESTING_EXPECT_EXCEPTION(paving.adjoin_inner_approximation(wrong_open,0u,1u));
    ARIADNE_TESTING_EXPECT_EXCEPTION(paving.adjoin_inner_approximation(wrong_open,wrong_box,1u));
    ARIADNE_TESTING_EXPECT_EXCEPTION(paving.adjoin_inner_approximation(open,wrong_box,1u));
}
};

} // namespace

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_binary_tree_paving");
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_grid_paving_cursor);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_cursor_lazy_cell_materialization);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_grid_paving_const_iterator);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_grid_cell);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_grid_open_cell_one);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_grid_open_cell_two);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_grid_open_cell_three);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_grid_open_cell_four);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_grid_open_cell_five);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_grid_open_cell_six);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_grid_sub_paving_one);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_grid_sub_paving_two);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_grid_sub_paving_three);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_grid_paving);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_adjoin_operation_one);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_adjoin_operation_two);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_adjoin_operation_three);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_adjoin_outer_approximation_operation);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_adjoin_inner_approximation_operation_one);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_adjoin_inner_approximation_operation_two);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_adjoin_inner_approximation_operation_three);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_restrict);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_remove_one);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_remove_two);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_cell_subset_subset);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_subsets_join);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_subsets_intersection);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_subsets_difference);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_cell_intersect_subset);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_subset_intersect_subset);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_subset_subset_subset);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_subset_intersects_box);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_subset_subset_box);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_subset_superset_box);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_products);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_branch_from_uniform_leaf);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_cell_range_contracts);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_snapshot_iterator_identity_and_borrowed_union);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_partial_tree_operations);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_ancestor_early_stops);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_projection_levels);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_paving_set_algebra);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_queries_cursors_and_constructors);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_approximation_apis);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTreePaving, test_products_and_drawing);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}
