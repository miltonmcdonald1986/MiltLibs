#include <raylib.h>

#include "framework/app_loop.hpp"
#include "horner_view.hpp"

int main() {
    constexpr milt::runner::AppConfig config{.width = 1100, .height = 650, .title = "NURBS runner"};

    constexpr ScreenSize screen{.width = config.width, .height = config.height};

    ViewState state;
    Scene scene = make_scene(state.preset, screen);

    milt::runner::run_app(config, [&] {
        ClearBackground(RAYWHITE);
        BeginMode3D(scene.camera);
        draw_scene(scene, state.u);
        EndMode3D();
        draw_labels(scene, state.u);

        const ViewState next = draw_controls(scene.preset, state);
        if (next.preset != state.preset) {
            scene = make_scene(next.preset, screen);
        }
        state = next;
    });
}
