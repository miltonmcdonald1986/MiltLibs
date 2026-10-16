#include <raylib.h>

#include "framework/app_loop.hpp"

int main() {
    milt::runner::run_app({.width = 800, .height = 600, .title = "NURBS runner"},
                          [] { ClearBackground(RAYWHITE); });
}
