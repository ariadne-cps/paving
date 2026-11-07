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
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc,argv);
}
