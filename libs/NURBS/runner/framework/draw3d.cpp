#include "draw3d.hpp"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <raylib.h>

#include "raylib_convert.hpp"

namespace milt::runner {

namespace {

// Facets around the shaft and head; enough to look round at the runner's arrow sizes.
constexpr int arrow_sides = 12;

} // namespace

void draw_arrow_3d(glm::dvec3 from, glm::dvec3 to, double radius, Color color) {
    const glm::dvec3 direction = to - from;
    const double length = glm::length(direction);
    if (length == 0.0) {
        return;
    }

    const double head_length = 4.0 * radius;
    const double head_radius = 2.0 * radius;

    if (length <= head_length) {
        // Too short for a shaft: draw just the head, scaled to fit.
        const double scale = length / head_length;
        DrawCylinderEx(to_vector3(from), to_vector3(to), static_cast<float>(head_radius * scale),
                       0.0f, arrow_sides, color);
        return;
    }

    const glm::dvec3 neck = to - direction * (head_length / length);
    DrawCylinderEx(to_vector3(from), to_vector3(neck), static_cast<float>(radius),
                   static_cast<float>(radius), arrow_sides, color);
    DrawCylinderEx(to_vector3(neck), to_vector3(to), static_cast<float>(head_radius), 0.0f,
                   arrow_sides, color);
}

} // namespace milt::runner
