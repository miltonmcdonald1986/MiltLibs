/// @file
/// @brief Mapping between world coordinates and screen pixels.

#pragma once

#include <glm/vec2.hpp>

namespace milt::runner {

/// @brief Maps a y-up world onto a y-down screen.
///
/// World units are scaled uniformly by `pixels_per_unit`, and the world origin lands on
/// `center`.
struct Viewport {
    glm::dvec2 center;      ///< Screen position of the world origin, in pixels.
    double pixels_per_unit; ///< Screen pixels per world unit.

    /// @brief Converts a world point to screen pixels.
    ///
    /// The y axis is flipped, so increasing world y moves up the screen.
    ///
    /// @param p A point in world coordinates.
    ///
    /// @return The point in screen pixels.
    [[nodiscard]] constexpr glm::dvec2 world_to_screen(glm::dvec2 p) const {
        return {center.x + p.x * pixels_per_unit, center.y - p.y * pixels_per_unit};
    }
};

} // namespace milt::runner
