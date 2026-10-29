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

// Whether a drag is under way after this frame: one starts when its button is pressed outside
// the ignored rectangle, and goes on until the button is let go.
bool next_drag_state(bool dragging, bool pressed, bool down, bool in_ignored) {
    if (pressed) {
        return !in_ignored;
    }
    return dragging && down;
}

} // namespace

OrbitControl update_orbit(OrbitControl control, Rectangle ignored) {
    const bool in_ignored = CheckCollisionPointRec(GetMousePosition(), ignored);
    const Vector2 delta = GetMouseDelta();

    control.turning = next_drag_state(control.turning, IsMouseButtonPressed(MOUSE_BUTTON_LEFT),
                                      IsMouseButtonDown(MOUSE_BUTTON_LEFT), in_ignored);
    if (control.turning) {
        // Dragging right swings the eye left, so the scene turns the way the mouse moves.
        // Dragging down raises the eye, tipping the top of the scene toward the viewer.
        control.orbit = rotated(control.orbit, {.yaw = -delta.x * radians_per_pixel,
                                                .pitch = delta.y * radians_per_pixel});
    }

    control.panning = next_drag_state(
        control.panning,
        IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE),
        IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsMouseButtonDown(MOUSE_BUTTON_MIDDLE),
        in_ignored);
    if (control.panning) {
        control.orbit = panned(control.orbit, {delta.x, delta.y}, GetScreenHeight());
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
