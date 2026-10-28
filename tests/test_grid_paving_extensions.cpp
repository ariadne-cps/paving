/***************************************************************************
 *            test_grid_paving_extensions.cpp
 *
 *  This file is part of Ariadne, distributed under the GNU GPL version 3
 *  or (at your option) any later version.
 ****************************************************************************/
#include "paving/grid_paving.hpp"
#include "paving/binary_tree.hpp"
#include "set/function_set.hpp"
#include "geometry/set_wrapper.hpp"
#include "function/projection.hpp"
#include "macro/testing.hpp"
#include <memory>
#include <sstream>
using namespace Ariadne;

Void test_projection_levels() {
    Grid grid({1.0_x,-2.0_x},{2.0_x,4.0_x});
    GridCell cell(grid,0u,BinaryWord("0110"));
    GridTreePaving source(cell);
    auto projected=image(source,Projection(2u,{1u}));
    auto expected=ExactBoxType{cell.box()[1]};
    ARIADNE_TEST_ASSERT(same(cast_exact_box(projected.bounding_box()),expected));
    ARIADNE_TEST_EQUALS(projected.grid().origin()[0],grid.origin()[1]);
    ARIADNE_TEST_EQUALS(projected.grid().lengths()[0],grid.lengths()[1]);
    for(const auto& word: {String("0"),String("011"),String("01101"),String("011010")}) {
        GridTreePaving partial(GridCell(grid,1u,BinaryWord(word)));
        for(const auto& indices: {Array<SizeType>{0u},Array<SizeType>{1u},Array<SizeType>{1u,0u}}) {
            Projection projection(2u,indices);
            auto image_set=image(partial,projection);
            ExactBoxType box(indices.size());
            // A paving's primary root differs from the single stored cell.
            const auto original=*partial.begin();
            for(SizeType i=0;i!=indices.size();++i) { box[i]=original.box()[indices[i]]; }
            ARIADNE_TEST_ASSERT(same(cast_exact_box(image_set.bounding_box()),box));
        }
    }
    ARIADNE_TEST_ASSERT(!image(source,Projection(2u,{})).is_empty());
    ARIADNE_TEST_ASSERT(image(GridTreePaving(grid),Projection(2u,{})).is_empty());
    ARIADNE_TEST_FAIL(image(source,Projection(1u,{0u})));
}
GridTreePaving mask_paving(Nat mask,Nat extent) {
    Grid grid(1u);
    GridTreePaving paving(grid,extent,new BinaryTreeNode(false));
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
            ARIADNE_TEST_EQUALS(subset(lhs,rhs),(a&~b)==0u);
            ARIADNE_TEST_EQUALS(intersect(lhs,rhs),(a&b)!=0u);
            auto united=join(lhs,rhs),common=intersection(lhs,rhs),remaining=difference(lhs,rhs);
            for(Nat i=0;i!=4u;++i) {
                BinaryWord word;word.append(static_cast<Bool>(i&2u));word.append(static_cast<Bool>(i&1u));
                GridCell cell(Grid(1u),0u,word);
                ARIADNE_TEST_EQUALS(united.superset(cell),static_cast<Bool>((a|b)&(1u<<i)));
                ARIADNE_TEST_EQUALS(common.superset(cell),static_cast<Bool>((a&b)&(1u<<i)));
                ARIADNE_TEST_EQUALS(remaining.superset(cell),static_cast<Bool>((a&~b)&(1u<<i)));
            }
        }
    }
}

