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
#include "paving/grid_paving.hpp"

using namespace Ariadne;

// These tests intentionally run on one thread. Prior to the fix in
// GridTreePaving::restrict_to_lower/remove_from_lower, each test blocked
// re-acquiring the source owner's non-recursive mutex via virtual root_cell().
class TestPavingLockReentrancy : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

    Void test_distinct_owner_restrict() {
        Grid grid(1u);
        GridCell left(grid,0u,make_binary_word("0"));
        GridCell right(grid,0u,make_binary_word("1"));
        GridTreePaving destination(grid,false);
        destination.adjoin(left);
        destination.adjoin(right);
        GridTreePaving source(left);

        destination.restrict(source);
        ARIADNE_TESTING_PROVE(destination.superset(left));
        ARIADNE_TESTING_PROVE(!destination.superset(right));
        ARIADNE_TESTING_PROVE(source.superset(left));

        // Exercise the virtual SubPavingInterface dispatch as well.
        GridTreePaving through_interface(grid,false);
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
        GridTreePaving destination(grid,false);
        destination.adjoin(left);
        destination.adjoin(right);
        GridTreePaving source(right);

        destination.remove(source);
        ARIADNE_TESTING_PROVE(destination.superset(left));
        ARIADNE_TESTING_PROVE(!destination.superset(right));
        ARIADNE_TESTING_PROVE(source.superset(right));

        GridTreePaving through_interface(grid,false);
        through_interface.adjoin(left);
        through_interface.adjoin(right);
        through_interface.remove(
            static_cast<SubPavingInterface const&>(source));
        ARIADNE_TESTING_PROVE(through_interface.superset(left));
        ARIADNE_TESTING_PROVE(!through_interface.superset(right));
    }

    Void test_self_set_operations() {
        Grid grid(1u);
        GridCell cell(grid,0u,make_binary_word("0"));
        GridTreePaving paving(cell);

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
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc,argv);
}
