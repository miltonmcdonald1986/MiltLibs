/// @file
/// @brief An orthographic view orbiting a point, without raylib, so it can be tested.

#pragma once

#include <algorithm>
#include <cmath>
#include <numbers>

#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
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
    glm::dvec3 center;   ///< Where the view starts out centered, before any panning.
    glm::dvec3 pan;      ///< How far panning has moved the view's center from `center`.
    double max_pan;      ///< The longest `pan` can get, so the scene can't be lost off-screen.
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

/// @brief The unit vector pointing up the screen.
///
/// It's perpendicular to toward_eye(yaw, pitch) and screen_right(yaw), and is math z itself
/// at pitch 0.
///
/// @param yaw   The angle around z in radians, from the -y side toward +x.
/// @param pitch The angle above the x-y plane in radians.
///
/// @return The direction, toward_eye(yaw, pitch) × screen_right(yaw).
[[nodiscard]] inline glm::dvec3 screen_up(double yaw, double pitch) {
    return {-std::sin(pitch) * std::sin(yaw), std::sin(pitch) * std::cos(yaw), std::cos(pitch)};
}

/// @brief The point the view turns around: `center` moved by `pan`.
///
/// @param orbit The view.
///
/// @return The pivot.
[[nodiscard]] inline glm::dvec3 orbit_pivot(const Orbit& orbit) {
    return orbit.center + orbit.pan;
}

/// @brief The point the view looks at: the pivot, moved so it shows center_shift right of
/// mid-screen.
///
/// @param orbit The view.
///
/// @return The target.
[[nodiscard]] inline glm::dvec3 orbit_target(const Orbit& orbit) {
    return orbit_pivot(orbit) - screen_right(orbit.yaw) * (orbit.center_shift * orbit.fovy);
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

/// @brief Turns the view around its pivot.
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

/// @brief Slides the view across the screen, so the scene follows a mouse drag.
///
/// The pivot moves within the plane of the screen, and `pan` is then shortened to at most
/// `max_pan`.
///
/// @param orbit         The view.
/// @param drag          How far the scene is dragged, in pixels, with y down the screen.
/// @param screen_height The screen's height in pixels, which the view's fovy spans.
///
/// @pre `screen_height > 0`.
///
/// @return The panned view.
[[nodiscard]] inline Orbit panned(Orbit orbit, glm::dvec2 drag, double screen_height) {
    const double units_per_pixel = orbit.fovy / screen_height;
    // The scene follows the mouse, so the view moves the other way: left for a drag to the
    // right, and up for a drag down the screen.
    orbit.pan += (screen_up(orbit.yaw, orbit.pitch) * drag.y - screen_right(orbit.yaw) * drag.x) *
                 units_per_pixel;
    if (const double length = glm::length(orbit.pan); length > orbit.max_pan) {
        orbit.pan *= orbit.max_pan / length;
    }
    return orbit;
}

/// @brief Where two fingers are on a touch screen, in pixels, with y down the screen.
struct FingerPair {
    glm::dvec2 first;  ///< The first finger.
    glm::dvec2 second; ///< The second finger.
};

/// @brief How two fingers moved between frames.
struct FingerMove {
    FingerPair before; ///< Where they were.
    FingerPair after;  ///< Where they are now.
};

/// @brief Pans and zooms the view as two fingers move, the way a touch screen map does.
///
/// The scene follows the fingers' midpoint, as panned() has it follow a drag. Spreading the
/// fingers apart zooms in, and pinching them together zooms out, by the ratio of how far
/// apart they are, through zoomed(). If either distance is 0, the view only pans.
///
/// @param orbit         The view.
/// @param move          Where the fingers were and are.
/// @param screen_height The screen's height in pixels, which the view's fovy spans.
///
/// @pre `screen_height > 0`.
///
/// @return The panned and zoomed view.
[[nodiscard]] inline Orbit pinched(Orbit orbit, FingerMove move, double screen_height) {
    const glm::dvec2 midpoint_before = (move.before.first + move.before.second) / 2.0;
    const glm::dvec2 midpoint_after = (move.after.first + move.after.second) / 2.0;
    orbit = panned(orbit, midpoint_after - midpoint_before, screen_height);

    const double spread_before = glm::distance(move.before.first, move.before.second);
    const double spread_after = glm::distance(move.after.first, move.after.second);
    if (spread_before > 0.0 && spread_after > 0.0) {
        orbit = zoomed(orbit, spread_before / spread_after);
    }
    return orbit;
}

} // namespace milt::runner
