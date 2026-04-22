#include <expected>
#include <print>
#include <string>

#include <entt/entt.hpp>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <graphics/animation_systems.h>
#include <graphics/app.h>
#include <graphics/components/color.h>
#include <graphics/components/flash.h>
#include <graphics/components/mesh_gl.h>
#include <graphics/components/shader.h>
#include <graphics/factories/mesh_factories.h>
#include <graphics/factories/prefab_factories.h>
#include <graphics/gl_config.h>
#include <graphics/gl_state.h>
#include <graphics/glfw_callbacks.h>
#include <graphics/mesh/mesh_factory_backend.h>
#include <graphics/shader_factory.h>

using graphics::app::App;
using graphics::animation_systems::update_flash;
using graphics::components::color::Color;
using graphics::components::flash::Flash;
using graphics::components::mesh_gl::MeshGL;
using graphics::components::shader::Shader;
using graphics::factories::mesh_factories::create_rainbow_triangle_mesh;
using graphics::factories::prefab_factories::create_rainbow_triangle_ent;
using graphics::glfw_callbacks::glfw_error_callback;
using graphics::glfw_callbacks::glfw_framebuffer_size_callback;
using graphics::mesh::mesh_factory_backend::create_mesh_gl_pos_only;
using graphics::shader_factory::create_color_shader;
using graphics::shader_factory::create_vertex_color_shader;

void Shutdown(graphics::app::App& app)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(app.winState.pHandle);
    app.winState.pHandle = nullptr;
    glfwTerminate();
}

// Initializes GLFW, creates a window, and sets up the OpenGL context. Returns an error message if any step fails.
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
    {
        Shutdown(app);
		return std::unexpected("Failed to create GLFW window");
    }

    glfwSetWindowUserPointer(winState.pHandle, &app);
    glfwSetFramebufferSizeCallback(winState.pHandle, glfw_framebuffer_size_callback);
    
	return {};
}

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

std::expected<void, std::string> InitGLState(App &app)
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

std::expected<void, std::string> Update(App& app)
{
    // Handle ImGui updates first.
    ImGui::Begin("Render Settings");
    ImGui::Checkbox("Wireframe", &app.glState.display_wireframe);
    ImGui::ColorEdit3("Background", app.glState.clear_color);
    ImGui::End();

    // Edit per‑entity color
    ImGui::Begin("Entity Colors");
    auto view = app.reg.view<Color>();
    for (auto [entity, color] : view.each()) {
        std::string label = "entity " + std::to_string((uint32_t)entity);
        ImGui::ColorEdit4(label.c_str(), color.base);
    }
    ImGui::End();

	update_flash(app.reg, app.delta_time);

    return {};
}

std::expected<void, std::string> Render(App& app)
{
    glClearColor(
        app.glState.clear_color[0],
        app.glState.clear_color[1],
        app.glState.clear_color[2],
        app.glState.clear_color[3]
    );

    glClear(app.glState.clear_buffer_mask);

    if (app.glState.display_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    auto ents = app.reg.view<Shader, MeshGL>();
    for (auto [entity, shader, mesh] : ents.each())
    {
        glUseProgram(shader.id);

		// If the entity has a Color component, set the shader uniform.
		if (auto result = app.reg.try_get<Color>(entity); result) 
        {
            Color& color = *result; 
            GLint loc = glGetUniformLocation(shader.id, "u_color");
            glUniform4fv(loc, 1, color.rgba);
        }

        if (mesh.indexCount > 0) {
            glBindVertexArray(mesh.vao);
            glDrawElements(mesh.primitive, mesh.indexCount, GL_UNSIGNED_INT, 0);
        }
        else {
            glBindVertexArray(mesh.vao);
            glDrawArrays(mesh.primitive, 0, mesh.vertexCount);
        }
    }

    return {};
}

std::expected<MeshGL, std::string>
CreateIndexedMeshGL(const float* vertices,
    std::size_t floatCount,
    const unsigned int* indices,
    std::size_t indexCount,
    GLint componentsPerVertex = 3, GLenum primitive = GL_TRIANGLES)
{
    MeshGL mesh{};
    mesh.primitive = primitive;

    // Validate vertex layout
    if (floatCount % componentsPerVertex != 0)
        return std::unexpected("Vertex data size does not match attribute layout");

    mesh.indexCount = static_cast<GLsizei>(indexCount);

    // --- VAO ---
    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    // --- VBO ---
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER,
        floatCount * sizeof(float),
        vertices,
        GL_STATIC_DRAW);

    // --- EBO ---
    glGenBuffers(1, &mesh.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indexCount * sizeof(unsigned int),
        indices,
        GL_STATIC_DRAW);

    // --- Vertex Attribute ---
    glVertexAttribPointer(
        0,
        componentsPerVertex,
        GL_FLOAT,
        GL_FALSE,
        componentsPerVertex * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    if (glGetError() != GL_NO_ERROR)
        return std::unexpected("Failed to create indexed MeshGL");

    return mesh;
}

void InitImGui(App& app)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(app.winState.pHandle, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

int main(void)
{
	// Create an instance of our application state (if we had any).
	App app{};
	
    if (auto result = InitPlatform(app); !result)
    {
        std::print("Failed to initialize GLFW: {}\n", result.error());
        return -1;
    }

	if (auto result = InitGLContext(app); !result)
    {
        std::print("Failed to initialize OpenGL context: {}\n", result.error());
        return -1;
    }

    InitImGui(app);

    if (auto result = InitGLState(app); !result)
    {
        std::print("Failed to initialize OpenGL state: {}\n", result.error());
        return -1;
    }

	// Let's create a triangle for demo purposes.
    auto color_shader_result = create_color_shader();
    if (!color_shader_result)
    {
        std::print("Failed to create shader: {}\n", color_shader_result.error());
        return -1;
	}

    auto ent_rainbow_triangle = create_rainbow_triangle_ent(app.reg);

    entt::entity ent_right_triangle = app.reg.create();
    float right_triangle_vertices[] = {
         0.25f,  0.25f, 0.0f,   // top-left
         0.75f,  0.25f, 0.0f,   // top-right
         0.50f, -0.25f, 0.0f    // bottom
    };

	Color right_triangle_color(0.2f, 0.4f, 1.0f, 1.0f);
    app.reg.emplace<Color>(ent_right_triangle, right_triangle_color);
    app.reg.emplace<Shader>(ent_right_triangle, *color_shader_result);
    app.reg.emplace<MeshGL>(ent_right_triangle, *create_mesh_gl_pos_only(right_triangle_vertices));
    app.reg.emplace<Flash>(ent_right_triangle, Flash{});

    double last_time = glfwGetTime();
	auto* pWin = app.winState.pHandle;
    while (!glfwWindowShouldClose(pWin))
    {
        double now = glfwGetTime();
        app.delta_time = now - last_time;
        last_time = now;

		// Poll for and process events (e.g., keyboard input, mouse movement, window resizing).
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		if (auto result = Update(app); !result)
			std::print("Failed to update frame: {}\n", result.error());

        if (auto result = Render(app); !result)
			std::print("Failed to render frame: {}\n", result.error());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap front and back buffers (display the rendered image).
        glfwSwapBuffers(pWin);
    }

    Shutdown(app);
    return 0;
}
