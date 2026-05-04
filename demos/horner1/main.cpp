#include <string>

#include <entt/entt.hpp>

#include <graphics/camera/camera_factory.h>
#include <graphics/components/color.h>
#include <graphics/components/flash.h>
#include <graphics/components/mesh_gl.h>
#include <graphics/components/shader.h>
#include <graphics/engine/app_data.h>
#include <graphics/engine/engine.h>
#include <graphics/engine/result.h>
#include <graphics/factories/mesh_factories.h>
#include <graphics/factories/shader_factories.h>
#include <graphics/factories/texture_factories.h>
#include <graphics/scene/scene.h>
#include <graphics/ui/entity_list.h>
#include <graphics/ui/inspector.h>
#include <graphics/ui/widgets.h>

bool init(graphics::engine::AppData* p_data)
{
    if (!p_data)
        return false;

    graphics::scene::Scene* p_scene = p_data->p_active_scene;
    if (!p_scene)
        return false;

    graphics::platform::Window* p_window = p_data->p_window;
    if (!p_window)
        return false;

    entt::registry& reg = p_scene->reg;

    graphics::engine::Result<graphics::components::Texture> tex_result = graphics::factories::create_texture_from_file(R"(C:\Users\milto\Downloads\wall.jpg)");
    for (int i = 0; i < 5; i++) {
        entt::entity e = reg.create();
        reg.emplace<graphics::components::Transform>(e,
            glm::vec3(i * 2.0f, 0, -5.0f),
            glm::vec3(0, 0, 0),
            glm::vec3(1, 1, 1)
        );
        reg.emplace<graphics::components::Color>(e, graphics::components::Color{});
        reg.emplace<graphics::components::Flash>(e, graphics::components::Flash{});
        reg.emplace<graphics::components::MeshGL>(e, *graphics::factories::create_textured_cube_mesh());
        reg.emplace<graphics::components::Shader>(e, *graphics::factories::create_textured_color_mvp_shader());
        if (tex_result)
            reg.emplace<graphics::components::Texture>(e, *tex_result);
    }

    entt::entity camera = reg.create();
    graphics::camera::CameraConfig camera_config{};
    graphics::camera::add_camera(reg, camera, camera_config);
    
    return true;
}

bool update(graphics::engine::AppData* p_data)
{
    if (!p_data)
        return false;

    // Optional UI
    graphics::ui::draw_entity_list(p_data);
    graphics::ui::draw_inspector(p_data);
    graphics::ui::draw_log_widget();

    return true;
}

int main(void)
{
    auto result = run(init, update);
    return result.has_value() ? 0 : 1;
}
