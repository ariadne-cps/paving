/***************************************************************************
 *            test_paving_lock_reentrancy.cpp
 *
 *  Copyright  2026  Luca Geretti
 ****************************************************************************/
/*
 *  This file is part of Ariadne Paving.
 *  Ariadne Paving is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License, version 3 or later.
 */

#include "ariadne-testing.hpp"
#include "paving/binary_tree_paving.hpp"
#include "tree/binary_tree.hpp"

using namespace Ariadne;

// These tests intentionally run on one thread. Prior to the fix in
// BinaryTreePaving::restrict_to_lower/remove_from_lower, each test blocked
// re-acquiring the source owner's non-recursive mutex via virtual root_cell().
class TestPavingLockReentrancy : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

    Void test_distinct_owner_restrict() {
        Grid grid(1u);
        GridCell left(grid,0u,make_binary_word("0"));
        GridCell right(grid,0u,make_binary_word("1"));
        BinaryTreePaving destination(grid,false);
        destination.adjoin(left);
        destination.adjoin(right);
        BinaryTreePaving source(left);

        destination.restrict(source);
        ARIADNE_TESTING_PROVE(destination.superset(left));
        ARIADNE_TESTING_PROVE(!destination.superset(right));
        ARIADNE_TESTING_PROVE(source.superset(left));

        // Exercise the virtual SubPavingInterface dispatch as well.
        BinaryTreePaving through_interface(grid,false);
        through_interface.adjoin(left);
        through_interface.adjoin(right);
        through_interface.restrict(
            static_cast<SubPavingInterface const&>(source));
        ARIADNE_TESTING_PROVE(through_interface.superset(left));
        ARIADNE_TESTING_PROVE(!through_interface.superset(right));
    }

    Void test_distinct_owner_remove() {
        Grid grid(1u);
        GridCell left(grid,0u,make_binary_word("0"));
        GridCell right(grid,0u,make_binary_word("1"));
        BinaryTreePaving destination(grid,false);
        destination.adjoin(left);
        destination.adjoin(right);
        BinaryTreePaving source(right);

        destination.remove(source);
        ARIADNE_TESTING_PROVE(destination.superset(left));
        ARIADNE_TESTING_PROVE(!destination.superset(right));
        ARIADNE_TESTING_PROVE(source.superset(right));

        BinaryTreePaving through_interface(grid,false);
        through_interface.adjoin(left);
        through_interface.adjoin(right);
        through_interface.remove(
            static_cast<SubPavingInterface const&>(source));
        ARIADNE_TESTING_PROVE(through_interface.superset(left));
        ARIADNE_TESTING_PROVE(!through_interface.superset(right));
    }

    Void test_nonrecursive_is_empty_queries() {
        Grid grid(1u);
        GridCell cell(grid,0u,make_binary_word("0"));
        BinaryTreePaving paving(grid,false);
        // A single-threaded query previously deadlocked because the locked
        // owner called the base is_empty(), whose virtual size() tried to
        // acquire the same mutex for a second time.
        ARIADNE_TESTING_PROVE(paving.is_empty());
        ARIADNE_TESTING_PROVE(paving.size()==0u);

        paving.adjoin(cell);
        ARIADNE_TESTING_PROVE(!paving.is_empty());
        ARIADNE_TESTING_PROVE(paving.size()!=0u);

        // The base view dispatches to the owner's locked size() exactly once.
        BinaryTreeSubpaving const& base=paving;
        ARIADNE_TESTING_PROVE(!base.is_empty());

        paving.set_root_cell(false);
        ARIADNE_TESTING_PROVE(paving.is_empty());
        ARIADNE_TESTING_PROVE(base.is_empty());

        // Also exercise the empty/nonempty branch of the geometric query
        // that checks emptiness through a separate owned snapshot.
        (void)paving.bounding_box();
        paving.adjoin(cell);
        (void)paving.bounding_box();
    }


    Void test_restrict_to_extent_does_not_reenter_dimension_lock() {
        // The owner holds its non-recursive mutex throughout extent
        // restriction. Calling this->dimension() there re-entered that
        // mutex after dimension() became a synchronized owner query.
        for(DimensionType dimensions : {1u,2u,4u}) {
            Grid grid(dimensions);
            BinaryTreePaving paving(grid,2u,new BinaryTreeNode(true));
            BinaryTreePaving original(paving);

            // The no-op branch preserves structure and membership.
            paving.restrict_to_extent(3u);
            ARIADNE_TESTING_PROVE(paving==original);

            // The modification branch previously threw std::system_error
            // on MSVC and could hang on other standard libraries.
            paving.restrict_to_extent(1u);
            ARIADNE_TESTING_PROVE(!paving.is_empty());
            ARIADNE_TESTING_PROVE(paving.root_cell().root_extent()==2u);

            paving.restrict_to_extent(0u);
            ARIADNE_TESTING_PROVE(!paving.is_empty());
            ARIADNE_TESTING_PROVE(paving.root_cell().root_extent()==2u);
        }
    }

    Void test_self_set_operations() {
        Grid grid(1u);
        GridCell cell(grid,0u,make_binary_word("0"));
        BinaryTreePaving paving(cell);

        paving.restrict(static_cast<SubPavingInterface const&>(paving));
        ARIADNE_TESTING_PROVE(paving.superset(cell));
        paving.remove(static_cast<SubPavingInterface const&>(paving));
        ARIADNE_TESTING_PROVE(!paving.superset(cell));
    }
};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_paving_lock_reentrancy");
    ARIADNE_TESTING_ADD_TEST(TestPavingLockReentrancy,test_distinct_owner_restrict);
    ARIADNE_TESTING_ADD_TEST(TestPavingLockReentrancy,test_distinct_owner_remove);
    ARIADNE_TESTING_ADD_TEST(TestPavingLockReentrancy,test_self_set_operations);
    ARIADNE_TESTING_ADD_TEST(TestPavingLockReentrancy,test_nonrecursive_is_empty_queries);
    ARIADNE_TESTING_ADD_TEST(TestPavingLockReentrancy,test_restrict_to_extent_does_not_reenter_dimension_lock);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc,argv);
}
