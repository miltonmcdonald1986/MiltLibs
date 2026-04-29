#ifndef GRAPHICS_SYSTEMS_CAMERA_H
#define GRAPHICS_SYSTEMS_CAMERA_H

#include <entt/entt.hpp>

#include <graphics/components/camera.h>
#include <graphics/components/transform.h>

namespace graphics::systems::camera
{

    glm::mat4 compute_projection(const components::camera::Camera& cam);
    glm::vec3 get_forward(const entt::registry& reg, entt::entity e);
    glm::vec3 get_right(const entt::registry& reg, entt::entity e);
    glm::vec3 get_up(const entt::registry& reg, entt::entity e);
    void apply_mouse_look(components::transform::Transform& t, float dx, float dy, float sensitivity);
    void move_camera_forward(components::transform::Transform& t, const entt::registry& reg, entt::entity cam, float amount);
    void move_camera_right(components::transform::Transform& t, const entt::registry& reg, entt::entity cam, float amount);
    void move_camera_up(components::transform::Transform& t, const entt::registry& reg, entt::entity cam, float amount);
    void update_camera_system(entt::registry& reg);
    void zoom_camera(entt::registry& reg, entt::entity camEntity, float amount);

}

#endif // GRAPHICS_SYSTEMS_CAMERA_H
