#include "triangle_view.hpp"

#include <format>
#include <string>

#include <raygui.h>
#include <raylib.h>

#include "framework/raylib_convert.hpp"
#include "framework/viewport.hpp"
#include "triangle_model.hpp"

void draw_triangle(const TriangleModel& model, const milt::runner::Viewport& viewport) {
    using milt::runner::to_screen;
    const auto [a, b, c] = model.vertices();
    // The model's vertices are counter-clockwise, which raylib needs to avoid culling them.
    DrawTriangle(to_screen(viewport, a), to_screen(viewport, b), to_screen(viewport, c), MAROON);
}

TriangleSettings draw_settings_panel(TriangleSettings current) {
    GuiGroupBox({.x = 10, .y = 10, .width = 230, .height = 80}, "Triangle");
    GuiCheckBox({.x = 20, .y = 25, .width = 20, .height = 20}, "Spin", &current.spinning);

    // raygui sliders work in float.
    auto speed = static_cast<float>(current.speed);
    const std::string label = std::format("{:.1f}", current.speed);
    GuiSlider({.x = 70, .y = 55, .width = 120, .height = 20}, "Speed", label.c_str(), &speed, 0.0f,
              6.0f);
    current.speed = speed;

    return current;
}
