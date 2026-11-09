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
    ARIADNE_TESTING_ADD_TEST(TestCachedBinaryTreePaving,test_concurrent_queries_mutations_and_compilation);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc,argv);
}
