/***************************************************************************
 *            tests/test_robdd_paving.cpp
 *
 *  Copyright  2026  Luca Geretti
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne Paving.
 *
 *  Ariadne Paving is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne Paving is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne Paving. If not, see <https://www.gnu.org/licenses/>.
 */

/** @file tests/test_robdd_paving.cpp
 *  @brief Test paving geometry and backend behavior.
 */

#include <limits>
#include <memory>
#include <sstream>
#include <type_traits>
#include <vector>

#include "paving/binary_tree_paving.hpp"
#include "paving/binary_tree_paving.hpp"
#include "paving/robdd_paving.hpp"
#include "paving/paving_cell_operations.hpp"
#include "geometry/set_wrapper.hpp"
#include "set/function_set.hpp"
#include "ariadne-testing.hpp"

using namespace Ariadne;

static_assert(std::is_base_of_v<PavingInterface,BinaryTreePaving>);


class TestRobddPaving : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

    Void test_shared_import_and_geometric_box_queries() {
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        auto manager=std::make_shared<RobddManager>();
        BinaryTreePaving source(grid);
        source.adjoin(root.split(false));
        SubPavingInterface const& neutral=source;
        RobddPaving imported(manager,neutral);
        RobddPaving borrowed(*manager,neutral);
        RobddPaving empty(manager,root,manager->constant(false),0u);
        auto united=RobddPaving::from_cells(manager,root,
            std::vector<GridCell>{root.split(false),root.split(true)});
        ARIADNE_TESTING_PROVE(imported.superset(root.split(false)));
        ARIADNE_TESTING_PROVE(borrowed.superset(root.split(false)));
        ARIADNE_TESTING_PROVE(united.superset(root));
        ARIADNE_TESTING_PROVE(empty.is_empty());
        ARIADNE_TESTING_PROVE(imported.superset(root.split(false).box()));
        ARIADNE_TESTING_PROVE(imported.superset(root.split(false).split(true).box()));
        ARIADNE_TESTING_PROVE(!imported.superset(root.box()));
        ARIADNE_TESTING_PROVE(!imported.superset(root.split(true).box()));
        ARIADNE_TESTING_PROVE(!empty.superset(root.box()));
        ARIADNE_TESTING_PROVE(!imported.superset(GridCell(grid,1u,BinaryWord{}).box()));
        ARIADNE_TESTING_EXPECT_EXCEPTION(imported.superset(ExactBoxType({{0,1},{0,1}})));
        ARIADNE_TESTING_EXPECT_EXCEPTION((RobddPaving(std::shared_ptr<RobddManager>{},neutral)));
    }

    Void test_validated_and_overt_approximation_paths() {
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        RobddManager manager;
        ExactBoxType box{{0.125_x,0.875_x}};
        ValidatedConstrainedImageSet validated(box);
        RobddPaving outer(manager,root,manager.constant(false),0u);
        BinaryTreePaving expected(grid);
        outer.adjoin_outer_approximation(validated,3u);
        expected.adjoin_outer_approximation(validated,3u);
        ARIADNE_TESTING_PROVE(!outer.is_empty());
        for(auto it=outer.begin();it!=outer.end();++it)
            ARIADNE_TESTING_PROVE(expected.superset(*it));
        for(auto it=expected.begin();it!=expected.end();++it)
            ARIADNE_TESTING_PROVE(outer.superset(*it));
        auto previous=outer.size();
        outer.adjoin_outer_approximation(validated,3u);
        ARIADNE_TESTING_EXPECT_EQUAL(outer.size(),previous);

        OvertSet<EffectiveTag,RealVector> overt(box);
        ExactBoxSetType regular(box);
        for(auto const* input:std::vector<EffectiveEuclideanOvertSetInterface const*>{
                &overt.reference(),&regular}) {
            RobddPaving lower(manager,root,manager.constant(false),0u);
            BinaryTreePaving reference(grid);
            lower.adjoin_lower_approximation(*input,0u,3u);
            reference.adjoin_lower_approximation(*input,0u,3u);
            ARIADNE_TESTING_PROVE(!lower.is_empty());
            for(auto it=lower.begin();it!=lower.end();++it)
                ARIADNE_TESTING_PROVE(reference.superset(*it));
            for(auto it=reference.begin();it!=reference.end();++it)
                ARIADNE_TESTING_PROVE(lower.superset(*it));
            auto count=lower.size();
            lower.adjoin_lower_approximation(*input,0u,3u);
            ARIADNE_TESTING_EXPECT_EQUAL(lower.size(),count);
        }
        ExactBoxSetType enclosing(ExactBoxType({{-2,2}}));
        RobddPaving covered(manager,root,manager.constant(false),0u);
        covered.adjoin_lower_approximation(enclosing,0u,3u);
        ARIADNE_TESTING_PROVE(covered.superset(root));
        ValidatedConstrainedImageSet wrong(ExactBoxType({{0,1},{0,1}}));
        ARIADNE_TESTING_EXPECT_EXCEPTION(outer.adjoin_outer_approximation(wrong,1u));
        OvertSet<EffectiveTag,RealVector> wrong_overt(ExactBoxType({{0,1},{0,1}}));
        ARIADNE_TESTING_EXPECT_EXCEPTION(outer.adjoin_lower_approximation(wrong_overt.reference(),0u,1u));
    }


    Void test_shared_manager_lifetime_and_interoperability() {
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        std::weak_ptr<RobddManager> weak;
        std::unique_ptr<Paving> retained;
        {
            auto manager=std::make_shared<RobddManager>();
            weak=manager;
            Paving first(new RobddPaving(manager,grid));
            Paving second(new RobddPaving(manager,grid));
            first.adjoin(root.split(false));
            second.adjoin(root.split(true));
            ARIADNE_TESTING_PROVE(!intersect(
                static_cast<SubPavingInterface const&>(
                    static_cast<PavingInterface const&>(first)),
                static_cast<SubPavingInterface const&>(
                    static_cast<PavingInterface const&>(second))));
            Paving united=join(
                static_cast<SubPavingInterface const&>(
                    static_cast<PavingInterface const&>(first)),
                static_cast<SubPavingInterface const&>(
                    static_cast<PavingInterface const&>(second)));
            ARIADNE_TESTING_PROVE(united.superset(root));
            retained=std::make_unique<Paving>(united);
        }
        ARIADNE_TESTING_PROVE(!weak.expired());
        auto detached=std::make_unique<Paving::ConstIterator>(retained->begin());
        ARIADNE_TESTING_PROVE(retained->superset(root));
        retained.reset();
        ARIADNE_TESTING_PROVE(!weak.expired());
        ARIADNE_TESTING_PROVE(detached->operator*().grid()==grid);
        detached.reset();
        ARIADNE_TESTING_PROVE(weak.expired());

        auto other_manager=std::make_shared<RobddManager>();
        auto manager=std::make_shared<RobddManager>();
        RobddPaving one(manager,grid);
        RobddPaving different_manager_paving(other_manager,grid);
        ARIADNE_TESTING_EXPECT_EXCEPTION(one.join(different_manager_paving));
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            RobddPaving(std::shared_ptr<RobddManager>{},grid));
    }

    Void test_iterator_identity_end_contracts_and_display() {
        RobddManager manager, foreign;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        RobddPaving full(manager,root,manager.constant(true),0u);
        RobddPaving empty(manager,root,manager.constant(false),0u);
        RobddPaving foreign_full(foreign,root,foreign.constant(true),0u);
        RobddPaving child(manager,root.split(false),manager.constant(true),0u);
        RobddPaving refined(manager,root,manager.constant(true),1u);
        BinaryTreePaving tree(grid,true);
        SubPavingInterface const& tree_interface=tree;
        auto first=full.begin(), end=full.end();
        ARIADNE_TESTING_PROVE(first!=tree_interface.begin());
        ARIADNE_TESTING_PROVE(first!=foreign_full.begin());
        ARIADNE_TESTING_PROVE(first!=empty.begin());
        ARIADNE_TESTING_PROVE(first!=child.begin());
        ARIADNE_TESTING_PROVE(first!=refined.begin());
        ARIADNE_TESTING_PROVE(first!=end);
        ARIADNE_TESTING_PROVE(end!=first);
        ARIADNE_TESTING_PROVE(end==full.end());
        std::ostringstream display;
        display<<first<<' '<<end;
        ARIADNE_TESTING_PROVE(display.str().find("RobddPavingLeafIterator(")!=std::string::npos);
        ARIADNE_TESTING_PROVE(display.str().find("RobddPavingLeafIterator(end)")!=std::string::npos);
        ARIADNE_TESTING_EXPECT_EXCEPTION(*end);
        ARIADNE_TESTING_EXPECT_EXCEPTION(++end);
        ARIADNE_TESTING_PROVE(first->box()==root.box());
    }

    Void test_public_depth_geometry_and_assignment_contracts() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        RobddPaving full(manager,root,manager.constant(true),0u);
        constexpr SizeType limit=std::numeric_limits<SizeType>::max();
        ARIADNE_TESTING_EXPECT_EXCEPTION(RobddPaving(manager,root,manager.constant(true),limit));
        ARIADNE_TESTING_EXPECT_EXCEPTION(full.mince_to_depth(limit));
        ARIADNE_TESTING_EXPECT_EQUAL(full.depth(),0u);
        if constexpr (std::numeric_limits<SizeType>::digits>std::numeric_limits<Nat>::digits) {
            RobddPaving too_deep(manager,root,manager.constant(true),
                static_cast<SizeType>(std::numeric_limits<Nat>::max())+1u);
            ARIADNE_TESTING_EXPECT_EXCEPTION(too_deep.to_binary_tree());
        }
        RobddPaving very_deep(manager,root.split(false),manager.constant(true),limit-1u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(full.join(very_deep));
        RobddPaving incompatible(manager,GridCell(Grid(2u),0u,BinaryWord{}),manager.constant(true),0u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(full=incompatible);
        ARIADNE_TESTING_PROVE(full.root_cell()==root);
        ExactBoxType wrong_dimension{{0.0_x,1.0_x},{0.0_x,1.0_x}};
        ARIADNE_TESTING_EXPECT_EXCEPTION(full.smallest_enclosing_primary_cell(wrong_dimension));
        ARIADNE_TESTING_EXPECT_EXCEPTION(full.inside(wrong_dimension));
        ARIADNE_TESTING_EXPECT_EXCEPTION(full.separated(wrong_dimension));
        ARIADNE_TESTING_EXPECT_EXCEPTION(full.overlaps(wrong_dimension));
        ExactBoxSetType shape=root.box();
        EffectiveEuclideanCompactSetInterface const& compact=shape;
        ARIADNE_TESTING_EXPECT_EXCEPTION(full.adjoin_outer_approximation(compact,std::numeric_limits<Nat>::max()));

        auto* same_owner=&full;
        full=*same_owner;
        ARIADNE_TESTING_PROVE(full.root_cell()==root);
        std::ostringstream display;
        display<<full;
        ARIADNE_TESTING_PROVE(display.str().find("RobddPaving(")!=std::string::npos);
        SubPavingInterface const& self=full;
        ARIADNE_TESTING_PROVE(full.equals(self));
        ARIADNE_TESTING_PROVE(full.subset(self));
        ARIADNE_TESTING_PROVE(full.intersects(self));
        RobddPaving half(manager,root.split(false),manager.constant(true),0u);
        ARIADNE_TESTING_PROVE(!half.equals(self));
        RobddPaving empty(manager,root,manager.constant(false),0u);
        SubPavingInterface const& empty_self=empty;
        ARIADNE_TESTING_PROVE(!empty.intersects(empty_self));
        RobddPaving zero(manager,GridCell(Grid(0u),0u,BinaryWord{}),manager.constant(true),0u);
        zero.mince(3u);
        ARIADNE_TESTING_EXPECT_EQUAL(zero.depth(),0u);
    }

    Void test_optional_set_capabilities_and_minced_empty_batch() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        CompactSetWrapper<ExactBoxType,EffectiveTag,RealVector> compact(root.box());
        OpenSetWrapper<ExactBoxType,EffectiveTag,RealVector> open(root.box());
        RobddPaving outer(manager,root,manager.constant(false),0u);
        BinaryTreePaving outer_oracle(grid);
        outer.adjoin_outer_approximation(compact,2u);
        outer_oracle.adjoin_outer_approximation(compact,2u);
        ARIADNE_TESTING_PROVE(outer.superset(root));
        for (auto const& cell:outer) { ARIADNE_TESTING_PROVE(outer_oracle.superset(cell)); }
        for (auto const& cell:outer_oracle) { ARIADNE_TESTING_PROVE(outer.superset(cell)); }
        RobddPaving inner(manager,root,manager.constant(false),0u);
        BinaryTreePaving inner_oracle(grid);
        inner.adjoin_inner_approximation(open,0u,2u);
        inner_oracle.adjoin_inner_approximation(open,0u,2u);
        ARIADNE_TESTING_PROVE(!inner.is_empty());
        for (auto const& cell:inner) { ARIADNE_TESTING_PROVE(inner_oracle.superset(cell)); }
        for (auto const& cell:inner_oracle) { ARIADNE_TESTING_PROVE(inner.superset(cell)); }
        RobddPaving minced(manager,root,manager.constant(false),0u);
        minced.mince_to_depth(5u);
        minced.adjoin(std::vector<GridCell>{root.split(false)});
        ARIADNE_TESTING_EXPECT_EQUAL(minced.depth(),5u);
        ARIADNE_TESTING_PROVE(minced.superset(root.split(false)));
        ARIADNE_TESTING_PROVE(!minced.superset(root.split(true)));
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
        BinaryTreeSubpaving source(grid,0u,BinaryWord{},&enabled);
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

        BinaryTreeSubpaving left_explicit(grid,0u,BinaryWord{},&left_tree);
        BinaryTreeSubpaving right_explicit(grid,0u,BinaryWord{},&right_tree);
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

    Void test_detached_subpaving_for_descendants() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        for (unsigned table=0u; table<256u; ++table) {
            RobddPaving original(
                manager,root,manager.constant(false),0u);
            for (unsigned mask=0u; mask<8u; ++mask) {
                if ((table & (1u<<mask))==0u) { continue; }
                BinaryWord word;
                word.push_back((mask & 4u)!=0u);
                word.push_back((mask & 2u)!=0u);
                word.push_back((mask & 1u)!=0u);
                original.adjoin(GridCell(grid,0u,word));
            }
            original.mince_to_depth(3u);
            for (BinaryWord const& prefix : {
                     BinaryWord{},make_binary_word("0"),
                     make_binary_word("1"),make_binary_word("00"),
                     make_binary_word("01"),make_binary_word("10"),
                     make_binary_word("11"),make_binary_word("101")}) {
                GridCell descendant(grid,0u,prefix);
                RobddPaving sliced=original.subpaving(descendant);
                RobddPaving expected(
                    manager,descendant,manager.constant(false),0u);
                for (unsigned mask=0u; mask<8u; ++mask) {
                    if ((table & (1u<<mask))==0u) { continue; }
                    BinaryWord word;
                    word.push_back((mask & 4u)!=0u);
                    word.push_back((mask & 2u)!=0u);
                    word.push_back((mask & 1u)!=0u);
                    GridCell enabled(grid,0u,word);
                    if (enabled.box().subset(descendant.box())) {
                        expected.adjoin(enabled);
                    }
                }
                ARIADNE_TESTING_PROVE(
                    sliced.root_cell() == descendant);
                ARIADNE_TESTING_PROVE(sliced.subset(expected));
                ARIADNE_TESTING_PROVE(expected.subset(sliced));
                ARIADNE_TESTING_EXPECT_EQUAL(
                    sliced.size(),expected.size());
                // A subpaving snapshot must not alias mutable root state.
                sliced.set_root_cell(false);
                ARIADNE_TESTING_PROVE(sliced.is_empty());
                ARIADNE_TESTING_EXPECT_EQUAL(original.depth(),3u);
            }
        }

        GridCell raised(grid,1u,BinaryWord{});
        GridCell fine(grid,0u,make_binary_word("01"));
        RobddPaving parent(manager,raised,manager.constant(true),0u);
        RobddPaving child=parent.subpaving(fine);
        ARIADNE_TESTING_PROVE(child.root_cell() == fine);
        ARIADNE_TESTING_PROVE(child.robdd().is_true());
        ARIADNE_TESTING_EXPECT_EQUAL(child.depth(),0u);
        child.clear();
        ARIADNE_TESTING_PROVE(parent.robdd().is_true());
        ARIADNE_TESTING_PROVE(child.is_empty());

        ARIADNE_TESTING_EXPECT_EXCEPTION(parent.subpaving(
            GridCell(grid,2u,BinaryWord{})));
        Grid different_grid(2u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(parent.subpaving(
            GridCell(different_grid,0u,BinaryWord{})));
        ARIADNE_TESTING_PROVE(parent.robdd().is_true());
    }


};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_robdd_paving");

    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_shared_manager_lifetime_and_interoperability);
    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_shared_import_and_geometric_box_queries);
    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_validated_and_overt_approximation_paths);
    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_iterator_identity_end_contracts_and_display);
    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_public_depth_geometry_and_assignment_contracts);
    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_optional_set_capabilities_and_minced_empty_batch);
    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_terminals_and_complemented_roots);
    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_set_operations_and_preconditions);
    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_distinct_roots_and_extents);
    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_compact_leaf_iteration);








    ARIADNE_TESTING_ADD_TEST(TestRobddPaving, test_detached_subpaving_for_descendants);








    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc,argv);
}
