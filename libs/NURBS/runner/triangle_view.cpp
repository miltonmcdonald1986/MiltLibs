#include "triangle_view.hpp"

#include <raygui.h>
#include <raylib.h>

#include "framework/raylib_convert.hpp"

void draw_triangle(const TriangleModel& model, const milt::runner::Viewport& viewport) {
    using milt::runner::to_screen;
    const auto [a, b, c] = model.vertices();
    // The model's vertices are counter-clockwise, which raylib needs to avoid culling them.
    DrawTriangle(to_screen(viewport, a), to_screen(viewport, b), to_screen(viewport, c), MAROON);
}

TriangleSettings draw_settings_panel(TriangleSettings current) {
    GuiGroupBox({10, 10, 230, 80}, "Triangle");
    GuiCheckBox({20, 25, 20, 20}, "Spin", &current.spinning);

    // raygui sliders work in float.
    auto speed = static_cast<float>(current.speed);
    GuiSlider({70, 55, 120, 20}, "Speed", TextFormat("%.1f", current.speed), &speed, 0.0f, 6.0f);
    current.speed = speed;

    return current;
}
