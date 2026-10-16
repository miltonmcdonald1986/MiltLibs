#include "app_loop.hpp"

#include <raylib.h>

namespace milt::runner {

void run_app(const AppConfig& config, std::function<void()> frame) {
    InitWindow(config.width, config.height, config.title);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        frame();
        EndDrawing();
    }

    CloseWindow();
}

} // namespace milt::runner
