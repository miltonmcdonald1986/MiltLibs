#include <graphics/app/lifecycle.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <graphics/platform/glfw_callbacks.h>

using graphics::app::app::App;
using graphics::platform::glfw_callbacks::glfw_error_callback;
using graphics::platform::glfw_callbacks::glfw_framebuffer_size_callback;

namespace graphics::app::lifecycle
{

    std::expected<void, std::string> InitGLContext(App& app)
    {
        // Make the OpenGL context of our window current on the calling thread.
        glfwMakeContextCurrent(app.winState.pHandle);

        // Enable V-Sync
        glfwSwapInterval(1);

        // Initialize OpenGL function pointers using gl3w (or any other OpenGL loader).
        if (gl3wInit() != 0)
            return std::unexpected("Failed to initialize gl3w");

        return {};
    }

    std::expected<void, std::string> InitGLState(App& app)
    {
        int w, h;
        glfwGetFramebufferSize(app.winState.pHandle, &w, &h);
        glViewport(0, 0, w, h);

        if (app.glState.enable_depth_test)
        {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(app.glState.depth_func);
        }

        glPolygonMode(GL_FRONT_AND_BACK, app.glState.polygon_mode);

        auto& clearColor = app.glState.clear_color;
        glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

        // Check for any OpenGL errors that may have occurred during initialization.
        if (glGetError() != GL_NO_ERROR)
            return std::unexpected("Failed to init GL state");

        return {};
    }

    std::expected<void, std::string> InitPlatform(App& app)
    {
        glfwSetErrorCallback(glfw_error_callback);

        if (glfwInit() != GLFW_TRUE)
            return std::unexpected("Failed to initialize GLFW");

        // Set the OpenGL version and profile we want to use.
        auto& glConfig = app.glConfig;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glConfig.version_major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glConfig.version_minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, glConfig.profile);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // Create a window.
        auto& winState = app.winState;
        auto& winConfig = app.winConfig;
        if (GLFWwindow* window = glfwCreateWindow(winState.width, winState.height, winConfig.title, winConfig.pMonitor, winConfig.pShare))
            winState.pHandle = window;
        else
            return std::unexpected("Failed to create GLFW window");

        glfwSetWindowUserPointer(winState.pHandle, &app);
        glfwSetFramebufferSizeCallback(winState.pHandle, glfw_framebuffer_size_callback);

        return {};
    }

    void Shutdown(App& app)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(app.winState.pHandle);
        app.winState.pHandle = nullptr;
        glfwTerminate();
    }

}
