/// @file
/// @brief Mouse control of an Orbit, and the raylib camera it gives.

#pragma once

#include <raylib.h>

#include "orbit.hpp"

namespace milt::runner {

/// @brief An Orbit and the state of the mouse drags moving it.
struct OrbitControl {
    Orbit orbit;          ///< The view.
    bool turning = false; ///< Whether a left-button drag that turns the view is under way.
    bool panning = false; ///< Whether a right- or middle-button drag that pans is under way.
};

/// @brief Turns, pans and zooms the view from this frame's mouse input.
///
/// A left-button drag turns the view: across the screen changes the yaw, and up and down the
/// pitch. A right- or middle-button drag pans, so the scene follows the mouse. The mouse wheel
/// zooms. Input that starts inside `ignored` does none of these, so that working a control
/// drawn there doesn't move the view.
///
/// @param control The view and drag state.
/// @param ignored A screen rectangle, in pixels, where drags and the wheel are left alone.
///
/// @return The view and drag state after this frame's input.
[[nodiscard]] OrbitControl update_orbit(OrbitControl control, Rectangle ignored);

/// @brief The raylib camera showing an Orbit, with an orthographic projection.
///
/// @param orbit The view.
///
/// @return The camera, in raylib's coordinates.
[[nodiscard]] Camera3D orbit_camera(const Orbit& orbit);

} // namespace milt::runner
