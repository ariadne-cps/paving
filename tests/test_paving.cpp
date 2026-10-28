/***************************************************************************
 *            test_paving.cpp
 *
 *  Copyright  2012-20  Pieter Collins
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
#include <memory>
#include <sstream>
#include "paving/paving_interface.hpp"
#include "paving/grid_paving.hpp"
#include "function/function.hpp"
#include "set/function_set.hpp"
#include "logging/logging.hpp"

#include "macro/testing.hpp"


using namespace Ariadne;
using namespace std;

class TestPaving
{
  public:
    TestPaving() = default;
  public:
    Void test() const;
    Void test_constructors() const;
    Void test_iterator() const;
    Void test_branch() const;
    Void test_clone() const;
    Void test_geometry() const;
    Void test_approximation() const;
};

Void TestPaving::test() const {
    ARIADNE_TEST_CALL(test_constructors());
    ARIADNE_TEST_CALL(test_iterator());
    ARIADNE_TEST_CALL(test_branch());
    ARIADNE_TEST_CALL(test_clone());
    ARIADNE_TEST_CALL(test_geometry());
    ARIADNE_TEST_CALL(test_approximation());
}

Void TestPaving::test_constructors() const {
    Grid grid(2);
    GridTreePaving grid_set(grid);
    PavingInterface& paving = grid_set;

    ARIADNE_TEST_PRINT(paving);
    ARIADNE_TEST_EQUAL(paving.grid(),grid);
    BinaryWord path("10010");
    GridCell cell(paving.grid(),1u,path);
    ARIADNE_TEST_PRINT(cell);
    paving.adjoin(cell);
    ARIADNE_TEST_PRINT(paving);
    path=BinaryWord("110110");
    cell=GridCell(paving.grid(),1u,path);
    paving.adjoin(GridCell(grid,Nat(1),BinaryWord("110110")));
    ARIADNE_TEST_PRINT(paving);
    ARIADNE_TEST_EQUALS(paving.size(),2);

    PavingHandle paving_copy = paving;
    ARIADNE_TEST_EQUAL(paving_copy.grid(),grid);
    ARIADNE_TEST_EQUALS(paving_copy.size(),2);

    paving.adjoin(GridCell(grid,Nat(1),BinaryWord("0101")));
    ARIADNE_TEST_EQUALS(paving.size(),3);
    ARIADNE_TEST_EQUALS(paving_copy.size(),2);

}

Void TestPaving::test_iterator() const {
    Grid grid(2);
    GridTreePaving grid_set(grid);
    PavingInterface& paving = grid_set;
    Nat height = 1;

    GridCell cell1(grid,height,BinaryWord("10010"));
    GridCell cell2(grid,height,BinaryWord("110110"));
    paving.adjoin(cell2);
    paving.adjoin(cell1);

    PavingInterface::ConstIterator iter=paving.begin();
    ARIADNE_TEST_EQUAL(*iter,cell1);
    ++iter;
    ARIADNE_TEST_EQUAL(*iter,cell2);
    ++iter;
    ARIADNE_TEST_EQUALS(iter,paving.end());
}

Void TestPaving::test_branch() const {
    Grid grid(2);
    GridTreePaving grid_set(grid);
    PavingInterface& paving = grid_set;
    GridCell cell(grid,0,BinaryWord(""));

    Nat height = 0;

    BinaryWord word1("0001");
    BinaryWord word2("0010");
    BinaryWord word3("1100");
    BinaryWord word4("1110");

    ExactBoxType box1{{0.0_x,0.25_x},{0.25_x,0.5_x}};
    ExactBoxType box2{{0.25_x,0.5_x},{0.0_x,0.25_x}};
    ExactBoxType box3{{0.5_x,0.75_x},{0.5_x,0.75_x}};
    ExactBoxType box4{{0.75_x,1.0_x},{0.5_x,0.75_x}};

    paving.adjoin(GridCell(grid,height,word1));
    paving.adjoin(GridCell(grid,height,word2));
    paving.adjoin(GridCell(grid,height,word3));
    paving.adjoin(GridCell(grid,height,word4));

    ARIADNE_TEST_PRINT(paving);
    ARIADNE_TEST_ASSERT(paving.size()==4);

    GridTreeSubpaving direct_branch0 =
        static_cast<GridTreeSubpaving const&>(grid_set).branch(0);
    ARIADNE_TEST_ASSERT(direct_branch0.size()==2);
    ARIADNE_TEST_PRINT(direct_branch0.root_cell());

    SubPavingHandle branch0 = paving.branch(0);
    ARIADNE_TEST_ASSERT(branch0.size()==2);
    ARIADNE_TEST_PRINT(branch0.root_cell());
    ARIADNE_TEST_PRINT(branch0);
    ARIADNE_TEST_ASSERT(branch0.begin()->word()==word1);
    ARIADNE_TEST_ASSERT((++branch0.begin())->word()==word2);

    SubPavingHandle branch1 = paving.branch(1);
    ARIADNE_TEST_ASSERT(branch1.size()==2);
    ARIADNE_TEST_PRINT(branch1.root_cell());
    ARIADNE_TEST_PRINT(branch1);
    ARIADNE_TEST_ASSERT(branch1.begin()->word()==word3);
    ARIADNE_TEST_ASSERT((++branch1.begin())->word()==word4);
}

Void TestPaving::test_clone() const {
    Grid grid(2);
    BinaryWord word1("0001");
    BinaryWord word2("0010");
    BinaryWord word3("1100");
    std::unique_ptr<PavingHandle> cloned_branch;

    {
        GridTreePaving grid_set(grid);
        PavingInterface& paving = grid_set;
        paving.adjoin(GridCell(grid,0,word1));
        paving.adjoin(GridCell(grid,0,word2));
        paving.adjoin(GridCell(grid,0,word3));

        GridTreeSubpaving branch0 =
            static_cast<GridTreeSubpaving const&>(grid_set).branch(0);
        ARIADNE_TEST_EQUALS(branch0.size(),2);

        cloned_branch = std::make_unique<PavingHandle>(branch0);
        const PavingInterface& cloned_paving = *cloned_branch;
        const SubPavingInterface& original_interface = branch0;
        const SubPavingInterface& cloned_interface = cloned_paving;

        ARIADNE_TEST_EQUALS(cloned_branch->size(),2);
        ARIADNE_TEST_ASSERT(subset(original_interface,cloned_interface));
        ARIADNE_TEST_ASSERT(subset(cloned_interface,original_interface));
    }

    ARIADNE_TEST_EQUALS(cloned_branch->size(),2);
    PavingHandle::ConstIterator iter=cloned_branch->begin();
    ARIADNE_TEST_EQUAL(*iter,GridCell(grid,0,word1));
    ++iter;
    ARIADNE_TEST_EQUAL(*iter,GridCell(grid,0,word2));
    ++iter;
    ARIADNE_TEST_EQUALS(iter,cloned_branch->end());
}


Void TestPaving::test_geometry() const {
    Grid grid(2);
    GridTreePaving grid_tree_set(grid);
    Nat height(0);

    PavingHandle paving_handle1(grid_tree_set);
    PavingInterface& paving_interface1 = paving_handle1;
    paving_handle1.adjoin(GridCell(grid,height,BinaryWord("10")));
    PavingHandle paving_handle2(grid_tree_set);
    PavingInterface& paving_interface2 = paving_handle2;
    paving_handle2.adjoin(GridCell(grid,height,BinaryWord("1")));
    PavingHandle paving_handle3(grid_tree_set);
    PavingInterface& paving_interface3 = paving_handle3;
    paving_handle3.adjoin(GridCell(grid,height,BinaryWord("0110")));

    ARIADNE_TEST_PRINT(paving_handle1);
    ARIADNE_TEST_PRINT(paving_handle2);
    ARIADNE_TEST_PRINT(paving_handle3);

    paving_handle1.subset(paving_handle2);
    paving_handle1.subset(paving_interface2);
    paving_interface1.subset(paving_handle2);
    paving_interface1.subset(paving_interface2);
    subset(paving_handle1,paving_handle2);
    subset(paving_handle1,paving_interface2);
    subset(paving_interface1,paving_handle2);
    subset(paving_interface1,paving_interface2);

    ARIADNE_TEST_ASSERT(subset(paving_interface1,paving_interface1));
    ARIADNE_TEST_ASSERT(subset(paving_interface1,paving_interface2));
    ARIADNE_TEST_ASSERT(!subset(paving_interface2,paving_interface1));
    ARIADNE_TEST_ASSERT(!subset(paving_interface1,paving_interface3));
    ARIADNE_TEST_ASSERT(!subset(paving_interface3,paving_interface1));
    ARIADNE_TEST_ASSERT(!subset(paving_interface2,paving_interface3));
    ARIADNE_TEST_ASSERT(!subset(paving_interface3,paving_interface2));

    ARIADNE_TEST_ASSERT(paving_handle1.intersects(paving_handle2));
    ARIADNE_TEST_ASSERT(intersect(paving_handle1,paving_handle2));

    ARIADNE_TEST_ASSERT(intersect(paving_interface1,paving_interface1));
    ARIADNE_TEST_ASSERT(intersect(paving_interface1,paving_interface2));
    ARIADNE_TEST_ASSERT(intersect(paving_interface2,paving_interface1));
    ARIADNE_TEST_ASSERT(!intersect(paving_interface1,paving_interface3));
    ARIADNE_TEST_ASSERT(!intersect(paving_interface3,paving_interface1));
    ARIADNE_TEST_ASSERT(!intersect(paving_interface2,paving_interface3));
    ARIADNE_TEST_ASSERT(!intersect(paving_interface3,paving_interface2));

    ARIADNE_TEST_EQUALS(geometric_union(paving_handle1,paving_handle2).size(),1);
    ARIADNE_TEST_EQUALS(geometric_union(paving_handle1,paving_handle3).size(),2);
    ARIADNE_TEST_EQUALS(geometric_union(paving_handle2,paving_handle3).size(),2);
    ARIADNE_TEST_EQUALS(intersection(paving_handle1,paving_handle2).size(),1);
    ARIADNE_TEST_EQUALS(intersection(paving_handle2,paving_handle3).size(),0);
    ARIADNE_TEST_EQUALS(difference(paving_handle1,paving_handle2).size(),0);
    ARIADNE_TEST_EQUALS(difference(paving_handle2,paving_handle1).size(),1);

    PavingHandle paving_handle4(grid_tree_set);
    paving_handle4.adjoin(GridCell(grid,height,BinaryWord("11")));
    ARIADNE_TEST_EQUAL(intersection(paving_handle1,paving_handle2),paving_handle1);
    ARIADNE_TEST_EQUAL(difference(paving_handle2,paving_handle1),paving_handle4);

    ARIADNE_TEST_SAME(geometric_union(paving_handle1,paving_handle3).bounding_box(),UpperBoxType({{0.25_x,1.0_x},{0.0_x,0.75_x}}));


}

Void TestPaving::test_approximation() const {
    Grid grid(2);
    GridTreePaving grid_set(grid);
    PavingInterface& paving = grid_set;

    EffectiveVectorMultivariateFunction x=EffectiveVectorMultivariateFunction::identity(2);
    EffectiveConstrainedImageSet set(RealBox({{-1.0_x,1.0_x},{-1.0_x,1.0_x}}));
    set.apply( {2*x[0]+x[1]+x[0]*x[0]/4,x[0]+x[1]} );
    Nat depth = 2;
    paving.adjoin_outer_approximation(set,depth);
    paving.recombine();

    Nat height = 6;
    BinaryWord tree("111010111110101111011010100101100001111101000101010001111"
                    "100100010101001110100010101001011110110000101101000101111"
                    "000010101001011011111101000101010001011101000101010000111"
                    "011011011011000000011111110110100001111100001010100011111"
                    "010001110000011101010000111110110000101101000101111001000"
                    "100110111100100010010110110010001011011011010000011111011"
                    "011001001011000010111110000101100001011011110000101100001"
                    "111111100100010001110110010000011101111001000100111110000"
                    "101100011010100101100000111011011110111100001011000011111"
                    "01000111000000000000");
    BinaryWord leaves("0000000010010001001010001011010101001010000100110110010"
                      "1010100010010100101001010000000010000011011101010101101"
                      "0010100101010001001101100101101110100101110101000010000"
                      "0001010010010101001010001010010010100100110101001101001"
                      "00101001010101101000000101001000100100000000000");
    GridTreePaving expected_grid_set(grid,height,tree,leaves);
    PavingInterface& expected_paving = expected_grid_set;

    ARIADNE_TEST_BINARY_PREDICATE(subset,expected_paving,paving);

    PavingHandle error_paving = expected_paving;
    error_paving.remove(paving);
}


class NonnegativePredicate : public PredicateInterface<ExactBoxType> {
  public:
    ValidatedKleenean check(const ExactBoxType& box) override { return box[0].lower_bound()>=FloatDP(0,dp); }
};

Void test_paving_handle_forwarding() {
    Grid grid(1u);
    GridTreePaving native(grid);
    GridCell root(grid,0u,BinaryWord());
    GridCell left(grid,0u,BinaryWord("0"));
    GridCell right(grid,0u,BinaryWord("1"));
    native.adjoin(root);
    native.mince(1u);
    const SubPavingInterface& native_interface=native;
    PavingHandle paving(native_interface);
    PavingHandle copied(paving);
    PavingHandle owned(new GridTreePaving(grid));
    owned=copied;
    const PavingHandle& alias=owned;
    owned=alias;
    const PavingHandle& const_owned=owned;
    const PavingInterface& const_interface=const_owned;
    ARIADNE_TEST_EQUALS(const_owned.grid(),grid);
    ARIADNE_TEST_EQUALS(const_owned.size(),2u);
    ARIADNE_TEST_ASSERT(const_owned.superset(left));
    ARIADNE_TEST_ASSERT(const_owned.subset(native_interface));
    ARIADNE_TEST_ASSERT(const_owned.intersects(native_interface));
    ARIADNE_TEST_ASSERT(const_owned.root_cell()==root);
    ARIADNE_TEST_ASSERT(const_interface==native_interface);
    ARIADNE_TEST_ASSERT(subset(const_interface,native_interface));
    ARIADNE_TEST_ASSERT(intersect(const_interface,native_interface));
    auto iter=const_owned.begin();
    auto old=iter++;
    ARIADNE_TEST_ASSERT(*old==left);
    ARIADNE_TEST_ASSERT(iter->box()==right.box());
    old=iter;
    const auto& old_alias=old;
    old=old_alias;
    ARIADNE_TEST_ASSERT(old==iter);
    ARIADNE_TEST_ASSERT(old!=const_owned.end());
    ++iter;
    ARIADNE_TEST_ASSERT(iter==const_owned.end());
    auto native_iterator=native.begin();
    ForwardConstantIteratorHandle<GridCell> adapted(static_cast<const ForwardConstantIteratorInterface<GridCell>&>(native_iterator));
    ARIADNE_TEST_ASSERT(*adapted==left);
    std::ostringstream repr;
    repr<<adapted<<old<<const_owned;
    auto& mutable_interface=static_cast<PavingInterface&>(paving);
    SubPavingHandle sub=mutable_interface.branch(false);
    SubPavingHandle sub_copy(sub);
    SubPavingHandle from_interface(static_cast<const SubPavingInterface&>(sub));
    sub_copy=from_interface;
    const SubPavingHandle& sub_alias=sub_copy;
    sub_copy=sub_alias;
    const SubPavingHandle& const_sub=sub;
    const SubPavingInterface& const_sub_interface=const_sub;
    ARIADNE_TEST_EQUALS(const_sub_interface.size(),1u);
    ARIADNE_TEST_EQUALS(const_sub.grid(),grid);
    ARIADNE_TEST_EQUALS(const_sub.size(),1u);
    ARIADNE_TEST_ASSERT(const_sub.root_cell()==left);
    ARIADNE_TEST_ASSERT(const_sub.superset(left));
    ARIADNE_TEST_ASSERT(const_sub.subset(native_interface));
    ARIADNE_TEST_ASSERT(const_sub.intersects(native_interface));
    ARIADNE_TEST_ASSERT(const_sub.begin()!=const_sub.end());
    ARIADNE_TEST_EQUALS(const_sub.bounding_box().dimension(),1u);
    ExactBoxType large{{-1.0_x,2.0_x}};
    ExactBoxType far{{3.0_x,4.0_x}};
    ExactBoxType middle{{0.125_x,0.375_x}};
    ARIADNE_TEST_ASSERT(definitely(const_sub.inside(large)));
    ARIADNE_TEST_ASSERT(definitely(const_sub.separated(far)));
    ARIADNE_TEST_ASSERT(definitely(const_sub.overlaps(middle)));
    repr<<const_sub;
    sub_copy.mince(2u);
    sub_copy.recombine();
    sub_copy.set_root_cell(false);
    ARIADNE_TEST_EQUALS(sub_copy.size(),0u);
    ARIADNE_TEST_EQUALS(const_sub.size(),1u);
    paving.set_root_cell(true);
    ARIADNE_TEST_EQUALS(paving.bounding_box().dimension(),1u);
    ARIADNE_TEST_ASSERT(definitely(paving.inside(large)));
    ARIADNE_TEST_ASSERT(definitely(paving.separated(far)));
    ARIADNE_TEST_ASSERT(definitely(paving.overlaps(middle)));
    paving.remove(right);
    ARIADNE_TEST_ASSERT(!paving.superset(right));
    paving.adjoin(right);
    ARIADNE_TEST_ASSERT(paving.superset(right));
    paving.remove(native_interface);
    ARIADNE_TEST_EQUALS(paving.size(),0u);
    paving.adjoin(native_interface);
    paving.restrict(native_interface);
    ARIADNE_TEST_ASSERT(paving.subset(native_interface));
    ARIADNE_TEST_ASSERT(join(native_interface,native_interface).superset(left));
    ARIADNE_TEST_ASSERT(geometric_union(native_interface,native_interface).superset(left));
    ARIADNE_TEST_ASSERT(intersection(native_interface,native_interface).superset(left));
    ARIADNE_TEST_EQUALS(difference(native_interface,native_interface).size(),0u);
    owned.set_root_cell(false);
    ExactBoxSetType set(ExactBoxType{{0.125_x,0.875_x}});
    const EffectiveEuclideanCompactSetInterface& compact=set;
    const EffectiveEuclideanOpenSetInterface& open=set;
    const EffectiveEuclideanSetInterface& generic=set;
    owned.adjoin_outer_approximation(compact,2u);
    ARIADNE_TEST_ASSERT(owned.size()>0u);
    owned.set_root_cell(false);
    owned.adjoin_inner_approximation(open,0u,2u);
    ARIADNE_TEST_ASSERT(owned.size()>0u);
    owned.set_root_cell(false);
    owned.adjoin_inner_approximation(generic,2u);
    ARIADNE_TEST_ASSERT(owned.size()>0u);
    NonnegativePredicate predicate;
    ARIADNE_TEST_THROWS(owned.adjoin_cells(predicate,2u),std::runtime_error);
    ARIADNE_TEST_ASSERT(!repr.str().empty());
}

Int main()
{
    ARIADNE_TEST_CALL(test_paving_handle_forwarding());
    TestPaving().test();
    return ARIADNE_TEST_FAILURES;
}
