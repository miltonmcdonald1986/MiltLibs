#include <raylib.h>

#include "framework/app_loop.hpp"
#include "horner_presets.hpp"
#include "horner_view.hpp"

int main() {
    constexpr milt::runner::AppConfig config{.width = 800, .height = 600, .title = "NURBS runner"};

    ViewState state;
    const Scene scene =
        make_scene(curve_presets.front(), static_cast<double>(config.width) / config.height);

    milt::runner::run_app(config, [&] {
        ClearBackground(RAYWHITE);
        BeginMode3D(scene.camera);
        draw_scene(scene, state.u);
        EndMode3D();
        state = draw_controls(scene.preset, state);
    });
}
