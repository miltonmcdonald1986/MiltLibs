#include <graphics/app/app.h>
#include <graphics/platform/glfw_callbacks.h>

#include <print>

using graphics::app::app::App;

namespace graphics::platform::glfw_callbacks
{

    void glfw_error_callback(int error_code, const char* description)
    {
        std::print("GLFW Error (code {}): {}\n", error_code, description);
    }

    void glfw_framebuffer_size_callback(GLFWwindow* pWindow, int w, int h)
    {
        if (App* app = static_cast<App*>(glfwGetWindowUserPointer(pWindow)))
        {
            app->winState.width = w;
            app->winState.height = h;
        }

        glViewport(0, 0, w, h);
    }

} // namespace graphics::glfw_callbacks
