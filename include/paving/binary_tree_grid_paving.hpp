/***************************************************************************
 *            binary_tree_grid_paving.hpp
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

#ifndef ARIADNE_PAVING_BINARY_TREE_GRID_PAVING_HPP
#define ARIADNE_PAVING_BINARY_TREE_GRID_PAVING_HPP

#include "paving/grid_paving.hpp"

namespace Ariadne {

//! \brief Explicit binary-tree implementation of a grid paving.
//! \details This is the public representation-specific name for the
//! existing GridTreePaving implementation. The alias preserves source
//! compatibility while the two storage backends are brought to parity.
//! GridTreePaving and BinaryTreeGridPaving denote exactly the same type.
using BinaryTreeGridPaving = GridTreePaving;

} // namespace Ariadne

#endif // ARIADNE_PAVING_BINARY_TREE_GRID_PAVING_HPP
