/// @file
/// @brief Conversions from glm types to raylib types.

#pragma once

#include <glm/vec3.hpp>
#include <raylib.h>

namespace milt::runner {

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

} // namespace milt::runner
