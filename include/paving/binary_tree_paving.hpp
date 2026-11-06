/***************************************************************************
 *            binary_tree_paving.hpp
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

#ifndef ARIADNE_PAVING_BINARY_TREE_PAVING_HPP
#define ARIADNE_PAVING_BINARY_TREE_PAVING_HPP

#include "paving/grid_paving.hpp"

namespace Ariadne {

//! \brief Binary-tree-backed implementation of a paving.
//! \details The established GridTreePaving type already provides the
//! complete PavingInterface contract and remains in use by legacy code.
//! BinaryTreePaving names that same concrete implementation during migration.
using BinaryTreePaving = GridTreePaving;

} // namespace Ariadne

#endif // ARIADNE_PAVING_BINARY_TREE_PAVING_HPP
