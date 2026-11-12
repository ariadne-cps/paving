/***************************************************************************
 *            benchmarks/benchmark_cached_binary_tree_paving.cpp
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

/** @file benchmarks/benchmark_cached_binary_tree_paving.cpp
 *  @brief Benchmark paving operations and backend implementations.
 */

#include <cmath>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "ariadne-benchmarking.hpp"
#include "logging/logging.hpp"
#include "paving/cached_binary_tree_paving.hpp"

using namespace Ariadne;
namespace Bench=Ariadne::Benchmarking;

namespace {

struct Options {
    unsigned depth=9u;
    unsigned queries=128u;
    unsigned samples=7u;
    unsigned iterations=256u;
};

GridCell at_index(GridCell cell,unsigned index,unsigned depth) {
    for(unsigned bit=depth;bit!=0u;--bit)
        cell=cell.split(((index>>(bit-1u))&1u)!=0u);
    return cell;
}

struct Prepared {
    RobddManager manager;
    std::unique_ptr<BinaryTreePaving> snapshot;
    std::unique_ptr<RobddPaving> bdd;
};

void run(Options const& p) {
    if(p.depth<2u or p.depth>13u or p.queries==0u
       or p.samples==0u or p.iterations==0u)
        throw std::invalid_argument("Invalid depth/queries/samples/iterations");

    Grid grid(2u);
    GridCell root(grid,0u,BinaryWord{});
    BinaryTreePaving tree(grid);
    for(unsigned i=0u;i<(1u<<p.depth);++i)
        if(i%3u==0u or i%11u==0u)
            tree.adjoin(at_index(root,i,p.depth));
    tree.recombine();

    std::vector<GridCell> probes{root,root.split(false),root.split(true)};
    probes.reserve(p.queries+3u);
    for(unsigned i=0u;i<p.queries;++i) {
        auto cell=at_index(root,(i*257u+31u)%(1u<<p.depth),p.depth);
        if(i%5u==0u) cell=cell.split((i&1u)!=0u);
        probes.push_back(cell);
    }

    auto median=[&](std::string const& label,auto prepare,
                    auto timed,auto verify,double work=1.0) {
        Bench::Runner runner(0u,1u);
        auto observations=runner.run(label,p.samples,prepare,timed,verify,work);
        double result=Bench::summarize(observations).median;
        if(not std::isfinite(result) or result<=0.0)
            throw std::runtime_error("Invalid cached BDD benchmark sample");
        return result;
    };

    std::unique_ptr<BinaryTreePaving> detached;
    double const snapshot_ns=median(
        "tree-snapshot",
        [&]{detached.reset();},
        [&]{detached=std::make_unique<BinaryTreePaving>(tree);},
        [&]{
            if(not detached or detached->size()!=tree.size())
                throw std::runtime_error("Invalid Tree snapshot");
        });

    // Fresh manager for every sample. Snapshot excluded from timer.
    std::unique_ptr<Prepared> encoding;
    double const encoding_ns=median(
        "tree-to-bdd-encoding",
        [&]{
            encoding=std::make_unique<Prepared>();
            encoding->snapshot=std::make_unique<BinaryTreePaving>(tree);
        },
        [&]{
            encoding->bdd=std::make_unique<RobddPaving>(
                encoding->manager,
                static_cast<BinaryTreeSubpaving const&>(*encoding->snapshot));
        },
        [&]{
            if(not encoding->bdd)
                throw std::runtime_error("Missing compiled BDD");
            for(auto const& c:probes)
                if(encoding->bdd->superset(c)!=tree.superset(c))
                    throw std::runtime_error("Tree and BDD disagree");
        });

    CachedBinaryTreePaving cache(tree);
    if(not cache.compile_cache() or not cache.has_cache())
        throw std::runtime_error("Unable to compile index");
    for(auto const& c:probes)
        if(cache.superset(c)!=tree.superset(c))
            throw std::runtime_error("Cached query differs from Tree");

    auto batch=[&](auto query) {
        SizeType hits=0u;
        for(unsigned round=0u;round<p.iterations;++round)
            for(auto const& c:probes)
                hits+=static_cast<SizeType>(query(c));
        Bench::keep_alive(hits);
    };
    double const operations=static_cast<double>(p.iterations)*probes.size();
    double const tree_ns=median(
        "tree-superset",[]{},
        [&]{batch([&](GridCell const& c){return tree.superset(c);});},
        []{},operations);
    double const bdd_ns=median(
        "cached-bdd-superset",[]{},
        [&]{batch([&](GridCell const& c){return cache.superset(c);});},
        []{},operations);
    double const savings=tree_ns-bdd_ns;
    double const compile_ns=snapshot_ns+encoding_ns;
    double const break_even=savings>0.0
        ?std::ceil(compile_ns/savings):-1.0;

    LOGGING_PRINTLN("CACHED_TREE_BDD depth="<<p.depth
        <<" probes="<<probes.size()
        <<" snapshot_ns="<<std::setprecision(10)<<snapshot_ns
        <<" encoding_ns="<<encoding_ns
        <<" compile_total_ns="<<compile_ns
        <<" tree_query_ns="<<tree_ns
        <<" bdd_query_ns="<<bdd_ns
        <<" savings_ns="<<savings
        <<" break_even_queries="<<break_even
        <<" compiled_nodes="<<cache.cached_nodes())
}
} // namespace

int main(int argc,char const* argv[]) {
    try {
        Options p;
        Bench::CommandLine cli;
        cli.add(CommandLineContribution("cached-tree-bdd",{
            make_valued_argument_parser(
                "","depth","Tree depth (2..13)",
                [&p](String const& t)->VoidFunction {
                    unsigned n=Bench::parse_unsigned<unsigned>(t);
                    return [&p,n]{p.depth=n;};
                }),
            make_valued_argument_parser(
                "","queries","Number of prepared probes",
                [&p](String const& t)->VoidFunction {
                    unsigned n=Bench::parse_unsigned<unsigned>(t);
                    return [&p,n]{p.queries=n;};
                }),
            make_valued_argument_parser(
                "","samples","Number of samples",
                [&p](String const& t)->VoidFunction {
                    unsigned n=Bench::parse_unsigned<unsigned>(t);
                    return [&p,n]{p.samples=n;};
                }),
            make_valued_argument_parser(
                "","iterations","Query batches per sample",
                [&p](String const& t)->VoidFunction {
                    unsigned n=Bench::parse_unsigned<unsigned>(t);
                    return [&p,n]{p.iterations=n;};
                })
        }));
        if(not cli.acquire(argc,argv)) return 0;
        run(p);
        return 0;
    } catch(std::exception const& error) {
        LOGGING_PRINTLN("benchmark_cached_binary_tree_paving: "<<error.what())
        return 1;
    }
}
