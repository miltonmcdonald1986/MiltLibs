#include <graphics/systems/render.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <graphics/components/camera.h>
#include <graphics/components/color.h>
#include <graphics/components/mesh_gl.h>
#include <graphics/components/shader.h>
#include <graphics/components/texture.h>
#include <graphics/components/transform.h>
#include <graphics/components/world_matrix.h>
#include <graphics/systems/camera.h>
#include <graphics/systems/transform.h>

using graphics::app::app::App;
using graphics::components::camera::Camera;
using graphics::components::camera::ProjectionType;
using graphics::components::color::Color;
using graphics::components::mesh_gl::MeshGL;
using graphics::components::shader::Shader;
using graphics::components::texture::Texture;
using graphics::components::transform::Transform;
using graphics::components::world_matrix::WorldMatrix;
using graphics::systems::camera::compute_projection;
using graphics::systems::transform::compute_model_matrix;

namespace graphics::systems::render
{

    std::expected<void, std::string> render_system_update(App& app)
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

        // ---------------------------------------------------------
        // 1. Find the active camera
        // ---------------------------------------------------------
        entt::entity camEntity = entt::null;
        auto camView = app.reg.view<Camera, WorldMatrix>();

        for (auto e : camView) {
            if (camView.get<Camera>(e).primary) {
                camEntity = e;
                break;
            }
        }

        if (camEntity == entt::null) {
            return std::unexpected("No active camera found");
        }

        auto& cam = app.reg.get<Camera>(camEntity);
        auto& camWM = app.reg.get<WorldMatrix>(camEntity);

        // ---------------------------------------------------------
        // 2. Compute view + projection
        // ---------------------------------------------------------
        glm::mat4 view = glm::inverse(camWM.value);
        glm::mat4 proj = compute_projection(cam);

        // ---------------------------------------------------------
        // 3. Render all meshes
        // ---------------------------------------------------------
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
                    glUniform1i(loc, 0);
            }

            // --- Upload uModel ---
            if (GLint loc = glGetUniformLocation(shader.id, "uModel"); loc >= 0)
                glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(app.reg.get<WorldMatrix>(entity).value));

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
