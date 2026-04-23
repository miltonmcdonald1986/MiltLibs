#include <expected>
#include <print>
#include <string>

#include <entt/entt.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <graphics/animation_systems.h>
#include <graphics/app/app.h>
#include <graphics/app/app_guard.h>
#include <graphics/app/lifecycle.h>
#include <graphics/components/color.h>
#include <graphics/components/flash.h>
#include <graphics/components/mesh_gl.h>
#include <graphics/components/shader.h>
#include <graphics/components/texture.h>
#include <graphics/factories/mesh_factories.h>
#include <graphics/factories/prefab_factories.h>
#include <graphics/factories/texture_factories.h>
#include <graphics/gl_config.h>
#include <graphics/gl_state.h>
#include <graphics/mesh/mesh_factory_backend.h>
#include <graphics/shader_factory.h>
#include <graphics/ui/widgets.h>

using graphics::app::app::App;
using graphics::app::app_guard::AppGuard;
using graphics::app::lifecycle::InitGLContext;
using graphics::app::lifecycle::InitGLState;
using graphics::app::lifecycle::InitPlatform;
using graphics::animation_systems::update_flash;
using graphics::components::color::Color;
using graphics::components::flash::Flash;
using graphics::components::mesh_gl::MeshGL;
using graphics::components::shader::Shader;
using graphics::components::texture::Texture;
using graphics::factories::mesh_factories::create_rainbow_triangle_mesh;
using graphics::factories::mesh_factories::create_textured_triangle_mesh;
using graphics::factories::prefab_factories::create_rainbow_triangle_ent;
using graphics::factories::prefab_factories::create_solid_color_triangle_ent;
using graphics::factories::texture_factories::create_texture_from_file;
using graphics::mesh::mesh_factory_backend::create_mesh_gl_layout;
using graphics::mesh::mesh_factory_backend::create_mesh_gl_pos_only;
using graphics::shader_factory::create_color_shader;
using graphics::shader_factory::create_textured_shader;
using graphics::shader_factory::create_vertex_color_shader;
using graphics::ui::widgets::draw_flash_widget;
using graphics::ui::widgets::draw_per_entity_color_widget;
using graphics::ui::widgets::draw_render_settings_widget;

std::expected<void, std::string> Update(App& app)
{
	// draw widgets before updating animations so that UI changes are reflected immediately in the same frame
    draw_render_settings_widget(app);
    draw_per_entity_color_widget(app);
    draw_flash_widget(app);

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

        // --- Optional Color uniform ---
        if (auto color = app.reg.try_get<Color>(entity))
        {
            GLint loc = glGetUniformLocation(shader.id, "u_color");
            if (loc >= 0)
                glUniform4fv(loc, 1, color->rgba);
        }

        // --- Optional Texture binding ---
        if (auto tex = app.reg.try_get<Texture>(entity))
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex->id);

            GLint loc = glGetUniformLocation(shader.id, "uTexture");
            if (loc >= 0)
                glUniform1i(loc, 0); // texture unit 0
        }

        // --- Draw ---
        glBindVertexArray(mesh.vao);

        if (mesh.indexCount > 0)
            glDrawElements(mesh.primitive, mesh.indexCount, GL_UNSIGNED_INT, 0);
        else
            glDrawArrays(mesh.primitive, 0, mesh.vertexCount);
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
    
    // RAII guard: runs Shutdown(app) when this scope ends.
    AppGuard guard{ app };

    entt::entity e = app.reg.create();

    if (auto mesh_result = create_textured_triangle_mesh())
        app.reg.emplace<MeshGL>(e, *mesh_result);
    
    if (auto texture_result = create_texture_from_file(R"(C:\Users\milto\Downloads\wall.jpg)"))
        app.reg.emplace<Texture>(e, *texture_result);

    if (auto shader_result = create_textured_shader())
		app.reg.emplace<Shader>(e, *shader_result);

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

    return 0;
}
