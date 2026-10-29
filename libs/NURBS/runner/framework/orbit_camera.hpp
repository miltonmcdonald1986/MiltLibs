/// @file
/// @brief Mouse control of an Orbit, and the raylib camera it gives.

#pragma once

#include <raylib.h>

#include "orbit.hpp"

namespace milt::runner {

/// @brief An Orbit and the state of the mouse drags and touches moving it.
struct OrbitControl {
    Orbit orbit;                ///< The view.
    bool turning = false;       ///< Whether a left-button drag that turns the view is under way.
    bool panning = false;       ///< Whether a right- or middle-button drag that pans is under way.
    int touch_count = 0;        ///< How many fingers, up to 2, were down last frame.
    bool touch_ignored = false; ///< Whether the fingers down started over the ignored area.
    FingerPair touches{};       ///< Where the first two fingers were last frame.
};

/// @brief Turns, pans and zooms the view from this frame's mouse or touch input.
///
/// With a mouse, a left-button drag turns the view: across the screen changes the yaw, and up
/// and down the pitch. A right- or middle-button drag pans, so the scene follows the mouse.
/// The mouse wheel zooms.
///
/// On a touch screen, dragging one finger turns the view the same way. Two fingers pan with
/// their midpoint and zoom as they spread or pinch, through pinched().
///
/// Input that starts inside `ignored` does none of these, so that working a control drawn
/// there doesn't move the view. Neither does the first frame of a drag, or a frame in which a
/// finger goes down or lifts, so the view never jumps.
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
