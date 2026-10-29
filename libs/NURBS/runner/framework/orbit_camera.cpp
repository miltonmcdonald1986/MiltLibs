#include "orbit_camera.hpp"

#include <cmath>

#include <raylib.h>

#include "orbit.hpp"
#include "raylib_convert.hpp"

namespace milt::runner {

namespace {

// Radians the view turns per pixel dragged.
constexpr double radians_per_pixel = 0.01;

// What one notch of the mouse wheel multiplies fovy by, zooming in.
constexpr double zoom_per_notch = 0.9;

} // namespace

OrbitControl update_orbit(OrbitControl control, Rectangle ignored) {
    const bool in_ignored = CheckCollisionPointRec(GetMousePosition(), ignored);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        control.dragging = !in_ignored;
    } else if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        control.dragging = false;
    }

    if (control.dragging) {
        const Vector2 delta = GetMouseDelta();
        // Dragging right swings the eye left, so the scene turns the way the mouse moves.
        // Dragging down raises the eye, tipping the top of the scene toward the viewer.
        control.orbit = rotated(control.orbit, {.yaw = -delta.x * radians_per_pixel,
                                                .pitch = delta.y * radians_per_pixel});
    }

    const float wheel = GetMouseWheelMove();
    if (wheel != 0.0f && !in_ignored) {
        control.orbit = zoomed(control.orbit, std::pow(zoom_per_notch, wheel));
    }

    return control;
}

Camera3D orbit_camera(const Orbit& orbit) {
    return {
        .position = to_vector3(orbit_eye(orbit)),
        .target = to_vector3(orbit_target(orbit)),
        .up = to_vector3({0.0, 0.0, 1.0}),
        .fovy = static_cast<float>(orbit.fovy),
        .projection = CAMERA_ORTHOGRAPHIC,
    };
}

} // namespace milt::runner
