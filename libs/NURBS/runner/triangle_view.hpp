/// @file
/// @brief Drawing and controls for the NURBS runner's triangle.

#pragma once

#include "framework/viewport.hpp"
#include "triangle_model.hpp"

/// @brief Draws the triangle as a filled shape.
///
/// Call it between `BeginDrawing` and `EndDrawing`.
///
/// @param model    The triangle to draw.
/// @param viewport Maps the model's world coordinates to the screen.
void draw_triangle(const TriangleModel& model, const milt::runner::Viewport& viewport);

/// @brief Draws a raygui panel for editing the triangle's settings.
///
/// Call it between `BeginDrawing` and `EndDrawing`.
///
/// @param current The settings to show.
///
/// @return The settings after this frame's user input.
[[nodiscard]] TriangleSettings draw_settings_panel(TriangleSettings current);
