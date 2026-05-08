#include <graphics/rendering/renderer.h>

#include <glm/gtc/type_ptr.hpp>

#include <graphics/camera/camera.hpp>
#include <graphics/camera/camera_matrices.hpp>
#include <graphics/camera/camera_systems.hpp>
#include <graphics/components/color.hpp>
#include <graphics/components/flash.hpp>
#include <graphics/components/shader.hpp>
#include <graphics/components/texture.hpp>
#include <graphics/components/world_matrix.hpp>
#include <graphics/mesh/mesh_gl.hpp>
#include <graphics/platform/gl_includes.h>

#include <math/convert_mat4.hpp>
#include <math/convert_vec4.hpp>
#include <utils/math_utils.h>

namespace graphics::rendering
{

    glm::vec4 compute_final_color(const components::Color& color, const components::Flash* flash)
    {
        if (!flash->enabled)
            return math::to_glm(color.rgba);

        float intensity = std::cos(flash->t) * 0.5f + 0.5f;
        return glm::vec4(color.rgba.v[0] * intensity, color.rgba.v[1] * intensity, color.rgba.v[2] * intensity, color.rgba.v[3]);
    }

    engine::Status Renderer::init(int framebuffer_width, int framebuffer_height)
    {
        glViewport(0, 0, framebuffer_width, framebuffer_height);

        if (gl_state.enable_depth_test)
        {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(gl_state.depth_func);
        }

        glPolygonMode(GL_FRONT_AND_BACK, gl_state.polygon_mode);

        auto& clearColor = gl_state.clear_color;
        glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

        // Check for any OpenGL errors that may have occurred during initialization.
        if (glGetError() != GL_NO_ERROR)
            return UNEXPECTED(engine::ErrorCategory::Rendering, "An OpenGL occurred while trying to init GL state");

        return {};
    }

    engine::Status Renderer::update(scene::Scene* p_scene)
    {
        if (!p_scene)
            return UNEXPECTED(engine::ErrorCategory::Rendering, "No active scene found");

        entt::registry& reg = p_scene->reg;

        glClearColor(
            gl_state.clear_color[0],
            gl_state.clear_color[1],
            gl_state.clear_color[2],
            gl_state.clear_color[3]
        );

        glClear(gl_state.clear_buffer_mask);

        if (gl_state.display_wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        entt::entity camera = reg.view<camera::Camera>().front();
        camera::CameraMatrices* p_camera_matrices = reg.try_get<camera::CameraMatrices>(camera);
        glm::mat4 view = p_camera_matrices ? math::to_glm(p_camera_matrices->view) : glm::mat4(1.F);
        glm::mat4 proj = p_camera_matrices ? math::to_glm(p_camera_matrices->projection) : glm::mat4(1.F);

        // ---------------------------------------------------------
        // 3. Render all meshes
        // ---------------------------------------------------------
        auto ents = reg.view<components::Shader, mesh::MeshGL>();
        for (auto [entity, shader, mesh] : ents.each())
        {
            glUseProgram(shader.id);

            // --- Optional Color uniform ---
            if (components::Color* p_color_component = reg.try_get<components::Color>(entity))
            {
                glm::vec4 final_color = compute_final_color(*p_color_component, reg.try_get<components::Flash>(entity));
                GLint loc = glGetUniformLocation(shader.id, "u_color");
                if (loc >= 0)
                    glUniform4fv(loc, 1, glm::value_ptr(final_color));
            }

            // --- Optional Texture binding ---
            if (auto tex = reg.try_get<components::Texture>(entity))
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, tex->id);

                GLint loc = glGetUniformLocation(shader.id, "uTexture");
                if (loc >= 0)
                    glUniform1i(loc, 0);
            }

            // --- Upload uModel ---
            if (GLint loc = glGetUniformLocation(shader.id, "uModel"); loc >= 0)
                glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(math::to_glm(reg.get<components::WorldMatrix>(entity).value)));

            // --- Upload uView ---
            if (GLint loc = glGetUniformLocation(shader.id, "uView"); loc >= 0)
                glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));

            // --- Upload uProjection ---
            if (GLint loc = glGetUniformLocation(shader.id, "uProjection"); loc >= 0)
                glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(proj));

            // --- Draw ---
            glBindVertexArray(mesh.vao);

            if (mesh.indexCount > 0)
                glDrawElements(mesh.primitive, mesh.indexCount, GL_UNSIGNED_INT, 0);
            else
                glDrawArrays(mesh.primitive, 0, mesh.vertexCount);
        }

        return {};
    }

}