Void test_queries_cursors_and_constructors() {
    Grid grid(1u);
    ARIADNE_TEST_ASSERT(Grid(1u)!=Grid(2u));
    ARIADNE_TEST_ASSERT(Grid(2u)!=Grid(1u));
    GridTreePaving default_paving;
    ARIADNE_TEST_ASSERT(default_paving.is_empty());
    GridTreePaving enabled(grid,true),disabled(1u,false);
    ARIADNE_TEST_ASSERT(disabled.is_empty());
    GridTreePaving enclosing(grid,ExactBoxType{{-1.0_x,2.0_x}});
    ARIADNE_TEST_ASSERT(enclosing.is_empty());
    GridTreePaving encoded(grid,0u,BooleanArray{true,false,false},BooleanArray{true,false});
    ARIADNE_TEST_EQUALS(encoded.size(),1u);
    GridTreePaving copied(encoded);
    ARIADNE_TEST_ASSERT(copied==encoded);
    const GridTreePaving& alias=copied; copied=alias;
    ARIADNE_TEST_ASSERT(copied==encoded);
    const auto empty_bounds=disabled.bounding_box();
    ARIADNE_TEST_ASSERT(definitely(empty_bounds.is_empty()));
    ARIADNE_TEST_EQUALS(disabled.measure().raw(),FloatDP(0,dp));
    ARIADNE_TEST_EQUALS(enabled.measure().raw(),FloatDP(1,dp));
    auto boxes=static_cast<ListSet<ExactBoxType>>(encoded);
    ARIADNE_TEST_EQUALS(boxes.size(),1u);
    ARIADNE_TEST_ASSERT(static_cast<ListSet<ExactBoxType>>(disabled).empty());
    GridTreeSubpaving view(encoded);
    ARIADNE_TEST_ASSERT(view==encoded);
    auto full=mask_paving(15u,0u),half=mask_paving(3u,0u);
    for(auto* paving:{&full,&half,&disabled}) {
        ARIADNE_TEST_PRINT(*paving);
        for(const auto& query:{ExactBoxType{{-1.0_x,2.0_x}},ExactBoxType{{0.125_x,0.375_x}},ExactBoxType{{0.625_x,0.875_x}},ExactBoxType{{1.0_x,2.0_x}}}) {
            ARIADNE_TEST_EQUALS(paving->disjoint(query),!paving->intersects(query));
            ARIADNE_TEST_EQUALS(paving->subset(query),definitely(subset(paving->bounding_box(),query)));
            ARIADNE_TEST_ASSERT(!definitely(paving->covers(query)) || paving->superset(query));
        }
    }
    // Boundary-only contact gives the same result before and after subdivision.
    ARIADNE_TEST_ASSERT(enabled.disjoint(ExactBoxType{{1.0_x,2.0_x}}));
    enabled.mince_to_depth(2u);
    ARIADNE_TEST_ASSERT(enabled.disjoint(ExactBoxType{{1.0_x,2.0_x}}));
    enabled.mince_to_depth(0u);
    auto branch=enabled.branch(false);
    branch.mince_to_depth(0u);
    branch.subdivide(ApproximateDouble(0.125));
    GridTreeCursor singular,other_singular;
    ARIADNE_TEST_ASSERT(singular==other_singular);
    GridTreeCursor cursor(&encoded);
    ARIADNE_TEST_ASSERT(!(singular==cursor));
    ARIADNE_TEST_ASSERT(!(cursor==singular));
    GridTreeCursor assigned;assigned=cursor;
    ARIADNE_TEST_ASSERT(assigned==cursor);
    ARIADNE_TEST_ASSERT(!cursor.is_right_child());
    const auto& const_cursor=cursor;
    ARIADNE_TEST_ASSERT((*const_cursor)==encoded);
    std::ostringstream text;text<<singular;
    ARIADNE_TEST_ASSERT(!text.str().empty());
    GridTreeConstIterator invalid;
    ARIADNE_TEST_ASSERT(invalid==GridTreeConstIterator());
    auto end=encoded.end();++end;
    ARIADNE_TEST_ASSERT(end==encoded.end());
    GridTreeConstIterator last(&encoded,false),none(&encoded,indeterminate);
    ARIADNE_TEST_ASSERT(last!=none);
    ARIADNE_TEST_ASSERT(none!=last);
    ARIADNE_TEST_ASSERT(last->box().subset(ExactBoxType{{0.0_x,0.5_x}}));
    GridTreePaving only_right=mask_paving(8u,0u);
    GridTreeConstIterator right_last(&only_right,false);
    ARIADNE_TEST_ASSERT(same(right_last->box(),ExactBoxType{{0.75_x,1.0_x}}));
    GridCell zero;GridOpenCell open_zero;
    ARIADNE_TEST_EQUALS(zero.dimension(),0u);
    ARIADNE_TEST_EQUALS(open_zero.box().dimension(),0u);
    GridCell cell(grid,1u,BinaryWord("10"));
    const GridAbstractCell& abstract=cell;
    ARIADNE_TEST_EQUALS(abstract.dimension(),1u);
    ARIADNE_TEST_EQUALS(abstract.root_height(),1u);
    ARIADNE_TEST_ASSERT(same(abstract[0u],cell.box()[0u]));
    auto small=cell.interior(),large=GridCell(grid,1u,BinaryWord("1")).interior();
    ARIADNE_TEST_ASSERT(small<large || large<small);
    std::ostringstream open_text;open_text<<small;
    ARIADNE_TEST_ASSERT(!open_text.str().empty());
    ARIADNE_TEST_ASSERT(!GridOpenCell::intersection(small,large).empty());
    BinaryWord first_path,second_path;Nat extent=0u;
    ARIADNE_TEST_ASSERT(subset(cell,cell,nullptr,nullptr,nullptr));
    ARIADNE_TEST_ASSERT(subset(cell,cell,&first_path,&second_path,&extent));
    ARIADNE_TEST_EQUALS(extent,1u);
    ARIADNE_TEST_FAIL(subset(cell,cell,&first_path,nullptr,nullptr));
    ARIADNE_TEST_FAIL(subset(cell,cell,nullptr,&second_path,nullptr));
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
    GridTreePaving lower(grid),open_lower(grid),bounded_lower(grid);
    lower.adjoin_lower_approximation(overt.reference(),0u,3u);
    lower.adjoin_lower_approximation(overt.reference(),0u,3u);
    open_lower.adjoin_lower_approximation(shape,3u);
    bounded_lower.adjoin_lower_approximation(overt.reference(),box,3u);
    bounded_lower.adjoin_lower_approximation(open.reference(),box,3u);
    ARIADNE_TEST_ASSERT(!lower.is_empty());
    ARIADNE_TEST_ASSERT(!open_lower.is_empty());
    ARIADNE_TEST_ASSERT(!bounded_lower.is_empty());
    ARIADNE_TEST_ASSERT(lower.subset(ExactBoxType{{0.0_x,1.0_x}}));
    auto inner_by_extent=inner_approximation(open.reference(),grid,0u,3u);
    auto inner_by_box=inner_approximation(open.reference(),grid,box,3u);
    auto inner_regular=inner_approximation(regular_shape,grid,3u);
    ARIADNE_TEST_ASSERT(!inner_by_extent.is_empty());
    ARIADNE_TEST_ASSERT(inner_by_extent.subset(box));
    ARIADNE_TEST_ASSERT(inner_by_box.subset(box));
    ARIADNE_TEST_ASSERT(inner_regular.subset(box));
    GridTreePaving inner_lower(grid);
    inner_lower.adjoin_inner_approximation(LowerBoxType(box),3u);
    ARIADNE_TEST_ASSERT(inner_lower.subset(box));
    auto outer_box=outer_approximation(box,2u);
    auto outer_compact=outer_approximation(compact_shape,2u);
    ValidatedConstrainedImageSet validated(box);
    const ValidatedEuclideanCompactSetInterface& validated_compact=validated;
    auto outer_validated=outer_approximation(validated_compact,2u);
    ARIADNE_TEST_ASSERT(outer_box.superset(box));
    ARIADNE_TEST_ASSERT(outer_compact.superset(box));
    ARIADNE_TEST_ASSERT(outer_validated.superset(box));
    GridTreePaving over(grid);
    over.adjoin_over_approximation(box,2u);
    ARIADNE_TEST_ASSERT(over.superset(box));
    ARIADNE_TEST_FAIL(over.adjoin_over_approximation(ExactBoxType{{0.0_x,0.0_x}},2u));
    GridTreePaving compact_outer(grid);
    compact_outer.adjoin_outer_approximation(compact.reference(),2u);
    compact_outer.adjoin_outer_approximation(shape,2u);
    ARIADNE_TEST_ASSERT(compact_outer.superset(box));
    GridTreePaving full(grid,true);
    full.adjoin_outer_approximation(shape,2u);
    full.adjoin_lower_approximation(overt.reference(),0u,2u);
    full.adjoin_inner_approximation(open.reference(),0u,2u);
    ARIADNE_TEST_ASSERT(full.superset(GridCell(grid,0u,BinaryWord())));
    ExactBoxType wrong{{0.0_x,1.0_x},{0.0_x,1.0_x}};
    ARIADNE_TEST_FAIL(over.subset(wrong));
    ARIADNE_TEST_FAIL(over.superset(wrong));
    ARIADNE_TEST_FAIL(over.disjoint(wrong));
    ARIADNE_TEST_FAIL(over.intersects(wrong));
    ARIADNE_TEST_FAIL(over.covers(wrong));
    ARIADNE_TEST_FAIL(over.inside(wrong));
    ARIADNE_TEST_FAIL(over.separated(wrong));
    ARIADNE_TEST_FAIL(over.overlaps(wrong));
    ARIADNE_TEST_FAIL(over.smallest_enclosing_primary_cell(wrong));
    GridTreePaving wrong_grid(Grid(2u));
    ARIADNE_TEST_FAIL(over.adjoin(wrong_grid.root_cell()));
    ARIADNE_TEST_FAIL(over.adjoin(wrong_grid));
    ARIADNE_TEST_FAIL(subset(over,wrong_grid));
    ARIADNE_TEST_FAIL(intersect(over,wrong_grid));
    ARIADNE_TEST_FAIL(join(over,wrong_grid));
    ARIADNE_TEST_FAIL(intersection(over,wrong_grid));
    ARIADNE_TEST_FAIL(difference(over,wrong_grid));
}

