/***************************************************************************
 *            benchmarks/paving_benchmark_shapes.hpp
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

/** @file benchmarks/paving_benchmark_shapes.hpp
 *  @brief Shared benchmark geometries for backend and Paver comparisons.
 */

#ifndef ARIADNE_PAVING_BENCHMARK_SHAPES_HPP
#define ARIADNE_PAVING_BENCHMARK_SHAPES_HPP

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

#include "set/function_set.hpp"

namespace Ariadne::PavingBenchmarkCases {

inline std::vector<std::string> parse_shapes(const String& value) {
    std::string text(value);
    if (text.empty() or text.front() == ',' or text.back() == ',') {
        throw std::invalid_argument("invalid shape list");
    }
    std::vector<std::string> result;
    std::size_t begin = 0u;
    do {
        std::size_t end = text.find(',', begin);
        auto name = text.substr(
            begin, end == std::string::npos ? end : end - begin);
        constexpr const char* known[] = {
            "aligned-box", "rotated-box", "aligned-ellipsoid",
            "rotated-ellipsoid", "ball"};
        if (std::find(std::begin(known), std::end(known), name) == std::end(known) or
            std::find(result.begin(), result.end(), name) != result.end()) {
            throw std::invalid_argument("unknown or duplicate shape");
        }
        result.push_back(name);
        if (end == std::string::npos) {
            break;
        }
        begin = end + 1u;
    } while (true);
    return result;
}

inline EffectiveConstrainedImageSet make_shape(
    unsigned dimension, const std::string& shape) {
    // Exact dyadic parameters avoid ExactDouble rejecting inexact decimals.
    // Rotation uses the rational orthogonal matrix [3/5,4/5;-4/5,3/5].
    // The paired boxes and ellipsoids have identical volumes and radii.
    auto x = EffectiveScalarMultivariateFunction::coordinates(dimension);
    RealBox domain(dimension, RealInterval(0, 1));
    EffectiveConstrainedImageSet result(
        domain, EffectiveVectorMultivariateFunction::identity(dimension));

    auto first = x[0] - 0.5_x;
    auto second = x[1] - 0.5_x;
    auto rotated_first = 3 * first + 4 * second;
    auto rotated_second = -4 * first + 3 * second;

    if (shape == "aligned-box") {
        result.new_parameter_constraint(first >= -0.25_x);
        result.new_parameter_constraint(first <= 0.25_x);
        result.new_parameter_constraint(second >= -0.125_x);
        result.new_parameter_constraint(second <= 0.125_x);
    } else if (shape == "rotated-box") {
        // Multiplication by five makes the rotated constraints exact.
        result.new_parameter_constraint(rotated_first >= -1.25_x);
        result.new_parameter_constraint(rotated_first <= 1.25_x);
        result.new_parameter_constraint(rotated_second >= -0.625_x);
        result.new_parameter_constraint(rotated_second <= 0.625_x);
    } else {
        EffectiveScalarMultivariateFunction quadratic = first * first * 16 + second * second * 16;
        if (shape == "aligned-ellipsoid") {
            quadratic = first * first * 16 + second * second * 64;
        } else if (shape == "rotated-ellipsoid") {
            // 16*(u/5)^2 + 64*(v/5)^2 <= 1 iff 16*u^2+64*v^2 <= 25.
            quadratic = rotated_first * rotated_first * 16 + rotated_second * rotated_second * 64;
        } else if (shape != "ball") {
            throw std::invalid_argument("unknown shape");
        }
        for (unsigned i = 2u; i < dimension; ++i) {
            auto centered = x[i] - 0.5_x;
            quadratic = quadratic + centered * centered *
                                        (shape == "rotated-ellipsoid" ? 400 : 16);
        }
        result.new_parameter_constraint(
            quadratic <= (shape == "rotated-ellipsoid" ? 25.0_x : 1.0_x));
    }
    if (shape == "aligned-box" or shape == "rotated-box") {
        for (unsigned i = 2u; i < dimension; ++i) {
            result.new_parameter_constraint(x[i] >= 0.25_x);
            result.new_parameter_constraint(x[i] <= 0.75_x);
        }
    }
    return result;
}

// Both factories define exactly the same set: an identity mapping over the
// unit box with the same parameter constraints. Construct the validated form
// outside timed callbacks; RealBox(0,1) and ExactIntervalType(0,1) coincide.
inline ValidatedConstrainedImageSet make_validated_shape(
    unsigned dimension, const std::string& shape) {
    const auto effective = make_shape(dimension, shape);
    return ValidatedConstrainedImageSet(
        ExactBoxType(dimension, ExactIntervalType(0, 1)),
        effective.function(), effective.constraints());
}

} // namespace Ariadne::PavingBenchmarkCases

#endif /* ARIADNE_PAVING_BENCHMARK_SHAPES_HPP */
