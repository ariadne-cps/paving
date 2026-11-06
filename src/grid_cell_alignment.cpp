/***************************************************************************
 *            grid_cell_alignment.cpp
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

#include "utility/macros.hpp"
#include "paving/grid_cell_alignment.hpp"

namespace Ariadne {

BinaryWord cell_path_at_extent(GridCell const& cell, Nat extent) {
    ARIADNE_PRECONDITION_MSG(extent >= cell.root_extent(),
        "Cannot align a cell to a smaller primary-cell extent");
    BinaryWord path = GridCell::primary_cell_path(
        cell.grid().dimension(),extent,cell.root_extent());
    path.append(cell.word());
    return path;
}

GridCell common_root_cell(GridCell const& first, GridCell const& second) {
    ARIADNE_PRECONDITION_MSG(first.grid() == second.grid(),
        "Cannot align cells from different grids");
    Nat extent = std::max(first.root_extent(),second.root_extent());
    BinaryWord lhs = cell_path_at_extent(first,extent);
    BinaryWord rhs = cell_path_at_extent(second,extent);
    BinaryWord common;
    SizeType limit = std::min(lhs.size(),rhs.size());
    for (SizeType i=0u; i<limit and lhs[i]==rhs[i]; ++i) {
        common.push_back(lhs[i]);
    }
    return GridCell(first.grid(),extent,common);
}

BinaryWord relative_cell_path(
        GridCell const& ancestor, GridCell const& descendant) {
    ARIADNE_PRECONDITION_MSG(ancestor.grid() == descendant.grid(),
        "Cannot compute relative cell paths across different grids");
    BinaryWord path = cell_path_at_extent(
        descendant,ancestor.root_extent());
    ARIADNE_PRECONDITION_MSG(ancestor.word().is_prefix(path),
        "The first cell must contain the second cell");
    BinaryWord suffix;
    for (SizeType i=ancestor.word().size(); i<path.size(); ++i) {
        suffix.push_back(path[i]);
    }
    return suffix;
}

} // namespace Ariadne