Void test_products_and_drawing() {
    GridCell cell(Grid(1u),0u,BinaryWord("01"));
    GridTreePaving set(cell);
    auto product_set=product(cell,set);
    ARIADNE_TEST_ASSERT(same(cast_exact_box(product_set.bounding_box()),product(cell.box(),cell.box())));
    ARIADNE_TEST_ASSERT(product(cell,GridTreePaving(Grid(1u))).is_empty());
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

Void test_partial_tree_operations() {
    Grid grid(1u);
    GridTreePaving owner(grid,true);
    owner.mince(2u);
    auto left=owner.branch(false),right=owner.branch(true);
    GridTreeSubpaving assigned(left);assigned=right;
    ARIADNE_TEST_ASSERT(assigned==right);
    ARIADNE_TEST_ASSERT(!(left==right));
    auto disabled=mask_paving(0u,0u);
    ARIADNE_TEST_ASSERT(!(owner==disabled));
    ARIADNE_TEST_ASSERT(!subset(left,right));
    ARIADNE_TEST_ASSERT(!subset(right,left));
    ARIADNE_TEST_ASSERT(subset(left,owner));
    ARIADNE_TEST_ASSERT(!subset(owner,left));
    auto quarter=left.branch(false);
    ARIADNE_TEST_ASSERT(!subset(owner,quarter));
    GridTreePaving whole_cell(grid,true);
    ARIADNE_TEST_ASSERT(subset(whole_cell,whole_cell));
    ARIADNE_TEST_ASSERT(subset(disabled,left));
    ARIADNE_TEST_ASSERT(!subset(left,disabled));
    GridTreeSubpaving whole(owner);
    std::unique_ptr<GridTreeSubpaving> cloned(whole.clone());
    ARIADNE_TEST_ASSERT(cloned->superset(owner.root_cell()));
    GridTreePaving larger(grid,2u,new BinaryTreeNode(true));
    ARIADNE_TEST_ASSERT(!subset(larger,left));
    auto first_half=mask_paving(3u,0u);
    ARIADNE_TEST_ASSERT(subset(first_half,left));
    auto last_half=mask_paving(12u,0u);
    ARIADNE_TEST_ASSERT(subset(last_half,right));
    GridTreePaving empty(grid);
    ARIADNE_TEST_ASSERT(subset(empty,left));
    GridTreeCursor root_cursor(&owner);
    ARIADNE_TEST_ASSERT(!root_cursor.is_right_child());
    root_cursor.move_left();
    ARIADNE_TEST_ASSERT(!root_cursor.is_right_child());
    GridTreeConstIterator grid_iterator(&owner,true);
    VectorCellIterator vector_iterator(std::vector<GridCell>{*grid_iterator});
    ForwardConstantIteratorHandle<GridCell> grid_handle(grid_iterator),vector_handle(vector_iterator);
    ARIADNE_TEST_ASSERT(grid_handle!=vector_handle);
    GridTreePaving false_tree(grid,false);
    GridTreeConstIterator last_disabled(&false_tree,false);
    ARIADNE_TEST_ASSERT(last_disabled==false_tree.end());
    GridTreePaving left_only=mask_paving(1u,0u);
    GridTreeConstIterator last_left(&left_only,false);
    ARIADNE_TEST_ASSERT(same(last_left->box(),ExactBoxType{{0.0_x,0.25_x}}));
    auto small=GridCell(grid,0u,BinaryWord("01")).interior();
    auto large=GridCell(grid,0u,BinaryWord()).interior();
    ARIADNE_TEST_ASSERT(!GridOpenCell::intersection(small,large).empty());
    Grid skew({0.0_x,0.0_x},{8.0_x,1.0_x});
    GridTreePaving skew_set(GridCell(skew,0u,BinaryWord("00")));
    auto skew_branch=skew_set.branch(false).branch(false);
    skew_branch.subdivide(ApproximateDouble(0.125));
    ARIADNE_TEST_EQUALS(skew_branch.tree_depth(),9u);
    for(const auto& cell:skew_branch) {
        ARIADNE_TEST_ASSERT(cell.box()[0].width().upper_raw()<=FloatDP(0.125_x,dp));
        ARIADNE_TEST_ASSERT(cell.box()[1].width().upper_raw()<=FloatDP(0.125_x,dp));
    }
    GridTreePaving another(Grid(2u),true);
    another.subdivide(ApproximateDouble(0.5));
    ARIADNE_TEST_EQUALS(another.tree_depth(),2u);
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
    GridTreePaving full(grid,2u,new BinaryTreeNode(true));
    full.adjoin(cell);
    full.adjoin(GridTreePaving(cell));
    full.adjoin_outer_approximation(compact.reference(),2u);
    full.adjoin_outer_approximation(validated,2u);
    full.adjoin_lower_approximation(overt.reference(),0u,2u);
    full.adjoin_inner_approximation(open.reference(),0u,2u);
    ARIADNE_TEST_ASSERT(full.superset(GridCell(grid,2u,BinaryWord())));
    full.remove(cell);
    ARIADNE_TEST_ASSERT(!full.superset(cell));
    GridTreePaving empty(grid,2u,new BinaryTreeNode(false));
    empty.remove(cell);
    ARIADNE_TEST_ASSERT(empty.is_empty());
    auto previous_lower=mask_paving(1u,0u);
    previous_lower.adjoin_lower_approximation(shape,0u);
    ARIADNE_TEST_ASSERT(previous_lower.superset(GridCell(grid,0u,BinaryWord("00"))));
    GridTreePaving deep_removal(grid,true);
    deep_removal.remove(GridCell(grid,0u,BinaryWord("000")));
    ARIADNE_TEST_ASSERT(!deep_removal.superset(GridCell(grid,0u,BinaryWord("000"))));
    GridTreePaving small(grid,true);
    small.mince(2u);
    small.remove(GridCell(grid,0u,BinaryWord("01")));
    ARIADNE_TEST_ASSERT(!small.superset(GridCell(grid,0u,BinaryWord("01"))));
    auto partial=mask_paving(1u,0u);
    auto rhs=partial.branch(false);
    GridTreePaving root_enabled(grid,true);
    root_enabled.restrict(rhs);
    ARIADNE_TEST_ASSERT(root_enabled==partial);
    GridTreePaving root_empty(grid,false);
    root_empty.restrict(rhs);root_empty.remove(rhs);
    ARIADNE_TEST_ASSERT(root_empty.is_empty());
    GridTreePaving root_full(grid,true);
    root_full.remove(rhs);
    ARIADNE_TEST_ASSERT(!root_full.intersects(ExactBoxType{{0.0_x,0.25_x}}));
    GridTreePaving restricted(grid,2u,new BinaryTreeNode(true));
    restricted.restrict_to_extent(0u);
    ARIADNE_TEST_ASSERT(restricted.superset(cell));
    auto effective=mask_paving(1u,0u);
    effective.adjoin_outer_approximation(shape,0u);
    ARIADNE_TEST_ASSERT(effective.superset(box));
    auto validated_paving=mask_paving(1u,0u);
    RegularLocatedSet<ValidatedTag,RealVector> validated_regular(box);
    validated_paving.adjoin_outer_approximation(validated_regular.reference(),0u);
    ARIADNE_TEST_ASSERT(validated_paving.superset(box));
    validated_paving.adjoin_outer_approximation(validated_regular.reference(),2u);
    GridTreePaving validated_fresh(grid);
    validated_fresh.adjoin_outer_approximation(validated_regular.reference(),3u);
    ARIADNE_TEST_ASSERT(validated_fresh.superset(box));
    GridTreePaving validated_full(grid,true);
    validated_full.adjoin_outer_approximation(validated,2u);
    ARIADNE_TEST_ASSERT(validated_full.superset(cell));
}

Void test_cell_range_contracts() {
    Grid grid(1u);
    GridCell large=GridCell::smallest_enclosing_primary_cell(ExactBoxType{{1099511627776.0_x,1099511627777.0_x}},grid);
    ARIADNE_TEST_ASSERT(large.box().covers(ExactBoxType{{1099511627776.0_x,1099511627777.0_x}}));
    GridCell largest(grid,53u,BinaryWord());
    ARIADNE_TEST_EQUALS(largest.root_height(),53u);
    ARIADNE_TEST_FAIL(GridCell::primary_cell_lattice_box(54u,1u));
    ARIADNE_TEST_FAIL(GridCell::smallest_enclosing_primary_cell(ExactBoxType{{100000000000000000.0_x,100000000000000016.0_x}},grid));
    ARIADNE_TEST_FAIL(GridCell::compute_lattice_box(65536u,0u,BinaryWord()));
    ARIADNE_TEST_FAIL(GridCell::compute_lattice_box(0u,0u,BinaryWord("0")));
    BinaryWord overlong;
    overlong.resize(static_cast<SizeType>(std::numeric_limits<Int>::max())+1u);
    ARIADNE_TEST_FAIL(GridCell::compute_lattice_box(1u,0u,overlong));
    Grid wrong(2u);
    GridCell cell(grid,0u,BinaryWord()),other(wrong,0u,BinaryWord());
    ARIADNE_TEST_FAIL(static_cast<Void>(cell==other));
    ARIADNE_TEST_FAIL(subset(cell,other,nullptr,nullptr,nullptr));
    GridTreePaving paving(grid),wrong_paving(wrong);
    ARIADNE_TEST_FAIL(subset(cell,wrong_paving));
    ARIADNE_TEST_FAIL(intersect(cell,wrong_paving));
    ARIADNE_TEST_FAIL(paving.remove(other));
    ARIADNE_TEST_FAIL(paving.remove(wrong_paving));
    ARIADNE_TEST_FAIL(paving.restrict(wrong_paving));
    ARIADNE_TEST_FAIL(product(cell,GridCell(grid,0u,BinaryWord("0"))));
    ExactBoxType wrong_box{{0.0_x,1.0_x},{0.0_x,1.0_x}};
    ExactBoxSetType wrong_shape(wrong_box);
    ValidatedConstrainedImageSet wrong_validated(wrong_box);
    ARIADNE_TEST_FAIL(paving.adjoin_outer_approximation(wrong_shape,1u));
    ARIADNE_TEST_FAIL(paving.adjoin_outer_approximation(wrong_validated,1u));
    const EffectiveEuclideanOvertSetInterface& wrong_overt=wrong_shape;
    const EffectiveEuclideanOpenSetInterface& wrong_open=wrong_shape;
    ExactBoxSetType shape(ExactBoxType{{0.0_x,1.0_x}});
    const EffectiveEuclideanOvertSetInterface& overt=shape;
    const EffectiveEuclideanOpenSetInterface& open=shape;
    ARIADNE_TEST_FAIL(paving.adjoin_lower_approximation(wrong_overt,0u,1u));
    ARIADNE_TEST_FAIL(paving.adjoin_lower_approximation(wrong_overt,wrong_box,1u));
    ARIADNE_TEST_FAIL(paving.adjoin_lower_approximation(overt,wrong_box,1u));
    ARIADNE_TEST_FAIL(paving.adjoin_inner_approximation(wrong_open,0u,1u));
    ARIADNE_TEST_FAIL(paving.adjoin_inner_approximation(wrong_open,wrong_box,1u));
    ARIADNE_TEST_FAIL(paving.adjoin_inner_approximation(open,wrong_box,1u));
}
Int main() {
    ARIADNE_TEST_CALL(test_cell_range_contracts());
    ARIADNE_TEST_CALL(test_partial_tree_operations());
    ARIADNE_TEST_CALL(test_ancestor_early_stops());
    ARIADNE_TEST_CALL(test_projection_levels());
    ARIADNE_TEST_CALL(test_paving_set_algebra());
    ARIADNE_TEST_CALL(test_queries_cursors_and_constructors());
    ARIADNE_TEST_CALL(test_approximation_apis());
    ARIADNE_TEST_CALL(test_products_and_drawing());
    return ARIADNE_TEST_FAILURES;
}
