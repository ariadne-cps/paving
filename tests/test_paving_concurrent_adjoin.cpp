/***************************************************************************
 *            test_paving_concurrent_adjoin.cpp
 *
 *  Copyright  2026  Luca Geretti
 ****************************************************************************/
/*
 *  This file is part of Ariadne Paving.
 *  Ariadne Paving is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License, version 3 or later.
 */
#include <atomic>
#include <sstream>
#include <thread>
#include <vector>

#include "ariadne-testing.hpp"
#include "paving/grid_paving.hpp"
#include "geometry/set_wrapper.hpp"
#include "paving/robdd_paving.hpp"

using namespace Ariadne;

namespace {

std::vector<GridCell> make_cells(Grid const& grid) {
    std::vector<GridCell> cells;
    cells.reserve(128u);
    for(unsigned n=0u;n<128u;++n) {
        BinaryWord path;
        for(unsigned bit=0u;bit<7u;++bit) {
            path.push_back(((n>>(6u-bit)) & 1u) != 0u);
        }
        cells.emplace_back(grid,1u,path);
    }
    return cells;
}

template<class MakeSource>
void concurrent_adjoins(PavingInterface& destination,
                        std::vector<GridCell> const& cells,
                        MakeSource make_source) {
    constexpr unsigned workers=8u;
    constexpr unsigned cells_per_worker=16u;
    std::atomic<unsigned> ready{0u};
    std::atomic<bool> start{false};
    std::vector<std::thread> threads;
    threads.reserve(workers);
    for(unsigned worker=0u;worker<workers;++worker) {
        threads.emplace_back([&,worker] {
            ready.fetch_add(1u,std::memory_order_release);
            while(not start.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            for(unsigned round=0u;round<3u;++round) {
                for(unsigned k=0u;k<cells_per_worker;++k) {
                    auto const& cell=cells[worker*cells_per_worker+k];
                    switch((k+round)%3u) {
                        case 0u:
                            destination.adjoin(cell);
                            break;
                        case 1u:
                            destination.adjoin(std::vector<GridCell>{cell});
                            break;
                        default: {
                            auto source=make_source(cell);
                            destination.adjoin(
                                static_cast<SubPavingInterface const&>(source));
                            break;
                        }
                    }
                }
            }
        });
    }
    while(ready.load(std::memory_order_acquire)!=workers) {
        std::this_thread::yield();
    }
    start.store(true,std::memory_order_release);
    for(auto& worker:threads) { worker.join(); }
}

} // namespace

class TestPavingConcurrentAdjoin : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

    template<class Paving,class MakeSource>
    Void test_reader_writer(Paving& destination,GridCell const& sentinel,
            std::vector<GridCell> const& cells,MakeSource make_source) {
        PavingInterface& api=destination;
        api.adjoin(sentinel);
        std::atomic<bool> finished{false};
        std::atomic<bool> correct{true};
        std::thread reader([&] {
            while(not finished.load(std::memory_order_acquire)) {
                if(not api.superset(sentinel)) {
                    correct.store(false,std::memory_order_relaxed);
                }
                (void)api.size();
                if(api.root_cell().grid()!=sentinel.grid()) {
                    correct.store(false,std::memory_order_relaxed);
                }
                Paving snapshot(destination);
                if(not snapshot.superset(sentinel)) {
                    correct.store(false,std::memory_order_relaxed);
                }
            }
        });
        concurrent_adjoins(api,cells,make_source);
        finished.store(true,std::memory_order_release);
        reader.join();
        ARIADNE_TESTING_PROVE(correct.load(std::memory_order_acquire));
        for(GridCell const& cell:cells) {
            ARIADNE_TESTING_PROVE(api.superset(cell));
        }
    }

    Void test_tree_reader_writer_adjoin() {
        Grid grid(1u);
        auto cells=make_cells(grid);
        GridCell sentinel(grid,1u,make_binary_word("0000000"));
        GridTreePaving destination(grid,false);
        test_reader_writer(destination,sentinel,cells,
            [&](GridCell const& cell) { return GridTreePaving(cell); });
    }

