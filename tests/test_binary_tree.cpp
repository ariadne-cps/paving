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

#include "utility/macros.hpp"
#include "foundation/logical.hpp"
#include "numeric/numeric.hpp"

#include "paving/binary_tree.hpp"

#include "ariadne-testing.hpp"

using namespace Ariadne;
using namespace std;

class TestBinaryTree : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

    Void test_binary_tree() {
    const BinaryTreeNode * binary_tree_node_null_pointer = nullptr;

    // !!!
    ARIADNE_TESTING_NOTE("Allocate an ebabled node and manipulate it");

    BinaryTreeNode theBinaryTreeRoot(true);
    ARIADNE_TESTING_NOTE("Initial tree node");
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.is_leaf(), true );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.is_enabled(), true );

    theBinaryTreeRoot.set_disabled();
    ARIADNE_TESTING_NOTE("Disable the node");
    BinaryTreeNode expected_node1(false);
    ARIADNE_TESTING_EXPECT_EQUAL(expected_node1, theBinaryTreeRoot);

    ARIADNE_TESTING_NOTE("Making the leaf node intermediate should cause the IsALeafNodeException exception");
    ARIADNE_TESTING_EXPECT_THROWS(IsALeafNodeException, theBinaryTreeRoot.set_unknown());

    ARIADNE_TESTING_NOTE("The node still have to be disabled");
    ARIADNE_TESTING_EXPECT_EQUAL(expected_node1, theBinaryTreeRoot);

    ARIADNE_TESTING_NOTE("Enable the node");
    theBinaryTreeRoot.set_enabled();
    BinaryTreeNode expected_node2(true);
    ARIADNE_TESTING_EXPECT_EQUAL(expected_node2, theBinaryTreeRoot);

    ARIADNE_TESTING_NOTE("Compare unequal non-null subtrees and null/non-null nodes");
    BinaryTreeNode unequal_tree_one(false);
    unequal_tree_one.split();
    BinaryTreeNode unequal_tree_two(unequal_tree_one);
    unequal_tree_one.left_node()->set_enabled();
    ARIADNE_TESTING_EXPECT_EQUAL(BinaryTreeNode::is_equal_nodes(&unequal_tree_one,&unequal_tree_two),false);
    ARIADNE_TESTING_EXPECT_EQUAL(BinaryTreeNode::is_equal_nodes(nullptr,&unequal_tree_one),false);
    ARIADNE_TESTING_EXPECT_EQUAL(BinaryTreeNode::is_equal_nodes(&unequal_tree_one,nullptr),false);

    BinaryTreeNode unequal_right_one(false);
    unequal_right_one.split();
    BinaryTreeNode unequal_right_two(unequal_right_one);
    unequal_right_one.right_node()->set_enabled();
    ARIADNE_TESTING_EXPECT_EQUAL(unequal_right_one==unequal_right_two,false);

    BinaryTreeNode unknown_node(false);
    unknown_node.split();
    BinaryTreeNode enabled_leaf(true);
    ARIADNE_TESTING_EXPECT_EQUAL(unknown_node==enabled_leaf,false);

    BooleanArray truncated_tree(1), truncated_leaves(1);
    truncated_tree[0]=true;
    truncated_leaves[0]=false;
    // Even a failed reconstruction must release all previously created subnodes.
    ARIADNE_TESTING_EXPECT_EXCEPTION(BinaryTreeNode(truncated_tree,truncated_leaves));
    BooleanArray nested_truncated_tree = {true,true,false};
    BooleanArray nested_truncated_leaves = {true};
    ARIADNE_TESTING_EXPECT_EXCEPTION(BinaryTreeNode(nested_truncated_tree,nested_truncated_leaves));

    BinaryTreeNode contract_node(false);
    ARIADNE_TESTING_EXPECT_EXCEPTION(BinaryTreeNode::restrict(nullptr,&contract_node));
    ARIADNE_TESTING_EXPECT_EXCEPTION(BinaryTreeNode::restrict(&contract_node,nullptr));
    ARIADNE_TESTING_EXPECT_EXCEPTION(BinaryTreeNode::remove(nullptr,&contract_node));
    ARIADNE_TESTING_EXPECT_EXCEPTION(BinaryTreeNode::remove(&contract_node,nullptr));

    // !!!
    ARIADNE_TESTING_NOTE("Split the enabled node");
    ARIADNE_TESTING_NOTE("Should mark the node as intermediate and create two enabled subnodes");
    theBinaryTreeRoot.split();
    ARIADNE_TESTING_PROVE(theBinaryTreeRoot.left_node() != binary_tree_node_null_pointer);
    ARIADNE_TESTING_PROVE(theBinaryTreeRoot.right_node() != binary_tree_node_null_pointer);
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.is_enabled(), false );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.is_disabled(), false );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.left_node()->is_leaf(), true );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.right_node()->is_leaf(), true );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.left_node()->is_enabled(), true );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.right_node()->is_enabled(), true );

    // !!!
    ARIADNE_TESTING_NOTE("Split a splited node");
    ARIADNE_TESTING_NOTE("We expect an unchanged tree, since it is already split");
    theBinaryTreeRoot.mince(1);
    ARIADNE_TESTING_PROVE(theBinaryTreeRoot.left_node() != binary_tree_node_null_pointer);
    ARIADNE_TESTING_PROVE(theBinaryTreeRoot.right_node() != binary_tree_node_null_pointer);
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.is_enabled(), false );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.is_disabled(), false );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.left_node()->is_leaf(), true );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.right_node()->is_leaf(), true );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.left_node()->is_enabled(), true );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.right_node()->is_enabled(), true );

    // !!!
    ARIADNE_TESTING_NOTE("Copy the node by using the copy constructor");
    ARIADNE_TESTING_NOTE("The entire subtree should be copied");
    BinaryTreeNode theBinaryTreeCopy( theBinaryTreeRoot );
    ARIADNE_TESTING_PROVE(theBinaryTreeCopy.left_node() != binary_tree_node_null_pointer);
    ARIADNE_TESTING_PROVE(theBinaryTreeCopy.right_node() != binary_tree_node_null_pointer);
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeCopy.is_enabled(), false );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeCopy.is_disabled(), false );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeCopy.left_node()->is_leaf(), true );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeCopy.right_node()->is_leaf(), true );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeCopy.left_node()->is_enabled(), true );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeCopy.right_node()->is_enabled(), true );

    // !!!
    ARIADNE_TESTING_NOTE("Recombine the new tree and see what happens");
    ARIADNE_TESTING_NOTE("The new tree should be reduced to a node");
    ARIADNE_TESTING_NOTE("The old tree should remain unchanged");
    theBinaryTreeCopy.recombine();
    ARIADNE_TESTING_NOTE( "Reduced tree" );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeCopy.is_leaf(), true );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeCopy.is_enabled(), true );
    ARIADNE_TESTING_NOTE( "The original tree" );
    ARIADNE_TESTING_PROVE(theBinaryTreeRoot.left_node() != binary_tree_node_null_pointer);
    ARIADNE_TESTING_PROVE(theBinaryTreeRoot.right_node() != binary_tree_node_null_pointer);
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.is_enabled(), false );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.is_disabled(), false );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.left_node()->is_leaf(), true );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.right_node()->is_leaf(), true );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.left_node()->is_enabled(), true );
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.right_node()->is_enabled(), true );

    // !!!
    ARIADNE_TESTING_NOTE("Disable the left tree leaf, recombine the tree and split the disabled leaf to level 3");
    ARIADNE_TESTING_NOTE("A tree with disabled left node");
    theBinaryTreeRoot.left_node()->set_disabled();
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.left_node()->is_disabled(), true );

    ARIADNE_TESTING_NOTE("A tree after recombination" );
    theBinaryTreeRoot.recombine();
    ARIADNE_TESTING_PROVE(theBinaryTreeRoot.left_node() != binary_tree_node_null_pointer);
    ARIADNE_TESTING_PROVE(theBinaryTreeRoot.right_node() != binary_tree_node_null_pointer);

    ARIADNE_TESTING_NOTE( "A tree after splitting the disabled left node" );
    theBinaryTreeRoot.left_node()->mince(3);
    ARIADNE_TESTING_EXPECT_EQUAL( theBinaryTreeRoot.left_node()->is_leaf(), true );

    ARIADNE_TESTING_NOTE("A tree after splitting the root node" );
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
    ARIADNE_TESTING_EXPECT_EQUAL(expected_binary_tree, theBinaryTreeRoot);

    ARIADNE_TESTING_NOTE("Testing the depth of the binary tree");
    ARIADNE_TESTING_EXPECT_EQUAL(theBinaryTreeRoot.depth(), 3u);

    // !!!
    ARIADNE_TESTING_NOTE("Disable the left most enabled leaf, recombine the tree");
    theBinaryTreeRoot.right_node()->left_node()->left_node()->set_disabled();
    ARIADNE_TESTING_NOTE("A tree with enabled left-most leaf");
    leafArray[1] = false;
    BinaryTreeNode expected_binary_tree1( treeArray , leafArray );
    ARIADNE_TESTING_EXPECT_EQUAL(expected_binary_tree1, theBinaryTreeRoot);

    ARIADNE_TESTING_NOTE("A tree after recombination");
    theBinaryTreeRoot.recombine();
    expected_binary_tree1.right_node()->right_node()->make_leaf(true);
    ARIADNE_TESTING_EXPECT_EQUAL(expected_binary_tree1, theBinaryTreeRoot);

    // !!!
    ARIADNE_TESTING_NOTE("Testing the add_enabled( BinaryWord& ) method");
    ARIADNE_TESTING_NOTE("Adding enabled node to an enabled node, the former one is defined by an empty path");

    BinaryTreeNode theNewBinaryTreeRoot(true);
    BinaryWord binaryWord;
    theNewBinaryTreeRoot.add_enabled( binaryWord );
    ARIADNE_TESTING_NOTE("The binary tree should stay intact, i.e. consist of one enabled node");
    BinaryTreeNode expected_binary_tree2( true );
    ARIADNE_TESTING_EXPECT_EQUAL(expected_binary_tree2, theNewBinaryTreeRoot);

    ARIADNE_TESTING_NOTE("Adding enabled sub node, path: (true,false) to a disabled node");
    theNewBinaryTreeRoot.set_disabled();
    binaryWord.push_back(true);
    binaryWord.push_back(false);
    theNewBinaryTreeRoot.add_enabled( binaryWord );
    ARIADNE_TESTING_NOTE("The binary tree have five nodes, four leafs and one (true, false) is enabled");
    expected_binary_tree2.set_disabled();
    expected_binary_tree2.split();
    expected_binary_tree2.right_node()->split();
    expected_binary_tree2.right_node()->left_node()->set_enabled();
    ARIADNE_TESTING_EXPECT_EQUAL(expected_binary_tree2, theNewBinaryTreeRoot);

    ARIADNE_TESTING_NOTE("Adding enabled sub node, path: (true) to a disabled node");
    binaryWord = BinaryWord();
    binaryWord.push_back(true);
    theNewBinaryTreeRoot.add_enabled( binaryWord );
    ARIADNE_TESTING_NOTE("The binary tree have three nodes, two leafs and one (true) is enabled");
    expected_binary_tree2.right_node()->make_leaf(true);
    ARIADNE_TESTING_EXPECT_EQUAL(expected_binary_tree2, theNewBinaryTreeRoot);

    ARIADNE_TESTING_NOTE("Adding a deeper path below an already enabled leaf changes nothing");
    BinaryWord deeperEnabledPath;
    deeperEnabledPath.push_back(true);
    deeperEnabledPath.push_back(false);
    theNewBinaryTreeRoot.add_enabled(deeperEnabledPath);
    ARIADNE_TESTING_EXPECT_EQUAL(expected_binary_tree2, theNewBinaryTreeRoot);

    ARIADNE_TESTING_NOTE("Adding enabled sub node, path: (false, true) to a disabled node");
    binaryWord = BinaryWord();
    binaryWord.push_back(false);
    binaryWord.push_back(true);
    theNewBinaryTreeRoot.add_enabled( binaryWord );
    ARIADNE_TESTING_NOTE("The binary tree have five nodes, four leafs and two (false, true) and (true) are enabled");
    expected_binary_tree2.left_node()->split();
    expected_binary_tree2.left_node()->right_node()->set_enabled();
    ARIADNE_TESTING_EXPECT_EQUAL(expected_binary_tree2, theNewBinaryTreeRoot);

    ARIADNE_TESTING_NOTE("Testing add_enabled(subtree,path) early enabled and left path");
    BinaryTreeNode source_subtree(true);
    BinaryWord subtreePath("0");
    ARIADNE_TESTING_EXPECT_EQUAL(subtreePath.size(),1u);
    ARIADNE_TESTING_EXPECT_EQUAL(source_subtree.is_enabled(),true);

    BinaryTreeNode enabled_destination(true);
    enabled_destination.add_enabled(&source_subtree,subtreePath);
    ARIADNE_TESTING_EXPECT_EQUAL(enabled_destination.is_enabled(),true);

    BinaryTreeNode left_destination(false);
    ARIADNE_TESTING_EXPECT_EQUAL(left_destination.is_leaf(),true);
    ARIADNE_TESTING_EXPECT_EQUAL(left_destination.is_disabled(),true);
    left_destination.add_enabled(&source_subtree,subtreePath);
    ARIADNE_TESTING_EXPECT_EQUAL(left_destination.is_enabled(subtreePath),true);

    ARIADNE_TESTING_NOTE("Adding an enabled subtree along a right-hand path");
    BinaryWord rightSubtreePath;
    rightSubtreePath.push_back(true);
    BinaryTreeNode right_destination(false);
    right_destination.add_enabled(&source_subtree,rightSubtreePath);
    ARIADNE_TESTING_EXPECT_EQUAL(right_destination.is_leaf(),false);
    ARIADNE_TESTING_EXPECT_EQUAL(right_destination.left_node()->is_disabled(),true);
    ARIADNE_TESTING_EXPECT_EQUAL(right_destination.right_node()->is_enabled(),true);

    ARIADNE_TESTING_NOTE("Testing intersect/subset short-circuit outcomes");
    BinaryTreeNode disabled_subtree(false);
    disabled_subtree.split();
    BinaryTreeNode enabled_subtree(false);
    enabled_subtree.split();
    enabled_subtree.left_node()->set_enabled();
    BinaryTreeNode enabled_leaf_for_relation(true);
    BinaryTreeNode disabled_leaf_for_relation(false);

    ARIADNE_TESTING_EXPECT_EQUAL(BinaryTreeNode::intersect(&disabled_subtree,&enabled_leaf_for_relation),false);
    ARIADNE_TESTING_EXPECT_EQUAL(BinaryTreeNode::intersect(&enabled_leaf_for_relation,&disabled_subtree),false);
    ARIADNE_TESTING_EXPECT_EQUAL(BinaryTreeNode::subset(&enabled_subtree,&disabled_leaf_for_relation),false);

    ARIADNE_TESTING_NOTE("An entirely disabled subtree is a subset of a disabled leaf");
    ARIADNE_TESTING_EXPECT_EQUAL(disabled_subtree.is_leaf(),false);
    ARIADNE_TESTING_EXPECT_EQUAL(disabled_subtree.has_enabled(),false);
    ARIADNE_TESTING_EXPECT_EQUAL(BinaryTreeNode::subset(&disabled_subtree,&disabled_leaf_for_relation),true);
}


