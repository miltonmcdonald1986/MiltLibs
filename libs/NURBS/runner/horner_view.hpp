/// @file
/// @brief Drawing and controls for the NURBS runner's Horner evaluation scene.

#pragma once

#include <raylib.h>

#include "horner_model.hpp"
#include "horner_presets.hpp"

/// @brief What the user has chosen to look at.
struct ViewState {
    double u = 0.4; ///< The parameter value being evaluated, in [0, 1].
};

/// @brief A preset together with the view fitted to it.
struct Scene {
    CurvePreset preset; ///< The curve shown.
    Bounds bounds{};    ///< Everything drawn for the curve, from fit_bounds.
    Camera3D camera{};  ///< An orthographic camera framing `bounds`.
};

/// @brief Fits a view to a preset.
///
/// The camera is orthographic and looks straight down at the math z = 0 plane, with math x
/// pointing right and math y up the screen. It's centered on the preset's bounds and sized to
/// hold their x-y extent with a margin, whichever of width or height is the tighter fit.
///
/// @param preset The curve to show.
/// @param aspect The window's width divided by its height.
///
/// @return The preset, its bounds and the camera.
[[nodiscard]] Scene make_scene(const CurvePreset& preset, double aspect);

/// @brief Draws a ground grid, the curve on u in [0, 1] and a marker at the curve point C(u).
///
/// Call it between `BeginMode3D(scene.camera)` and `EndMode3D`.
///
/// @param scene The curve to draw and its bounds, which set the marker's size.
/// @param u     The parameter value to mark.
void draw_scene(const Scene& scene, double u);

/// @brief Draws a raygui panel, titled with the preset's name, holding the u slider.
///
/// Call it between `BeginDrawing` and `EndDrawing`, after `EndMode3D`.
///
/// @param preset  The curve being shown.
/// @param current The state to show.
///
/// @return The state after this frame's user input.
[[nodiscard]] ViewState draw_controls(const CurvePreset& preset, ViewState current);
