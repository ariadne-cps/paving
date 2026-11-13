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
#include <cmath>
#include <optional>
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

// Benchmark-only mapped and constrained workloads. The five original
// shapes remain unchanged for the independent backend benchmark.
inline std::vector<std::string> parse_paver_cases(const String& value) {
    const std::string input(value);
    if (input.empty() or input.front() == ',' or input.back() == ',')
        throw std::invalid_argument("invalid Paver case list");
    constexpr const char* known[] = {
        "aligned-box", "rotated-box", "aligned-ellipsoid",
        "rotated-ellipsoid", "ball", "affine-shear", "quadratic-shear",
        "folded-quadratic", "uv-triangle", "affine-projection",
        "correlated-strip", "product-cap", "parabolic-curve",
        "empty-constraints"
    };
    std::vector<std::string> result;
    std::size_t start = 0u;
    do {
        const std::size_t end = input.find(',', start);
        const auto name = input.substr(
            start, end == std::string::npos ? end : end - start);
        if (std::find(std::begin(known), std::end(known), name) ==
                std::end(known) or
            std::find(result.begin(), result.end(), name) != result.end())
            throw std::invalid_argument("unknown or duplicate Paver case: " + name);
        result.push_back(name);
        if (end == std::string::npos)
            break;
        start = end + 1u;
    } while (true);
    return result;
}

// Zero-volume and empty sets have no full-dimensional interior box. They
// are validated separately rather than requiring a fictitious witness.
struct PaverBenchmarkCase {
    ValidatedConstrainedImageSet set;
    double exact_volume;
    bool expected_empty = false;
    std::optional<ExactBoxType> interior;
};

inline PaverBenchmarkCase make_paver_case(unsigned dimension,
                                          const std::string& name) {
    if (dimension < 2u or dimension > 6u)
        throw std::invalid_argument("Paver case dimension must be 2..6");
    (void)parse_paver_cases(String(name));
    const ExactBoxType centered(
        dimension, ExactIntervalType(0.49609375_x, 0.50390625_x));
    const double h = static_cast<double>(dimension) / 2.0;
    const double unit_ball_volume =
        std::pow(std::acos(-1.0), h) / std::tgamma(h + 1.0);

    if (name == "aligned-box" or name == "rotated-box")
        return {make_validated_shape(dimension, name),
                0.125 * std::pow(0.5, static_cast<double>(dimension - 2u)),
                false, centered};
    if (name == "aligned-ellipsoid" or name == "rotated-ellipsoid")
        return {make_validated_shape(dimension, name),
                unit_ball_volume * 0.5 *
                    std::pow(0.25, static_cast<double>(dimension)),
                false, centered};
    if (name == "ball")
        return {make_validated_shape(dimension, name),
                unit_ball_volume * std::pow(0.25, static_cast<double>(dimension)),
                false, centered};

    const bool curve = name == "parabolic-curve";
    const unsigned parameters = dimension +
                                (name == "affine-projection" ? 1u : 0u) -
                                (curve ? 1u : 0u);
    const auto x = EffectiveScalarMultivariateFunction::coordinates(parameters);
    List<EffectiveScalarMultivariateFunction> components;
    for (unsigned i = 0u; i != dimension; ++i) {
        if (name == "affine-shear" and i == 1u) {
            components.append(x[1] + x[0] * 0.25_x);
        } else if (name == "quadratic-shear" and i == 1u) {
            const auto u = x[0] - 0.5_x;
            components.append(x[1] + u * u * 0.25_x);
        } else if (name == "folded-quadratic" and i == 0u) {
            const auto u = x[0] - 0.5_x;
            components.append(4 * u * u);
        } else if (name == "uv-triangle" and i == 1u) {
            components.append(x[0] * x[1]);
        } else if (curve and i == 1u) {
            components.append(x[0] * x[0]);
        } else {
            components.append(x[i - (curve and i > 1u ? 1u : 0u)]);
        }
    }
    EffectiveConstrainedImageSet effective(
        RealBox(parameters, RealInterval(0, 1)),
        EffectiveVectorMultivariateFunction(components));
    if (name == "correlated-strip") {
        effective.new_parameter_constraint(x[0] + x[1] >= 0.75_x);
        effective.new_parameter_constraint(x[0] + x[1] <= 1.25_x);
    } else if (name == "product-cap") {
        effective.new_parameter_constraint(x[0] * x[1] >= 0.25_x);
    } else if (name == "empty-constraints") {
        effective.new_parameter_constraint(x[0] <= -1.0_x);
    }

    ValidatedConstrainedImageSet validated(
        ExactBoxType(parameters, ExactIntervalType(0, 1)),
        effective.function(), effective.constraints());
    if (name == "empty-constraints")
        return {validated, 0.0, true, std::nullopt};
    if (curve)
        return {validated, 0.0, false, std::nullopt};
    if (name == "uv-triangle") {
        ExactBoxType witness = centered;
        witness[0] = ExactIntervalType(0.74609375_x, 0.75390625_x);
        witness[1] = ExactIntervalType(0.24609375_x, 0.25390625_x);
        return {validated, 0.5, false, witness};
    }
    if (name == "product-cap") {
        ExactBoxType witness = centered;
        witness[0] = ExactIntervalType(0.74609375_x, 0.75390625_x);
        witness[1] = ExactIntervalType(0.74609375_x, 0.75390625_x);
        return {validated, 0.75 + 0.25 * std::log(0.25), false, witness};
    }
    if (name == "correlated-strip")
        return {validated, 0.4375, false, centered};
    // Affine/quadratic shear (unit Jacobian), folding (surjective onto
    // [0,1]), and projection all have unit output-volume.
    return {validated, 1.0, false, centered};
}

} // namespace Ariadne::PavingBenchmarkCases

#endif /* ARIADNE_PAVING_BENCHMARK_SHAPES_HPP */
