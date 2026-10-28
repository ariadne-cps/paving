/***************************************************************************
 *            test_binary_tree.cpp
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
#include "utility/stlio.hpp"

#include "macro/general.hpp"
#include "foundation/logical.hpp"
#include "numeric/numeric.hpp"

#include "paving/binary_tree.hpp"

#include "macro/testing.hpp"

using namespace Ariadne;
using namespace std;

inline Void test_binary_tree() {
    const BinaryTreeNode * binary_tree_node_null_pointer = nullptr;

    // !!!
    ARIADNE_PRINT_TEST_CASE_TITLE("Allocate an ebabled node and manipulate it");

    BinaryTreeNode theBinaryTreeRoot(true);
    ARIADNE_PRINT_TEST_COMMENT("Initial tree node");
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.is_leaf(), true );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.is_enabled(), true );

    theBinaryTreeRoot.set_disabled();
    ARIADNE_PRINT_TEST_COMMENT("Disable the node");
    BinaryTreeNode expected_node1(false);
    ARIADNE_TEST_COMPARE( expected_node1, ==, theBinaryTreeRoot );

    ARIADNE_PRINT_TEST_COMMENT("Making the leaf node intermediate should cause the IsALeafNodeException exception");
    ARIADNE_TEST_THROWS( theBinaryTreeRoot.set_unknown(), IsALeafNodeException);

    ARIADNE_PRINT_TEST_COMMENT("The node still have to be disabled");
    ARIADNE_TEST_COMPARE( expected_node1, ==, theBinaryTreeRoot );

    ARIADNE_PRINT_TEST_COMMENT("Enable the node");
    theBinaryTreeRoot.set_enabled();
    BinaryTreeNode expected_node2(true);
    ARIADNE_TEST_COMPARE( expected_node2, ==, theBinaryTreeRoot );

    ARIADNE_PRINT_TEST_COMMENT("Compare unequal non-null subtrees and null/non-null nodes");
    BinaryTreeNode unequal_tree_one(false);
    unequal_tree_one.split();
    BinaryTreeNode unequal_tree_two(unequal_tree_one);
    unequal_tree_one.left_node()->set_enabled();
    ARIADNE_TEST_EQUAL(BinaryTreeNode::is_equal_nodes(&unequal_tree_one,&unequal_tree_two),false);
    ARIADNE_TEST_EQUAL(BinaryTreeNode::is_equal_nodes(nullptr,&unequal_tree_one),false);
    ARIADNE_TEST_EQUAL(BinaryTreeNode::is_equal_nodes(&unequal_tree_one,nullptr),false);

    BinaryTreeNode unequal_right_one(false);
    unequal_right_one.split();
    BinaryTreeNode unequal_right_two(unequal_right_one);
    unequal_right_one.right_node()->set_enabled();
    ARIADNE_TEST_EQUAL(unequal_right_one==unequal_right_two,false);

    BinaryTreeNode unknown_node(false);
    unknown_node.split();
    BinaryTreeNode enabled_leaf(true);
    ARIADNE_TEST_EQUAL(unknown_node==enabled_leaf,false);

    BooleanArray truncated_tree(1), truncated_leaves(1);
    truncated_tree[0]=true;
    truncated_leaves[0]=false;
    // Even a failed reconstruction must release all previously created subnodes.
    ARIADNE_TEST_FAIL(BinaryTreeNode(truncated_tree,truncated_leaves));
    BooleanArray nested_truncated_tree = {true,true,false};
    BooleanArray nested_truncated_leaves = {true};
    ARIADNE_TEST_FAIL(BinaryTreeNode(nested_truncated_tree,nested_truncated_leaves));

    BinaryTreeNode contract_node(false);
    ARIADNE_TEST_FAIL(BinaryTreeNode::restrict(nullptr,&contract_node));
    ARIADNE_TEST_FAIL(BinaryTreeNode::restrict(&contract_node,nullptr));
    ARIADNE_TEST_FAIL(BinaryTreeNode::remove(nullptr,&contract_node));
    ARIADNE_TEST_FAIL(BinaryTreeNode::remove(&contract_node,nullptr));

    // !!!
    ARIADNE_PRINT_TEST_CASE_TITLE("Split the enabled node");
    ARIADNE_PRINT_TEST_COMMENT("Should mark the node as intermediate and create two enabled subnodes");
    theBinaryTreeRoot.split();
    ARIADNE_TEST_COMPARE( theBinaryTreeRoot.left_node(), !=, binary_tree_node_null_pointer );
    ARIADNE_TEST_COMPARE( theBinaryTreeRoot.right_node(), !=, binary_tree_node_null_pointer );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.is_enabled(), false );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.is_disabled(), false );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.left_node()->is_leaf(), true );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.right_node()->is_leaf(), true );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.left_node()->is_enabled(), true );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.right_node()->is_enabled(), true );

    // !!!
    ARIADNE_PRINT_TEST_CASE_TITLE("Split a splited node");
    ARIADNE_PRINT_TEST_COMMENT("We expect an unchanged tree, since it is already split");
    theBinaryTreeRoot.mince(1);
    ARIADNE_TEST_COMPARE( theBinaryTreeRoot.left_node(), !=, binary_tree_node_null_pointer );
    ARIADNE_TEST_COMPARE( theBinaryTreeRoot.right_node(), !=, binary_tree_node_null_pointer );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.is_enabled(), false );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.is_disabled(), false );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.left_node()->is_leaf(), true );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.right_node()->is_leaf(), true );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.left_node()->is_enabled(), true );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.right_node()->is_enabled(), true );

    // !!!
    ARIADNE_PRINT_TEST_CASE_TITLE("Copy the node by using the copy constructor");
    ARIADNE_PRINT_TEST_COMMENT("The entire subtree should be copied");
    BinaryTreeNode theBinaryTreeCopy( theBinaryTreeRoot );
    ARIADNE_TEST_COMPARE( theBinaryTreeCopy.left_node(), !=, binary_tree_node_null_pointer );
    ARIADNE_TEST_COMPARE( theBinaryTreeCopy.right_node(), !=, binary_tree_node_null_pointer );
    ARIADNE_TEST_EQUAL( theBinaryTreeCopy.is_enabled(), false );
    ARIADNE_TEST_EQUAL( theBinaryTreeCopy.is_disabled(), false );
    ARIADNE_TEST_EQUAL( theBinaryTreeCopy.left_node()->is_leaf(), true );
    ARIADNE_TEST_EQUAL( theBinaryTreeCopy.right_node()->is_leaf(), true );
    ARIADNE_TEST_EQUAL( theBinaryTreeCopy.left_node()->is_enabled(), true );
    ARIADNE_TEST_EQUAL( theBinaryTreeCopy.right_node()->is_enabled(), true );

    // !!!
    ARIADNE_PRINT_TEST_CASE_TITLE("Recombine the new tree and see what happens");
    ARIADNE_PRINT_TEST_COMMENT("The new tree should be reduced to a node");
    ARIADNE_PRINT_TEST_COMMENT("The old tree should remain unchanged");
    theBinaryTreeCopy.recombine();
    ARIADNE_PRINT_TEST_COMMENT( "Reduced tree" );
    ARIADNE_TEST_EQUAL( theBinaryTreeCopy.is_leaf(), true );
    ARIADNE_TEST_EQUAL( theBinaryTreeCopy.is_enabled(), true );
    ARIADNE_PRINT_TEST_COMMENT( "The original tree" );
    ARIADNE_TEST_COMPARE( theBinaryTreeRoot.left_node(), !=, binary_tree_node_null_pointer );
    ARIADNE_TEST_COMPARE( theBinaryTreeRoot.right_node(), !=, binary_tree_node_null_pointer );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.is_enabled(), false );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.is_disabled(), false );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.left_node()->is_leaf(), true );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.right_node()->is_leaf(), true );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.left_node()->is_enabled(), true );
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.right_node()->is_enabled(), true );

    // !!!
    ARIADNE_PRINT_TEST_CASE_TITLE("Disable the left tree leaf, recombine the tree and split the disabled leaf to level 3");
    ARIADNE_PRINT_TEST_COMMENT("A tree with disabled left node");
    theBinaryTreeRoot.left_node()->set_disabled();
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.left_node()->is_disabled(), true );

    ARIADNE_PRINT_TEST_COMMENT("A tree after recombination" );
    theBinaryTreeRoot.recombine();
    ARIADNE_TEST_COMPARE( theBinaryTreeRoot.left_node(), !=, binary_tree_node_null_pointer );
    ARIADNE_TEST_COMPARE( theBinaryTreeRoot.right_node(), !=, binary_tree_node_null_pointer );

    ARIADNE_PRINT_TEST_COMMENT( "A tree after splitting the disabled left node" );
    theBinaryTreeRoot.left_node()->mince(3);
    ARIADNE_TEST_EQUAL( theBinaryTreeRoot.left_node()->is_leaf(), true );

    ARIADNE_PRINT_TEST_COMMENT("A tree after splitting the root node" );
    theBinaryTreeRoot.mince(3);
    BooleanArray treeArray(9), leafArray(5);
    treeArray[0] = true;
    treeArray[1] = false; leafArray[0] = false;
    treeArray[2] = true;
    treeArray[3] = true;
    treeArray[4] = false; leafArray[1] = true;
    treeArray[5] = false; leafArray[2] = true;
    treeArray[6] = true;
    treeArray[7] = false; leafArray[3] = true;
    treeArray[8] = false; leafArray[4] = true;
    BinaryTreeNode expected_binary_tree( treeArray , leafArray );
    ARIADNE_TEST_COMPARE( expected_binary_tree, ==, theBinaryTreeRoot );

    ARIADNE_PRINT_TEST_COMMENT("Testing the depth of the binary tree");
    ARIADNE_TEST_EQUAL(theBinaryTreeRoot.depth(), 3u);

    // !!!
    ARIADNE_PRINT_TEST_CASE_TITLE("Disable the left most enabled leaf, recombine the tree");
    theBinaryTreeRoot.right_node()->left_node()->left_node()->set_disabled();
    ARIADNE_PRINT_TEST_COMMENT("A tree with enabled left-most leaf");
    leafArray[1] = false;
    BinaryTreeNode expected_binary_tree1( treeArray , leafArray );
    ARIADNE_TEST_COMPARE( expected_binary_tree1, ==, theBinaryTreeRoot );

    ARIADNE_PRINT_TEST_COMMENT("A tree after recombination");
    theBinaryTreeRoot.recombine();
    expected_binary_tree1.right_node()->right_node()->make_leaf(true);
    ARIADNE_TEST_COMPARE( expected_binary_tree1, ==, theBinaryTreeRoot );

    // !!!
    ARIADNE_PRINT_TEST_CASE_TITLE("Testing the add_enabled( BinaryWord& ) method");
    ARIADNE_PRINT_TEST_COMMENT("Adding enabled node to an enabled node, the former one is defined by an empty path");

    BinaryTreeNode theNewBinaryTreeRoot(true);
    BinaryWord binaryWord;
    theNewBinaryTreeRoot.add_enabled( binaryWord );
    ARIADNE_PRINT_TEST_COMMENT("The binary tree should stay intact, i.e. consist of one enabled node");
    BinaryTreeNode expected_binary_tree2( true );
    ARIADNE_TEST_COMPARE( expected_binary_tree2, ==, theNewBinaryTreeRoot );

    ARIADNE_PRINT_TEST_COMMENT("Adding enabled sub node, path: (true,false) to a disabled node");
    theNewBinaryTreeRoot.set_disabled();
    binaryWord.push_back(true);
    binaryWord.push_back(false);
    theNewBinaryTreeRoot.add_enabled( binaryWord );
    ARIADNE_PRINT_TEST_COMMENT("The binary tree have five nodes, four leafs and one (true, false) is enabled");
    expected_binary_tree2.set_disabled();
    expected_binary_tree2.split();
    expected_binary_tree2.right_node()->split();
    expected_binary_tree2.right_node()->left_node()->set_enabled();
    ARIADNE_TEST_COMPARE( expected_binary_tree2, ==, theNewBinaryTreeRoot );

    ARIADNE_PRINT_TEST_COMMENT("Adding enabled sub node, path: (true) to a disabled node");
    binaryWord = BinaryWord();
    binaryWord.push_back(true);
    theNewBinaryTreeRoot.add_enabled( binaryWord );
    ARIADNE_PRINT_TEST_COMMENT("The binary tree have three nodes, two leafs and one (true) is enabled");
    expected_binary_tree2.right_node()->make_leaf(true);
    ARIADNE_TEST_COMPARE( expected_binary_tree2, ==, theNewBinaryTreeRoot );

    ARIADNE_PRINT_TEST_COMMENT("Adding a deeper path below an already enabled leaf changes nothing");
    BinaryWord deeperEnabledPath;
    deeperEnabledPath.push_back(true);
    deeperEnabledPath.push_back(false);
    theNewBinaryTreeRoot.add_enabled(deeperEnabledPath);
    ARIADNE_TEST_COMPARE( expected_binary_tree2, ==, theNewBinaryTreeRoot );

    ARIADNE_PRINT_TEST_COMMENT("Adding enabled sub node, path: (false, true) to a disabled node");
    binaryWord = BinaryWord();
    binaryWord.push_back(false);
    binaryWord.push_back(true);
    theNewBinaryTreeRoot.add_enabled( binaryWord );
    ARIADNE_PRINT_TEST_COMMENT("The binary tree have five nodes, four leafs and two (false, true) and (true) are enabled");
    expected_binary_tree2.left_node()->split();
    expected_binary_tree2.left_node()->right_node()->set_enabled();
    ARIADNE_TEST_COMPARE( expected_binary_tree2, ==, theNewBinaryTreeRoot );

    ARIADNE_PRINT_TEST_COMMENT("Testing add_enabled(subtree,path) early enabled and left path");
    BinaryTreeNode source_subtree(true);
    BinaryWord subtreePath("0");
    ARIADNE_TEST_EQUAL(subtreePath.size(),1u);
    ARIADNE_TEST_EQUAL(source_subtree.is_enabled(),true);

    BinaryTreeNode enabled_destination(true);
    enabled_destination.add_enabled(&source_subtree,subtreePath);
    ARIADNE_TEST_EQUAL(enabled_destination.is_enabled(),true);

    BinaryTreeNode left_destination(false);
    ARIADNE_TEST_EQUAL(left_destination.is_leaf(),true);
    ARIADNE_TEST_EQUAL(left_destination.is_disabled(),true);
    left_destination.add_enabled(&source_subtree,subtreePath);
    ARIADNE_TEST_EQUAL(left_destination.is_enabled(subtreePath),true);

    ARIADNE_PRINT_TEST_COMMENT("Adding an enabled subtree along a right-hand path");
    BinaryWord rightSubtreePath;
    rightSubtreePath.push_back(true);
    BinaryTreeNode right_destination(false);
    right_destination.add_enabled(&source_subtree,rightSubtreePath);
    ARIADNE_TEST_EQUAL(right_destination.is_leaf(),false);
    ARIADNE_TEST_EQUAL(right_destination.left_node()->is_disabled(),true);
    ARIADNE_TEST_EQUAL(right_destination.right_node()->is_enabled(),true);

    ARIADNE_PRINT_TEST_COMMENT("Testing intersect/subset short-circuit outcomes");
    BinaryTreeNode disabled_subtree(false);
    disabled_subtree.split();
    BinaryTreeNode enabled_subtree(false);
    enabled_subtree.split();
    enabled_subtree.left_node()->set_enabled();
    BinaryTreeNode enabled_leaf_for_relation(true);
    BinaryTreeNode disabled_leaf_for_relation(false);

    ARIADNE_TEST_EQUAL(BinaryTreeNode::intersect(&disabled_subtree,&enabled_leaf_for_relation),false);
    ARIADNE_TEST_EQUAL(BinaryTreeNode::intersect(&enabled_leaf_for_relation,&disabled_subtree),false);
    ARIADNE_TEST_EQUAL(BinaryTreeNode::subset(&enabled_subtree,&disabled_leaf_for_relation),false);

    ARIADNE_PRINT_TEST_COMMENT("An entirely disabled subtree is a subset of a disabled leaf");
    ARIADNE_TEST_EQUAL(disabled_subtree.is_leaf(),false);
    ARIADNE_TEST_EQUAL(disabled_subtree.has_enabled(),false);
    ARIADNE_TEST_EQUAL(BinaryTreeNode::subset(&disabled_subtree,&disabled_leaf_for_relation),true);
}


Void test_copy_and_reset() {
    BinaryTreeNode empty(BooleanArray{},BooleanArray{});
    BinaryTreeNode no_leaves(BooleanArray{false},BooleanArray{});
    ARIADNE_TEST_ASSERT(empty.is_disabled());
    ARIADNE_TEST_ASSERT(no_leaves.is_disabled());
    ARIADNE_TEST_ASSERT(empty.node_to_string().find("BinaryTreeNode")!=String::npos);

    BinaryTreeNode branch(false);
    branch.split();
    branch.left_node()->set_enabled();
    ARIADNE_TEST_THROWS(branch.set_enabled(),NotALeafNodeException);
    BinaryTreeNode copied(false);
    copied=branch;
    ARIADNE_TEST_ASSERT(copied==branch);
    copied.right_node()->set_enabled();
    ARIADNE_TEST_ASSERT(branch.right_node()->is_disabled());
    const BinaryTreeNode& alias=copied;
    copied=alias;
    ARIADNE_TEST_ASSERT(copied.all_enabled());
    copied=empty;
    ARIADNE_TEST_ASSERT(copied.is_leaf());
    ARIADNE_TEST_ASSERT(copied.is_disabled());
    copied=branch;
    copied.copy_from(&empty);
    ARIADNE_TEST_ASSERT(copied.is_leaf());
    copied.copy_from(&branch);
    copied.copy_from(&copied);
    ARIADNE_TEST_ASSERT(copied==branch);
    copied.copy_from(nullptr);
    ARIADNE_TEST_ASSERT(copied==branch);
    copied.set(true);
    ARIADNE_TEST_ASSERT(copied.is_leaf());
    ARIADNE_TEST_ASSERT(copied.left_node()==nullptr);
    ARIADNE_TEST_ASSERT(copied.right_node()==nullptr);
    copied.set(false);
    ARIADNE_TEST_ASSERT(copied.is_disabled());
    copied.split();
    copied.left_node()->set_enabled();
    ARIADNE_TEST_ASSERT(copied.has_enabled());
}

Int main() {
    ARIADNE_TEST_CALL(test_copy_and_reset());

    test_binary_tree();

    return ARIADNE_TEST_FAILURES;
}

