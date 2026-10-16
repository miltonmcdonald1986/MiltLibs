/// @file
/// @brief Conversions from glm and world types to raylib types.

#pragma once

#include <glm/vec2.hpp>
#include <raylib.h>

#include "viewport.hpp"

namespace milt::runner {

/// @brief Converts a glm vector to a raylib vector.
///
/// @param v The vector to convert. Its components are narrowed to `float`.
///
/// @return The same vector as a raylib `Vector2`.
[[nodiscard]] inline Vector2 to_vector2(glm::dvec2 v) {
    return {static_cast<float>(v.x), static_cast<float>(v.y)};
}

/// @brief Converts a world point to a raylib screen position.
///
/// @param viewport Maps world coordinates to the screen.
/// @param p        A point in world coordinates.
///
/// @return The point in screen pixels, as a raylib `Vector2`.
[[nodiscard]] inline Vector2 to_screen(const Viewport& viewport, glm::dvec2 p) {
    return to_vector2(viewport.world_to_screen(p));
}

} // namespace milt::runner
