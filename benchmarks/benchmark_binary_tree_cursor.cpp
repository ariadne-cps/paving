/***************************************************************************
 *            benchmark_binary_tree_cursor.cpp
 *
 *  Isolated traversal profiling on prebuilt deterministic binary trees.
 *  This file is part of Ariadne Paving, GPL-3.0-or-later.
 ***************************************************************************/

#include "paving/binary_tree_paving.hpp"
#include "tree/binary_tree.hpp"
#include "logging/logging.hpp"
#include "threading/threading_command_line_interface.hpp"
#include "benchmark_command_line.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

using namespace Ariadne;
namespace CLI = Ariadne::PavingBenchmarkCommandLine;

namespace {

struct Selection {
    std::vector<unsigned> dimensions{2u,4u};
    std::vector<unsigned> depths{8u,12u,16u};
    std::vector<unsigned> strides{2u,16u};
    unsigned samples=7u;
    unsigned min_sample_ms=12u;
    unsigned max_iterations=32768u;
    bool verify_only=false;
};

List<ArgumentParser> parsers(Selection& selection) {
    List<ArgumentParser> result;
    result.append(make_valued_argument_parser(
        "d","dimensions","Comma-separated grid dimensions (1..8)",
        [&selection](String const& value)->VoidFunction {
            auto values=CLI::parse_unique_unsigned_list(value,1u);
            if(std::any_of(values.begin(),values.end(),
                    [](unsigned v) { return v>8u; }))
                throw std::invalid_argument("dimension must be <= 8");
            return [&selection,values] { selection.dimensions=values; };
        }));
    result.append(make_valued_argument_parser(
        "","depths","Comma-separated full tree depths (1..18)",
        [&selection](String const& value)->VoidFunction {
            auto values=CLI::parse_unique_unsigned_list(value,1u);
            if(std::any_of(values.begin(),values.end(),
                    [](unsigned v) { return v>18u; }))
                throw std::invalid_argument("depth must be <= 18");
            return [&selection,values] { selection.depths=values; };
        }));
    result.append(make_valued_argument_parser(
        "","strides","Enable leaf zero and every Nth leaf (positive)",
        [&selection](String const& value)->VoidFunction {
            auto values=CLI::parse_unique_unsigned_list(value,1u);
            return [&selection,values] { selection.strides=values; };
        }));
    result.append(make_valued_argument_parser(
        "n","samples","Odd number of samples, at least 3",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            if(n<3u or n%2u==0u)
                throw std::invalid_argument("samples must be odd and >= 3");
            return [&selection,n] { selection.samples=n; };
        }));
    result.append(make_valued_argument_parser(
        "","min-sample-ms","Minimum calibrated sample duration in ms",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            if(n==0u or n>10000u)
                throw std::invalid_argument("min-sample-ms out of range");
            return [&selection,n] { selection.min_sample_ms=n; };
        }));
    result.append(make_valued_argument_parser(
        "","max-iterations","Maximum calibrated repetitions",
        [&selection](String const& value)->VoidFunction {
            unsigned n=CLI::parse_unsigned<unsigned>(value);
            if(n==0u)
                throw std::invalid_argument("max-iterations must be positive");
            return [&selection,n] { selection.max_iterations=n; };
        }));
    result.append(make_unvalued_argument_parser(
        "","verify-only","Check cell sequences without timing",
        [&selection] { selection.verify_only=true; }));
    return result;
}

// Build the physical tree outside timing. Disabled branches stay present.
void fill_tree(BinaryTreeNode& node,unsigned remaining,
               std::uint64_t leaf_index,unsigned stride) {
    if(remaining==0u) {
        if(leaf_index%stride==0u) node.set_enabled();
        return;
    }
    node.split();
    fill_tree(*node.left_node(),remaining-1u,leaf_index*2u,stride);
    fill_tree(*node.right_node(),remaining-1u,leaf_index*2u+1u,stride);
}

template<class Visitor>
void raw_dfs(BinaryTreeNode const& node,BinaryWord& path,Visitor& visitor) {
    if(node.is_leaf()) {
        if(node.is_enabled()) visitor(path);
        return;
    }
    path.push_back(false);
    raw_dfs(*node.left_node(),path,visitor);
    path.pop_back();
    path.push_back(true);
    raw_dfs(*node.right_node(),path,visitor);
    path.pop_back();
}

template<class Visitor>
void visit_raw(BinaryTreeNode const& root,Visitor&& visitor) {
    BinaryWord path;
    raw_dfs(root,path,visitor);
}