Void test_copy_and_reset() {
    BinaryTreeNode empty(BooleanArray{},BooleanArray{});
    BinaryTreeNode no_leaves(BooleanArray{false},BooleanArray{});
    ARIADNE_TESTING_PROVE(empty.is_disabled());
    ARIADNE_TESTING_PROVE(no_leaves.is_disabled());
    ARIADNE_TESTING_PROVE(empty.node_to_string().find("BinaryTreeNode")!=String::npos);

    BinaryTreeNode branch(false);
    branch.split();
    branch.left_node()->set_enabled();
    ARIADNE_TESTING_EXPECT_THROWS(NotALeafNodeException, branch.set_enabled());
    BinaryTreeNode copied(false);
    copied=branch;
    ARIADNE_TESTING_PROVE(copied==branch);
    copied.right_node()->set_enabled();
    ARIADNE_TESTING_PROVE(branch.right_node()->is_disabled());
    const BinaryTreeNode& alias=copied;
    copied=alias;
    ARIADNE_TESTING_PROVE(copied.all_enabled());
    copied=empty;
    ARIADNE_TESTING_PROVE(copied.is_leaf());
    ARIADNE_TESTING_PROVE(copied.is_disabled());
    copied=branch;
    copied.copy_from(&empty);
    ARIADNE_TESTING_PROVE(copied.is_leaf());
    copied.copy_from(&branch);
    copied.copy_from(&copied);
    ARIADNE_TESTING_PROVE(copied==branch);
    copied.copy_from(nullptr);
    ARIADNE_TESTING_PROVE(copied==branch);
    copied.set(true);
    ARIADNE_TESTING_PROVE(copied.is_leaf());
    ARIADNE_TESTING_PROVE(copied.left_node()==nullptr);
    ARIADNE_TESTING_PROVE(copied.right_node()==nullptr);
    copied.set(false);
    ARIADNE_TESTING_PROVE(copied.is_disabled());
    copied.split();
    copied.left_node()->set_enabled();
    ARIADNE_TESTING_PROVE(copied.has_enabled());
}

    Void test_independent_binary_prefix_semantics() {
        // No Grid or paving object is involved: only abstract binary paths.
        BinaryTreeNode full(true);
        BinaryTreeNode split_full(true);
        split_full.split();
        ARIADNE_TESTING_PROVE(!(full==split_full)); // Different structure.
        ARIADNE_TESTING_PROVE(BinaryTreeNode::subset(&full,&split_full));
        ARIADNE_TESTING_PROVE(BinaryTreeNode::subset(&split_full,&full));
        split_full.recombine();
        ARIADNE_TESTING_PROVE(full==split_full);

        BinaryTreeNode prefixes(false);
        prefixes.add_enabled(BinaryWord("01"));
        ARIADNE_TESTING_PROVE(prefixes.is_enabled(BinaryWord("01")));
        ARIADNE_TESTING_PROVE(prefixes.is_enabled(BinaryWord("0101")));
        ARIADNE_TESTING_PROVE(!prefixes.is_enabled(BinaryWord("00")));
        ARIADNE_TESTING_PROVE(!prefixes.is_enabled(BinaryWord("1")));
        prefixes.add_enabled(BinaryWord("0100")); // Already covered by "01".
        ARIADNE_TESTING_PROVE(prefixes.is_enabled(BinaryWord("0100")));
        ARIADNE_TESTING_PROVE(BinaryTreeNode::subset(&prefixes,&full));
        ARIADNE_TESTING_PROVE(!BinaryTreeNode::subset(&full,&prefixes));

        BinaryTreeNode disjoint(false);
        disjoint.add_enabled(BinaryWord("1"));
        ARIADNE_TESTING_PROVE(!BinaryTreeNode::intersect(&prefixes,&disjoint));
        disjoint.add_enabled(BinaryWord("011"));
        ARIADNE_TESTING_PROVE(BinaryTreeNode::intersect(&prefixes,&disjoint));
    }

};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_binary_tree");
    ARIADNE_TESTING_ADD_TEST(TestBinaryTree, test_independent_binary_prefix_semantics);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTree, test_copy_and_reset);
    ARIADNE_TESTING_ADD_TEST(TestBinaryTree, test_binary_tree);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}
