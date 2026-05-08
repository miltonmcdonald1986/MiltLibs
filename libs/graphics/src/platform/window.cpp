#include <graphics/platform/window.h>

#include <imgui_impl_glfw.h>

#include <graphics/platform/glfw_callbacks.h>

namespace graphics::platform
{

    void ResizeAndCenterWindow(GLFWwindow* window)
    {
        //GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        //if (!monitor)
        //    return;

        //const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        //if (!mode)
        //    return;

        //// Shrink the window slightly to avoid taskbar/titlebar issues
        //const int marginX = 80;   // shrink horizontally
        //const int marginY = 80;   // shrink vertically

        //int targetWidth = (mode->width * 3) / 4 - marginX;
        //int targetHeight = mode->height - marginY;

        //// Apply size
        //glfwSetWindowSize(window, targetWidth, targetHeight);

        //// Center horizontally, and leave a small top margin
        //int xpos = (mode->width - targetWidth) / 2;
        //int ypos = marginY / 2;

        //glfwSetWindowPos(window, xpos, ypos);

        glfwMaximizeWindow(window);
    }

    Window::~Window()
    {
        terminate_glfw();
    }

    engine::Status Window::init_glfw(engine::AppData* p_app, int gl_version_major, int gl_version_minor, int gl_profile)
    {
        glfwSetErrorCallback(glfw_error_callback);

        if (glfwInit() != GLFW_TRUE)
            return UNEXPECTED(engine::ErrorCategory::Platform, "Failed to initialize GLFW");

        // Set the OpenGL version and profile we want to use.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_version_major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_version_minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, gl_profile);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // Create a window.
        if (GLFWwindow* window = glfwCreateWindow(window_state.width, window_state.height, window_config.title, window_config.pMonitor, window_config.pShare))
        {
            window_state.pHandle = window;
            ResizeAndCenterWindow(window);
        }
        else
            return UNEXPECTED(engine::ErrorCategory::Platform, "Failed to create GLFW window");

        // Make the OpenGL context of our window current on the calling thread.
        glfwMakeContextCurrent(window_state.pHandle);

        glfwSetWindowUserPointer(window_state.pHandle, p_app);

        glfwSetCharCallback(window_state.pHandle, glfw_char_callback);
        glfwSetCursorPosCallback(window_state.pHandle, glfw_cursor_pos_callback);
        glfwSetFramebufferSizeCallback(window_state.pHandle, glfw_framebuffer_size_callback);
        glfwSetKeyCallback(window_state.pHandle, glfw_key_callback);
        glfwSetMouseButtonCallback(window_state.pHandle, glfw_mouse_button_callback);

        // Do not let imgui install a scroll callback of its own.
        glfwSetScrollCallback(window_state.pHandle, glfw_scroll_callback);

        // Enable V-Sync
        glfwSwapInterval(1);

        // Initialize OpenGL function pointers using gl3w (or any other OpenGL loader).
        if (gl3wInit() != 0)
            return UNEXPECTED(engine::ErrorCategory::Platform, "Failed to initialize gl3w");

        return {};
    }

    void Window::poll_events()
    {
        glfwPollEvents();
    }

    void Window::swap_buffers()
    {
        glfwSwapBuffers(window_state.pHandle);
    }

    void Window::terminate_glfw()
    {
        glfwDestroyWindow(window_state.pHandle);
        window_state.pHandle = nullptr;

        glfwTerminate();
    }

}
