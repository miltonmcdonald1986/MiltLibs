/// @file
/// @brief The triangle shown by the NURBS runner, without any drawing code.

#pragma once

#include <array>
#include <cmath>
#include <numbers>

#include <glm/mat2x2.hpp>
#include <glm/vec2.hpp>

/// @brief User-adjustable settings for the triangle's motion.
struct TriangleSettings {
    bool spinning = true; ///< Whether the triangle rotates on `update`.
    double speed = 1.0;   ///< Rotation speed, in radians per second, counter-clockwise.
};

/// @brief An equilateral triangle in world coordinates that can spin about the origin.
struct TriangleModel {
    /// @brief The unrotated vertices, in counter-clockwise order.
    ///
    /// The triangle is centered on the origin and points up.
    static constexpr std::array<glm::dvec2, 3> rest_vertices{{
        {0.0, 1.0},
        {-std::numbers::sqrt3 / 2.0, -0.5},
        {std::numbers::sqrt3 / 2.0, -0.5},
    }};

    double angle = 0.0;        ///< Counter-clockwise rotation, in radians, in [0, 2π).
    TriangleSettings settings; ///< How the triangle moves.

    /// @brief Advances the rotation by one time step.
    ///
    /// Does nothing unless `settings.spinning` is set.
    ///
    /// @param dt Elapsed time, in seconds.
    void update(double dt) {
        if (settings.spinning) {
            // Wrapping keeps the angle small, so it doesn't lose precision over a long run.
            angle = std::fmod(angle + settings.speed * dt, 2.0 * std::numbers::pi);
        }
    }

    /// @brief Returns the vertices rotated by `angle` about the origin.
    ///
    /// @return The rotated vertices, still in counter-clockwise order.
    [[nodiscard]] std::array<glm::dvec2, 3> vertices() const {
        const double c = std::cos(angle);
        const double s = std::sin(angle);
        // glm matrices are column-major: the columns are (c, s) and (-s, c).
        const glm::dmat2 rotation{c, s, -s, c};
        return {rotation * rest_vertices[0], rotation * rest_vertices[1],
                rotation * rest_vertices[2]};
    }
};