    Void test_robdd_reader_writer_adjoin() {
        RobddManager manager;
        Grid grid(1u);
        auto cells=make_cells(grid);
        GridCell root(grid,1u,BinaryWord{});
        GridCell sentinel(grid,1u,make_binary_word("0000000"));
        RobddPaving destination(manager,root,manager.constant(false),0u);
        test_reader_writer(destination,sentinel,cells,
            [&](GridCell const& cell) {
                return RobddPaving(manager,cell,manager.constant(true),0u);
            });
    }

    template<class Paving>
    Void check_concurrent_root_mutations(Paving& paving,GridCell const& cell) {
        std::atomic<bool> complete{false};
        std::atomic<bool> coherent{true};
        std::thread reader([&] {
            while(!complete.load(std::memory_order_acquire)) {
                Paving snapshot(paving);
                if(snapshot.root_cell().grid()!=cell.grid()) {
                    coherent.store(false,std::memory_order_relaxed);
                }
                (void)paving.size();
                (void)paving.root_cell();
            }
        });
        std::thread writer([&] {
            for(unsigned i=0;i<80u;++i) {
                paving.set_root_cell(false);
                paving.mince(1u);
                paving.recombine();
                paving.adjoin(cell);
            }
        });
        writer.join();
        complete.store(true,std::memory_order_release);
        reader.join();
        ARIADNE_TESTING_PROVE(coherent.load(std::memory_order_acquire));
        ARIADNE_TESTING_PROVE(paving.superset(cell));
    }

    Void test_tree_concurrent_root_mutations() {
        Grid grid(1u);
        GridTreePaving paving(grid,false);
        check_concurrent_root_mutations(paving,
            GridCell(grid,0u,make_binary_word("0")));
    }