// Replicate production navigate_to/find_next_enabled_leaf control flow.
// Both instantiations use identical navigation; only GridCell recomputation
// differs. This is comparative profiling, not an instrumented CPU breakdown.
template<bool RebuildGeometry>
class DiagnosticCursor {
    GridCell const& _root;
    std::vector<BinaryTreeNode const*> _stack;
    BinaryWord _path;
    std::optional<GridCell> _cell;
    bool _at_end=false;

    bool at_root() const { return _stack.size()==1u; }
    bool is_left_child() const {
        return _stack.size()>1u &&
            _stack[_stack.size()-2u]->left_node()==_stack.back();
    }
    bool is_right_child() const {
        return _stack.size()>1u &&
            _stack[_stack.size()-2u]->right_node()==_stack.back();
    }
    void update_geometry() {
        if constexpr(RebuildGeometry) {
            *_cell=GridCell(_root.grid(),_root.root_extent(),_path);
        }
    }
    void move_up() {
        if(at_root()) throw std::logic_error("diagnostic cursor above root");
        _stack.pop_back();
        _path.pop_back();
        update_geometry();
    }
    void move_down(bool right) {
        if(_stack.back()->is_leaf())
            throw std::logic_error("diagnostic cursor below leaf");
        _stack.push_back(right ? _stack.back()->right_node()
                               : _stack.back()->left_node());
        _path.push_back(right);
        update_geometry();
    }
    bool navigate_to(bool first) {
        bool found=false;
        if(_stack.back()->is_leaf()) {
            found=_stack.back()->is_enabled();
        } else {
            move_down(!first);
            found=navigate_to(first);
            if(!found) {
                move_down(first);
                found=navigate_to(first);
            }
        }
        if(!found && !at_root()) move_up();
        return found;
    }
    void find_next_enabled_leaf() {
        if(is_left_child()) {
            move_up();
            move_down(true);
            if(!navigate_to(true)) find_next_enabled_leaf();
        } else if(is_right_child()) {
            move_up();
            find_next_enabled_leaf();
        } else {
            _at_end=true;
        }
    }

public:
    DiagnosticCursor(BinaryTreeNode const& root,GridCell const& cell)
        : _root(cell),_stack{&root},_path(cell.word()) {
        _stack.reserve(24u);
        if constexpr(RebuildGeometry) _cell.emplace(cell);
        _at_end=!navigate_to(true);
    }
    bool at_end() const { return _at_end; }
    BinaryWord const& path() const { return _path; }
    void next() { if(!_at_end) find_next_enabled_leaf(); }
};

template<bool RebuildGeometry,class Visitor>
void visit_diagnostic(BinaryTreeNode const& root,GridCell const& cell,
                      Visitor&& visitor) {
    DiagnosticCursor<RebuildGeometry> cursor(root,cell);
    while(!cursor.at_end()) {
        visitor(cursor.path());
        cursor.next();
    }
}

template<class Visitor>
void visit_production(BinaryTreeSubpaving const& view,Visitor&& visitor) {
    for(auto it=view.begin(),end=view.end();it!=end;++it) {
        visitor(it->word());
    }
}

template<class Walk>
std::vector<BinaryWord> all_paths(Walk&& walk) {
    std::vector<BinaryWord> result;
    walk([&](BinaryWord const& path) { result.push_back(path); });
    return result;
}

template<class Walk>
SizeType checksum(Walk&& walk) {
    SizeType sum=0u;
    walk([&](BinaryWord const& path) { sum+=path.size()+1u; });
    return sum;
}

struct Timing { double milliseconds; unsigned iterations; };

volatile SizeType timing_sink=0u;
template<class Work>
Timing median_ms(Work&& work,Selection const& selection) {
    using Clock=std::chrono::steady_clock;
    auto run=[&](unsigned count) {
        auto start=Clock::now();
        for(unsigned i=0u;i<count;++i) timing_sink=work();
        return std::chrono::duration<double,std::milli>(
            Clock::now()-start).count();
    };
    unsigned iterations=1u;
    double elapsed=run(iterations);
    while(elapsed<selection.min_sample_ms &&
          iterations<selection.max_iterations) {
        iterations=iterations>selection.max_iterations/2u
            ?selection.max_iterations:iterations*2u;
        elapsed=run(iterations);
    }
    std::vector<double> samples;
    samples.reserve(selection.samples);
    for(unsigned i=0u;i<selection.samples;++i) {
        double value=run(iterations)/iterations;
        if(!std::isfinite(value) || value<=0.0)
            throw std::runtime_error("invalid cursor timing");
        samples.push_back(value);
    }
    std::sort(samples.begin(),samples.end());
    return {samples[samples.size()/2u],iterations};
}

