/***************************************************************************
 *            paving_submodule.cpp
 *
 *  Copyright  2008-20  Pieter Collins
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne.
 *
 *  Ariadne is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "pybind11.hpp"
#include "set-utilities.hpp"

#include "numeric/numeric.hpp"
#include "set/function_set.hpp"
#include "paving/binary_tree_paving.hpp"
#include "paving/set_approximation.hpp"
#include "set/affine_set.hpp"
#include "set/list_set.hpp"

using namespace Ariadne;


Void export_grid(pybind11::module& module)
{
    pybind11::class_< Grid > grid_class(module,"Grid");
    grid_class.def(pybind11::init<Nat>());
    grid_class.def(pybind11::init<Nat,FloatDP>());
    grid_class.def(pybind11::init<Vector<FloatDP>,Vector<FloatDP>>());
    grid_class.def("dimension", &Grid::dimension);
    grid_class.def("origin", &Grid::origin);
    grid_class.def("lengths", &Grid::lengths);
    grid_class.def("__str__",&__cstr__<Grid>);
}

Void export_grid_cell(pybind11::module& module)
{
    pybind11::class_<GridCell> grid_cell_class(module,"GridCell");
    grid_cell_class.def("dimension", &GridCell::dimension);
    grid_cell_class.def("depth", &GridCell::depth);
    grid_cell_class.def("split", (Pair<GridCell,GridCell>(GridCell::*)()const) &GridCell::split);
    grid_cell_class.def("split", (GridCell(GridCell::*)(Bool)const) &GridCell::split);
    grid_cell_class.def("box", &GridCell::box);
    grid_cell_class.def("__str__",&__cstr__<GridCell>);

    module.def("smallest_enclosing_primary_cell", &GridCell::smallest_enclosing_primary_cell);
}


Void export_binary_tree_paving(pybind11::module& module) {

    pybind11::class_<BinaryTreeSubpaving> binary_tree_subpaving_class(module,"BinaryTreeSubpaving");

    pybind11::class_<BinaryTreePaving, BinaryTreeSubpaving> binary_tree_paving_class(module,"BinaryTreePaving");
    binary_tree_paving_class.def(pybind11::init<BinaryTreePaving>());
    binary_tree_paving_class.def(pybind11::init<Nat>());
    binary_tree_paving_class.def(pybind11::init<Grid>());
    binary_tree_paving_class.def("bounding_box", &BinaryTreePaving::bounding_box);
    binary_tree_paving_class.def("is_empty", &BinaryTreePaving::is_empty);
    binary_tree_paving_class.def("size", &BinaryTreePaving::size);
    binary_tree_paving_class.def("dimension", &BinaryTreePaving::dimension);
    binary_tree_paving_class.def("clear", &BinaryTreePaving::clear);
    binary_tree_paving_class.def("mince", &BinaryTreePaving::mince);
    binary_tree_paving_class.def("recombine", &BinaryTreePaving::recombine);
    binary_tree_paving_class.def("grid", &BinaryTreePaving::grid);
    binary_tree_paving_class.def("measure", &BinaryTreePaving::measure);
    binary_tree_paving_class.def("adjoin", (Void(BinaryTreePaving::*)(const GridCell&))(&BinaryTreePaving::adjoin));
    binary_tree_paving_class.def("adjoin", (Void(BinaryTreePaving::*)(const BinaryTreeSubpaving&))(&BinaryTreePaving::adjoin));
    binary_tree_paving_class.def("restrict", (Void(BinaryTreePaving::*)(const BinaryTreeSubpaving&))(&BinaryTreePaving::restrict));
    binary_tree_paving_class.def("remove", (Void(BinaryTreePaving::*)(const BinaryTreeSubpaving&))(&BinaryTreePaving::remove));
    binary_tree_paving_class.def("adjoin_over_approximation", (Void(BinaryTreePaving::*)(const ExactBoxType&,const Nat)) &BinaryTreePaving::adjoin_over_approximation);
    binary_tree_paving_class.def("adjoin_outer_approximation", (Void(BinaryTreePaving::*)(const EffectiveEuclideanCompactSetInterface&,const Nat)) &BinaryTreePaving::adjoin_outer_approximation);
    binary_tree_paving_class.def("adjoin_inner_approximation", (Void(BinaryTreePaving::*)(const EffectiveEuclideanOpenSetInterface&,const Nat,const Nat)) &BinaryTreePaving::adjoin_inner_approximation);
    binary_tree_paving_class.def("__len__", &BinaryTreePaving::size);
    binary_tree_paving_class.def("__iter__", [](BinaryTreePaving const& g){return pybind11::make_iterator(g.begin(), g.end());});
    binary_tree_paving_class.def("__str__",&__cstr__<BinaryTreePaving>);

    module.def("union",(BinaryTreePaving(*)(const BinaryTreeSubpaving&,const BinaryTreeSubpaving&))(&join));
    module.def("difference",(BinaryTreePaving(*)(const BinaryTreeSubpaving&,const BinaryTreeSubpaving&))(&difference));
    module.def("intersection",(BinaryTreePaving(*)(const BinaryTreeSubpaving&,const BinaryTreeSubpaving&))(&intersection));
    module.def("intersect",(Bool(*)(const BinaryTreeSubpaving&,const BinaryTreeSubpaving&))(&intersect));
    module.def("subpaving",(Bool(*)(const BinaryTreeSubpaving&,const BinaryTreeSubpaving&))(&subset));
    module.def("intersect",(Bool(*)(const GridCell&,const BinaryTreeSubpaving&))(&intersect));
    module.def("subpaving",(Bool(*)(const GridCell&,const BinaryTreeSubpaving&))(&subset));

    module.def("outer_approximation",(BinaryTreePaving(*)(const EffectiveEuclideanCompactSetInterface&,const Grid&,const Nat)) &outer_approximation);
    module.def("inner_approximation",(BinaryTreePaving(*)(const EffectiveEuclideanOpenSetInterface&,const Grid&,const Nat,const Nat)) &inner_approximation);

}


void export_set_approximation(pybind11::module& module)
{
    // These methods belong to paving but extend set types registered earlier.
    // is_method preserves the existing Python instance-method API.
    auto validated=module.attr("ValidatedConstrainedImageSet");
    validated.attr("outer_approximation")=pybind11::cpp_function(
        [](const ValidatedConstrainedImageSet& self, const Grid& grid, Nat depth) {
            return outer_approximation(self,grid,depth);
        }, pybind11::is_method(validated));
    validated.attr("adjoin_outer_approximation_to")=pybind11::cpp_function(
        [](const ValidatedConstrainedImageSet& self, BinaryTreePaving& paving, Nat depth) {
            adjoin_outer_approximation_to(paving,self,depth);
        }, pybind11::is_method(validated));

    auto affine=module.attr("ValidatedAffineConstrainedImageSet");
    affine.attr("outer_approximation")=pybind11::cpp_function(
        [](const ValidatedAffineConstrainedImageSet& self, const Grid& grid, Nat depth) {
            return outer_approximation(self,grid,depth);
        }, pybind11::is_method(affine));
    affine.attr("adjoin_outer_approximation_to")=pybind11::cpp_function(
        [](const ValidatedAffineConstrainedImageSet& self, BinaryTreePaving& paving, Nat depth) {
            adjoin_outer_approximation_to(paving,self,depth);
        }, pybind11::is_method(affine));

    module.def("outer_approximation",
        [](const ValidatedConstrainedImageSet& set, const Grid& grid, Nat depth) {
            return outer_approximation(set,grid,depth);
        });
    module.def("outer_approximation",
        [](const ValidatedAffineConstrainedImageSet& set, const Grid& grid, Nat depth) {
            return outer_approximation(set,grid,depth);
        });
    module.def("adjoin_outer_approximation_to",
        [](BinaryTreePaving& paving, const ValidatedConstrainedImageSet& set, Nat depth) {
            adjoin_outer_approximation_to(paving,set,depth);
        });
    module.def("adjoin_outer_approximation_to",
        [](BinaryTreePaving& paving, const ValidatedAffineConstrainedImageSet& set, Nat depth) {
            adjoin_outer_approximation_to(paving,set,depth);
        });
}

void paving_submodule(pybind11::module& module)
{
    export_grid(module);
    export_grid_cell(module);
    export_binary_tree_paving(module);
    export_set_approximation(module);
}
