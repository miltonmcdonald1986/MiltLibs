#include "orbit_camera.hpp"

#include <algorithm>
#include <cmath>

#include <glm/vec2.hpp>
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

// Turns the view by a drag of `delta` pixels.
Orbit turned_by_drag(const Orbit& orbit, glm::dvec2 delta) {
    // Dragging right swings the eye left, so the scene turns the way the mouse moves.
    // Dragging down raises the eye, tipping the top of the scene toward the viewer.
    return rotated(orbit,
                   {.yaw = -delta.x * radians_per_pixel, .pitch = delta.y * radians_per_pixel});
}

glm::dvec2 touch_position(int index) {
    const Vector2 p = GetTouchPosition(index);
    return {p.x, p.y};
}

// The mouse: a left drag turns, a right or middle drag pans and the wheel zooms.
OrbitControl update_from_mouse(OrbitControl control, Rectangle ignored) {
    const bool in_ignored = CheckCollisionPointRec(GetMousePosition(), ignored);
    const Vector2 mouse_delta = GetMouseDelta();
    const glm::dvec2 delta{mouse_delta.x, mouse_delta.y};

    // A drag moves the view from its second frame on. In its first, the delta is how far the
    // pointer jumped to where the drag starts, which is nothing for a mouse but can be across
    // the screen for a finger.
    const bool was_turning = control.turning;
    control.turning = next_drag_state(control.turning, IsMouseButtonPressed(MOUSE_BUTTON_LEFT),
                                      IsMouseButtonDown(MOUSE_BUTTON_LEFT), in_ignored);
    if (was_turning && control.turning) {
        control.orbit = turned_by_drag(control.orbit, delta);
    }

    const bool was_panning = control.panning;
    control.panning = next_drag_state(
        control.panning,
        IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE),
        IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsMouseButtonDown(MOUSE_BUTTON_MIDDLE),
        in_ignored);
    if (was_panning && control.panning) {
        control.orbit = panned(control.orbit, delta, GetScreenHeight());
    }

    const float wheel = GetMouseWheelMove();
    if (wheel != 0.0f && !in_ignored) {
        control.orbit = zoomed(control.orbit, std::pow(zoom_per_notch, wheel));
    }

    return control;
}

// Fingers on a touch screen: one turns, and two pan and pinch-zoom.
//
// raylib also reports touches as mouse buttons, the first finger as the left one and the
// second as the right, but it moves the mouse position only while one finger is down. So
// touches are read here directly instead.
OrbitControl update_from_touch(OrbitControl control, Rectangle ignored, int touch_count) {
    // Fingers past the second don't do anything more.
    const int count = std::min(touch_count, 2);
    const FingerPair now{.first = touch_position(0),
                         .second = count >= 2 ? touch_position(1) : touch_position(0)};

    if (control.touch_count == 0) {
        // A new gesture, which is left alone until every finger lifts if it starts over the
        // ignored rectangle.
        control.touch_ignored = CheckCollisionPointRec(GetTouchPosition(0), ignored);
    } else if (count == control.touch_count && !control.touch_ignored) {
        // Only frames with the same fingers down as the last one move the view: when a finger
        // goes down or lifts, raylib renumbers the rest, so the positions jump.
        const FingerPair& before = control.touches;
        if (count == 1) {
            control.orbit = turned_by_drag(control.orbit, now.first - before.first);
        } else {
            control.orbit =
                pinched(control.orbit, {.before = before, .after = now}, GetScreenHeight());
        }
    }

    control.touch_count = count;
    control.touches = now;
    return control;
}

} // namespace

OrbitControl update_orbit(OrbitControl control, Rectangle ignored) {
    if (const int touch_count = GetTouchPointCount(); touch_count > 0) {
        // The mouse buttons raylib fakes for the fingers mustn't start drags of their own.
        control.turning = false;
        control.panning = false;
        return update_from_touch(control, ignored, touch_count);
    }

    control.touch_count = 0;
    return update_from_mouse(control, ignored);
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
