#ifndef GRAPHICS_WINDOW_STATE_H
#define GRAPHICS_WINDOW_STATE_H

#include <GLFW/glfw3.h>

namespace graphics::window_state
{

    struct WindowState
    {
        GLFWwindow* pHandle{ nullptr };
        int height{ 768 };
        int width{ 1024 };
    };

}

#endif
