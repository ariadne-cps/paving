/***************************************************************************
 *            test_cached_binary_tree_paving.cpp
 *
 *  Explicit Tree-to-ROBDD query cache, invalidation and concurrency.
 *  Part of Ariadne Paving, GPL-3.0-or-later.
 ****************************************************************************/
#include <atomic>
#include <thread>
#include <vector>

#include "ariadne-testing.hpp"
#include "paving/cached_binary_tree_paving.hpp"
#include "paving/augmented_paving.hpp"
#include "set/function_set.hpp"
#include "geometry/set_wrapper.hpp"

using namespace Ariadne;

namespace {

GridCell cell_at(GridCell const& root,unsigned value,unsigned depth) {
    GridCell result=root;
    for(unsigned bit=depth;bit>0u;--bit)
        result=result.split(((value>>(bit-1u))&1u)!=0u);
    return result;
}

} // namespace

class TestCachedBinaryTreePaving : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

    template<class Mutation> Void verify_mutation(Mutation mutate) {
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        BinaryTreePaving source(grid);
        source.adjoin(root.split(false));
        CachedBinaryTreePaving cached(source);
        BinaryTreePaving expected(source);
        ARIADNE_TESTING_PROVE(cached.compile_cache());
        auto previous=cached.version();
        mutate(cached);
        mutate(expected);
        ARIADNE_TESTING_PROVE(!cached.has_cache());
        ARIADNE_TESTING_PROVE(cached.version()>previous);
        for(unsigned index=0u;index<8u;++index) {
            auto probe=cell_at(root,index,3u);
            ARIADNE_TESTING_EXPECT_EQUAL(cached.superset(probe),expected.superset(probe));
        }
        ARIADNE_TESTING_PROVE(cached.compile_cache());
        for(unsigned index=0u;index<8u;++index) {
            auto probe=cell_at(root,index,3u);
            ARIADNE_TESTING_EXPECT_EQUAL(cached.superset(probe),expected.superset(probe));
        }
    }

    Void test_every_mutation_invalidates_the_query_index() {
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        std::vector<GridCell> cells{root.split(true)};
        BinaryTreePaving other(grid);
        other.adjoin(root.split(true));
        ExactBoxType box{{0.125_x,0.875_x}};
        ExactBoxSetType regular(box);
        OvertSet<EffectiveTag,RealVector> overt(box);
        OpenSet<EffectiveTag,RealVector> open(box);
        LocatedSet<EffectiveTag,RealVector> located(box);
        ValidatedConstrainedImageSet validated(box);
        verify_mutation([&](auto& p){ p.adjoin(cells); });
        verify_mutation([&](auto& p){ p.adjoin(static_cast<SubPavingInterface const&>(other)); });
        verify_mutation([&](auto& p){ p.restrict(static_cast<SubPavingInterface const&>(other)); });
        verify_mutation([&](auto& p){ p.remove(static_cast<SubPavingInterface const&>(other)); });
        verify_mutation([&](auto& p){ p.adjoin(static_cast<BinaryTreeSubpaving const&>(other)); });
        verify_mutation([&](auto& p){ p.restrict(static_cast<BinaryTreeSubpaving const&>(other)); });
        verify_mutation([&](auto& p){ p.remove(static_cast<BinaryTreeSubpaving const&>(other)); });
        verify_mutation([&](auto& p){ p.adjoin_outer_approximation(validated,2u); });
        verify_mutation([&](auto& p){ p.adjoin_outer_approximation(widen(box),2u); });
        verify_mutation([&](auto& p){ p.adjoin_inner_approximation(open.reference(),0u,2u); });
        verify_mutation([&](auto& p){ p.adjoin_inner_approximation(regular,2u); });
        verify_mutation([&](auto& p){ p.mince_to_depth(3u); });
        verify_mutation([&](auto& p){ p.mince_to_tree_depth(3u); });
        verify_mutation([&](auto& p){ p.subdivide(0.25); });
        verify_mutation([&](auto& p){ p.adjoin_over_approximation(box,2u); });
        verify_mutation([&](auto& p){ p.adjoin_lower_approximation(overt.reference(),box,2u); });
        verify_mutation([&](auto& p){ p.adjoin_lower_approximation(located.reference(),2u); });
        verify_mutation([&](auto& p){ p.adjoin_inner_approximation(open.reference(),box,2u); });
        verify_mutation([&](auto& p){ p.adjoin_inner_approximation(LowerBoxType({{0.125_x,0.875_x}}),2u); });
    }

    Void test_box_queries_and_borrowed_branches() {
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        BinaryTreePaving source(grid);
        source.adjoin(root.split(false));
        CachedBinaryTreePaving manual(source),automatic(source,true);
        auto box=root.split(false).box();
        ARIADNE_TESTING_EXPECT_EQUAL(manual.superset(box),source.superset(box));
        ARIADNE_TESTING_PROVE(!manual.has_cache());
        ARIADNE_TESTING_PROVE(manual.compile_cache());
        ARIADNE_TESTING_EXPECT_EQUAL(manual.superset(box),source.superset(box));
        ARIADNE_TESTING_EXPECT_EQUAL(automatic.superset(box),source.superset(box));
        ARIADNE_TESTING_PROVE(automatic.has_cache());
        auto branch=manual.branch(false);
        ARIADNE_TESTING_PROVE(!manual.has_cache());
        ARIADNE_TESTING_PROVE(branch.superset(root.split(false)));
        ARIADNE_TESTING_PROVE(manual.compile_cache());
        SubPaving abstract=static_cast<SubPavingInterface const&>(manual).branch(true);
        ARIADNE_TESTING_PROVE(!manual.has_cache());
        ARIADNE_TESTING_PROVE(abstract.begin()==abstract.end());
    }

    Void test_polymorphic_backend_and_automatic_cache() {
        Grid grid(1u);
        GridCell root(grid,0u,BinaryWord{});
        Paving handle(new CachedBinaryTreePaving(grid,false,true));
        auto& cached=dynamic_cast<CachedBinaryTreePaving&>(
            static_cast<PavingInterface&>(handle));
        ARIADNE_TESTING_PROVE(not cached.has_cache());
        handle.adjoin(root.split(false));
        ARIADNE_TESTING_PROVE(not cached.has_cache());
        ARIADNE_TESTING_PROVE(handle.superset(root.split(false)));
        ARIADNE_TESTING_PROVE(cached.has_cache());
        Paving clone(handle);
        auto& cloned=dynamic_cast<CachedBinaryTreePaving&>(
            static_cast<PavingInterface&>(clone));
        ARIADNE_TESTING_PROVE(cloned.automatic_cache());
        ARIADNE_TESTING_PROVE(not cloned.has_cache());
        handle.adjoin(root.split(true));
        ARIADNE_TESTING_PROVE(not cached.has_cache());
        ARIADNE_TESTING_PROVE(handle.superset(root));
        ARIADNE_TESTING_PROVE(cached.has_cache());
        ARIADNE_TESTING_PROVE(not clone.superset(root));
        ARIADNE_TESTING_PROVE(cloned.has_cache());
        handle.remove(root.split(false));
        ARIADNE_TESTING_PROVE(not cached.has_cache());
        ARIADNE_TESTING_PROVE(handle.superset(root.split(true)));
        ARIADNE_TESTING_PROVE(cached.has_cache());
        PavingInterface& abstract=cached;
        abstract.mince(3u);
        ARIADNE_TESTING_PROVE(not cached.has_cache());
        abstract.recombine();
        ARIADNE_TESTING_PROVE(not cached.has_cache());
        abstract.restrict_to_extent(0u);
        ARIADNE_TESTING_PROVE(not cached.has_cache());
        ARIADNE_TESTING_PROVE(handle.superset(root.split(true)));
        ARIADNE_TESTING_PROVE(cached.has_cache());
        abstract.set_root_cell(false);
        ARIADNE_TESTING_PROVE(not cached.has_cache());
        ARIADNE_TESTING_PROVE(handle.is_empty());
    }

    Void test_augmented_reachability_storage_approximations() {
        Grid grid(1u);
        Mapping mapping=Mapping::identity(1u);
        AugmentedPaving storage(
            Paving(new CachedBinaryTreePaving(grid,false,true)),mapping);
        auto& backend=dynamic_cast<CachedBinaryTreePaving&>(
            static_cast<PavingInterface&>(storage.state_paving()));
        ARIADNE_TESTING_PROVE(backend.automatic_cache());
        ARIADNE_TESTING_PROVE(not backend.has_cache());

        ExactBoxType box{{0.125_x,0.875_x}};
        OvertSet<EffectiveTag,RealVector> overt(box);
        CompactSet<EffectiveTag,RealVector> compact(box);
        BinaryTreePaving expected(grid);

        storage.adjoin_lower_approximation(overt.reference(),0u,3u);
        expected.adjoin_lower_approximation(overt.reference(),0u,3u);
        ARIADNE_TESTING_PROVE(subset(storage.state_paving(),expected));
        ARIADNE_TESTING_PROVE(subset(expected,storage.state_paving()));
        (void)storage.state_paving().superset(GridCell(grid,0u,BinaryWord{}));
        ARIADNE_TESTING_PROVE(backend.has_cache());

        storage.adjoin_outer_approximation(compact.reference(),2u);
        expected.adjoin_outer_approximation(compact.reference(),2u);
        ARIADNE_TESTING_PROVE(not backend.has_cache());
        ARIADNE_TESTING_PROVE(subset(storage.state_paving(),expected));
        ARIADNE_TESTING_PROVE(subset(expected,storage.state_paving()));
        (void)storage.state_paving().superset(GridCell(grid,0u,BinaryWord{}));
        ARIADNE_TESTING_PROVE(backend.has_cache());

        storage.restrict_to_extent(0u);
        expected.restrict_to_extent(0u);
        ARIADNE_TESTING_PROVE(not backend.has_cache());
        ARIADNE_TESTING_PROVE(subset(storage.state_paving(),expected));
        ARIADNE_TESTING_PROVE(subset(expected,storage.state_paving()));

        AugmentedPaving copy(storage);
        auto& cloned=dynamic_cast<CachedBinaryTreePaving&>(
            static_cast<PavingInterface&>(copy.state_paving()));
        ARIADNE_TESTING_PROVE(cloned.automatic_cache());
        storage.clear();
        ARIADNE_TESTING_PROVE(storage.is_empty());
        ARIADNE_TESTING_PROVE(not copy.is_empty());
    }

    Void test_compile_equivalence_and_version_invalidation() {
        Grid grid(2u);
        GridCell root(grid,0u,BinaryWord{});
        CachedBinaryTreePaving hybrid(grid);
        ARIADNE_TESTING_PROVE(not hybrid.has_cache());
        ARIADNE_TESTING_EXPECT_EQUAL(hybrid.cached_nodes(),0u);
        for(unsigned i=0u;i<64u;++i)
            if(i%3u==0u or i%11u==0u)
                hybrid.adjoin(cell_at(root,i,6u));
        auto oracle=hybrid.tree_snapshot();
        std::vector<GridCell> probes{root,root.split(false),root.split(true)};
        for(unsigned i=0u;i<64u;++i) {
            auto cell=cell_at(root,i,6u);
            probes.push_back(cell);
            probes.push_back(cell.split(i%2u==0u));
        }
        for(auto const& cell:probes)
            ARIADNE_TESTING_EXPECT_EQUAL(hybrid.superset(cell),oracle->superset(cell));

        auto const first_version=hybrid.version();
        ARIADNE_TESTING_PROVE(hybrid.compile_cache());
        ARIADNE_TESTING_PROVE(hybrid.has_cache());
        ARIADNE_TESTING_PROVE(hybrid.cached_nodes()>0u);
        ARIADNE_TESTING_PROVE(hybrid.compile_cache());
        ARIADNE_TESTING_EXPECT_EQUAL(hybrid.version(),first_version);
        for(auto const& cell:probes)
            ARIADNE_TESTING_EXPECT_EQUAL(hybrid.superset(cell),oracle->superset(cell));

        // A mutation must invalidate even when the covered set is unchanged.
        hybrid.recombine();
        ARIADNE_TESTING_PROVE(not hybrid.has_cache());
        ARIADNE_TESTING_EXPECT_EQUAL(hybrid.version(),first_version+1u);
        for(auto const& cell:probes)
            ARIADNE_TESTING_EXPECT_EQUAL(hybrid.superset(cell),oracle->superset(cell));

        auto newly_enabled=cell_at(root,1u,6u);
        hybrid.adjoin(newly_enabled);
        ARIADNE_TESTING_PROVE(not hybrid.has_cache());
        ARIADNE_TESTING_PROVE(hybrid.superset(newly_enabled));
        ARIADNE_TESTING_PROVE(hybrid.compile_cache());
        ARIADNE_TESTING_PROVE(hybrid.superset(newly_enabled));

        hybrid.remove(newly_enabled);
        ARIADNE_TESTING_PROVE(not hybrid.has_cache());
        ARIADNE_TESTING_PROVE(not hybrid.superset(newly_enabled));
        ARIADNE_TESTING_PROVE(hybrid.compile_cache());
        ARIADNE_TESTING_PROVE(not hybrid.superset(newly_enabled));

        hybrid.clear();
        ARIADNE_TESTING_PROVE(not hybrid.has_cache());
        for(auto const& cell:probes)
            ARIADNE_TESTING_PROVE(not hybrid.superset(cell));
        ARIADNE_TESTING_PROVE(hybrid.compile_cache());
        ARIADNE_TESTING_PROVE(not hybrid.superset(root));
    }

    Void test_wrong_grid_and_snapshot_detachment() {
        Grid grid(2u);
        GridCell root(grid,0u,BinaryWord{});
        CachedBinaryTreePaving hybrid(grid);
        hybrid.adjoin(root.split(false));
        auto detached=hybrid.tree_snapshot();
        ARIADNE_TESTING_PROVE(hybrid.compile_cache());
        GridCell foreign(Grid(3u),0u,BinaryWord{});
        ARIADNE_TESTING_EXPECT_EXCEPTION(hybrid.superset(foreign));
        hybrid.adjoin(root.split(true));
        ARIADNE_TESTING_PROVE(not hybrid.has_cache());
        ARIADNE_TESTING_PROVE(hybrid.superset(root));
        ARIADNE_TESTING_PROVE(not detached->superset(root));
        ARIADNE_TESTING_PROVE(hybrid.compile_cache());
        ARIADNE_TESTING_PROVE(hybrid.superset(root));
    }

    Void test_concurrent_queries_mutations_and_compilation() {
        Grid grid(2u);
        GridCell root(grid,0u,BinaryWord{});
        CachedBinaryTreePaving hybrid(grid);
        GridCell const sentinel=cell_at(root,0u,7u);
        hybrid.adjoin(sentinel);
        for(unsigned i=1u;i<128u;++i)
            if(i%3u==0u)
                hybrid.adjoin(cell_at(root,i,7u));
        ARIADNE_TESTING_PROVE(hybrid.compile_cache());

        std::atomic<bool> correct{true};
        std::thread writer([&] {
            for(unsigned i=0u;i<128u;++i) {
                hybrid.adjoin(cell_at(root,i,7u));
                if(i%7u==0u) hybrid.recombine();
            }
        });
        std::thread compiler([&] {
            for(unsigned i=0u;i<32u;++i) {
                try { (void)hybrid.compile_cache(); }
                catch(...) { correct.store(false,std::memory_order_relaxed); }
            }
        });
        std::thread reader([&] {
            for(unsigned i=0u;i<1024u;++i) {
                if(not hybrid.superset(sentinel))
                    correct.store(false,std::memory_order_relaxed);
                (void)hybrid.superset(cell_at(root,i%128u,7u));
                (void)hybrid.cached_nodes();
                (void)hybrid.has_cache();
                (void)hybrid.version();
            }
        });
        writer.join();
        compiler.join();
        reader.join();
        ARIADNE_TESTING_PROVE(correct.load(std::memory_order_relaxed));
        // A final synchronous compile is not allowed to publish old data.
        ARIADNE_TESTING_PROVE(hybrid.compile_cache());
        for(unsigned i=0u;i<128u;++i)
            ARIADNE_TESTING_PROVE(hybrid.superset(cell_at(root,i,7u)));
    }
};

int main(int argc,char** argv) {
    ARIADNE_TESTING_SUITE("test_cached_binary_tree_paving");
    ARIADNE_TESTING_ADD_TEST(TestCachedBinaryTreePaving,test_compile_equivalence_and_version_invalidation);
    ARIADNE_TESTING_ADD_TEST(TestCachedBinaryTreePaving,test_wrong_grid_and_snapshot_detachment);
    ARIADNE_TESTING_ADD_TEST(TestCachedBinaryTreePaving,test_polymorphic_backend_and_automatic_cache);
    ARIADNE_TESTING_ADD_TEST(TestCachedBinaryTreePaving,test_augmented_reachability_storage_approximations);
    ARIADNE_TESTING_ADD_TEST(TestCachedBinaryTreePaving,test_concurrent_queries_mutations_and_compilation);
    ARIADNE_TESTING_ADD_TEST(TestCachedBinaryTreePaving,test_every_mutation_invalidates_the_query_index);
    ARIADNE_TESTING_ADD_TEST(TestCachedBinaryTreePaving,test_box_queries_and_borrowed_branches);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc,argv);
}
