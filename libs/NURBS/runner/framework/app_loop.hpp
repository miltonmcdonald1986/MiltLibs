/// @file
/// @brief Window setup and main loop for the NURBS runner.

#pragma once

#include <functional>

namespace milt::runner {

/// @brief Settings for the runner's window.
struct AppConfig {
    int width;         ///< Window width in pixels.
    int height;        ///< Window height in pixels.
    const char* title; ///< Window title.
};

/// @brief Opens a window and runs the main loop until the user closes it.
///
/// Each frame, `frame` is called between `BeginDrawing` and `EndDrawing`, so it doesn't call
/// them itself. The window is closed before this function returns.
///
/// @param config The window's size and title.
/// @param frame  Called once per frame to update and draw the app.
void run_app(const AppConfig& config, std::function<void()> frame);

} // namespace milt::runner
