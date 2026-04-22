#include <graphics/app.h>
#include <graphics/glfw_callbacks.h>

#include <print>

namespace graphics::glfw_callbacks
{

    void glfw_error_callback(int error_code, const char* description)
    {
        std::print("GLFW Error (code {}): {}\n", error_code, description);
    }

    void glfw_framebuffer_size_callback(GLFWwindow* pWindow, int w, int h)
    {
        using graphics::app::App;

        if (App* app = static_cast<App*>(glfwGetWindowUserPointer(pWindow)))
        {
            app->winState.width = w;
            app->winState.height = h;
        }

        glViewport(0, 0, w, h);
    }

} // namespace graphics::glfw_callbacks
