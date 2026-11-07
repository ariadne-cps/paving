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
#include <thread>
#include <vector>

#include "ariadne-testing.hpp"
#include "paving/grid_paving.hpp"
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
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_robdd_concurrent_root_mutations);
    ARIADNE_TESTING_ADD_TEST(TestPavingConcurrentAdjoin,test_robdd_reader_writer_adjoin);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc,argv);
}
