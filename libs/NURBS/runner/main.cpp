#include <raylib.h>

#include "framework/app_loop.hpp"
#include "framework/viewport.hpp"
#include "triangle_model.hpp"
#include "triangle_view.hpp"

int main() {
    constexpr milt::runner::AppConfig config{.width = 800, .height = 600, .title = "NURBS runner"};
    const milt::runner::Viewport viewport{.center = {config.width / 2.0, config.height / 2.0},
                                          .pixels_per_unit = 200.0};
    TriangleModel model;

    milt::runner::run_app(config, [&] {
        ClearBackground(RAYWHITE);
        model.settings = draw_settings_panel(model.settings);
        model.update(GetFrameTime());
        draw_triangle(model, viewport);
    });
}
