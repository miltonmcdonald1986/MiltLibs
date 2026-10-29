/// @file
/// @brief Drawing and controls for the NURBS runner's Horner evaluation scene.

#pragma once

#include <cstddef>
#include <optional>

#include <raylib.h>

#include "framework/orbit_camera.hpp"
#include "horner_model.hpp"
#include "horner_presets.hpp"

/// @brief What the user has chosen to look at.
struct ViewState {
    std::size_t preset = 0; ///< The index into curve_presets of the curve shown.
    double u = 0.4;         ///< The parameter value being evaluated, in [0, 1].
};

/// @brief The window's size in pixels.
struct ScreenSize {
    int width;  ///< Width in pixels.
    int height; ///< Height in pixels.
};

/// @brief A preset together with the view fitted to it.
struct Scene {
    CurvePreset preset; ///< The curve shown.
    Bounds bounds{};    ///< Everything drawn for the curve, from fit_bounds.
    Camera3D camera{};  ///< An orthographic camera framing `bounds`.
    /// For a 3D preset, the turnable view `camera` shows. 2D presets have a fixed view.
    std::optional<milt::runner::OrbitControl> orbit;
};

/// @brief Fits a view to a preset.
///
/// The camera is orthographic and frames the preset's bounds, with a margin, in the part of
/// the screen right of the control panel.
///
/// - A 2D preset is seen straight down the math z axis, with math x pointing right and math y
///   up the screen. The view is sized to the bounds' x-y extent.
/// - A 3D preset is seen from an oblique angle, above and in front, with math z up the screen.
///   The view is sized to hold the bounds' enclosing sphere, so it holds them from any angle
///   update_camera turns it to.
///
/// @param preset The index into curve_presets of the curve to show.
/// @param screen The window's size.
///
/// @pre `preset < curve_presets.size()`.
///
/// @return The preset, its bounds and the camera.
[[nodiscard]] Scene make_scene(std::size_t preset, ScreenSize screen);

/// @brief Turns and zooms a 3D preset's view from this frame's mouse input.
///
/// Dragging with the left button turns the view and the wheel zooms, except when they start
/// over the control panel, which the panel's own controls use. A 2D preset's view is fixed,
/// so its scene comes back unchanged.
///
/// Call it each frame before drawing the scene.
///
/// @param scene The scene to update.
///
/// @return The scene, with its camera following the mouse.
[[nodiscard]] Scene update_camera(Scene scene);

/// @brief Draws a ground grid, the curve on u in [0, 1] and how Horner's method reaches C(u).
///
/// The evaluation is drawn as the term chain: a dot at a0, then arrows u*a1, u^2*a2, ...
/// placed tip to tail and colored by degree, the last of which ends at a marker on the curve
/// point C(u).
///
/// Call it between `BeginMode3D(scene.camera)` and `EndMode3D`.
///
/// @param scene The curve to draw and its bounds, which set the sizes of the dots and arrows.
/// @param u     The parameter value to evaluate.
void draw_scene(const Scene& scene, double u);

/// @brief Labels each arrow of the term chain with its term, such as "u²·a₂".
///
/// Labels sit beside their arrow's midpoint on screen, in the arrow's color, in ui_font(). Arrows
/// too short on screen to label clearly are skipped.
///
/// Call it between `BeginDrawing` and `EndDrawing`, after `EndMode3D`.
///
/// @param scene The curve and the camera it's drawn with.
/// @param u     The parameter value being evaluated.
void draw_labels(const Scene& scene, double u);

/// @brief Draws the raygui control panel down the left of the screen.
///
/// From the top, it holds the preset picker, the u slider, the preset's caption and its
/// coefficients, each in the color of its dot or arrow. The coefficients are read-only.
///
/// Call it between `BeginDrawing` and `EndDrawing`, after `EndMode3D`.
///
/// @param preset  The curve being shown.
/// @param current The state to show.
///
/// @return The state after this frame's user input. When its `preset` differs from
///         `current.preset`, make a new scene for it.
[[nodiscard]] ViewState draw_controls(const CurvePreset& preset, ViewState current);
