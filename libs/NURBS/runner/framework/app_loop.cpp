#include "app_loop.hpp"

#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

namespace milt::runner {

namespace {

// Draws one frame. `arg` points to the std::function passed to run_app; it's a void* so
// Emscripten can call this as its main loop callback.
void run_frame(void* arg) {
    const auto& frame = *static_cast<std::function<void()>*>(arg);
    BeginDrawing();
    frame();
    EndDrawing();
}

} // namespace

void run_app(const AppConfig& config, std::function<void()> frame) {
    InitWindow(config.width, config.height, config.title);

#ifdef __EMSCRIPTEN__
    // The browser paces frames (fps 0 means requestAnimationFrame). Simulating an infinite
    // loop keeps this stack frame, and so `frame`, alive while the main loop runs.
    emscripten_set_main_loop_arg(run_frame, &frame, 0, true);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        run_frame(&frame);
    }
    CloseWindow();
#endif
}

} // namespace milt::runner
