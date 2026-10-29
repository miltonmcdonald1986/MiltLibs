/// @file
/// @brief An orthographic view orbiting a point, without raylib, so it can be tested.

#pragma once

#include <algorithm>
#include <cmath>
#include <numbers>

#include <glm/vec3.hpp>

namespace milt::runner {

/// @brief How far the eye can rise above or sink below the x-y plane, in radians.
///
/// Short of straight up or down, where the view's up direction, math z, would point along the
/// line of sight and the view would spin.
inline constexpr double max_orbit_pitch = 85.0 * std::numbers::pi / 180.0;

/// @brief An orthographic view of a point from a direction given by two angles.
///
/// All points and directions are in z-up math coordinates. The view's up direction is math z.
struct Orbit {
    glm::dvec3 center;   ///< The point orbited.
    double distance;     ///< How far the eye is from the target. Doesn't change the view's size.
    double yaw;          ///< The eye's angle around z in radians, from the -y side toward +x.
    double pitch;        ///< The eye's angle above the x-y plane in radians.
    double fovy;         ///< The view's height in world units.
    double min_fovy;     ///< The smallest fovy zooming can reach.
    double max_fovy;     ///< The largest fovy zooming can reach.
    double center_shift; ///< How far right of mid-screen the center shows, in view heights.
};

/// @brief The unit vector from the target toward the eye.
///
/// @param yaw   The angle around z in radians, from the -y side toward +x.
/// @param pitch The angle above the x-y plane in radians.
///
/// @return The direction. Yaw 0 and pitch 0 give (0, -1, 0).
[[nodiscard]] inline glm::dvec3 toward_eye(double yaw, double pitch) {
    return {std::sin(yaw) * std::cos(pitch), -std::cos(yaw) * std::cos(pitch), std::sin(pitch)};
}

/// @brief The unit vector pointing right across the screen.
///
/// It's horizontal and perpendicular to toward_eye(yaw, pitch) for every pitch.
///
/// @param yaw The angle around z in radians, from the -y side toward +x.
///
/// @return The direction. Yaw 0 gives (1, 0, 0).
[[nodiscard]] inline glm::dvec3 screen_right(double yaw) {
    return {std::cos(yaw), std::sin(yaw), 0.0};
}

/// @brief The point the view looks at: the center, moved so it shows center_shift right of
/// mid-screen.
///
/// @param orbit The view.
///
/// @return The target.
[[nodiscard]] inline glm::dvec3 orbit_target(const Orbit& orbit) {
    return orbit.center - screen_right(orbit.yaw) * (orbit.center_shift * orbit.fovy);
}

/// @brief The eye's position, `distance` from the target toward the eye.
///
/// @param orbit The view.
///
/// @return The eye.
[[nodiscard]] inline glm::dvec3 orbit_eye(const Orbit& orbit) {
    return orbit_target(orbit) + toward_eye(orbit.yaw, orbit.pitch) * orbit.distance;
}

/// @brief How far to turn an Orbit.
struct OrbitTurn {
    double yaw;   ///< Radians to add to the yaw.
    double pitch; ///< Radians to add to the pitch.
};

/// @brief Turns the view around its center.
///
/// @param orbit The view.
/// @param turn  The angles to add. The pitch is then clamped to max_orbit_pitch.
///
/// @return The turned view.
[[nodiscard]] inline Orbit rotated(Orbit orbit, OrbitTurn turn) {
    orbit.yaw += turn.yaw;
    orbit.pitch = std::clamp(orbit.pitch + turn.pitch, -max_orbit_pitch, max_orbit_pitch);
    return orbit;
}

/// @brief Scales the view's size, which zooms an orthographic view.
///
/// @param orbit  The view.
/// @param factor What to multiply fovy by; under 1 zooms in. fovy is then clamped to
///               [min_fovy, max_fovy].
///
/// @pre `factor > 0`.
///
/// @return The zoomed view.
[[nodiscard]] inline Orbit zoomed(Orbit orbit, double factor) {
    orbit.fovy = std::clamp(orbit.fovy * factor, orbit.min_fovy, orbit.max_fovy);
    return orbit;
}

} // namespace milt::runner