    Void test_robdd_concurrent_root_mutations() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        RobddPaving paving(manager,root,manager.constant(false),0u);
        check_concurrent_root_mutations(paving,
            GridCell(grid,0u,make_binary_word("0")));
    }


    template<class Paving,class MakePaving>
    Void check_concurrent_set_mutations(Paving& destination,
            GridCell const& retained,GridCell const& discarded,
            MakePaving make_paving) {
        Paving keeper=make_paving(retained);
        Paving removal=make_paving(discarded);
        destination.adjoin(retained);
        std::atomic<bool> start{false};
        std::atomic<bool> finished{false};
        std::atomic<bool> coherent{true};

        std::thread writer([&] {
            while(!start.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            for(unsigned i=0u;i<96u;++i) {
                destination.adjoin(retained);
                destination.adjoin(discarded);
            }
            finished.store(true,std::memory_order_release);
        });
        std::thread algebra([&] {
            start.store(true,std::memory_order_release);
            for(unsigned i=0u;i<96u;++i) {
                // Exercise the virtual PavingInterface dispatch as well as
                // the backend-native overload with stable source owners.
                destination.remove(static_cast<SubPavingInterface const&>(removal));
                destination.restrict(static_cast<SubPavingInterface const&>(keeper));
            }
        });
        std::thread reader([&] {
            while(!finished.load(std::memory_order_acquire)) {
                Paving snapshot(destination);
                if(!snapshot.superset(retained)
                        || snapshot.root_cell().grid()!=retained.grid()) {
                    coherent.store(false,std::memory_order_relaxed);
                }
                (void)destination.size();
            }
        });
        writer.join();
        algebra.join();
        reader.join();
        ARIADNE_TESTING_PROVE(coherent.load(std::memory_order_acquire));
        ARIADNE_TESTING_PROVE(destination.superset(retained));

        // Self-operations are algebraic identities, not recursive locks.
        destination.restrict(static_cast<SubPavingInterface const&>(destination));
        ARIADNE_TESTING_PROVE(destination.superset(retained));
        destination.remove(static_cast<SubPavingInterface const&>(destination));
        ARIADNE_TESTING_PROVE(!destination.superset(retained));
    }

    Void test_tree_concurrent_set_mutations() {
        Grid grid(1u);
        GridCell retained(grid,0u,make_binary_word("0"));
        GridCell discarded(grid,0u,make_binary_word("1"));
        GridTreePaving destination(grid,false);
        check_concurrent_set_mutations(destination,retained,discarded,
            [&](GridCell const& cell) { return GridTreePaving(cell); });
    }

    Void test_robdd_concurrent_set_mutations() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        GridCell retained(grid,0u,make_binary_word("0"));
        GridCell discarded(grid,0u,make_binary_word("1"));
        RobddPaving destination(manager,root,manager.constant(false),0u);
        check_concurrent_set_mutations(destination,retained,discarded,
            [&](GridCell const& cell) {
                return RobddPaving(manager,cell,manager.constant(true),0u);
            });
    }


    Void test_tree_snapshot_iterators_survive_concurrent_mutations() {
        Grid grid(1u);
        auto cells=make_cells(grid);
        GridTreePaving destination(grid,false);
        destination.adjoin(cells);

        auto saved_first=destination.begin();
        auto saved_end=destination.end();
        ARIADNE_TESTING_PROVE(saved_first==destination.begin());

        PavingInterface const& api=destination;
        auto saved_interface_first=api.begin();
        auto saved_interface_end=api.end();

        std::atomic<bool> start{false};
        std::thread writer([&] {
            while(!start.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            for(unsigned i=0u;i<48u;++i) {
                destination.set_root_cell(false);
                destination.adjoin(cells);
                destination.mince(7u);
                destination.recombine();
            }
        });
        start.store(true,std::memory_order_release);

        SizeType concrete_count=0u;
        for(auto it=saved_first;it!=saved_end;++it) {
            ARIADNE_TESTING_PROVE(it->grid()==grid);
            ++concrete_count;
        }
        SizeType interface_count=0u;
        for(auto it=saved_interface_first;it!=saved_interface_end;++it) {
            ARIADNE_TESTING_PROVE(it->grid()==grid);
            ++interface_count;
        }
        writer.join();
        ARIADNE_TESTING_EXPECT_EQUAL(concrete_count,static_cast<SizeType>(cells.size()));
        ARIADNE_TESTING_EXPECT_EQUAL(interface_count,static_cast<SizeType>(cells.size()));

        // The snapshot also survives destruction of the original owner.
        GridTreePaving::ConstIterator retained;
        {
            GridTreePaving ephemeral(grid,false);
            ephemeral.adjoin(cells.front());
            retained=ephemeral.begin();
        }
        ARIADNE_TESTING_PROVE(retained!=GridTreePaving::ConstIterator());
        ARIADNE_TESTING_PROVE(retained->grid()==grid);
    }

    Void test_tree_concurrent_geometric_queries() {
        Grid grid(1u);
        GridCell left(grid,0u,make_binary_word("0"));
        GridCell right(grid,0u,make_binary_word("1"));
        GridTreePaving destination(left);
        GridTreePaving left_only(left);
        ExactBoxType query=left.box();
        std::atomic<bool> finished{false};
        std::atomic<bool> correct{true};

        std::thread reader([&] {
            while(!finished.load(std::memory_order_acquire)) {
                (void)destination.bounding_box();
                (void)destination.inside(query);
                (void)destination.separated(query);
                (void)destination.overlaps(query);
                (void)destination.equals(left_only);
                (void)destination.subset(left_only);
                if(!destination.intersects(left_only)
                        || destination.is_empty()
                        || destination.dimension()!=grid.dimension()) {
                    correct.store(false,std::memory_order_relaxed);
                }
                (void)destination.measure();
                PavingInterface const& api=destination;
                (void)api.bounding_box();
                (void)api.inside(query);
                (void)api.separated(query);
                (void)api.overlaps(query);
                (void)api.equals(left_only);
                (void)api.subset(left_only);
                (void)api.intersects(left_only);
            }
        });
        std::thread writer([&] {
            for(unsigned i=0u;i<48u;++i) {
                destination.adjoin(right);
                destination.restrict(left_only);
            }
            finished.store(true,std::memory_order_release);
        });
        writer.join();
        reader.join();
        ARIADNE_TESTING_PROVE(correct.load(std::memory_order_acquire));
        ARIADNE_TESTING_PROVE(destination.equals(left_only));
        ARIADNE_TESTING_PROVE(destination.subset(left_only));
        ARIADNE_TESTING_PROVE(destination.intersects(left_only));
        ARIADNE_TESTING_PROVE(destination.equals(destination));
        ARIADNE_TESTING_PROVE(destination.subset(destination));
        ARIADNE_TESTING_PROVE(destination.intersects(destination));
    }

    Void test_tree_scoped_borrowed_mutations() {
        Grid grid(1u);
        GridCell left(grid,0u,make_binary_word("0"));
        GridCell right(grid,0u,make_binary_word("1"));
        GridTreePaving paving(grid,true);

        paving.with_locked_branch(false,[&](GridTreeSubpaving& child) {
            ARIADNE_TESTING_PROVE(child.root_cell()==left);
            child.set_root_cell(false);
        });
        ARIADNE_TESTING_PROVE(!paving.superset(left));
        ARIADNE_TESTING_PROVE(paving.superset(right));

        paving.with_locked_subpaving([&](GridTreeSubpaving& root) {
            // All borrowed descendants remain valid while the root is locked.
            GridTreeSubpaving right_child=root.branch(true);
            ARIADNE_TESTING_PROVE(right_child.root_cell()==right);
            right_child.set_root_cell(false);
        });
        ARIADNE_TESTING_PROVE(paving.is_empty());
        paving.recombine();
        ARIADNE_TESTING_PROVE(paving.is_empty());
    }

    Void test_tree_scoped_branch_with_concurrent_adjoin() {
        Grid grid(1u);
        GridCell left(grid,0u,make_binary_word("0"));
        GridCell right(grid,0u,make_binary_word("1"));
        GridTreePaving paving(grid,true);
        std::atomic<bool> entered{false};
        std::atomic<bool> release{false};

        std::thread scoped([&] {
            paving.with_locked_branch(false,[&](GridTreeSubpaving& child) {
                entered.store(true,std::memory_order_release);
                while(!release.load(std::memory_order_acquire)) {
                    std::this_thread::yield();
                }
                child.set_root_cell(false);
            });
        });
        while(!entered.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
        std::thread writer([&] {
            // Must not reallocate the borrowed nodes during the callback.
            paving.adjoin(right);
            paving.recombine();
        });
        release.store(true,std::memory_order_release);
        scoped.join();
        writer.join();

        ARIADNE_TESTING_PROVE(!paving.superset(left));
        ARIADNE_TESTING_PROVE(paving.superset(right));
    }


    Void test_tree_atomic_clear_and_geometric_approximations() {
        Grid grid(1u);
        GridCell sentinel(grid,0u,make_binary_word("0"));
        ExactBoxType box{{0.125_x,0.875_x}};
        CompactSet<EffectiveTag,RealVector> compact(box);
        OpenSet<EffectiveTag,RealVector> open(box);
        OvertSet<EffectiveTag,RealVector> overt(box);
        GridTreePaving destination(grid,false);

        std::atomic<bool> started{false};
        std::atomic<bool> finished{false};
        std::atomic<bool> coherent{true};
        std::thread reader([&] {
            started.store(true,std::memory_order_release);
            while(!finished.load(std::memory_order_acquire)) {
                GridTreePaving snapshot(destination);
                if(snapshot.root_cell().grid()!=grid
                        || snapshot.dimension()!=grid.dimension()) {
                    coherent.store(false,std::memory_order_relaxed);
                }
                (void)snapshot.size();
                (void)snapshot.bounding_box();
                for(auto it=snapshot.begin(),last=snapshot.end();it!=last;++it) {
                    if(it->grid()!=grid) {
                        coherent.store(false,std::memory_order_relaxed);
                    }
                }
                (void)destination.is_empty();
                (void)destination.size();
            }
        });
        std::thread writer([&] {
            while(!started.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            for(unsigned n=0u;n<12u;++n) {
                // Exercise all four direct tree approximation algorithms
                // and the wrapper paths, against owner snapshot readers.
                destination.adjoin_outer_approximation(compact.reference(),2u);
                destination.adjoin_lower_approximation(overt.reference(),0u,2u);
                destination.adjoin_inner_approximation(open.reference(),0u,2u);
                destination.clear();
            }
            destination.adjoin(sentinel);
            finished.store(true,std::memory_order_release);
        });
        writer.join();
        reader.join();
        ARIADNE_TESTING_PROVE(coherent.load(std::memory_order_acquire));
        ARIADNE_TESTING_PROVE(destination.superset(sentinel));

        // Clear must restore the default primary extent, even when the
        // previous owner geometry has a larger enclosing root.
        destination.adjoin(GridCell(grid,2u,BinaryWord{}));
        ARIADNE_TESTING_PROVE(destination.root_cell().root_extent()==2u);
        destination.clear();
        ARIADNE_TESTING_PROVE(destination.is_empty());
        ARIADNE_TESTING_PROVE(destination.size()==0u);
        ARIADNE_TESTING_PROVE(destination.root_cell().root_extent()==0u);
        ARIADNE_TESTING_PROVE(destination.root_cell().grid()==grid);
    }

    Void test_tree_direct_refinements_and_streaming() {
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        GridCell left(grid,0u,make_binary_word("0"));
        GridTreePaving paving(grid,true);
        std::atomic<bool> started{false};
        std::atomic<unsigned> completed{0u};
        std::atomic<bool> correct{true};

        std::thread reader([&] {
            started.store(true,std::memory_order_release);
            while(completed.load(std::memory_order_acquire)<2u) {
                GridTreePaving snapshot(paving);
                if(!snapshot.superset(root) || snapshot.root_cell().grid()!=grid) {
                    correct.store(false,std::memory_order_relaxed);
                }
                (void)paving.tree_depth();
                std::ostringstream output;
                output << paving;
                if(output.str().empty()) {
                    correct.store(false,std::memory_order_relaxed);
                }
            }
        });
        std::thread first([&] {
            while(!started.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            for(unsigned i=0u;i<32u;++i) {
                paving.mince_to_depth(2u);
                paving.mince_to_tree_depth(3u);
                paving.subdivide(ApproximateDouble(0.125));
                paving.recombine();
            }
            completed.fetch_add(1u,std::memory_order_release);
        });
        std::thread second([&] {
            while(!started.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            for(unsigned i=0u;i<32u;++i) {
                paving.mince(2u);
                paving.recombine();
            }
            completed.fetch_add(1u,std::memory_order_release);
        });
        first.join();
        second.join();
        reader.join();
        ARIADNE_TESTING_PROVE(correct.load(std::memory_order_acquire));
        ARIADNE_TESTING_PROVE(paving.superset(root));

        // The direct owner branch call must split safely and preserve the
        // existing borrowed mutability contract. Mutate only sequentially.
        GridTreeSubpaving branch=paving.branch(false);
        ARIADNE_TESTING_PROVE(branch.root_cell()==left);
        branch.set_root_cell(false);
        ARIADNE_TESTING_PROVE(!paving.superset(left));
    }

    Void test_robdd_native_algebra_with_concurrent_owner_updates() {
        RobddManager manager;
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        GridCell left(grid,0u,make_binary_word("0"));
        GridCell right(grid,0u,make_binary_word("1"));
        RobddPaving destination(manager,root,manager.constant(false),0u);
        destination.adjoin(left);
        RobddPaving left_only(manager,left,manager.constant(true),0u);
        RobddPaving right_only(manager,right,manager.constant(true),0u);
        RobddPaving full(manager,root,manager.constant(true),0u);

        std::atomic<bool> reading{false};
        std::atomic<bool> completed{false};
        std::atomic<bool> correct{true};
        std::thread reader([&] {
            reading.store(true,std::memory_order_release);
            while(!completed.load(std::memory_order_acquire)) {
                // The mutable owner always contains left; right alternates.
                // These are properties of every possible coherent snapshot.
                RobddPaving united=destination.join(right_only);
                RobddPaving common=destination.intersection(left_only);
                RobddPaving removed=destination.difference(right_only);
                if(!united.superset(left) || !united.superset(right)
                        || !common.superset(left) || common.superset(right)
                        || !removed.superset(left) || removed.superset(right)
                        || !destination.subset(full)
                        || !destination.intersects(left_only)) {
                    correct.store(false,std::memory_order_relaxed);
                }
                if(!destination.equals(
                        static_cast<SubPavingInterface const&>(destination))
                        || !destination.subset(destination)
                        || !destination.intersects(destination)
                        || !destination.difference(destination).is_empty()) {
                    correct.store(false,std::memory_order_relaxed);
                }
                RobddPaving self_union=destination.join(destination);
                RobddPaving self_intersection=destination.intersection(destination);
                if(!self_union.superset(left)
                        || !self_intersection.superset(left)) {
                    correct.store(false,std::memory_order_relaxed);
                }
            }
        });
        std::thread writer([&] {
            while(!reading.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            for(unsigned i=0u;i<80u;++i) {
                destination.adjoin(right);
                destination.restrict(left_only);
            }
            completed.store(true,std::memory_order_release);
        });
        writer.join();
        reader.join();
        ARIADNE_TESTING_PROVE(correct.load(std::memory_order_acquire));
        ARIADNE_TESTING_PROVE(destination.equals(left_only));
    }

    Void test_robdd_assignment_concurrent_single_cell_mutations() {
        // The current manager changes under assignment, while every
        // published source retains the same mandatory left cell.
        RobddManager first_manager;
        RobddManager second_manager;
        Grid grid(1u);
        GridCell left(grid,0u,make_binary_word("0"));
        GridCell right(grid,0u,make_binary_word("1"));
        RobddPaving first(first_manager,left,first_manager.constant(true),0u);
        RobddPaving second(second_manager,left,second_manager.constant(true),0u);
        RobddPaving destination(first);
        std::atomic<bool> started{false};
        std::atomic<bool> finished{false};
        std::atomic<bool> correct{true};

        std::thread reader([&] {
            started.store(true,std::memory_order_release);
            while(!finished.load(std::memory_order_acquire)) {
                destination.adjoin(left);
                destination.remove(right);
                if(destination.dimension()!=grid.dimension()
                        || destination.smallest_enclosing_primary_cell(
                            left.box()).grid()!=grid
                        || !destination.superset(left)) {
                    correct.store(false,std::memory_order_relaxed);
                }
            }
        });
        std::thread writer([&] {
            while(!started.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            for(unsigned n=0u;n<96u;++n) {
                destination=(n%2u==0u)?second:first;
            }
            finished.store(true,std::memory_order_release);
        });
        writer.join();
        reader.join();
        ARIADNE_TESTING_PROVE(correct.load(std::memory_order_acquire));
        ARIADNE_TESTING_PROVE(destination.superset(left));
    }

    Void test_grid_tree_concurrent_adjoin() {
        Grid grid(1u);
        auto cells=make_cells(grid);
        GridTreePaving destination(grid,false);
        concurrent_adjoins(destination,cells,[&](GridCell const& cell) {
            return GridTreePaving(cell);
        });
        for(GridCell const& cell:cells) {
            ARIADNE_TESTING_PROVE(destination.superset(cell));
        }
        GridTreePaving expected(grid,false);
        expected.adjoin(cells);
        ARIADNE_TESTING_PROVE(destination.equals(expected));

        // Copy/assignment after concurrent insertion preserve exact geometry.
        GridTreePaving copy(destination);
        GridTreePaving assigned(grid,false);
        assigned=destination;
        ARIADNE_TESTING_PROVE(copy.equals(expected));
        ARIADNE_TESTING_PROVE(assigned.equals(expected));
    }

    Void test_robdd_concurrent_adjoin() {
        RobddManager manager;
        Grid grid(1u);
        auto cells=make_cells(grid);
        GridCell root(grid,1u,BinaryWord{});
        RobddPaving destination(manager,root,manager.constant(false),0u);
        concurrent_adjoins(destination,cells,[&](GridCell const& cell) {
            return RobddPaving(manager,cell,manager.constant(true),0u);
        });
        for(GridCell const& cell:cells) {
            ARIADNE_TESTING_PROVE(destination.superset(cell));
        }
        RobddPaving expected(manager,root,manager.constant(false),0u);
        expected.adjoin(cells);
        ARIADNE_TESTING_PROVE(destination.equals(expected));

        RobddPaving copy(destination);
        RobddPaving assigned(manager,root,manager.constant(false),0u);
        assigned=destination;
        ARIADNE_TESTING_PROVE(copy.equals(expected));
        ARIADNE_TESTING_PROVE(assigned.equals(expected));
    }
};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_paving_concurrent_adjoin");
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_grid_tree_concurrent_adjoin);
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_robdd_concurrent_adjoin);
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_tree_reader_writer_adjoin);
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_tree_concurrent_root_mutations);
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_tree_concurrent_set_mutations);
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_tree_snapshot_iterators_survive_concurrent_mutations);
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_tree_concurrent_geometric_queries);
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_tree_scoped_borrowed_mutations);
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_tree_scoped_branch_with_concurrent_adjoin);
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_tree_atomic_clear_and_geometric_approximations);
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_tree_direct_refinements_and_streaming);
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_robdd_concurrent_set_mutations);
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_robdd_native_algebra_with_concurrent_owner_updates);
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_robdd_assignment_concurrent_single_cell_mutations);
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_robdd_concurrent_root_mutations);
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_robdd_reader_writer_adjoin);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc,argv);
}
