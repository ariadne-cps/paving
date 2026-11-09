// Backend-independent paving tutorial: one algorithm, two implementations.
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "ariadne-paving.hpp"

using namespace Ariadne;

namespace {

void require(bool condition, std::string const& message) {
    if (!condition) { throw std::runtime_error(message); }
}

SubPavingInterface const& as_subpaving(Paving const& paving) {
    return static_cast<PavingInterface const&>(paving);
}

// No concrete backend appears in this algorithm.
void demonstrate(std::string const& backend, Paving first, Paving second,
                 GridCell const& root) {
    auto left=root.split(false);
    auto right=root.split(true);
    auto left_left=left.split(false);
    auto left_right=left.split(true);
    auto right_left=right.split(false);

    first.adjoin(std::vector<GridCell>{left_left,right_left});
    second.adjoin(std::vector<GridCell>{left_right,right_left});

    Paving united=join(as_subpaving(first),as_subpaving(second));
    Paving common=intersection(as_subpaving(first),as_subpaving(second));
    Paving difference_set=difference(as_subpaving(first),as_subpaving(second));

    require(united.superset(left_left) && united.superset(left_right)
            && united.superset(right_left), "Incorrect union");
    require(common.superset(right_left) && !common.superset(left_left)
            && !common.superset(left_right), "Incorrect intersection");
    require(difference_set.superset(left_left)
            && !difference_set.superset(right_left), "Incorrect difference");

    std::cout << "\n== " << backend << " ==\n";
    std::cout << "union: " << united.size()
              << " cells, intersection: " << common.size()
              << " cells, difference: " << difference_set.size()
              << " cells\n";
    std::cout << "enabled cells:";
    for (auto it=united.begin(), end=united.end(); it!=end; ++it) {
        std::cout << ' ' << *it;
    }
    std::cout << '\n';

    // A Paving copy is independent. A SubPaving is a live view of its owner.
    Paving snapshot=first;
    SubPaving left_view=as_subpaving(first).branch(false);
    left_view.branch(false).set_root_cell(false);
    require(!first.superset(left_left), "SubPaving did not modify its owner");
    require(snapshot.superset(left_left), "Copy unexpectedly changed");
    std::cout << "live view modified original; copy remained unchanged\n";
}

} // namespace

int main() {
    Grid grid(1u);
    GridCell root(grid,0u,BinaryWord(""));

    demonstrate("Binary tree",
                Paving(new BinaryTreePaving(grid)),
                Paving(new BinaryTreePaving(grid)),root);

    // Both ROBDD-backed pavings use one canonical manager.
    // Paving and detached iterators retain its lifetime.
    auto manager=std::make_shared<RobddManager>();
    Paving first(new RobddPaving(manager,grid));
    Paving second(new RobddPaving(manager,grid));
    manager.reset(); // Ownership now lives in the two pavings.
    demonstrate("ROBDD (shared manager)",first,second,root);
    return 0;
}
