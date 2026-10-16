/// @file
/// @brief The triangle shown by the NURBS runner, without any drawing code.

#pragma once

#include <array>
#include <numbers>

#include <glm/vec2.hpp>

/// @brief An equilateral triangle in world coordinates.
struct TriangleModel {
    /// @brief The vertices, in counter-clockwise order.
    ///
    /// The triangle is centered on the origin and points up.
    std::array<glm::dvec2, 3> vertices{{
        {0.0, 1.0},
        {-std::numbers::sqrt3 / 2.0, -0.5},
        {std::numbers::sqrt3 / 2.0, -0.5},
    }};
};
