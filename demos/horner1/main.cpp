#include <graphics/app/app.h>
#include <graphics/app/app_loop.h>
#include <graphics/components/mesh_gl.h>
#include <graphics/components/shader.h>
#include <graphics/components/texture.h>
#include <graphics/components/transform.h>
#include <graphics/factories/mesh_factories.h>
#include <graphics/factories/texture_factories.h>
#include <graphics/factories/shader_factories.h>
#include <graphics/systems/animation.h>
#include <graphics/ui/widgets.h>

using graphics::app::app::App;
using graphics::app::app_loop::run_app;
using graphics::components::mesh_gl::MeshGL;
using graphics::components::shader::Shader;
using graphics::components::texture::Texture;
using graphics::components::transform::Transform;
using graphics::factories::mesh_factories::create_textured_triangle_mesh;
using graphics::factories::texture_factories::create_texture_from_file;
using graphics::factories::shader_factories::create_textured_mvp_shader;
using graphics::systems::animation::update_flash;
using graphics::ui::widgets::draw_flash_widget;
using graphics::ui::widgets::draw_per_entity_color_widget;
using graphics::ui::widgets::draw_render_settings_widget;

std::expected<void, std::string> init(App& app)
{
    entt::entity e = app.reg.create();

    // Mesh
    if (auto mesh_result = create_textured_triangle_mesh())
        app.reg.emplace<MeshGL>(e, *mesh_result);
    else
        return std::unexpected("Failed to create mesh");

    // Texture
    if (auto texture_result = create_texture_from_file(R"(C:\Users\milto\Downloads\wall.jpg)"))
        app.reg.emplace<Texture>(e, *texture_result);
    else
        return std::unexpected("Failed to create texture");

    // Shader (MVP-capable)
    if (auto shader_result = create_textured_mvp_shader())
        app.reg.emplace<Shader>(e, *shader_result);
    else
        return std::unexpected("Failed to create shader");

    // Transform (start at origin)
    app.reg.emplace<Transform>(e);

    return {};
}

std::expected<void, std::string> update(App& app)
{
    // UI first (optional)
    draw_render_settings_widget(app);
    draw_per_entity_color_widget(app);
    draw_flash_widget(app);

    // Update animation system
    update_flash(app.reg, app.delta_time);

    // Rotate the entity
    auto view = app.reg.view<Transform>();
    for (auto e : view)
    {
        auto& t = view.get<Transform>(e);
        t.rotation.y += 1.0f * static_cast<float>(app.delta_time); // spin slowly
    }

    return {};
}


int main(void)
{
    return run_app(init, update);
}
