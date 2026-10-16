#include "triangle_view.hpp"

#include <raylib.h>

#include "framework/raylib_convert.hpp"

void draw_triangle(const TriangleModel& model, const milt::runner::Viewport& viewport) {
    using milt::runner::to_screen;
    const auto& [a, b, c] = model.vertices;
    // The model's vertices are counter-clockwise, which raylib needs to avoid culling them.
    DrawTriangle(to_screen(viewport, a), to_screen(viewport, b), to_screen(viewport, c), MAROON);
}
