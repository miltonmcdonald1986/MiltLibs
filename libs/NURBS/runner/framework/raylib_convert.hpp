/// @file
/// @brief Conversions from glm and world types to raylib types.

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <raylib.h>

#include "viewport.hpp"

namespace milt::runner {

/// @brief Converts a glm vector to a raylib vector.
///
/// @param v The vector to convert. Its components are narrowed to `float`.
///
/// @return The same vector as a raylib `Vector2`.
[[nodiscard]] inline Vector2 to_vector2(glm::dvec2 v) {
    return {.x = static_cast<float>(v.x), .y = static_cast<float>(v.y)};
}

/// @brief Converts a point in z-up math coordinates to raylib's y-up 3D coordinates.
///
/// Math (x, y, z) becomes raylib (x, z, -y). That is a rotation of -90 degrees about x, so
/// it keeps handedness, and the math z = 0 plane lands on raylib's ground plane, where
/// `DrawGrid` draws. Seen from above with raylib -z as up, math x points right and math y up.
///
/// @param p The point in math coordinates. Its components are narrowed to `float`.
///
/// @return The same point as a raylib `Vector3`.
[[nodiscard]] inline Vector3 to_vector3(glm::dvec3 p) {
    return {
        .x = static_cast<float>(p.x), .y = static_cast<float>(p.z), .z = static_cast<float>(-p.y)};
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
