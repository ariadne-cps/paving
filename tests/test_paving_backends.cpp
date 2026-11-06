/***************************************************************************
 *            test_paving_backends.cpp
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

#include <algorithm>
#include <type_traits>
#include <vector>

#include "paving/binary_tree_paving.hpp"
#include "paving/grid_paving.hpp"
#include "paving/robdd_paving.hpp"
#include "paving/paving_cell_operations.hpp"
#include "ariadne-testing.hpp"

using namespace Ariadne;

static_assert(std::is_same_v<BinaryTreePaving,GridTreePaving>);


class TestPavingBackends : public Ariadne::Testing::ContextFixture {
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
        RobddPaving snapshot(manager,source);
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

    Void test_compact_leaves_exhaustive_truth_tables() {
        RobddManager manager;
        Grid grid(1u);
        BinaryWord prefix=make_binary_word("01");
        // All 256 truth tables of three subdivision variables.
        for (unsigned table=0u; table<256u; ++table) {
            BinaryTreeNode explicit_tree(false);
            for (unsigned mask=0u; mask<8u; ++mask) {
                if ((table & (1u<<mask)) == 0u) { continue; }
                BinaryWord path;
                for (unsigned bit=0u; bit<3u; ++bit) {
                    path.push_back(((mask>>(2u-bit)) & 1u) != 0u);
                }
                explicit_tree.add_enabled(path);
            }
            explicit_tree.recombine();
            GridTreeSubpaving source(grid,2u,prefix,&explicit_tree);
            RobddPaving snapshot(manager,source);
            // A fully uniform truth table can recombine to a terminal.
            // Restore the declared three-variable universe for evaluation.
            snapshot.mince_to_depth(3u);

            std::vector<BinaryWord> expected;
            for (auto it=source.begin(), end=source.end(); it!=end; ++it) {
                expected.push_back(it->word());
            }
            std::vector<BinaryWord> actual;
            snapshot.for_each_enabled_leaf([&](GridCell const& cell) {
                actual.push_back(cell.word());
            });
            ARIADNE_TESTING_EXPECT_EQUAL(actual.size(),expected.size());
            ARIADNE_TESTING_EXPECT_EQUAL(snapshot.enabled_leaf_count(),expected.size());
            ARIADNE_TESTING_EXPECT_EQUAL(snapshot.size(),expected.size());
            std::vector<BinaryWord> incrementally_visited;
            for (auto it=snapshot.begin(), end=snapshot.end(); it!=end; ++it) {
                incrementally_visited.push_back(it->word());
            }
            ARIADNE_TESTING_PROVE(incrementally_visited == actual);
            // Copying the iterator copies its pending search frontier,
            // and post-increment retains the old position.
            if (snapshot.begin() != snapshot.end()) {
                auto iterator=snapshot.begin();
                auto copy=iterator;
                ARIADNE_TESTING_PROVE(iterator == copy);
                auto old=iterator++;
                ARIADNE_TESTING_PROVE(old == copy);
                ++copy;
                ARIADNE_TESTING_PROVE(copy == iterator);
            }
            for (SizeType i=0u; i<expected.size(); ++i) {
                ARIADNE_TESTING_PROVE(actual[i] == expected[i]);
            }
            auto reconstructed=snapshot.to_binary_tree();
            reconstructed->recombine();
            ARIADNE_TESTING_EXPECT_EQUAL(
                snapshot.enabled_leaf_count(),
                BinaryTreeNode::count_enabled_leaf_nodes(reconstructed.get()));
            SizeType expected_satisfying=0u;
            for (unsigned mask=0u; mask<8u; ++mask) {
                BinaryWord path;
                for (unsigned bit=0u; bit<3u; ++bit) {
                    path.push_back(((mask>>(2u-bit)) & 1u) != 0u);
                }
                Bool enabled=(table & (1u<<mask)) != 0u;
                ARIADNE_TESTING_EXPECT_EQUAL(snapshot.is_enabled(path),enabled);
                if (enabled) { ++expected_satisfying; }
            }
            ARIADNE_TESTING_EXPECT_EQUAL(
                snapshot.enabled_cell_count(),expected_satisfying);
        }
    }

    Void test_superset_direct_no_manager_allocations() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,1u,BinaryWord{});
        GridCell bigger(grid,2u,BinaryWord{});
        GridCell equivalent(grid,2u,make_binary_word("00"));
        GridCell alternate(grid,0u,BinaryWord{});
        // In 1D, extent-2 primary "00" is geometrically the extent-1
        // primary root, and extent-0 root lies at the left half of it.
        std::vector<GridCell> probes{
            root,bigger,equivalent,alternate,
            GridCell(grid,1u,make_binary_word("0")),
            GridCell(grid,1u,make_binary_word("1")),
            GridCell(grid,1u,make_binary_word("00")),
            GridCell(grid,1u,make_binary_word("01")),
            GridCell(grid,1u,make_binary_word("10")),
            GridCell(grid,1u,make_binary_word("11")),
            GridCell(grid,1u,make_binary_word("000")),
            GridCell(grid,1u,make_binary_word("001")),
            GridCell(grid,1u,make_binary_word("010")),
            GridCell(grid,1u,make_binary_word("011")),
            GridCell(grid,1u,make_binary_word("100")),
            GridCell(grid,1u,make_binary_word("101")),
            GridCell(grid,1u,make_binary_word("110")),
            GridCell(grid,1u,make_binary_word("111")),
            GridCell(grid,1u,make_binary_word("1111")),
            // Equivalent and strict descendants expressed through a wider
            // primary-cell extent (both succeed and fail depending on mask).
            GridCell(grid,2u,make_binary_word("001")),
            GridCell(grid,2u,make_binary_word("0010")),
            GridCell(grid,2u,make_binary_word("00101")),
            GridCell(grid,3u,make_binary_word("0000010")),
            GridCell(grid,3u,make_binary_word("0000011"))};

        for(unsigned mask=0u;mask<256u;++mask) {
            BinaryTreePaving oracle(grid,1u,new BinaryTreeNode(false));
            RobddPaving subject(
                manager,root,manager.constant(false),0u);
            for(unsigned k=0u;k<8u;++k) {
                if((mask&(1u<<k))==0u) { continue; }
                BinaryWord word;
                for(unsigned bit=0u;bit<3u;++bit) {
                    word.push_back((k&(1u<<(2u-bit)))!=0u);
                }
                GridCell cell(grid,1u,word);
                oracle.adjoin(cell);
                subject.adjoin(cell);
            }
            oracle.recombine();
            subject.recombine();
            if((mask&1u)!=0u) subject.mince_to_depth(5u);

            auto before=manager.statistics();
            for(GridCell const& probe:probes) {
                ARIADNE_TESTING_EXPECT_EQUAL(
                    subject.superset(probe),oracle.superset(probe));
            }
            auto after=manager.statistics();
            ARIADNE_TESTING_EXPECT_EQUAL(after.node_count,before.node_count);
            ARIADNE_TESTING_EXPECT_EQUAL(after.node_capacity,before.node_capacity);
            ARIADNE_TESTING_EXPECT_EQUAL(
                after.unique_table_entries,before.unique_table_entries);
            ARIADNE_TESTING_EXPECT_EQUAL(
                after.ite_cache_entries,before.ite_cache_entries);
        }

        // Complemented edges and skipped variables are checked directly.
        auto x=manager.variable(1u);
        RobddPaving complemented(
            manager,root,manager.negation(x),3u);
        for(GridCell const& probe:probes) {
            ARIADNE_TESTING_EXPECT_EQUAL(
                complemented.superset(probe),
                [&] {
                    RobddPaving expected(manager,root,manager.negation(x),3u);
                    RobddPaving required(
                        manager,probe,manager.constant(true),0u);
                    return required.subset(expected);
                }());
        }
        // The reference operation above intentionally allocates temporary
        // BDD objects; check the no-allocation contract separately below.
        auto mid=manager.statistics();
        for(GridCell const& probe:probes) {
            (void)complemented.superset(probe);
        }
        auto after=manager.statistics();
        ARIADNE_TESTING_EXPECT_EQUAL(after.node_count,mid.node_count);
        ARIADNE_TESTING_EXPECT_EQUAL(
            after.unique_table_entries,mid.unique_table_entries);
        ARIADNE_TESTING_EXPECT_EQUAL(
            after.ite_cache_entries,mid.ite_cache_entries);

        Grid other(2u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            complemented.superset(GridCell(other,0u,BinaryWord{})));
    }

    Void test_polymorphic_cell_batch_and_explicit_import() {
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        std::vector<GridCell> cells{
            root.split(false),
            root.split(true).split(false)};

        // The tree uses PavingInterface's default per-cell implementation,
        // both when called directly and through its abstract base.
        BinaryTreePaving tree(grid);
        tree.adjoin(cells);
        ARIADNE_TESTING_PROVE(tree.superset(cells[0]));
        ARIADNE_TESTING_PROVE(tree.superset(cells[1]));
        ARIADNE_TESTING_PROVE(not tree.superset(root));

        BinaryTreePaving via_base(grid);
        PavingInterface& tree_base=via_base;
        tree_base.adjoin(cells);
        ARIADNE_TESTING_PROVE(via_base.superset(cells[0]));
        ARIADNE_TESTING_PROVE(via_base.superset(cells[1]));

        PavingHandle tree_handle(new BinaryTreePaving(grid));
        tree_handle.adjoin(cells);
        ARIADNE_TESTING_PROVE(tree_handle.superset(cells[0]));
        ARIADNE_TESTING_PROVE(tree_handle.superset(cells[1]));

        // Abstract-base dispatch must select the ROBDD bulk override.
        RobddManager manager;
        RobddPaving bdd(manager,root,manager.constant(false),0u);
        PavingInterface& bdd_base=bdd;
        bdd_base.adjoin(cells);
        ARIADNE_TESTING_PROVE(bdd.superset(cells[0]));
        ARIADNE_TESTING_PROVE(bdd.superset(cells[1]));
        ARIADNE_TESTING_PROVE(not bdd.superset(root));
        ARIADNE_TESTING_EXPECT_EQUAL(
            manager.statistics().ite_cache_entries,0u);

        RobddManager handle_manager;
        PavingHandle handle(new RobddPaving(
            handle_manager,root,handle_manager.constant(false),0u));
        handle.adjoin(cells);
        ARIADNE_TESTING_PROVE(handle.superset(cells[0]));
        ARIADNE_TESTING_PROVE(handle.superset(cells[1]));
        ARIADNE_TESTING_EXPECT_EQUAL(
            handle_manager.statistics().ite_cache_entries,0u);

        // The *explicit* generic-subpaving conversion should use bulk too,
        // rather than populating the manager through singleton unions.
        RobddManager conversion_manager;
        SubPavingInterface const& source=tree;
        RobddPaving imported(conversion_manager,source);
        for(auto it=tree.begin(), end=tree.end();it!=end;++it) {
            ARIADNE_TESTING_PROVE(imported.superset(*it));
        }
        for(auto it=imported.begin(), end=imported.end();it!=end;++it) {
            ARIADNE_TESTING_PROVE(tree.superset(*it));
        }
        ARIADNE_TESTING_EXPECT_EQUAL(
            conversion_manager.statistics().ite_cache_entries,0u);
    }

    Void test_bulk_cell_construction_and_union() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,2u,make_binary_word("01"));

        // Exhaust every three-bit occupancy function. Independent oracle:
        // leaf bits are interpreted as an exact Boolean truth table.
        for(unsigned mask=0u;mask<256u;++mask) {
            std::vector<GridCell> cells;
            for(unsigned k=0u;k<8u;++k) {
                if((mask&(1u<<k))==0u) { continue; }
                BinaryWord word=root.word();
                for(unsigned bit=0u;bit<3u;++bit) {
                    word.push_back((k&(1u<<(2u-bit)))!=0u);
                }
                cells.emplace_back(grid,2u,word);
            }
            if((mask&1u)!=0u) std::reverse(cells.begin(),cells.end());

            RobddPaving bulk=RobddPaving::from_cells(manager,root,cells);
            RobddPaving adjoined(
                manager,root,manager.constant(false),0u);
            RobddPaving incremental(
                manager,root,manager.constant(false),0u);
            adjoined.adjoin(cells);
            for(GridCell const& cell:cells) incremental.adjoin(cell);
            bulk.mince_to_depth(3u);
            adjoined.mince_to_depth(3u);
            incremental.mince_to_depth(3u);
            ARIADNE_TESTING_PROVE(bulk.root_cell()==root);
            ARIADNE_TESTING_PROVE(adjoined.root_cell()==root);
            for(unsigned k=0u;k<8u;++k) {
                BinaryWord word;
                for(unsigned bit=0u;bit<3u;++bit) {
                    word.push_back((k&(1u<<(2u-bit)))!=0u);
                }
                Bool expected=(mask&(1u<<k))!=0u;
                ARIADNE_TESTING_EXPECT_EQUAL(bulk.is_enabled(word),expected);
                ARIADNE_TESTING_EXPECT_EQUAL(adjoined.is_enabled(word),expected);
                ARIADNE_TESTING_EXPECT_EQUAL(incremental.is_enabled(word),expected);
            }
            bulk.recombine();
            adjoined.recombine();
            incremental.recombine();
            ARIADNE_TESTING_PROVE(bulk.robdd()==incremental.robdd());
            ARIADNE_TESTING_PROVE(adjoined.robdd()==incremental.robdd());
        }

        // Out-of-order duplicate/ancestor/descendant cells must represent
        // their geometric union regardless of insertion order.
        std::vector<GridCell> overlapped={
            root.split(false).split(false),
            root.split(true).split(true),
            root.split(false).split(true),
            root.split(false),
            root.split(false).split(false),
            root.split(true).split(true)};
        RobddPaving bulk=RobddPaving::from_cells(manager,root,overlapped);
        RobddPaving incremental(
            manager,root,manager.constant(false),0u);
        for(GridCell const& cell:overlapped) incremental.adjoin(cell);
        bulk.recombine();
        incremental.recombine();
        ARIADNE_TESTING_PROVE(bulk.robdd()==incremental.robdd());
        ARIADNE_TESTING_PROVE(bulk.superset(root.split(false)));
        ARIADNE_TESTING_PROVE(bulk.superset(root.split(true).split(true)));
        ARIADNE_TESTING_PROVE(not bulk.superset(root.split(true)));
        auto full=RobddPaving::from_cells(
            manager,root,{root.split(false),root.split(true)});
        ARIADNE_TESTING_PROVE(full.robdd().is_true());
        auto empty=RobddPaving::from_cells(manager,root,{});
        ARIADNE_TESTING_PROVE(empty.is_empty());

        // Geometrically compatible cells with different primary extents.
        GridCell wider(grid,1u,BinaryWord{});
        GridCell coarse(grid,1u,make_binary_word("0"));
        GridCell fine(grid,0u,make_binary_word("0"));
        std::vector<GridCell> mixed={fine,coarse,fine};
        auto aligned=RobddPaving::from_cells(manager,wider,mixed);
        RobddPaving via_union(
            manager,wider,manager.constant(false),0u);
        for(GridCell const& cell:mixed) via_union.adjoin(cell);
        aligned.recombine();
        via_union.recombine();
        ARIADNE_TESTING_PROVE(aligned.robdd()==via_union.robdd());
        ARIADNE_TESTING_PROVE(aligned.root_cell()==wider);

        // Bulk union must enlarge a nonempty paving's geometric root.
        GridCell smaller(grid,0u,BinaryWord{});
        RobddPaving existing(
            manager,smaller,manager.constant(false),0u);
        existing.adjoin(smaller.split(true));
        RobddPaving expected(existing);
        for(GridCell const& cell:mixed) expected.adjoin(cell);
        existing.adjoin(mixed);
        ARIADNE_TESTING_PROVE(existing.root_cell()==expected.root_cell());
        ARIADNE_TESTING_PROVE(existing.subset(expected));
        ARIADNE_TESTING_PROVE(expected.subset(existing));

        Grid other_grid(2u);
        GridCell wrong(other_grid,0u,BinaryWord{});
        RobddPaving unchanged(existing);
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            existing.adjoin(std::vector<GridCell>{coarse,wrong}));
        ARIADNE_TESTING_PROVE(existing.robdd()==unchanged.robdd());
        ARIADNE_TESTING_PROVE(existing.root_cell()==unchanged.root_cell());
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            RobddPaving::from_cells(manager,smaller,{coarse}));
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            RobddPaving::from_cells(manager,wider,{wrong}));
        existing.adjoin(std::vector<GridCell>{});
        ARIADNE_TESTING_PROVE(existing.robdd()==unchanged.robdd());
    }

    Void test_in_place_mutation_and_value_semantics() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        GridCell left(grid,0u,make_binary_word("0"));
        GridCell right(grid,0u,make_binary_word("1"));
        RobddPaving paving(manager,root,manager.constant(false),0u);
        BinaryTreePaving explicit_tree(grid);

        paving.adjoin(left);
        explicit_tree.adjoin(left);
        explicit_tree.recombine();
        ARIADNE_TESTING_PROVE(paving.root_cell() == root);
        ARIADNE_TESTING_EXPECT_EQUAL(paving.depth(),1u);
        ARIADNE_TESTING_EXPECT_EQUAL(paving.size(),explicit_tree.size());
        ARIADNE_TESTING_PROVE(paving.is_enabled(make_binary_word("0")));
        ARIADNE_TESTING_PROVE(not paving.is_enabled(make_binary_word("1")));

        // The snapshot and an iterator must survive subsequent root updates.
        RobddPaving snapshot(paving);
        auto previous=paving.begin();
        auto previous_end=paving.end();
        ARIADNE_TESTING_PROVE(previous->word() == left.word());

        paving.adjoin(right);
        explicit_tree.adjoin(right);
        explicit_tree.recombine();
        ARIADNE_TESTING_PROVE(paving.robdd().is_true());
        ARIADNE_TESTING_EXPECT_EQUAL(paving.size(),explicit_tree.size());
        ARIADNE_TESTING_EXPECT_EQUAL(snapshot.size(),1u);
        ARIADNE_TESTING_PROVE(snapshot.is_enabled(make_binary_word("0")));
        ARIADNE_TESTING_PROVE(not snapshot.is_enabled(make_binary_word("1")));
        ARIADNE_TESTING_PROVE(previous->word() == left.word());
        ++previous;
        ARIADNE_TESTING_PROVE(previous == previous_end);

        paving.remove(left);
        explicit_tree.remove(left);
        explicit_tree.recombine();
        ARIADNE_TESTING_PROVE(not paving.is_enabled(make_binary_word("0")));
        ARIADNE_TESTING_PROVE(paving.is_enabled(make_binary_word("1")));
        ARIADNE_TESTING_EXPECT_EQUAL(paving.size(),explicit_tree.size());

        RobddPaving other(manager,root,manager.constant(false),0u);
        other.adjoin(left);
        paving.adjoin(other);
        ARIADNE_TESTING_PROVE(paving.robdd().is_true());
        paving.restrict(other);
        ARIADNE_TESTING_PROVE(paving.is_enabled(make_binary_word("0")));
        ARIADNE_TESTING_PROVE(not paving.is_enabled(make_binary_word("1")));
        ARIADNE_TESTING_PROVE(other.is_enabled(make_binary_word("0")));
        paving.adjoin(paving);
        paving.restrict(paving);
        ARIADNE_TESTING_PROVE(paving.robdd() == other.robdd());
        paving.remove(paving);
        ARIADNE_TESTING_PROVE(paving.is_empty());

        paving = snapshot;
        ARIADNE_TESTING_PROVE(paving.robdd() == snapshot.robdd());
        paving.clear();
        ARIADNE_TESTING_PROVE(paving.is_empty());
        ARIADNE_TESTING_PROVE(snapshot.is_enabled(make_binary_word("0")));

        paving.mince_to_depth(4u);
        paving.set_root_cell(true);
        ARIADNE_TESTING_PROVE(paving.robdd().is_true());
        ARIADNE_TESTING_EXPECT_EQUAL(paving.depth(),0u);
        ARIADNE_TESTING_EXPECT_EQUAL(paving.size(),1u);
        paving.mince_to_depth(3u);
        paving.set_root_cell(false);
        ARIADNE_TESTING_PROVE(paving.is_empty());
        ARIADNE_TESTING_EXPECT_EQUAL(paving.depth(),0u);
        ARIADNE_TESTING_PROVE(paving.root_cell() == root);
    }

    Void test_in_place_mutation_across_extents() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,1u,BinaryWord{});
        GridCell coarse(grid,1u,make_binary_word("0"));
        GridCell fine(grid,0u,make_binary_word("0"));
        RobddPaving paving(manager,root,manager.constant(false),0u);
        BinaryTreePaving oracle(grid);

        paving.adjoin(coarse);
        paving.adjoin(fine);
        oracle.adjoin(coarse);
        oracle.adjoin(fine);
        oracle.recombine();
        ARIADNE_TESTING_PROVE(paving.root_cell() == root);
        ARIADNE_TESTING_EXPECT_EQUAL(paving.depth(),2u);
        ARIADNE_TESTING_EXPECT_EQUAL(paving.enabled_cell_count(),3u);
        ARIADNE_TESTING_EXPECT_EQUAL(paving.size(),oracle.size());
        for (unsigned mask=0u; mask!=4u; ++mask) {
            BinaryWord path;
            path.push_back((mask & 2u) != 0u);
            path.push_back((mask & 1u) != 0u);
            GridCell cell(grid,1u,path);
            ARIADNE_TESTING_EXPECT_EQUAL(
                paving.is_enabled(path),oracle.superset(cell));
        }

        paving.remove(coarse);
        oracle.remove(coarse);
        oracle.recombine();
        ARIADNE_TESTING_EXPECT_EQUAL(paving.size(),oracle.size());
        ARIADNE_TESTING_PROVE(not paving.is_enabled(make_binary_word("00")));
        ARIADNE_TESTING_PROVE(not paving.is_enabled(make_binary_word("01")));
        ARIADNE_TESTING_PROVE(paving.is_enabled(make_binary_word("10")));

        RobddPaving saved(paving);
        RobddManager foreign;
        RobddPaving foreign_paving(
            foreign,root,foreign.constant(true),0u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(paving.adjoin(foreign_paving));
        ARIADNE_TESTING_EXPECT_EXCEPTION(paving.restrict(foreign_paving));
        ARIADNE_TESTING_EXPECT_EXCEPTION(paving.remove(foreign_paving));
        ARIADNE_TESTING_PROVE(paving.robdd() == saved.robdd());
        ARIADNE_TESTING_PROVE(paving.root_cell() == saved.root_cell());
        ARIADNE_TESTING_EXPECT_EQUAL(paving.depth(),saved.depth());

        Grid other_grid(2u);
        GridCell incompatible(other_grid,0u,BinaryWord{});
        ARIADNE_TESTING_EXPECT_EXCEPTION(paving.adjoin(incompatible));
        ARIADNE_TESTING_EXPECT_EXCEPTION(paving.remove(incompatible));
        ARIADNE_TESTING_PROVE(paving.robdd() == saved.robdd());
    }

    Void test_explicit_subpaving_import_and_backend_rejection() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        BinaryTreeNode explicit_tree(false);
        explicit_tree.add_enabled(make_binary_word("0"));
        explicit_tree.add_enabled(make_binary_word("110"));
        explicit_tree.recombine();
        GridTreeSubpaving source(grid,0u,BinaryWord{},&explicit_tree);
        SubPavingInterface const& generic=source;

        // Explicit conversion is allowed for external migration/testing.
        // It must never happen implicitly inside paving set operations.
        RobddPaving imported(manager,generic);
        RobddPaving direct(manager,source);
        ARIADNE_TESTING_PROVE(imported.root_cell()==root);
        ARIADNE_TESTING_PROVE(imported.subset(direct));
        ARIADNE_TESTING_PROVE(direct.subset(imported));
        ARIADNE_TESTING_PROVE(imported.superset(
            GridCell(grid,0u,make_binary_word("0"))));
        ARIADNE_TESTING_PROVE(imported.superset(
            GridCell(grid,0u,make_binary_word("110"))));
        ARIADNE_TESTING_PROVE(not imported.superset(
            GridCell(grid,0u,make_binary_word("111"))));

        RobddPaving unchanged(imported);
        ARIADNE_TESTING_EXPECT_EXCEPTION(imported.adjoin(generic));
        ARIADNE_TESTING_EXPECT_EXCEPTION(imported.restrict(generic));
        ARIADNE_TESTING_EXPECT_EXCEPTION(imported.remove(generic));
        ARIADNE_TESTING_EXPECT_EXCEPTION(imported.subset(generic));
        ARIADNE_TESTING_EXPECT_EXCEPTION(imported.intersects(generic));
        ARIADNE_TESTING_EXPECT_EXCEPTION(imported.equals(generic));
        ARIADNE_TESTING_EXPECT_EXCEPTION(source.subset(imported));
        ARIADNE_TESTING_EXPECT_EXCEPTION(source.intersects(imported));
        ARIADNE_TESTING_EXPECT_EXCEPTION(source.equals(imported));
        ARIADNE_TESTING_PROVE(imported.robdd()==unchanged.robdd());
        ARIADNE_TESTING_PROVE(imported.root_cell()==unchanged.root_cell());

        // Same-backend operations accept the interface and child views.
        SubPavingInterface const& own=imported;
        RobddPaving same(manager,root,manager.constant(false),0u);
        same.adjoin(own);
        ARIADNE_TESTING_PROVE(same.equals(own));
        ARIADNE_TESTING_PROVE(same.subset(own));
        ARIADNE_TESTING_PROVE(same.intersects(own));
        auto left=static_cast<SubPavingInterface const&>(imported).branch(false);
        SubPavingInterface const& left_interface=left;
        RobddPaving left_snapshot=imported.subpaving(root.split(false));
        ARIADNE_TESTING_PROVE(left_snapshot.equals(left_interface));
        RobddPaving union_left(manager,root,manager.constant(false),0u);
        union_left.adjoin(left_interface);
        ARIADNE_TESTING_PROVE(union_left.superset(root.split(false)));
        same.restrict(left_interface);
        ARIADNE_TESTING_PROVE(same.subset(left_interface));
        same.remove(left_interface);
        ARIADNE_TESTING_PROVE(same.is_empty());

        Grid other_grid(2u);
        BinaryTreeNode foreign_node(true);
        GridTreeSubpaving foreign(
            other_grid,0u,BinaryWord{},&foreign_node);
        SubPavingInterface const& foreign_interface=foreign;
        ARIADNE_TESTING_EXPECT_EXCEPTION(imported.adjoin(foreign_interface));
        ARIADNE_TESTING_EXPECT_EXCEPTION(imported.restrict(foreign_interface));
        ARIADNE_TESTING_EXPECT_EXCEPTION(imported.remove(foreign_interface));
        ARIADNE_TESTING_EXPECT_EXCEPTION(imported.subset(foreign_interface));
        ARIADNE_TESTING_EXPECT_EXCEPTION(imported.intersects(foreign_interface));
        ARIADNE_TESTING_EXPECT_EXCEPTION(imported.equals(foreign_interface));
        ARIADNE_TESTING_PROVE(imported.robdd()==unchanged.robdd());
    }

    Void test_native_branch_cofactors() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,make_binary_word("01"));
        auto x1=manager.variable(1u);
        auto x2=manager.variable(2u);
        // Covers skipped variable zero and a complemented Boolean root.
        RobddPaving paving(
            manager,root,
            manager.negation(manager.conjunction(x1,x2)),3u);
        for (Bool right : {false,true}) {
            auto child=paving.branch(right);
            ARIADNE_TESTING_PROVE(child.root_cell() == root.split(right));
            ARIADNE_TESTING_EXPECT_EQUAL(child.depth(),2u);
            for (unsigned mask=0u; mask!=4u; ++mask) {
                BinaryWord suffix;
                suffix.push_back((mask & 2u) != 0u);
                suffix.push_back((mask & 1u) != 0u);
                BinaryWord full;
                full.push_back(right);
                full.append(suffix);
                ARIADNE_TESTING_EXPECT_EQUAL(
                    child.is_enabled(suffix),paving.is_enabled(full));
            }
        }
        auto before=manager.node_count();
        auto left=paving.branch(false);
        auto right=paving.branch(true);
        ARIADNE_TESTING_PROVE(left.robdd() == right.robdd());
        ARIADNE_TESTING_PROVE(manager.node_count() >= before);

        RobddPaving full(manager,root,manager.constant(true),0u);
        RobddPaving empty(manager,root,manager.constant(false),0u);
        ARIADNE_TESTING_PROVE(full.branch(false).robdd().is_true());
        ARIADNE_TESTING_PROVE(full.branch(true).robdd().is_true());
        ARIADNE_TESTING_PROVE(empty.branch(false).is_empty());
        ARIADNE_TESTING_EXPECT_EQUAL(full.branch(false).depth(),0u);
        ARIADNE_TESTING_PROVE(
            full.branch(false).root_cell() == root.split(false));

        // Exhaustively compare cofactored branches at fixed depth.
        GridCell primary(grid,0u,BinaryWord{});
        for (unsigned table=0u; table<256u; ++table) {
            BinaryTreeNode oracle(false);
            for (unsigned mask=0u; mask<8u; ++mask) {
                if ((table & (1u<<mask)) == 0u) { continue; }
                BinaryWord path;
                path.push_back((mask & 4u) != 0u);
                path.push_back((mask & 2u) != 0u);
                path.push_back((mask & 1u) != 0u);
                oracle.add_enabled(path);
            }
            oracle.recombine();
            GridTreeSubpaving source(grid,0u,BinaryWord{},&oracle);
            RobddPaving encoded(manager,source);
            encoded.mince_to_depth(3u);
            for (Bool side : {false,true}) {
                auto branch=encoded.branch(side);
                ARIADNE_TESTING_PROVE(
                    branch.root_cell() == primary.split(side));
                ARIADNE_TESTING_EXPECT_EQUAL(branch.depth(),2u);
                for (unsigned mask=0u; mask<4u; ++mask) {
                    BinaryWord suffix;
                    suffix.push_back((mask & 2u) != 0u);
                    suffix.push_back((mask & 1u) != 0u);
                    BinaryWord full_path;
                    full_path.push_back(side);
                    full_path.append(suffix);
                    ARIADNE_TESTING_EXPECT_EQUAL(
                        branch.is_enabled(suffix),oracle.is_enabled(full_path));
                }
            }
        }
    }

    Void test_geometric_queries_between_backends() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        GridCell left(grid,0u,make_binary_word("0"));
        GridCell right(grid,0u,make_binary_word("1"));
        GridCell left_left(grid,0u,make_binary_word("00"));
        GridCell left_right(grid,0u,make_binary_word("01"));
        GridCell distant(grid,1u,make_binary_word("0"));

        // Include both interior intersections and boundary-only contact.
        std::vector<ExactBoxType> queries{
            root.box(), left.box(), right.box(), left_left.box(),
            left_right.box(), distant.box()
        };
        // Generate one common geometric input set; independently populate
        // each backend without importing or sharing the other backend's tree.
        std::vector<GridCell> cells;
        for (unsigned mask=0u; mask<8u; ++mask) {
            BinaryWord path;
            path.push_back((mask & 4u) != 0u);
            path.push_back((mask & 2u) != 0u);
            path.push_back((mask & 1u) != 0u);
            cells.emplace_back(grid,0u,path);
        }
        for (unsigned table=0u; table<256u; ++table) {
            BinaryTreePaving binary_tree(grid);
            RobddPaving robdd(manager,root,manager.constant(false),0u);
            for (unsigned mask=0u; mask<8u; ++mask) {
                if ((table & (1u << mask)) == 0u) { continue; }
                binary_tree.adjoin(cells[mask]);
                robdd.adjoin(cells[mask]);
            }
            binary_tree.recombine();
            robdd.mince_to_depth(3u);

            ARIADNE_TESTING_PROVE(robdd.root_cell() == binary_tree.root_cell());
            ARIADNE_TESTING_EXPECT_EQUAL(robdd.size(),binary_tree.size());
            for (auto const& cell : cells) {
                ARIADNE_TESTING_EXPECT_EQUAL(
                    robdd.is_enabled(cell.word()),binary_tree.superset(cell));
            }
            auto tree_it=binary_tree.begin(), tree_end=binary_tree.end();
            for (auto it=robdd.begin(), end=robdd.end(); it!=end; ++it) {
                ARIADNE_TESTING_PROVE(tree_it != tree_end);
                ARIADNE_TESTING_PROVE(it->word() == tree_it->word());
                ++tree_it;
            }
            ARIADNE_TESTING_PROVE(tree_it == tree_end);

            ARIADNE_TESTING_EXPECT_EQUAL(
                robdd.bounding_box().dimension(),
                binary_tree.bounding_box().dimension());
            if (table != 0u) {
                ARIADNE_TESTING_PROVE(same(
                    cast_exact_box(robdd.bounding_box()),
                    cast_exact_box(binary_tree.bounding_box())));
            }
            for (auto const& box : queries) {
                ARIADNE_TESTING_EXPECT_EQUAL(
                    definitely(robdd.inside(box)),
                    definitely(binary_tree.inside(box)));
                ARIADNE_TESTING_EXPECT_EQUAL(
                    definitely(robdd.separated(box)),
                    definitely(binary_tree.separated(box)));
                ARIADNE_TESTING_EXPECT_EQUAL(
                    definitely(robdd.overlaps(box)),
                    definitely(binary_tree.overlaps(box)));
            }
        }

        // The geometry is unchanged by changing only logical depth.
        RobddPaving all(manager,root,manager.constant(true),0u);
        auto full_bounds=all.bounding_box();
        all.mince_to_depth(7u);
        ARIADNE_TESTING_PROVE(same(
            cast_exact_box(all.bounding_box()),cast_exact_box(full_bounds)));
        ARIADNE_TESTING_EXPECT_EQUAL(all.size(),1u);

        RobddPaving empty(manager,root,manager.constant(false),0u);
        ARIADNE_TESTING_EXPECT_EQUAL(empty.bounding_box().dimension(),1u);
        ARIADNE_TESTING_PROVE(definitely(empty.inside(root.box())));
        ARIADNE_TESTING_PROVE(definitely(empty.separated(root.box())));
        ARIADNE_TESTING_PROVE(not definitely(empty.overlaps(root.box())));

        Grid two_dimensional(2u);
        GridCell wrong_root(two_dimensional,0u,BinaryWord{});
        ARIADNE_TESTING_EXPECT_EXCEPTION(all.inside(wrong_root.box()));
        ARIADNE_TESTING_EXPECT_EXCEPTION(all.separated(wrong_root.box()));
        ARIADNE_TESTING_EXPECT_EXCEPTION(all.overlaps(wrong_root.box()));
    }

    Void test_grid_relative_mince_and_primary_cell_query() {
        RobddManager manager;
        for (Nat dimensions : {1u,2u,3u}) {
            Grid grid(dimensions);
            for (Nat extent : {0u,1u,3u}) {
                for (BinaryWord const& prefix : {
                         BinaryWord{},make_binary_word("0"),
                         make_binary_word("101")}) {
                    GridCell root(grid,extent,prefix);
                    RobddPaving robdd(
                        manager,root,manager.constant(true),0u);
                    BinaryTreeNode reference_node(true);
                    GridTreeSubpaving reference(
                        grid,extent,prefix,&reference_node);

                    auto geometry_before=robdd.bounding_box();
                    for (Nat fineness : {0u,1u,2u,4u}) {
                        robdd.mince(fineness);
                        SizeType requested=
                            (static_cast<SizeType>(extent)+fineness)*
                            static_cast<SizeType>(dimensions);
                        SizeType desired=requested>prefix.size()
                            ? requested-prefix.size() : 0u;
                        ARIADNE_TESTING_EXPECT_EQUAL(
                            robdd.depth(),desired);
                        // Materialize only small reference subdivisions:
                        // the largest combinations would create millions
                        // of redundant explicit-tree nodes.
                        if (desired <= 5u) {
                            reference.mince(fineness);
                            ARIADNE_TESTING_EXPECT_EQUAL(
                                reference.tree_depth(),desired);
                        }
                        ARIADNE_TESTING_PROVE(robdd.robdd().is_true());
                        ARIADNE_TESTING_EXPECT_EQUAL(robdd.size(),1u);
                        ARIADNE_TESTING_PROVE(same(
                            cast_exact_box(robdd.bounding_box()),
                            cast_exact_box(geometry_before)));
                        ARIADNE_TESTING_PROVE(
                            robdd.root_cell() == reference.root_cell());
                        ARIADNE_TESTING_PROVE(
                            definitely(reference.inside(root.box())) ==
                            definitely(robdd.inside(root.box())));
                    }
                    GridCell expected_primary=
                        GridCell::smallest_enclosing_primary_cell(
                            geometry_before,grid);
                    ARIADNE_TESTING_PROVE(
                        robdd.smallest_enclosing_primary_cell(
                            geometry_before) == expected_primary);
                }
            }
        }
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        RobddPaving empty(
            manager,root,manager.constant(false),0u);
        empty.mince(4u);
        ARIADNE_TESTING_EXPECT_EQUAL(empty.depth(),4u);
        ARIADNE_TESTING_PROVE(empty.is_empty());
        ARIADNE_TESTING_EXPECT_EQUAL(empty.size(),0u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            empty.smallest_enclosing_primary_cell(
                GridCell(Grid(2u),0u,BinaryWord{}).box()));
    }

    Void test_generic_cell_updates_across_backends() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        BinaryTreePaving explicit_paving(grid);
        PavingInterface& generic_explicit=explicit_paving;
        RobddPaving robdd(manager,root,manager.constant(false),0u);

        for (BinaryWord const& word : {
                 make_binary_word("010"),make_binary_word("011"),
                 make_binary_word("101"),make_binary_word("111")}) {
            GridCell cell(grid,0u,word);
            set_paving_cell(generic_explicit,cell,true);
            set_paving_cell(robdd,cell,true);
        }
        explicit_paving.recombine();
        ARIADNE_TESTING_PROVE(
            robdd.root_cell() == explicit_paving.root_cell());
        ARIADNE_TESTING_EXPECT_EQUAL(
            robdd.size(),explicit_paving.size());
        for (unsigned mask=0u; mask<8u; ++mask) {
            BinaryWord word;
            word.push_back((mask & 4u)!=0u);
            word.push_back((mask & 2u)!=0u);
            word.push_back((mask & 1u)!=0u);
            GridCell cell(grid,0u,word);
            ARIADNE_TESTING_EXPECT_EQUAL(
                robdd.superset(cell),explicit_paving.superset(cell));
        }

        GridCell remove_cell(grid,0u,make_binary_word("011"));
        set_paving_cell(generic_explicit,remove_cell,false);
        set_paving_cell(robdd,remove_cell,false);
        explicit_paving.recombine();
        ARIADNE_TESTING_EXPECT_EQUAL(
            robdd.size(),explicit_paving.size());
        ARIADNE_TESTING_PROVE(not robdd.superset(remove_cell));
        ARIADNE_TESTING_PROVE(not explicit_paving.superset(remove_cell));

        // Both backends reject cells outside their root, without mutation.
        GridCell outside(grid,1u,make_binary_word("0"));
        RobddPaving saved(robdd);
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            set_paving_cell(robdd,outside,true));
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            set_paving_cell(generic_explicit,outside,true));
        ARIADNE_TESTING_PROVE(robdd.robdd() == saved.robdd());
        ARIADNE_TESTING_PROVE(robdd.root_cell() == saved.root_cell());
        Grid other_grid(2u);
        GridCell incompatible(other_grid,0u,BinaryWord{});
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            set_paving_cell(robdd,incompatible,false));
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            set_paving_cell(generic_explicit,incompatible,false));
    }

    Void test_common_view_enabled_leaf_traversal() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        for (unsigned table=0u; table<256u; ++table) {
            BinaryTreePaving tree(grid);
            PavingInterface& tree_interface=tree;
            RobddPaving robdd(manager,root,manager.constant(false),0u);
            for (unsigned mask=0u; mask<8u; ++mask) {
                if ((table & (1u << mask)) == 0u) { continue; }
                BinaryWord word;
                word.push_back((mask & 4u) != 0u);
                word.push_back((mask & 2u) != 0u);
                word.push_back((mask & 1u) != 0u);
                GridCell cell(grid,0u,word);
                tree.adjoin(cell);
                robdd.adjoin(cell);
            }
            tree.recombine();
            for (BinaryWord const& word : {
                     BinaryWord{},make_binary_word("0"),
                     make_binary_word("1"),make_binary_word("00"),
                     make_binary_word("01"),make_binary_word("10"),
                     make_binary_word("11"),make_binary_word("101")}) {
                GridCell region(grid,0u,word);
                auto tree_view=paving_cell_view(tree_interface,region);
                auto robdd_view=paving_cell_view(robdd,region);
                std::vector<GridCell> tree_cells;
                std::vector<GridCell> robdd_cells;
                tree_view.for_each_enabled_leaf([&](GridCell const& cell) {
                    tree_cells.push_back(cell);
                });
                robdd_view.for_each_enabled_leaf([&](GridCell const& cell) {
                    robdd_cells.push_back(cell);
                });
                ARIADNE_TESTING_EXPECT_EQUAL(tree_view.size(),tree_cells.size());
                ARIADNE_TESTING_EXPECT_EQUAL(robdd_view.size(),robdd_cells.size());
                ARIADNE_TESTING_EXPECT_EQUAL(tree_cells.size(),robdd_cells.size());
                for (SizeType i=0u; i<tree_cells.size(); ++i) {
                    ARIADNE_TESTING_PROVE(tree_cells[i] == robdd_cells[i]);
                    ARIADNE_TESTING_PROVE(tree_cells[i].box().subset(region.box()));
                }
            }
        }
        // A single enabled parent leaf must be clipped to the view root.
        Grid grid2(1u);
        GridCell root2(grid2,0u,BinaryWord{});
        BinaryTreePaving tree(grid2,true);
        RobddPaving robdd(manager,root2,manager.constant(true),0u);
        GridCell deep(grid2,0u,make_binary_word("101"));
        auto tree_view=paving_cell_view(tree,deep);
        auto robdd_view=paving_cell_view(robdd,deep);
        ARIADNE_TESTING_EXPECT_EQUAL(tree_view.size(),1u);
        ARIADNE_TESTING_EXPECT_EQUAL(robdd_view.size(),1u);
        tree_view.for_each_enabled_leaf([&](GridCell const& cell) {
            ARIADNE_TESTING_PROVE(cell==deep);
        });
        robdd_view.for_each_enabled_leaf([&](GridCell const& cell) {
            ARIADNE_TESTING_PROVE(cell==deep);
        });
        // A view reflects subsequent changes in the owning paving.
        tree.remove(deep);
        robdd.remove(deep);
        ARIADNE_TESTING_EXPECT_EQUAL(tree_view.size(),0u);
        ARIADNE_TESTING_EXPECT_EQUAL(robdd_view.size(),0u);
    }

    Void test_common_view_geometric_queries() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        GridCell left(grid,0u,make_binary_word("0"));
        GridCell right(grid,0u,make_binary_word("1"));
        GridCell left_left(grid,0u,make_binary_word("00"));
        GridCell left_right(grid,0u,make_binary_word("01"));
        GridCell far_cell(grid,1u,make_binary_word("0"));
        std::vector<ExactBoxType> queries{
            root.box(),left.box(),right.box(),left_left.box(),
            left_right.box(),far_cell.box()
        };
        for (unsigned table=0u; table<256u; ++table) {
            BinaryTreePaving tree(grid);
            PavingInterface& tree_interface=tree;
            RobddPaving robdd(manager,root,manager.constant(false),0u);
            for (unsigned mask=0u; mask<8u; ++mask) {
                if ((table & (1u<<mask))==0u) { continue; }
                BinaryWord word;
                word.push_back((mask & 4u)!=0u);
                word.push_back((mask & 2u)!=0u);
                word.push_back((mask & 1u)!=0u);
                GridCell enabled(grid,0u,word);
                tree.adjoin(enabled);
                robdd.adjoin(enabled);
            }
            tree.recombine();
            for (BinaryWord const& prefix : {
                     BinaryWord{},make_binary_word("0"),
                     make_binary_word("1"),make_binary_word("00"),
                     make_binary_word("01"),make_binary_word("10"),
                     make_binary_word("11"),make_binary_word("101")}) {
                GridCell region(grid,0u,prefix);
                auto tree_view=paving_cell_view(tree_interface,region);
                auto robdd_view=paving_cell_view(robdd,region);
                ARIADNE_TESTING_EXPECT_EQUAL(
                    tree_view.bounding_box().dimension(),
                    robdd_view.bounding_box().dimension());
                if (tree_view.size()!=0u) {
                    ARIADNE_TESTING_PROVE(same(
                        cast_exact_box(tree_view.bounding_box()),
                        cast_exact_box(robdd_view.bounding_box())));
                }
                for (auto const& query : queries) {
                    ARIADNE_TESTING_EXPECT_EQUAL(
                        definitely(tree_view.inside(query)),
                        definitely(robdd_view.inside(query)));
                    ARIADNE_TESTING_EXPECT_EQUAL(
                        definitely(tree_view.separated(query)),
                        definitely(robdd_view.separated(query)));
                    ARIADNE_TESTING_EXPECT_EQUAL(
                        definitely(tree_view.overlaps(query)),
                        definitely(robdd_view.overlaps(query)));
                }
            }
        }
        // A single enabled parent leaf must be clipped to the view root.
        BinaryTreePaving full_tree(grid,true);
        RobddPaving full_robdd(manager,root,manager.constant(true),0u);
        GridCell descendant(grid,0u,make_binary_word("101"));
        auto tree_view=paving_cell_view(full_tree,descendant);
        auto robdd_view=paving_cell_view(full_robdd,descendant);
        ARIADNE_TESTING_PROVE(same(
            cast_exact_box(tree_view.bounding_box()),
            descendant.box()));
        ARIADNE_TESTING_PROVE(same(
            cast_exact_box(robdd_view.bounding_box()),
            descendant.box()));
        ARIADNE_TESTING_PROVE(definitely(tree_view.inside(root.box())));
        ARIADNE_TESTING_PROVE(definitely(robdd_view.inside(root.box())));
        ARIADNE_TESTING_PROVE(definitely(tree_view.overlaps(descendant.box())));
        ARIADNE_TESTING_PROVE(definitely(robdd_view.overlaps(descendant.box())));

        full_tree.remove(descendant);
        full_robdd.remove(descendant);
        ARIADNE_TESTING_EXPECT_EQUAL(tree_view.size(),0u);
        ARIADNE_TESTING_EXPECT_EQUAL(robdd_view.size(),0u);
        ARIADNE_TESTING_EXPECT_EQUAL(tree_view.bounding_box().dimension(),1u);
        ARIADNE_TESTING_EXPECT_EQUAL(robdd_view.bounding_box().dimension(),1u);
        ARIADNE_TESTING_PROVE(definitely(tree_view.inside(descendant.box())));
        ARIADNE_TESTING_PROVE(definitely(robdd_view.separated(descendant.box())));
        ARIADNE_TESTING_PROVE(not definitely(robdd_view.overlaps(descendant.box())));

        Grid another_grid(2u);
        GridCell wrong_dimension(another_grid,0u,BinaryWord{});
        ARIADNE_TESTING_EXPECT_EXCEPTION(tree_view.inside(wrong_dimension.box()));
        ARIADNE_TESTING_EXPECT_EXCEPTION(robdd_view.inside(wrong_dimension.box()));
        ARIADNE_TESTING_EXPECT_EXCEPTION(tree_view.separated(wrong_dimension.box()));
        ARIADNE_TESTING_EXPECT_EXCEPTION(robdd_view.separated(wrong_dimension.box()));
        ARIADNE_TESTING_EXPECT_EXCEPTION(tree_view.overlaps(wrong_dimension.box()));
        ARIADNE_TESTING_EXPECT_EXCEPTION(robdd_view.overlaps(wrong_dimension.box()));
    }

    Void test_common_view_forward_iterator() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        for (unsigned table=0u; table<256u; ++table) {
            BinaryTreePaving tree(grid);
            PavingInterface& tree_interface=tree;
            RobddPaving robdd(manager,root,manager.constant(false),0u);
            for (unsigned mask=0u; mask<8u; ++mask) {
                if ((table & (1u<<mask))==0u) { continue; }
                BinaryWord word;
                word.push_back((mask & 4u)!=0u);
                word.push_back((mask & 2u)!=0u);
                word.push_back((mask & 1u)!=0u);
                GridCell cell(grid,0u,word);
                tree.adjoin(cell);
                robdd.adjoin(cell);
            }
            tree.recombine();
            for (BinaryWord const& prefix : {
                     BinaryWord{},make_binary_word("0"),
                     make_binary_word("1"),make_binary_word("00"),
                     make_binary_word("01"),make_binary_word("10"),
                     make_binary_word("11"),make_binary_word("101")}) {
                GridCell region(grid,0u,prefix);
                auto tree_view=paving_cell_view(tree_interface,region);
                auto robdd_view=paving_cell_view(robdd,region);
                std::vector<GridCell> tree_cells;
                std::vector<GridCell> robdd_cells;
                auto tree_it=tree_view.begin(),tree_end=tree_view.end();
                auto robdd_it=robdd_view.begin(),robdd_end=robdd_view.end();
                while (tree_it != tree_end and robdd_it != robdd_end) {
                    ARIADNE_TESTING_PROVE(*tree_it==*robdd_it);
                    tree_cells.push_back(*tree_it);
                    robdd_cells.push_back(*robdd_it);
                    auto tree_saved=tree_it++;
                    auto robdd_saved=robdd_it++;
                    ARIADNE_TESTING_PROVE(*tree_saved==tree_cells.back());
                    ARIADNE_TESTING_PROVE(*robdd_saved==robdd_cells.back());
                }
                ARIADNE_TESTING_PROVE(tree_it==tree_end);
                ARIADNE_TESTING_PROVE(robdd_it==robdd_end);
                ARIADNE_TESTING_EXPECT_EQUAL(tree_cells.size(),tree_view.size());
                ARIADNE_TESTING_EXPECT_EQUAL(robdd_cells.size(),robdd_view.size());
                auto empty_tree_end=tree_view.end();
                auto empty_robdd_end=robdd_view.end();
                ARIADNE_TESTING_PROVE(empty_tree_end==tree_end);
                ARIADNE_TESTING_PROVE(empty_robdd_end==robdd_end);
            }
        }

        // Iterators own their cell snapshots even after the owner mutates.
        BinaryTreePaving tree(grid,true);
        RobddPaving robdd(manager,root,manager.constant(true),0u);
        GridCell region(grid,0u,make_binary_word("10"));
        auto tree_view=paving_cell_view(tree,region);
        auto robdd_view=paving_cell_view(robdd,region);
        auto tree_old=tree_view.begin(),robdd_old=robdd_view.begin();
        auto tree_old_copy=tree_old,robdd_old_copy=robdd_old;
        ARIADNE_TESTING_PROVE(*tree_old==region);
        ARIADNE_TESTING_PROVE(*robdd_old==region);
        tree.remove(region);
        robdd.remove(region);
        ARIADNE_TESTING_PROVE(*tree_old==region);
        ARIADNE_TESTING_PROVE(*robdd_old==region);
        ARIADNE_TESTING_PROVE(tree_old==tree_old_copy);
        ARIADNE_TESTING_PROVE(robdd_old==robdd_old_copy);
        ARIADNE_TESTING_PROVE(tree_view.begin()==tree_view.end());
        ARIADNE_TESTING_PROVE(robdd_view.begin()==robdd_view.end());
        ++tree_old;
        ++robdd_old;
        ARIADNE_TESTING_PROVE(tree_old==tree_view.end());
        ARIADNE_TESTING_PROVE(robdd_old==robdd_view.end());
    }

    Void test_outer_box_approximation_between_backends() {
        RobddManager manager;
        for (Nat dimensions : {1u,2u}) {
            Grid grid(dimensions);
            GridCell root(grid,0u,BinaryWord{});
            GridCell left=root.split(false);
            GridCell right=root.split(true);
            std::vector<UpperBoxType> boxes{
                root.box(),left.box(),right.box(),
                left.split(false).box(),right.split(true).box()
            };
            for (Nat fineness : {0u,1u,2u}) {
                for (SizeType index=0u; index<boxes.size(); ++index) {
                    BinaryTreePaving tree(grid);
                    RobddPaving robdd(
                        manager,root,manager.constant(false),0u);
                    tree.adjoin_outer_approximation(boxes[index],fineness);
                    robdd.adjoin_outer_approximation(boxes[index],fineness);
                    tree.recombine();
                    ARIADNE_TESTING_EXPECT_EQUAL(
                        tree.size(),robdd.size());
                    for (auto it=tree.begin(), end=tree.end(); it!=end; ++it) {
                        ARIADNE_TESTING_PROVE(robdd.superset(*it));
                    }
                    for (auto it=robdd.begin(), end=robdd.end(); it!=end; ++it) {
                        ARIADNE_TESTING_PROVE(tree.superset(*it));
                    }
                    ARIADNE_TESTING_PROVE(same(
                        cast_exact_box(tree.bounding_box()),
                        cast_exact_box(robdd.bounding_box())));
                    // Repeating the approximation is set-idempotent.
                    RobddPaving snapshot(robdd);
                    robdd.adjoin_outer_approximation(boxes[index],fineness);
                    ARIADNE_TESTING_PROVE(robdd.subset(snapshot));
                    ARIADNE_TESTING_PROVE(snapshot.subset(robdd));
                }
            }
        }
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        RobddPaving robdd(manager,root,manager.constant(false),0u);
        Grid incompatible(2u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            robdd.adjoin_outer_approximation(
                GridCell(incompatible,0u,BinaryWord{}).box(),1u));
        ARIADNE_TESTING_PROVE(robdd.is_empty());
    }

    Void test_generic_approximations_between_backends() {
        RobddManager manager;
        for (Nat dimensions : {1u,2u}) {
            Grid grid(dimensions);
            GridCell root(grid,0u,BinaryWord{});
            GridCell left=root.split(false);
            GridCell right=root.split(true);
            std::vector<ExactBoxType> queries{
                root.box(),left.box(),right.box(),
                left.split(false).box(),right.split(true).box()
            };
            for (Nat extent : {0u,1u,2u}) {
                for (Nat fineness : {0u,1u,2u}) {
                    for (ExactBoxType const& box : queries) {
                        ExactBoxSetType set=box;
                        EffectiveEuclideanOpenSetInterface const& open_set=set;
                        EffectiveEuclideanSetInterface const& generic_set=set;
                        EffectiveEuclideanCompactSetInterface const& compact_set=set;

                        BinaryTreePaving tree(grid);
                        RobddPaving robdd(
                            manager,root,manager.constant(false),0u);
                        tree.adjoin_inner_approximation(
                            open_set,extent,fineness);
                        robdd.adjoin_inner_approximation(
                            open_set,extent,fineness);
                        tree.recombine();
                        for (auto it=tree.begin(), end=tree.end();
                                it!=end; ++it) {
                            ARIADNE_TESTING_PROVE(robdd.superset(*it));
                        }
                        for (auto it=robdd.begin(), end=robdd.end();
                                it!=end; ++it) {
                            ARIADNE_TESTING_PROVE(tree.superset(*it));
                        }
                        ARIADNE_TESTING_EXPECT_EQUAL(tree.size(),robdd.size());

                        // Exercise the two-argument open-set overload as
                        // a separate construction, not an imported tree.
                        BinaryTreePaving inferred_tree(grid);
                        RobddPaving inferred_robdd(
                            manager,root,manager.constant(false),0u);
                        inferred_tree.adjoin_inner_approximation(
                            generic_set,fineness);
                        inferred_robdd.adjoin_inner_approximation(
                            generic_set,fineness);
                        inferred_tree.recombine();
                        for (auto it=inferred_tree.begin(),
                                end=inferred_tree.end(); it!=end; ++it) {
                            ARIADNE_TESTING_PROVE(
                                inferred_robdd.superset(*it));
                        }
                        for (auto it=inferred_robdd.begin(),
                                end=inferred_robdd.end(); it!=end; ++it) {
                            ARIADNE_TESTING_PROVE(
                                inferred_tree.superset(*it));
                        }
                        ARIADNE_TESTING_EXPECT_EQUAL(
                            inferred_tree.size(),inferred_robdd.size());

                        // The general compact-set overload has identical
                        // semantics to the existing box overload.
                        BinaryTreePaving outer_tree(grid);
                        RobddPaving outer_robdd(
                            manager,root,manager.constant(false),0u);
                        outer_tree.adjoin_outer_approximation(
                            compact_set,fineness);
                        outer_robdd.adjoin_outer_approximation(
                            compact_set,fineness);
                        outer_tree.recombine();
                        for (auto it=outer_tree.begin(),
                                end=outer_tree.end(); it!=end; ++it) {
                            ARIADNE_TESTING_PROVE(outer_robdd.superset(*it));
                        }
                        for (auto it=outer_robdd.begin(),
                                end=outer_robdd.end(); it!=end; ++it) {
                            ARIADNE_TESTING_PROVE(outer_tree.superset(*it));
                        }
                        ARIADNE_TESTING_EXPECT_EQUAL(
                            outer_tree.size(),outer_robdd.size());

                        // Re-adjoining is geometrically idempotent.
                        RobddPaving preserved(robdd);
                        robdd.adjoin_inner_approximation(
                            open_set,extent,fineness);
                        ARIADNE_TESTING_PROVE(robdd.subset(preserved));
                        ARIADNE_TESTING_PROVE(preserved.subset(robdd));
                    }
                }
            }
        }

        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        RobddPaving empty(manager,root,manager.constant(false),0u);
        ExactBoxSetType wrong_dimension=
            GridCell(Grid(2u),0u,BinaryWord{}).box();
        EffectiveEuclideanCompactSetInterface const& compact=wrong_dimension;
        EffectiveEuclideanOpenSetInterface const& open=wrong_dimension;
        EffectiveEuclideanSetInterface const& generic=wrong_dimension;
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            empty.adjoin_outer_approximation(compact,1u));
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            empty.adjoin_inner_approximation(open,0u,1u));
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            empty.adjoin_inner_approximation(generic,1u));
        ARIADNE_TESTING_PROVE(empty.is_empty());
    }

    Void test_polymorphic_pure_algebra_with_isolated_backends() {
        RobddManager manager;
        for (Nat dimensions : {1u,2u}) {
            Grid grid(dimensions);
            GridCell root(grid,0u,BinaryWord{});
            std::vector<GridCell> cells;
            for (unsigned bit=0u; bit<8u; ++bit) {
                BinaryWord word;
                word.push_back((bit & 4u)!=0u);
                word.push_back((bit & 2u)!=0u);
                word.push_back((bit & 1u)!=0u);
                cells.emplace_back(grid,0u,word);
            }

            auto check_mask=[&](PavingHandle const& tree,
                                PavingHandle const& robdd,
                                unsigned expected) {
                for (unsigned bit=0u; bit<8u; ++bit) {
                    Bool enabled=(expected & (1u<<bit))!=0u;
                    ARIADNE_TESTING_EXPECT_EQUAL(
                        tree.superset(cells[bit]),enabled);
                    ARIADNE_TESTING_EXPECT_EQUAL(
                        robdd.superset(cells[bit]),enabled);
                }
            };

            for (unsigned lhs : {0u,1u,0x0Fu,0x33u,
                                 0x5Au,0xA5u,0xF0u,0xFFu}) {
                for (unsigned rhs : {0u,0x0Fu,0x55u,0xA6u,0xFFu}) {
                    PavingHandle tree_left(new BinaryTreePaving(grid));
                    PavingHandle tree_right(new BinaryTreePaving(grid));
                    PavingHandle robdd_left(new RobddPaving(
                        manager,root,manager.constant(false),0u));
                    PavingHandle robdd_right(new RobddPaving(
                        manager,root,manager.constant(false),0u));

                    for (unsigned bit=0u; bit<8u; ++bit) {
                        if ((lhs & (1u<<bit))!=0u) {
                            tree_left.adjoin(cells[bit]);
                            robdd_left.adjoin(cells[bit]);
                        }
                        if ((rhs & (1u<<bit))!=0u) {
                            tree_right.adjoin(cells[bit]);
                            robdd_right.adjoin(cells[bit]);
                        }
                    }
                    PavingInterface const& tl=tree_left;
                    PavingInterface const& tr=tree_right;
                    PavingInterface const& rl=robdd_left;
                    PavingInterface const& rr=robdd_right;

                    // Free operations preserve backend identity: neither
                    // operand comes from another backend or is converted.
                    PavingHandle tree_join=Ariadne::join(tl,tr);
                    PavingHandle robdd_join=Ariadne::join(rl,rr);
                    PavingHandle tree_union=Ariadne::geometric_union(tl,tr);
                    PavingHandle robdd_union=Ariadne::geometric_union(rl,rr);
                    PavingHandle tree_intersection=Ariadne::intersection(tl,tr);
                    PavingHandle robdd_intersection=Ariadne::intersection(rl,rr);
                    PavingHandle tree_difference=Ariadne::difference(tl,tr);
                    PavingHandle robdd_difference=Ariadne::difference(rl,rr);

                    check_mask(tree_join,robdd_join,lhs|rhs);
                    check_mask(tree_union,robdd_union,lhs|rhs);
                    check_mask(tree_intersection,robdd_intersection,lhs&rhs);
                    check_mask(tree_difference,robdd_difference,lhs & ~rhs);
                    check_mask(tree_left,robdd_left,lhs);
                    check_mask(tree_right,robdd_right,rhs);

                    // Every result has independent writable state.
                    tree_join.remove(cells[0]);
                    robdd_join.remove(cells[0]);
                    check_mask(tree_left,robdd_left,lhs);
                    check_mask(tree_right,robdd_right,rhs);
                    check_mask(tree_union,robdd_union,lhs|rhs);
                }
            }

            // A child view is a valid *same-backend* operand. Its clone
            // must be detached even when the owner root is a true leaf.
            BinaryTreePaving full_tree(grid,true);
            RobddPaving full_robdd(
                manager,root,manager.constant(true),0u);
            GridCell left=root.split(false);
            GridCell right=root.split(true);
            {
                SubPavingHandle tree_view=
                    static_cast<SubPavingInterface const&>(full_tree)
                        .branch(false);
                SubPavingHandle robdd_view=
                    static_cast<SubPavingInterface const&>(full_robdd)
                        .branch(false);
                SubPavingInterface const& tv=tree_view;
                SubPavingInterface const& rv=robdd_view;
                PavingInterface const& ft=full_tree;
                PavingInterface const& fr=full_robdd;

                PavingHandle tree_union=Ariadne::join(tv,ft);
                PavingHandle robdd_union=Ariadne::join(rv,fr);
                PavingHandle tree_intersection=Ariadne::intersection(ft,tv);
                PavingHandle robdd_intersection=Ariadne::intersection(fr,rv);
                PavingHandle tree_difference=Ariadne::difference(ft,tv);
                PavingHandle robdd_difference=Ariadne::difference(fr,rv);
                ARIADNE_TESTING_PROVE(tree_union.superset(root));
                ARIADNE_TESTING_PROVE(robdd_union.superset(root));
                ARIADNE_TESTING_PROVE(
                    tree_intersection.superset(left));
                ARIADNE_TESTING_PROVE(
                    robdd_intersection.superset(left));
                ARIADNE_TESTING_PROVE(
                    not tree_intersection.superset(right));
                ARIADNE_TESTING_PROVE(
                    not robdd_intersection.superset(right));
                ARIADNE_TESTING_PROVE(tree_difference.superset(right));
                ARIADNE_TESTING_PROVE(robdd_difference.superset(right));
                ARIADNE_TESTING_PROVE(
                    not tree_difference.superset(left));
                ARIADNE_TESTING_PROVE(
                    not robdd_difference.superset(left));

                tree_intersection.set_root_cell(false);
                robdd_intersection.set_root_cell(false);
                ARIADNE_TESTING_PROVE(full_tree.superset(root));
                ARIADNE_TESTING_PROVE(full_robdd.superset(root));
                ARIADNE_TESTING_PROVE(tree_view.superset(left));
                ARIADNE_TESTING_PROVE(robdd_view.superset(left));

                // Same-backend operations may accept a view, but mixed
                // backend operations must fail before changing the owner.
                ARIADNE_TESTING_EXPECT_EXCEPTION(Ariadne::join(ft,fr));
                ARIADNE_TESTING_EXPECT_EXCEPTION(Ariadne::join(fr,ft));
                ARIADNE_TESTING_EXPECT_EXCEPTION(
                    Ariadne::geometric_union(ft,fr));
                ARIADNE_TESTING_EXPECT_EXCEPTION(
                    Ariadne::intersection(ft,fr));
                ARIADNE_TESTING_EXPECT_EXCEPTION(
                    Ariadne::difference(fr,ft));
                ARIADNE_TESTING_EXPECT_EXCEPTION(Ariadne::join(tv,fr));
                ARIADNE_TESTING_EXPECT_EXCEPTION(
                    Ariadne::intersection(rv,ft));
                ARIADNE_TESTING_PROVE(full_tree.superset(root));
                ARIADNE_TESTING_PROVE(full_robdd.superset(root));
            }
        }
    }

    Void test_polymorphic_alignment_across_primary_extents() {
        RobddManager manager;
        for (Nat dimensions : {1u,2u,3u}) {
            Grid grid(dimensions);
            for (Nat extent : {1u,2u}) {
                GridCell root(grid,extent,BinaryWord{});
                GridCell left=root.split(false);
                GridCell right=root.split(true);
                GridCell right_left=right.split(false);
                GridCell fine(grid,0u,make_binary_word("101"));

                // Both backends start at the same nonzero primary extent.
                // Lower-extent updates must be aligned without importing
                // the other backend's internal representation.
                PavingHandle tree_handle(new BinaryTreePaving(
                    grid,extent,new BinaryTreeNode(false)));
                PavingHandle robdd_handle(new RobddPaving(
                    manager,root,manager.constant(false),0u));
                PavingInterface& tree=tree_handle;
                PavingInterface& robdd=robdd_handle;

                auto check_observations=[&]() {
                    ARIADNE_TESTING_PROVE(tree.grid()==robdd.grid());
                    ARIADNE_TESTING_EXPECT_EQUAL(tree.size(),robdd.size());
                    ARIADNE_TESTING_EXPECT_EQUAL(
                        tree.bounding_box().dimension(),
                        robdd.bounding_box().dimension());
                    if (tree.size()!=0u) {
                        ARIADNE_TESTING_PROVE(same(
                            cast_exact_box(tree.bounding_box()),
                            cast_exact_box(robdd.bounding_box())));
                    }
                    // Only one-way membership queries with geometric cells:
                    // no backend participates as the other's set operand.
                    for (auto it=tree.begin(), end=tree.end(); it!=end; ++it) {
                        ARIADNE_TESTING_PROVE(robdd.superset(*it));
                    }
                    for (auto it=robdd.begin(), end=robdd.end(); it!=end; ++it) {
                        ARIADNE_TESTING_PROVE(tree.superset(*it));
                    }
                    for (unsigned mask=0u; mask<8u; ++mask) {
                        BinaryWord word;
                        word.push_back((mask & 4u)!=0u);
                        word.push_back((mask & 2u)!=0u);
                        word.push_back((mask & 1u)!=0u);
                        for (Nat cell_extent : {Nat(0u),extent}) {
                            GridCell probe(grid,cell_extent,word);
                            ARIADNE_TESTING_EXPECT_EQUAL(
                                tree.superset(probe),robdd.superset(probe));
                        }
                    }
                };

                tree_handle.adjoin(left);
                robdd_handle.adjoin(left);
                tree_handle.adjoin(right_left);
                robdd_handle.adjoin(right_left);
                tree_handle.adjoin(fine);
                robdd_handle.adjoin(fine);
                tree.recombine();
                robdd.recombine();
                check_observations();

                PavingHandle saved_tree(tree_handle);
                PavingHandle saved_robdd(robdd_handle);

                // Both child views remain live; their handle copies are
                // detached snapshots of the selected geometric region.
                {
                    SubPavingHandle tree_left=tree.branch(false);
                    SubPavingHandle robdd_left=robdd.branch(false);
                    SubPavingHandle tree_snapshot(tree_left);
                    SubPavingHandle robdd_snapshot(robdd_left);
                    ARIADNE_TESTING_PROVE(tree_left.superset(left));
                    ARIADNE_TESTING_PROVE(robdd_left.superset(left));
                    ARIADNE_TESTING_PROVE(tree_snapshot.superset(left));
                    ARIADNE_TESTING_PROVE(robdd_snapshot.superset(left));

                    {
                        auto tree_child=
                            static_cast<SubPavingInterface const&>(tree_left)
                                .branch(true);
                        auto robdd_child=
                            static_cast<SubPavingInterface const&>(robdd_left)
                                .branch(true);
                        ARIADNE_TESTING_PROVE(
                            tree_child.root_cell()==left.split(true));
                        ARIADNE_TESTING_PROVE(
                            robdd_child.root_cell()==left.split(true));
                        ARIADNE_TESTING_PROVE(
                            tree_child.superset(left.split(true)));
                        ARIADNE_TESTING_PROVE(
                            robdd_child.superset(left.split(true)));
                        tree_child.set_root_cell(false);
                        robdd_child.set_root_cell(false);
                    }
                    ARIADNE_TESTING_PROVE(not tree.superset(left.split(true)));
                    ARIADNE_TESTING_PROVE(not robdd.superset(left.split(true)));
                    ARIADNE_TESTING_PROVE(tree.superset(right_left));
                    ARIADNE_TESTING_PROVE(robdd.superset(right_left));

                    // Child views are destroyed before clearing their
                    // parent: the tree would otherwise delete the nodes
                    // borrowed by those descendants.
                    tree_left.set_root_cell(false);
                    robdd_left.set_root_cell(false);
                    ARIADNE_TESTING_PROVE(not tree.superset(left));
                    ARIADNE_TESTING_PROVE(not robdd.superset(left));
                    ARIADNE_TESTING_PROVE(tree_snapshot.superset(left));
                    ARIADNE_TESTING_PROVE(robdd_snapshot.superset(left));
                }
                // Borrowed tree-node pointers must be released before any
                // structural recombination of the owning binary tree.
                tree.recombine();
                robdd.recombine();
                check_observations();

                // Clones of the original owners are unaffected.
                PavingInterface const& former_tree=saved_tree;
                PavingInterface const& former_robdd=saved_robdd;
                ARIADNE_TESTING_PROVE(former_tree.superset(left));
                ARIADNE_TESTING_PROVE(former_robdd.superset(left));
                ARIADNE_TESTING_EXPECT_EQUAL(
                    former_tree.size(),former_robdd.size());
                for (auto it=former_tree.begin(), end=former_tree.end();
                        it!=end; ++it) {
                    ARIADNE_TESTING_PROVE(former_robdd.superset(*it));
                }
                for (auto it=former_robdd.begin(), end=former_robdd.end();
                        it!=end; ++it) {
                    ARIADNE_TESTING_PROVE(former_tree.superset(*it));
                }
            }
        }
    }

    Void test_polymorphic_sequences_backend_isolation() {
        RobddManager manager;
        for (Nat dimensions : {1u,2u}) {
            Grid grid(dimensions);
            GridCell root(grid,0u,BinaryWord{});
            std::vector<GridCell> cells;
            for (unsigned mask=0u; mask<8u; ++mask) {
                BinaryWord word;
                word.push_back((mask & 4u)!=0u);
                word.push_back((mask & 2u)!=0u);
                word.push_back((mask & 1u)!=0u);
                cells.emplace_back(grid,0u,word);
            }

            // Check geometric observations only; never pass either
            // backend as an operand to the other backend's set methods.
            auto verify_outputs=[&](PavingInterface const& tree,
                                    PavingInterface const& robdd) {
                ARIADNE_TESTING_PROVE(tree.grid()==robdd.grid());
                ARIADNE_TESTING_PROVE(tree.root_cell()==robdd.root_cell());
                ARIADNE_TESTING_EXPECT_EQUAL(tree.size(),robdd.size());
                ARIADNE_TESTING_EXPECT_EQUAL(
                    tree.bounding_box().dimension(),
                    robdd.bounding_box().dimension());
                if (tree.size()!=0u) {
                    ARIADNE_TESTING_PROVE(same(
                        cast_exact_box(tree.bounding_box()),
                        cast_exact_box(robdd.bounding_box())));
                }
                for (GridCell const& cell : cells) {
                    ARIADNE_TESTING_EXPECT_EQUAL(
                        tree.superset(cell),robdd.superset(cell));
                    ExactBoxType query=cell.box();
                    ARIADNE_TESTING_EXPECT_EQUAL(
                        definitely(tree.inside(query)),
                        definitely(robdd.inside(query)));
                    ARIADNE_TESTING_EXPECT_EQUAL(
                        definitely(tree.separated(query)),
                        definitely(robdd.separated(query)));
                    ARIADNE_TESTING_EXPECT_EQUAL(
                        definitely(tree.overlaps(query)),
                        definitely(robdd.overlaps(query)));
                }
            };

            for (unsigned table=0u; table<256u; ++table) {
                PavingHandle tree_handle(new BinaryTreePaving(grid));
                PavingHandle robdd_handle(new RobddPaving(
                    manager,root,manager.constant(false),0u));
                PavingInterface& tree=tree_handle;
                PavingInterface& robdd=robdd_handle;
                for (unsigned mask=0u; mask<8u; ++mask) {
                    if ((table & (1u<<mask))==0u) { continue; }
                    tree_handle.adjoin(cells[mask]);
                    robdd_handle.adjoin(cells[mask]);
                }
                tree.recombine();
                robdd.recombine();
                verify_outputs(tree,robdd);

                // Polymorphic cloning yields independent owners.
                PavingHandle original_tree(tree_handle);
                PavingHandle original_robdd(robdd_handle);

                // Intersect only with an operand of the same backend.
                BinaryTreePaving tree_mask(grid);
                RobddPaving robdd_mask(
                    manager,root,manager.constant(false),0u);
                for (unsigned mask=0u; mask<8u; ++mask) {
                    if ((0x5Au & (1u<<mask))==0u) { continue; }
                    tree_mask.adjoin(cells[mask]);
                    robdd_mask.adjoin(cells[mask]);
                }
                tree_handle.restrict(tree_mask);
                robdd_handle.restrict(robdd_mask);
                tree.recombine();
                robdd.recombine();
                verify_outputs(tree,robdd);
                ARIADNE_TESTING_PROVE(tree.subset(tree_mask));
                ARIADNE_TESTING_PROVE(robdd.subset(robdd_mask));

                // Difference and union are also strictly same-backend.
                BinaryTreePaving tree_delta(grid);
                RobddPaving robdd_delta(
                    manager,root,manager.constant(false),0u);
                for (unsigned mask=0u; mask<8u; ++mask) {
                    if ((0x99u & (1u<<mask))==0u) { continue; }
                    tree_delta.adjoin(cells[mask]);
                    robdd_delta.adjoin(cells[mask]);
                }
                tree_handle.remove(tree_delta);
                robdd_handle.remove(robdd_delta);
                tree.recombine();
                robdd.recombine();
                verify_outputs(tree,robdd);

                tree_handle.adjoin(tree_delta);
                robdd_handle.adjoin(robdd_delta);
                tree.recombine();
                robdd.recombine();
                verify_outputs(tree,robdd);

                // The earlier polymorphic copies keep their initial set.
                verify_outputs(
                    static_cast<PavingInterface const&>(original_tree),
                    static_cast<PavingInterface const&>(original_robdd));

                // Force a mixed root before creating live branch views;
                // mutations through those views must propagate to owners.
                GridCell left=root.split(false);
                GridCell right=root.split(true);
                GridCell right_left=right.split(false);
                tree_handle.adjoin(left);
                robdd_handle.adjoin(left);
                tree_handle.adjoin(right_left);
                robdd_handle.adjoin(right_left);
                // Keep the root mixed even if a previous sequence
                // happened to fill every cell of its right child.
                tree_handle.remove(right.split(true));
                robdd_handle.remove(right.split(true));
                {
                    SubPavingHandle tree_left=tree.branch(false);
                    SubPavingHandle robdd_left=robdd.branch(false);
                    ARIADNE_TESTING_PROVE(tree_left.root_cell()==left);
                    ARIADNE_TESTING_PROVE(robdd_left.root_cell()==left);
                    ARIADNE_TESTING_PROVE(tree_left.superset(left));
                    ARIADNE_TESTING_PROVE(robdd_left.superset(left));

                    SubPavingHandle tree_snapshot(tree_left);
                    SubPavingHandle robdd_snapshot(robdd_left);
                    tree_snapshot.set_root_cell(false);
                    robdd_snapshot.set_root_cell(false);
                    ARIADNE_TESTING_PROVE(tree_left.superset(left));
                    ARIADNE_TESTING_PROVE(robdd_left.superset(left));
                    ARIADNE_TESTING_PROVE(not tree_snapshot.superset(left));
                    ARIADNE_TESTING_PROVE(not robdd_snapshot.superset(left));

                    tree_left.set_root_cell(false);
                    robdd_left.set_root_cell(false);
                    ARIADNE_TESTING_PROVE(not tree.superset(left));
                    ARIADNE_TESTING_PROVE(not robdd.superset(left));
                    ARIADNE_TESTING_PROVE(tree.superset(right_left));
                    ARIADNE_TESTING_PROVE(robdd.superset(right_left));
                }
                // A tree recombine may invalidate borrowed node pointers,
                // so recombine only after the child handles are destroyed.
                tree.recombine();
                robdd.recombine();
                verify_outputs(tree,robdd);
            }
        }
    }

    Void test_polymorphic_paving_handle_and_subpaving_views() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        GridCell left=root.split(false);
        GridCell right=root.split(true);
        GridCell left_right=left.split(true);

        PavingHandle tree_handle(new BinaryTreePaving(grid));
        PavingHandle robdd_handle(new RobddPaving(
            manager,root,manager.constant(false),0u));
        PavingInterface& tree=tree_handle;
        PavingInterface& robdd=robdd_handle;
        tree.adjoin(left_right);
        tree.adjoin(right);
        robdd.adjoin(left_right);
        robdd.adjoin(right);

        ARIADNE_TESTING_PROVE(tree.superset(right));
        ARIADNE_TESTING_PROVE(robdd.superset(right));
        ARIADNE_TESTING_EXPECT_EQUAL(tree.size(),robdd.size());
        // Same observable geometry, but incompatible paving backends:
        // set operations must reject mixing rather than convert silently.
        ARIADNE_TESTING_EXPECT_EXCEPTION(tree.subset(robdd));
        ARIADNE_TESTING_EXPECT_EXCEPTION(robdd.subset(tree));
        ARIADNE_TESTING_EXPECT_EXCEPTION(tree.equals(robdd));
        ARIADNE_TESTING_EXPECT_EXCEPTION(robdd.equals(tree));
        ARIADNE_TESTING_EXPECT_EXCEPTION(tree.intersects(robdd));
        ARIADNE_TESTING_EXPECT_EXCEPTION(robdd.intersects(tree));
        ARIADNE_TESTING_EXPECT_EXCEPTION(tree.adjoin(robdd));
        ARIADNE_TESTING_EXPECT_EXCEPTION(robdd.adjoin(tree));
        ARIADNE_TESTING_EXPECT_EXCEPTION(tree.restrict(robdd));
        ARIADNE_TESTING_EXPECT_EXCEPTION(robdd.restrict(tree));
        ARIADNE_TESTING_EXPECT_EXCEPTION(tree.remove(robdd));
        ARIADNE_TESTING_EXPECT_EXCEPTION(robdd.remove(tree));
        ARIADNE_TESTING_PROVE(same(
            cast_exact_box(tree.bounding_box()),
            cast_exact_box(robdd.bounding_box())));

        // The root paving is cloned independently through PavingHandle.
        PavingHandle copied(robdd_handle);
        ARIADNE_TESTING_PROVE(copied.superset(right));
        copied.remove(right);
        ARIADNE_TESTING_PROVE(not copied.superset(right));
        ARIADNE_TESTING_PROVE(robdd.superset(right));

        // Child interfaces are live views and modify their owning paving.
        SubPavingHandle tree_left=tree.branch(false);
        SubPavingHandle robdd_left=robdd.branch(false);
        ARIADNE_TESTING_PROVE(tree_left.root_cell()==left);
        ARIADNE_TESTING_PROVE(robdd_left.root_cell()==left);
        ARIADNE_TESTING_PROVE(tree_left.superset(left_right));
        ARIADNE_TESTING_PROVE(robdd_left.superset(left_right));
        ARIADNE_TESTING_EXPECT_EQUAL(tree_left.size(),robdd_left.size());
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            tree_left.subset(static_cast<SubPavingInterface const&>(robdd_left)));
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            robdd_left.subset(static_cast<SubPavingInterface const&>(tree_left)));

        // Clone the view via both handle constructors. Mutating either
        // independent clone must not change the original parent.
        SubPavingHandle robdd_left_copy(robdd_left);
        PavingHandle independent_left(
            static_cast<SubPavingInterface const&>(robdd_left));
        ARIADNE_TESTING_PROVE(robdd_left_copy.superset(left_right));
        ARIADNE_TESTING_PROVE(independent_left.superset(left_right));
        robdd_left_copy.set_root_cell(false);
        independent_left.set_root_cell(false);
        ARIADNE_TESTING_PROVE(robdd_left.superset(left_right));
        ARIADNE_TESTING_PROVE(robdd.superset(left_right));
        ARIADNE_TESTING_PROVE(not robdd_left_copy.superset(left_right));
        ARIADNE_TESTING_PROVE(not independent_left.superset(left_right));

        // A grandchild returned through the interface is another live view.
        SubPavingHandle tree_left_right=
            static_cast<SubPavingInterface const&>(tree_left).branch(true);
        SubPavingHandle robdd_left_right=
            static_cast<SubPavingInterface const&>(robdd_left).branch(true);
        ARIADNE_TESTING_PROVE(tree_left_right.root_cell()==left_right);
        ARIADNE_TESTING_PROVE(robdd_left_right.root_cell()==left_right);
        tree_left_right.set_root_cell(false);
        robdd_left_right.set_root_cell(false);
        ARIADNE_TESTING_PROVE(not tree.superset(left_right));
        ARIADNE_TESTING_PROVE(not robdd.superset(left_right));
        ARIADNE_TESTING_PROVE(tree.superset(right));
        ARIADNE_TESTING_PROVE(robdd.superset(right));

        // Local mincing/recombining must leave sibling geometry unchanged.
        tree_left.mince(2u);
        robdd_left.mince(2u);
        tree_left.recombine();
        robdd_left.recombine();
        ARIADNE_TESTING_PROVE(tree.superset(right));
        ARIADNE_TESTING_PROVE(robdd.superset(right));

        // Iterators through the polymorphic interfaces agree.
        auto it_tree=tree.begin(),end_tree=tree.end();
        auto it_robdd=robdd.begin(),end_robdd=robdd.end();
        ARIADNE_TESTING_PROVE(it_tree!=end_tree);
        ARIADNE_TESTING_PROVE(it_robdd!=end_robdd);
        ARIADNE_TESTING_PROVE(*it_tree==*it_robdd);
        ++it_tree;
        ++it_robdd;
        ARIADNE_TESTING_PROVE(it_tree==end_tree);
        ARIADNE_TESTING_PROVE(it_robdd==end_robdd);

        // Constructing a handle from a detached ROBDD paving remains valid
        // when the source object goes out of scope.
        PavingHandle independent_full(
            static_cast<SubPavingInterface const&>(robdd));
        ARIADNE_TESTING_PROVE(independent_full.superset(right));
        independent_full.set_root_cell(false);
        ARIADNE_TESTING_PROVE(robdd.superset(right));
    }

    Void test_polymorphic_refinement_contract() {
        RobddManager manager;
        for (Nat dimensions : {1u,2u}) {
            Grid grid(dimensions);
            GridCell root(grid,0u,BinaryWord{});
            GridCell left=root.split(false);
            GridCell right=root.split(true);
            auto iterated_size=[](SubPavingInterface const& paving) {
                SizeType count=0u;
                for (auto it=paving.begin(), end=paving.end();
                        it!=end; ++it) { ++count; }
                return count;
            };

            // Equal geometry with different physical subdivisions.
            BinaryTreePaving tree_compact(grid,true);
            BinaryTreePaving tree_minced(grid,true);
            RobddPaving robdd_compact(
                manager,root,manager.constant(true),0u);
            RobddPaving robdd_minced(
                manager,root,manager.constant(true),0u);
            PavingInterface const& tc=tree_compact;
            PavingInterface& tm=tree_minced;
            PavingInterface const& rc=robdd_compact;
            PavingInterface& rm=robdd_minced;
            tm.mince(2u);
            rm.mince(2u);
            SizeType leaf_count=SizeType{1u} << (2u*dimensions);
            ARIADNE_TESTING_EXPECT_EQUAL(tc.size(),1u);
            ARIADNE_TESTING_EXPECT_EQUAL(tm.size(),leaf_count);
            ARIADNE_TESTING_EXPECT_EQUAL(rc.size(),1u);
            ARIADNE_TESTING_EXPECT_EQUAL(rm.size(),1u);
            ARIADNE_TESTING_EXPECT_EQUAL(iterated_size(tm),tm.size());
            ARIADNE_TESTING_EXPECT_EQUAL(iterated_size(rm),rm.size());
            ARIADNE_TESTING_PROVE(not (
                static_cast<GridTreeSubpaving const&>(tree_compact)
                == static_cast<GridTreeSubpaving const&>(tree_minced)));
            ARIADNE_TESTING_PROVE(tc.equals(tm));
            ARIADNE_TESTING_PROVE(tm.equals(tc));
            ARIADNE_TESTING_PROVE(rc.equals(rm));
            ARIADNE_TESTING_PROVE(rm.equals(rc));
            ARIADNE_TESTING_PROVE(tc.superset(root));
            ARIADNE_TESTING_PROVE(tm.superset(root));
            ARIADNE_TESTING_PROVE(rc.superset(root));
            ARIADNE_TESTING_PROVE(rm.superset(root));
            ARIADNE_TESTING_EXPECT_EQUAL(
                robdd_minced.enabled_cell_count(),leaf_count);
            tm.recombine();
            rm.recombine();
            ARIADNE_TESTING_EXPECT_EQUAL(tm.size(),1u);
            ARIADNE_TESTING_EXPECT_EQUAL(rm.size(),1u);
            ARIADNE_TESTING_EXPECT_EQUAL(robdd_minced.depth(),0u);
            ARIADNE_TESTING_PROVE(tm.equals(tc));
            ARIADNE_TESTING_PROVE(rm.equals(rc));

            // Child-view mincing is local and preserves sibling geometry.
            BinaryTreePaving tree(grid,true);
            RobddPaving robdd(manager,root,manager.constant(true),0u);
            PavingInterface& a=tree;
            PavingInterface& b=robdd;
            {
                SubPavingHandle al=a.branch(false);
                SubPavingHandle ar=a.branch(true);
                SubPavingHandle bl=b.branch(false);
                SubPavingHandle br=b.branch(true);
                ARIADNE_TESTING_PROVE(al.root_cell()==left);
                ARIADNE_TESTING_PROVE(bl.root_cell()==left);
                ARIADNE_TESTING_PROVE(ar.root_cell()==right);
                ARIADNE_TESTING_PROVE(br.root_cell()==right);
                al.mince(2u);
                bl.mince(2u);
                SizeType local=SizeType{1u} << (2u*dimensions-1u);
                ARIADNE_TESTING_EXPECT_EQUAL(al.size(),local);
                ARIADNE_TESTING_EXPECT_EQUAL(bl.size(),1u);
                ARIADNE_TESTING_EXPECT_EQUAL(iterated_size(al),al.size());
                ARIADNE_TESTING_EXPECT_EQUAL(iterated_size(bl),bl.size());
                ARIADNE_TESTING_EXPECT_EQUAL(ar.size(),1u);
                ARIADNE_TESTING_EXPECT_EQUAL(br.size(),1u);
                ARIADNE_TESTING_PROVE(a.superset(root));
                ARIADNE_TESTING_PROVE(b.superset(root));
                ARIADNE_TESTING_PROVE(ar.superset(right));
                ARIADNE_TESTING_PROVE(br.superset(right));

                // Geometric equality includes differing roots and views.
                BinaryTreePaving tree_left(grid);
                tree_left.adjoin(left);
                RobddPaving robdd_left(
                    manager,root,manager.constant(false),0u);
                robdd_left.adjoin(left);
                SubPavingInterface const& tv=al;
                SubPavingInterface const& rv=bl;
                ARIADNE_TESTING_PROVE(tv.equals(tree_left));
                ARIADNE_TESTING_PROVE(tree_left.equals(tv));
                ARIADNE_TESTING_PROVE(rv.equals(robdd_left));
                ARIADNE_TESTING_PROVE(robdd_left.equals(rv));

                al.recombine();
                bl.recombine();
                ARIADNE_TESTING_EXPECT_EQUAL(al.size(),1u);
                ARIADNE_TESTING_EXPECT_EQUAL(bl.size(),1u);
                ARIADNE_TESTING_EXPECT_EQUAL(ar.size(),1u);
                ARIADNE_TESTING_EXPECT_EQUAL(br.size(),1u);
                ARIADNE_TESTING_PROVE(a.superset(root));
                ARIADNE_TESTING_PROVE(b.superset(root));
                al.set_root_cell(false);
                bl.set_root_cell(false);
                ARIADNE_TESTING_PROVE(not a.superset(left));
                ARIADNE_TESTING_PROVE(not b.superset(left));
                ARIADNE_TESTING_PROVE(a.superset(right));
                ARIADNE_TESTING_PROVE(b.superset(right));
            }
            // Borrowed tree nodes must be released before owner reduction.
            a.recombine();
            b.recombine();
            ARIADNE_TESTING_EXPECT_EQUAL(a.size(),1u);
            ARIADNE_TESTING_EXPECT_EQUAL(b.size(),1u);
            ARIADNE_TESTING_PROVE(not a.superset(left));
            ARIADNE_TESTING_PROVE(not b.superset(left));
            ARIADNE_TESTING_PROVE(a.superset(right));
            ARIADNE_TESTING_PROVE(b.superset(right));
        }
    }

    Void test_polymorphic_enumeration_at_fineness() {
        RobddManager manager;
        auto collect=[](SubPavingInterface const& set, Nat fineness) {
            std::vector<GridCell> cells;
            for_each_enabled_cell_at_fineness(
                set,fineness,[&](GridCell const& cell) {
                    cells.push_back(cell);
                });
            return cells;
        };
        for (Nat dimensions : {1u,2u,3u}) {
            Grid grid(dimensions);
            for (Nat extent : {0u,1u}) {
                GridCell root(grid,extent,BinaryWord{});
                for (unsigned mask : {0u,1u,0x3Cu,0xAAu,0xFFu}) {
                    BinaryTreePaving tree(
                        grid,extent,new BinaryTreeNode(false));
                    RobddPaving robdd(
                        manager,root,manager.constant(false),0u);
                    for (unsigned bit=0u; bit<8u; ++bit) {
                        if ((mask & (1u<<bit))==0u) { continue; }
                        BinaryWord word;
                        word.push_back((bit & 4u)!=0u);
                        word.push_back((bit & 2u)!=0u);
                        word.push_back((bit & 1u)!=0u);
                        GridCell cell(grid,extent,word);
                        tree.adjoin(cell);
                        robdd.adjoin(cell);
                    }
                    tree.recombine();
                    PavingInterface const& tree_interface=tree;
                    PavingInterface const& robdd_interface=robdd;
                    Nat tree_depth=tree.tree_depth();
                    SizeType logical_depth=robdd.depth();
                    SizeType tree_size=tree.size();
                    SizeType robdd_size=robdd.size();
                    for (Nat fineness : {0u,1u,2u}) {
                        // A materialized, separately minced tree is the
                        // reference for what a traversal at this depth emits.
                        BinaryTreePaving oracle(tree);
                        oracle.mince(fineness);
                        std::vector<GridCell> expected;
                        for (auto it=oracle.begin(), end=oracle.end();
                                it!=end; ++it) { expected.push_back(*it); }
                        auto actual_tree=collect(tree_interface,fineness);
                        auto actual_robdd=collect(robdd_interface,fineness);
                        ARIADNE_TESTING_EXPECT_EQUAL(
                            actual_tree.size(),expected.size());
                        ARIADNE_TESTING_EXPECT_EQUAL(
                            actual_robdd.size(),expected.size());
                        ARIADNE_TESTING_PROVE(actual_tree==expected);
                        ARIADNE_TESTING_PROVE(actual_robdd==expected);
                        for (GridCell const& cell : actual_robdd) {
                            ARIADNE_TESTING_PROVE(
                                cell.box().subset(root.box()));
                            ARIADNE_TESTING_PROVE(tree.superset(cell));
                            ARIADNE_TESTING_PROVE(robdd.superset(cell));
                        }
                        ARIADNE_TESTING_EXPECT_EQUAL(tree.tree_depth(),tree_depth);
                        ARIADNE_TESTING_EXPECT_EQUAL(robdd.depth(),logical_depth);
                        ARIADNE_TESTING_EXPECT_EQUAL(tree.size(),tree_size);
                        ARIADNE_TESTING_EXPECT_EQUAL(robdd.size(),robdd_size);
                    }
                }
            }
        }
    }

    Void test_polymorphic_child_enumeration_at_fineness() {
        RobddManager manager;
        for (Nat dimensions : {1u,2u}) {
            Grid grid(dimensions);
            for (Nat extent : {0u,1u}) {
                GridCell root(grid,extent,BinaryWord{});
                BinaryTreePaving tree(grid,extent,new BinaryTreeNode(true));
                RobddPaving robdd(
                    manager,root,manager.constant(true),0u);
                PavingInterface& tree_interface=tree;
                PavingInterface& robdd_interface=robdd;
                auto tree_left=tree_interface.branch(false);
                auto robdd_left=robdd_interface.branch(false);
                auto tree_sub=
                    static_cast<SubPavingInterface const&>(tree_left)
                        .branch(true);
                auto robdd_sub=
                    static_cast<SubPavingInterface const&>(robdd_left)
                        .branch(true);
                GridCell cell=root.split(false).split(true);
                ARIADNE_TESTING_PROVE(tree_sub.root_cell()==cell);
                ARIADNE_TESTING_PROVE(robdd_sub.root_cell()==cell);
                SizeType tree_size=tree.size();
                SizeType robdd_size=robdd.size();
                auto collect=[](SubPavingInterface const& set, Nat f) {
                    std::vector<GridCell> cells;
                    for_each_enabled_cell_at_fineness(
                        set,f,[&](GridCell const& c) { cells.push_back(c); });
                    return cells;
                };
                SubPavingInterface const& tv=tree_sub;
                SubPavingInterface const& rv=robdd_sub;
                for (Nat fineness : {0u,1u,2u,3u}) {
                    BinaryTreeNode reference_node(true);
                    GridTreeSubpaving reference(
                        grid,extent,cell.word(),&reference_node);
                    reference.mince(fineness);
                    std::vector<GridCell> expected;
                    for (auto it=reference.begin(), end=reference.end();
                            it!=end; ++it) { expected.push_back(*it); }
                    auto from_tree=collect(tv,fineness);
                    auto from_robdd=collect(rv,fineness);
                    ARIADNE_TESTING_PROVE(from_tree==expected);
                    ARIADNE_TESTING_PROVE(from_robdd==expected);
                    ARIADNE_TESTING_EXPECT_EQUAL(tree.size(),tree_size);
                    ARIADNE_TESTING_EXPECT_EQUAL(robdd.size(),robdd_size);
                    ARIADNE_TESTING_PROVE(
                        tree_interface.superset(root));
                    ARIADNE_TESTING_PROVE(
                        robdd_interface.superset(root));
                    ARIADNE_TESTING_PROVE(
                        tree_interface.superset(root.split(true)));
                    ARIADNE_TESTING_PROVE(
                        robdd_interface.superset(root.split(true)));
                }
            }
        }
    }

    Void test_uniform_cell_paving_contract() {
        RobddManager manager;
        for (Nat dimensions : {1u,2u}) {
            Grid grid(dimensions);
            GridCell root(grid,0u,BinaryWord{});
            BinaryTreePaving tree(grid,true);
            RobddPaving robdd(manager,root,manager.constant(true),0u);
            PavingInterface const& t=tree;
            PavingInterface const& r=robdd;
            for (Nat fineness : {0u,1u,2u,3u}) {
                SizeType target=static_cast<SizeType>(fineness)*dimensions;
                SizeType count=SizeType{1u}<<target;
                std::vector<GridCell> from_tree,from_robdd;
                t.for_each_enabled_cell_at_exact_fineness(
                    fineness,[&](GridCell const& cell) {
                        from_tree.push_back(cell);
                    });
                r.for_each_enabled_cell_at_exact_fineness(
                    fineness,[&](GridCell const& cell) {
                        from_robdd.push_back(cell);
                    });
                ARIADNE_TESTING_PROVE(from_tree==from_robdd);
                ARIADNE_TESTING_EXPECT_EQUAL(from_tree.size(),count);
                for (GridCell const& cell : from_tree) {
                    ARIADNE_TESTING_EXPECT_EQUAL(cell.word().size(),target);
                }
                PavingHandle tree_uniform=t.uniform_cell_paving(fineness);
                PavingHandle robdd_uniform=r.uniform_cell_paving(fineness);
                ARIADNE_TESTING_EXPECT_EQUAL(tree_uniform.size(),count);
                ARIADNE_TESTING_EXPECT_EQUAL(robdd_uniform.size(),count);
                auto actual_tree=tree_uniform.begin();
                auto actual_robdd=robdd_uniform.begin();
                for (GridCell const& cell : from_tree) {
                    ARIADNE_TESTING_PROVE(actual_tree!=tree_uniform.end());
                    ARIADNE_TESTING_PROVE(actual_robdd!=robdd_uniform.end());
                    ARIADNE_TESTING_PROVE(*actual_tree==cell);
                    ARIADNE_TESTING_PROVE(*actual_robdd==cell);
                    ++actual_tree;
                    ++actual_robdd;
                }
                ARIADNE_TESTING_PROVE(actual_tree==tree_uniform.end());
                ARIADNE_TESTING_PROVE(actual_robdd==robdd_uniform.end());
                ARIADNE_TESTING_PROVE(t.superset(root));
                ARIADNE_TESTING_PROVE(r.superset(root));
                ARIADNE_TESTING_EXPECT_EQUAL(t.size(),1u);
                ARIADNE_TESTING_EXPECT_EQUAL(r.size(),1u);
                ARIADNE_TESTING_EXPECT_EQUAL(robdd.depth(),0u);
            }

            // An intentionally refined tree can be coarsened exactly
            // when the enabled descendants cover the whole target cell.
            tree.mince(3u);
            PavingHandle coarsened=t.uniform_cell_paving(1u);
            ARIADNE_TESTING_EXPECT_EQUAL(
                coarsened.size(),SizeType{1u}<<dimensions);
            ARIADNE_TESTING_EXPECT_EQUAL(
                tree.size(),SizeType{1u}<<(3u*dimensions));
            ARIADNE_TESTING_PROVE(t.superset(root));

            // Partial coverage of a target cell MUST be rejected,
            // rather than rounded inward or outward.
            BinaryTreePaving partial(grid);
            RobddPaving partial_robdd(
                manager,root,manager.constant(false),0u);
            GridCell fine=root.split(false).split(false);
            partial.adjoin(fine);
            partial_robdd.adjoin(fine);
            PavingInterface const& partial_tree_interface=partial;
            PavingInterface const& ri=partial_robdd;
            SizeType visited=0u;
            ARIADNE_TESTING_EXPECT_EXCEPTION(
                partial_tree_interface.for_each_enabled_cell_at_exact_fineness(
                    0u,[&](GridCell const&) { ++visited; }));
            ARIADNE_TESTING_EXPECT_EQUAL(visited,0u);
            ARIADNE_TESTING_EXPECT_EXCEPTION(
                ri.for_each_enabled_cell_at_exact_fineness(
                    0u,[&](GridCell const&) { ++visited; }));
            ARIADNE_TESTING_EXPECT_EQUAL(visited,0u);
            ARIADNE_TESTING_EXPECT_EXCEPTION(partial_tree_interface.uniform_cell_paving(0u));
            ARIADNE_TESTING_EXPECT_EXCEPTION(ri.uniform_cell_paving(0u));
            PavingHandle p_exact=partial_tree_interface.uniform_cell_paving(2u);
            PavingHandle r_exact=ri.uniform_cell_paving(2u);
            SizeType refined_count=SizeType{1u}<<(2u*dimensions-2u);
            ARIADNE_TESTING_EXPECT_EQUAL(p_exact.size(),refined_count);
            ARIADNE_TESTING_EXPECT_EQUAL(r_exact.size(),refined_count);
            ARIADNE_TESTING_PROVE(p_exact.superset(fine));
            ARIADNE_TESTING_PROVE(r_exact.superset(fine));

            // A non-primary view cannot be represented exactly by
            // cells wider than the view root unless it is empty.
            SubPavingHandle t_child=partial_tree_interface.branch(false);
            SubPavingHandle r_child=ri.branch(false);
            SubPavingInterface const& tv=t_child;
            SubPavingInterface const& rv=r_child;
            ARIADNE_TESTING_EXPECT_EXCEPTION(
                tv.uniform_cell_paving(0u));
            ARIADNE_TESTING_EXPECT_EXCEPTION(
                rv.uniform_cell_paving(0u));
            PavingHandle t_child_uniform=tv.uniform_cell_paving(2u);
            PavingHandle r_child_uniform=rv.uniform_cell_paving(2u);
            ARIADNE_TESTING_EXPECT_EQUAL(t_child_uniform.size(),refined_count);
            ARIADNE_TESTING_EXPECT_EQUAL(r_child_uniform.size(),refined_count);
            ARIADNE_TESTING_PROVE(t_child_uniform.superset(fine));
            ARIADNE_TESTING_PROVE(r_child_uniform.superset(fine));
        }
    }

    Void test_polymorphic_handle_uniform_cell_enumeration() {
        RobddManager manager;
        for (Nat dimension : {1u,2u}) {
            Grid grid(dimension);
            GridCell root(grid,0u,BinaryWord{});
            PavingHandle tree_handle(new BinaryTreePaving(grid,true));
            PavingHandle robdd_handle(new RobddPaving(
                manager,root,manager.constant(true),0u));

            std::vector<GridCell> tree_cells;
            std::vector<GridCell> robdd_cells;
            tree_handle.for_each_enabled_cell_at_exact_fineness(
                2u,[&](GridCell const& cell) {
                    tree_cells.push_back(cell);
                });
            robdd_handle.for_each_enabled_cell_at_exact_fineness(
                2u,[&](GridCell const& cell) {
                    robdd_cells.push_back(cell);
                });
            ARIADNE_TESTING_PROVE(tree_cells==robdd_cells);
            SizeType full_count=SizeType{1u}<<(2u*dimension);
            ARIADNE_TESTING_EXPECT_EQUAL(tree_cells.size(),full_count);
            for (GridCell const& cell : tree_cells) {
                ARIADNE_TESTING_EXPECT_EQUAL(
                    cell.word().size(),2u*dimension);
            }
            ARIADNE_TESTING_EXPECT_EQUAL(tree_handle.size(),1u);
            ARIADNE_TESTING_EXPECT_EQUAL(robdd_handle.size(),1u);

            PavingHandle uniform_from_tree=tree_handle.uniform_cell_paving(2u);
            PavingHandle uniform_from_robdd=robdd_handle.uniform_cell_paving(2u);
            ARIADNE_TESTING_EXPECT_EQUAL(uniform_from_tree.size(),full_count);
            ARIADNE_TESTING_EXPECT_EQUAL(uniform_from_robdd.size(),full_count);
            ARIADNE_TESTING_PROVE(uniform_from_tree.superset(root));
            ARIADNE_TESTING_PROVE(uniform_from_robdd.superset(root));

            // The handle forwards the exact-depth visitor even for a
            // borrowed child. Its sibling and owning paving remain enabled.
            PavingInterface const& tree_interface=tree_handle;
            PavingInterface const& robdd_interface=robdd_handle;
            SubPavingHandle tree_left=tree_interface.branch(false);
            SubPavingHandle robdd_left=robdd_interface.branch(false);
            tree_cells.clear();
            robdd_cells.clear();
            tree_left.for_each_enabled_cell_at_exact_fineness(
                2u,[&](GridCell const& cell) {
                    tree_cells.push_back(cell);
                });
            robdd_left.for_each_enabled_cell_at_exact_fineness(
                2u,[&](GridCell const& cell) {
                    robdd_cells.push_back(cell);
                });
            ARIADNE_TESTING_PROVE(tree_cells==robdd_cells);
            ARIADNE_TESTING_EXPECT_EQUAL(
                tree_cells.size(),full_count/2u);
            PavingHandle tree_left_uniform=tree_left.uniform_cell_paving(2u);
            PavingHandle robdd_left_uniform=robdd_left.uniform_cell_paving(2u);
            ARIADNE_TESTING_EXPECT_EQUAL(
                tree_left_uniform.size(),full_count/2u);
            ARIADNE_TESTING_EXPECT_EQUAL(
                robdd_left_uniform.size(),full_count/2u);
            ARIADNE_TESTING_PROVE(
                tree_handle.superset(root.split(true)));
            ARIADNE_TESTING_PROVE(
                robdd_handle.superset(root.split(true)));
            ARIADNE_TESTING_PROVE(tree_handle.superset(root));
            ARIADNE_TESTING_PROVE(robdd_handle.superset(root));
        }
    }

    Void test_exact_uniform_oracle_exhaustive_and_nested_views() {
        // The oracle is independent of both paving iterators and superset():
        // bits in a three-decision occupancy table specify the geometry.
        // All target cells must either be completely enabled or disabled.
        RobddManager manager;
        auto bits_to_word=[](unsigned value, Nat length) {
            BinaryWord word;
            for (Nat i=0u; i<length; ++i) {
                word.push_back(
                    (value & (1u << (length-i-1u))) != 0u);
            }
            return word;
        };

        for (Nat dimension : {1u,2u}) {
            Grid grid(dimension);
            for (Nat extent : {0u,1u}) {
                GridCell primary(grid,extent,BinaryWord{});
                for (unsigned mask=0u; mask<256u; ++mask) {
                    // Exhaust all 256 three-bit truth tables in 1D at
                    // extent zero; exercise a diverse, deterministic
                    // collection at the other dimension/extent pairs.
                    if ((dimension!=1u or extent!=0u)
                            and mask%17u!=0u
                            and mask!=0x55u and mask!=0xAAu
                            and mask!=0xFEu and mask!=0xEFu
                            and mask!=0xFFu) { continue; }
                    BinaryTreePaving tree(
                        grid,extent,new BinaryTreeNode(false));
                    RobddPaving robdd(
                        manager,primary,manager.constant(false),0u);
                    for (unsigned bit=0u; bit<8u; ++bit) {
                        if ((mask & (1u<<bit))==0u) { continue; }
                        GridCell enabled(
                            grid,extent,bits_to_word(bit,3u));
                        tree.adjoin(enabled);
                        robdd.adjoin(enabled);
                    }
                    tree.recombine();
                    // Keep redundant enabled subdivisions in a subset
                    // of the explicit trees, exercising coalescing.
                    if (mask%3u==0u) { tree.mince_to_tree_depth(4u); }

                    auto evaluate=[&](SubPavingInterface const& tree_view,
                            SubPavingInterface const& robdd_view,
                            unsigned prefix_value, Nat prefix_length) {
                        GridCell view_root(
                            grid,extent,
                            bits_to_word(prefix_value,prefix_length));
                        ARIADNE_TESTING_PROVE(
                            tree_view.root_cell()==view_root);
                        ARIADNE_TESTING_PROVE(
                            robdd_view.root_cell()==view_root);
                        BinaryTreePaving tree_before(tree);
                        auto robdd_root_before=robdd.robdd();
                        SizeType robdd_depth_before=robdd.depth();

                        for (Nat fineness : {0u,1u,2u,3u}) {
                            unsigned target=(extent+fineness)*dimension;
                            ARIADNE_TESTING_PROVE(target<=8u);
                            std::vector<GridCell> expected;
                            Bool impossible=false;
                            if (target<prefix_length) {
                                for (unsigned bit=0u; bit<8u; ++bit) {
                                    if ((mask & (1u<<bit))!=0u and
                                            (bit>>(3u-prefix_length))
                                                ==prefix_value) {
                                        impossible=true;
                                    }
                                }
                            } else {
                                unsigned cells=1u<<target;
                                for (unsigned cell=0u; cell<cells; ++cell) {
                                    if (prefix_length!=0u and
                                            (cell>>(target-prefix_length))
                                                !=prefix_value) { continue; }
                                    if (target<=3u) {
                                        unsigned group=1u<<(3u-target);
                                        unsigned first=cell*group;
                                        unsigned selected=0u;
                                        for (unsigned bit=first;
                                                bit<first+group; ++bit) {
                                            if ((mask & (1u<<bit))!=0u) {
                                                ++selected;
                                            }
                                        }
                                        if (selected!=0u and selected!=group) {
                                            impossible=true;
                                        } else if (selected==group) {
                                            expected.emplace_back(
                                                grid,extent,
                                                bits_to_word(cell,target));
                                        }
                                    } else {
                                        unsigned parent=cell>>(target-3u);
                                        if ((mask & (1u<<parent))!=0u) {
                                            expected.emplace_back(
                                                grid,extent,
                                                bits_to_word(cell,target));
                                        }
                                    }
                                }
                            }

                            std::vector<GridCell> observed_tree;
                            std::vector<GridCell> observed_robdd;
                            if (impossible) {
                                ARIADNE_TESTING_EXPECT_EXCEPTION(
                                    tree_view.for_each_enabled_cell_at_exact_fineness(
                                        fineness,[&](GridCell const& cell) {
                                            observed_tree.push_back(cell);
                                        }));
                                ARIADNE_TESTING_EXPECT_EXCEPTION(
                                    robdd_view.for_each_enabled_cell_at_exact_fineness(
                                        fineness,[&](GridCell const& cell) {
                                            observed_robdd.push_back(cell);
                                        }));
                                // An invalid coarsening is atomic for
                                // callers: no prefix may be emitted.
                                ARIADNE_TESTING_EXPECT_EQUAL(
                                    observed_tree.size(),0u);
                                ARIADNE_TESTING_EXPECT_EQUAL(
                                    observed_robdd.size(),0u);
                                ARIADNE_TESTING_EXPECT_EXCEPTION(
                                    tree_view.uniform_cell_paving(fineness));
                                ARIADNE_TESTING_EXPECT_EXCEPTION(
                                    robdd_view.uniform_cell_paving(fineness));
                            } else {
                                tree_view.for_each_enabled_cell_at_exact_fineness(
                                    fineness,[&](GridCell const& cell) {
                                        observed_tree.push_back(cell);
                                    });
                                robdd_view.for_each_enabled_cell_at_exact_fineness(
                                    fineness,[&](GridCell const& cell) {
                                        observed_robdd.push_back(cell);
                                    });
                                ARIADNE_TESTING_PROVE(observed_tree==expected);
                                ARIADNE_TESTING_PROVE(observed_robdd==expected);

                                PavingHandle tree_uniform=
                                    tree_view.uniform_cell_paving(fineness);
                                PavingHandle robdd_uniform=
                                    robdd_view.uniform_cell_paving(fineness);
                                ARIADNE_TESTING_EXPECT_EQUAL(
                                    tree_uniform.size(),expected.size());
                                ARIADNE_TESTING_EXPECT_EQUAL(
                                    robdd_uniform.size(),expected.size());
                                auto tree_it=tree_uniform.begin();
                                auto robdd_it=robdd_uniform.begin();
                                for (GridCell const& cell : expected) {
                                    ARIADNE_TESTING_EXPECT_EQUAL(
                                        cell.word().size(),target);
                                    ARIADNE_TESTING_PROVE(
                                        tree_it!=tree_uniform.end());
                                    ARIADNE_TESTING_PROVE(
                                        robdd_it!=robdd_uniform.end());
                                    ARIADNE_TESTING_PROVE(*tree_it==cell);
                                    ARIADNE_TESTING_PROVE(*robdd_it==cell);
                                    ++tree_it;
                                    ++robdd_it;
                                }
                                ARIADNE_TESTING_PROVE(
                                    tree_it==tree_uniform.end());
                                ARIADNE_TESTING_PROVE(
                                    robdd_it==robdd_uniform.end());

                                // Verify actual geometry at the input
                                // table's finest independent resolution,
                                // including points outside child views.
                                for (unsigned bit=0u; bit<8u; ++bit) {
                                    GridCell probe(
                                        grid,extent,bits_to_word(bit,3u));
                                    Bool within=(prefix_length==0u) or
                                        ((bit>>(3u-prefix_length))
                                            ==prefix_value);
                                    Bool occupied=within and
                                        ((mask & (1u<<bit))!=0u);
                                    ARIADNE_TESTING_EXPECT_EQUAL(
                                        tree_uniform.superset(probe),occupied);
                                    ARIADNE_TESTING_EXPECT_EQUAL(
                                        robdd_uniform.superset(probe),occupied);
                                }

                                // Detached outputs must not share
                                // mutable state with their sources.
                                if (not expected.empty()) {
                                    GridCell removed=expected.front();
                                    tree_uniform.remove(removed);
                                    robdd_uniform.remove(removed);
                                    ARIADNE_TESTING_PROVE(
                                        not tree_uniform.superset(removed));
                                    ARIADNE_TESTING_PROVE(
                                        not robdd_uniform.superset(removed));
                                    ARIADNE_TESTING_PROVE(
                                        tree_view.superset(removed));
                                    ARIADNE_TESTING_PROVE(
                                        robdd_view.superset(removed));
                                }
                            }
                            ARIADNE_TESTING_PROVE(
                                static_cast<GridTreeSubpaving const&>(tree)
                                == static_cast<GridTreeSubpaving const&>(
                                    tree_before));
                            ARIADNE_TESTING_PROVE(
                                robdd.robdd()==robdd_root_before);
                            ARIADNE_TESTING_EXPECT_EQUAL(
                                robdd.depth(),robdd_depth_before);
                        }
                    };

                    evaluate(tree,robdd,0u,0u);

                    // Branch() creates live child views. Their explicit
                    // tree layout may be materialized by branch(), but
                    // uniform-cell enumeration must cause no further
                    // mutation and must not escape each view's region.
                    SubPavingInterface const& ti=tree;
                    SubPavingInterface const& ri=robdd;
                    auto t0=ti.branch(false);
                    auto r0=ri.branch(false);
                    SubPavingInterface const& tv0=t0;
                    SubPavingInterface const& rv0=r0;
                    evaluate(tv0,rv0,0u,1u);

                    auto t01=tv0.branch(true);
                    auto r01=rv0.branch(true);
                    SubPavingInterface const& tv01=t01;
                    SubPavingInterface const& rv01=r01;
                    evaluate(tv01,rv01,1u,2u);

                    auto t1=ti.branch(true);
                    auto r1=ri.branch(true);
                    SubPavingInterface const& tv1=t1;
                    SubPavingInterface const& rv1=r1;
                    auto t10=tv1.branch(false);
                    auto r10=rv1.branch(false);
                    SubPavingInterface const& tv10=t10;
                    SubPavingInterface const& rv10=r10;
                    auto t101=tv10.branch(true);
                    auto r101=rv10.branch(true);
                    SubPavingInterface const& tv101=t101;
                    SubPavingInterface const& rv101=r101;
                    evaluate(tv101,rv101,5u,3u);

                    // The same three-bit geometry is still present in
                    // both owners, including siblings of the views.
                    for (unsigned bit=0u; bit<8u; ++bit) {
                        GridCell cell(
                            grid,extent,bits_to_word(bit,3u));
                        Bool enabled=(mask & (1u<<bit))!=0u;
                        ARIADNE_TESTING_EXPECT_EQUAL(
                            tree.superset(cell),enabled);
                        ARIADNE_TESTING_EXPECT_EQUAL(
                            robdd.superset(cell),enabled);
                    }
                }
            }
        }
    }

    Void test_common_writable_cell_views() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        BinaryTreePaving tree(grid);
        PavingInterface& tree_interface=tree;
        RobddPaving robdd(manager,root,manager.constant(false),0u);

        auto tree_view=paving_cell_view(tree_interface,root);
        auto robdd_view=paving_cell_view(robdd,root);
        auto tree_left=tree_view.branch(false);
        auto robdd_left=robdd_view.branch(false);
        auto tree_right=tree_view.branch(true);
        auto robdd_right=robdd_view.branch(true);
        GridCell left=root.split(false);
        GridCell right=root.split(true);
        ARIADNE_TESTING_PROVE(tree_left.root_cell()==left);
        ARIADNE_TESTING_PROVE(robdd_left.root_cell()==left);
        ARIADNE_TESTING_EXPECT_EQUAL(tree_left.dimension(),1u);
        ARIADNE_TESTING_EXPECT_EQUAL(robdd_left.dimension(),1u);
        ARIADNE_TESTING_PROVE(not tree_left.superset(left));
        ARIADNE_TESTING_PROVE(not robdd_left.superset(left));

        tree_left.set_root_cell(true);
        robdd_left.set_root_cell(true);
        tree.recombine();
        ARIADNE_TESTING_PROVE(tree.superset(left));
        ARIADNE_TESTING_PROVE(robdd.superset(left));
        ARIADNE_TESTING_PROVE(tree_left.superset(left));
        ARIADNE_TESTING_PROVE(robdd_left.superset(left));
        ARIADNE_TESTING_PROVE(not tree_left.superset(right));
        ARIADNE_TESTING_PROVE(not robdd_left.superset(right));

        // Copies of views alias their original paving, not a detached clone.
        auto tree_alias=tree_left;
        auto robdd_alias=robdd_left;
        tree_alias.set_root_cell(false);
        robdd_alias.set_root_cell(false);
        ARIADNE_TESTING_PROVE(not tree.superset(left));
        ARIADNE_TESTING_PROVE(not robdd.superset(left));

        tree_right.branch(false).set_root_cell(true);
        robdd_right.branch(false).set_root_cell(true);
        GridCell right_left=right.split(false);
        ARIADNE_TESTING_PROVE(tree.superset(right_left));
        ARIADNE_TESTING_PROVE(robdd.superset(right_left));
        ARIADNE_TESTING_PROVE(not tree.superset(right));
        ARIADNE_TESTING_PROVE(not robdd.superset(right));

        // A nested view can be disabled without clearing sibling cells.
        tree_right.branch(false).set_root_cell(false);
        robdd_right.branch(false).set_root_cell(false);
        ARIADNE_TESTING_PROVE(not tree.superset(right_left));
        ARIADNE_TESTING_PROVE(not robdd.superset(right_left));

        Grid other_grid(2u);
        ARIADNE_TESTING_EXPECT_EXCEPTION(paving_cell_view(
            tree_interface,GridCell(other_grid,0u,BinaryWord{})));
        ARIADNE_TESTING_EXPECT_EXCEPTION(paving_cell_view(
            robdd,GridCell(other_grid,0u,BinaryWord{})));
        GridCell outside(grid,1u,BinaryWord{});
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            paving_cell_view(tree_interface,outside));
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            paving_cell_view(robdd,outside));
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            tree_left.superset(GridCell(other_grid,0u,BinaryWord{})));
        ARIADNE_TESTING_EXPECT_EXCEPTION(
            robdd_left.superset(GridCell(other_grid,0u,BinaryWord{})));
    }

};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_paving_backends");
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_import_and_explicit_tree_oracle);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_compact_leaves_exhaustive_truth_tables);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_superset_direct_no_manager_allocations);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_polymorphic_cell_batch_and_explicit_import);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_bulk_cell_construction_and_union);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_in_place_mutation_and_value_semantics);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_in_place_mutation_across_extents);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_explicit_subpaving_import_and_backend_rejection);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_native_branch_cofactors);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_geometric_queries_between_backends);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_grid_relative_mince_and_primary_cell_query);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_generic_cell_updates_across_backends);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_common_writable_cell_views);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_common_view_enabled_leaf_traversal);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_common_view_geometric_queries);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_common_view_forward_iterator);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_outer_box_approximation_between_backends);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_generic_approximations_between_backends);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_polymorphic_paving_handle_and_subpaving_views);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_polymorphic_sequences_backend_isolation);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_polymorphic_alignment_across_primary_extents);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_polymorphic_pure_algebra_with_isolated_backends);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_polymorphic_refinement_contract);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_polymorphic_enumeration_at_fineness);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_polymorphic_child_enumeration_at_fineness);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_uniform_cell_paving_contract);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_polymorphic_handle_uniform_cell_enumeration);
    ARIADNE_TESTING_ADD_TEST(TestPavingBackends, test_exact_uniform_oracle_exhaustive_and_nested_views);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc,argv);
}
