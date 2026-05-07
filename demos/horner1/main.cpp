#include <entt/entt.hpp>

#include <graphics/camera/camera_factory.hpp>
#include <graphics/components/color.hpp>
#include <graphics/components/flash.hpp>
#include <graphics/components/mesh_gl.hpp>
#include <graphics/components/shader.hpp>
#include <graphics/components/texture.h>
#include <graphics/engine/app_data.h>
#include <graphics/engine/engine.h>
#include <graphics/factories/mesh_factories.h>
#include <graphics/factories/shader_factories.h>
#include <graphics/factories/texture_factories.h>
#include <graphics/scene/scene.h>
#include <graphics/ui/entity_list.h>
#include <graphics/ui/inspector.h>
#include <graphics/ui/widgets.h>

auto init(graphics::engine::AppData* p_data) -> bool
{
    if (p_data == nullptr)
    {
        return false;
    }

    graphics::scene::Scene* p_scene = p_data->p_active_scene;
    if (p_scene == nullptr)
    {
        return false;
    }

    graphics::platform::Window* p_window = p_data->p_window;
    if (p_window == nullptr)
    {
        return false;
    }

    entt::registry& reg = p_scene->reg;

    graphics::engine::Result<graphics::components::Texture> tex_result = graphics::factories::create_texture_from_file(R"(C:\Users\milto\Downloads\wall.jpg)");
    const int num_cubes = 5;
    for (int i = 0; i < num_cubes; i++) {
        entt::entity ent = reg.create();
        reg.emplace<graphics::components::Transform>(ent,
            glm::vec3(i * 2.0f, 0, -5.0f),
            glm::vec3(0, 0, 0),
            glm::vec3(1, 1, 1)
        );
        reg.emplace<graphics::components::Color>(ent, graphics::components::Color{});
        reg.emplace<graphics::components::Flash>(ent, graphics::components::Flash{});
        reg.emplace<graphics::components::MeshGL>(ent, *graphics::factories::create_textured_cube_mesh());
        reg.emplace<graphics::components::Shader>(ent, *graphics::factories::create_textured_color_mvp_shader());
        if (tex_result)
            reg.emplace<graphics::components::Texture>(ent, *tex_result);
    }

    entt::entity camera = reg.create();
    graphics::camera::CameraConfig camera_config{};
    graphics::camera::add_camera(reg, camera, camera_config);
    
    return true;
}

auto update(graphics::engine::AppData* p_data) -> bool
{
    if (p_data == nullptr)
    {
        return false;
    }

    // Optional UI
    graphics::ui::draw_entity_list(p_data);
    graphics::ui::draw_inspector(p_data);
    graphics::ui::draw_log_widget();

    return true;
}

auto main() -> int
{
    auto result = run(init, update);
    return result.has_value() ? 0 : 1;
}
