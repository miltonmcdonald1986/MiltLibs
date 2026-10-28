/// @file
/// @brief 3D shapes for the NURBS runner that raylib doesn't draw itself.

#pragma once

#include <glm/vec3.hpp>
#include <raylib.h>

namespace milt::runner {

/// @brief Draws a solid arrow from one point to another: a cylinder shaft and a cone head.
///
/// The head is `4 * radius` long and `2 * radius` wide at its base. An arrow shorter than
/// its head is drawn as a cone alone, scaled down to the arrow's length so it keeps the same
/// shape. A zero-length arrow draws nothing.
///
/// Call it between `BeginMode3D` and `EndMode3D`.
///
/// @param from   The tail, in z-up math coordinates.
/// @param to     The tip, in z-up math coordinates.
/// @param radius The shaft's radius, in world units.
/// @param color  The arrow's color.
void draw_arrow_3d(glm::dvec3 from, glm::dvec3 to, double radius, Color color);

} // namespace milt::runner