void profile_case(Selection const& selection,unsigned dimension,
                  unsigned depth,unsigned stride) {
    Grid grid(dimension);
    auto tree=std::make_unique<BinaryTreeNode>(false);
    fill_tree(*tree,depth,0u,stride);
    BinaryTreePaving paving(grid,0u,tree.release()); // takes ownership
    BinaryTreeSubpaving const& view=paving;
    BinaryTreeNode const& root=*view.binary_tree();
    GridCell root_cell=view.root_cell();

    auto baseline=all_paths([&](auto&& emit) {
        visit_production(view,emit);
    });
    auto raw=all_paths([&](auto&& emit) { visit_raw(root,emit); });
    auto searching=all_paths([&](auto&& emit) {
        visit_diagnostic<false>(root,root_cell,emit);
    });
    auto reconstructing=all_paths([&](auto&& emit) {
        visit_diagnostic<true>(root,root_cell,emit);
    });
    if(baseline.empty() || baseline!=raw || baseline!=searching ||
       baseline!=reconstructing) {
        throw std::runtime_error("cursor diagnostics disagree with production");
    }
    SizeType expected=0u;
    for(auto const& path:baseline) expected+=path.size()+1u;

    std::uint64_t nodes=(std::uint64_t{1}<<(depth+1u))-1u;
    if(selection.verify_only) {
        std::cout<<"VERIFIED d="<<dimension<<" depth="<<depth
                 <<" stride="<<stride<<" nodes="<<nodes
                 <<" enabled="<<baseline.size()<<" checksum="<<expected<<"\n";
        return;
    }
    auto raw_work=[&] {
        return checksum([&](auto&& emit) { visit_raw(root,emit); });
    };
    auto search_work=[&] {
        return checksum([&](auto&& emit) {
            visit_diagnostic<false>(root,root_cell,emit);
        });
    };
    auto geom_work=[&] {
        return checksum([&](auto&& emit) {
            visit_diagnostic<true>(root,root_cell,emit);
        });
    };
    auto prod_work=[&] {
        return checksum([&](auto&& emit) { visit_production(view,emit); });
    };
    if(raw_work()!=expected || search_work()!=expected ||
       geom_work()!=expected || prod_work()!=expected) {
        throw std::runtime_error("timed traversal checksum mismatch");
    }

    // No tree construction or owning snapshot occurs inside timings.
    Timing raw_time=median_ms(raw_work,selection);
    Timing search_time=median_ms(search_work,selection);
    Timing geom_time=median_ms(geom_work,selection);
    Timing prod_time=median_ms(prod_work,selection);
    std::cout<<std::right
             <<std::setw(3)<<dimension<<" "
             <<std::setw(5)<<depth<<" "
             <<std::setw(6)<<stride<<" "
             <<std::setw(8)<<nodes<<" "
             <<std::setw(8)<<baseline.size()<<" "
             <<std::fixed<<std::setprecision(5)
             <<std::setw(12)<<raw_time.milliseconds<<" "
             <<std::setw(12)<<search_time.milliseconds<<" "
             <<std::setw(12)<<geom_time.milliseconds<<" "
             <<std::setw(12)<<prod_time.milliseconds<<" "
             <<std::setprecision(3)
             <<std::setw(9)<<geom_time.milliseconds/search_time.milliseconds<<" "
             <<std::setw(9)<<prod_time.milliseconds/search_time.milliseconds
             <<"\n";
}

} // namespace

int main(int argc,char const* argv[]) {
    try {
        Logger::instance().configuration().set_verbosity(1u);
        Selection selection;
        ThreadingCommandLineInterface command_line;
        command_line.add(CommandLineContribution(
            "binary-tree-cursor-benchmark",parsers(selection)));
        List<String> args;
        for(int i=0;i<argc;++i) args.append(String(argv[i]));
        if(!command_line.acquire(args))
            return CLI::requests_help(args)?0:1;

        if(!selection.verify_only) {
            std::cout<<"Prebuilt full trees; borrowed single-threaded iteration.\n"
                     <<"Median calibrated ms/traversal, no owner snapshots.\n"
                     <<"RawDFS: topology walk; Search: production-style leaf search\n"
                     <<"without geometry; Geometry: same search + GridCell\n"
                     <<"reconstruction per move; Borrowed: actual iterator.\n"
                     <<"These columns are comparative, not additive CPU costs.\n"
                     <<"  D Depth Stride    Nodes  Enabled"
                     <<"   RawDFS(ms)   Search(ms) Geometry(ms) Borrowed(ms)"
                     <<" Geo/Search Prod/Search\n";
        }
        for(unsigned d:selection.dimensions)
            for(unsigned depth:selection.depths)
                for(unsigned stride:selection.strides)
                    profile_case(selection,d,depth,stride);
        return 0;
    } catch(std::exception const& error) {
        std::cerr<<"benchmark_binary_tree_cursor: "<<error.what()<<"\n";
        return 1;
    }
}
