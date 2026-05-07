#ifndef GRAPHICS_CAMERA_CAMERA_SYSTEMS_HPP
#define GRAPHICS_CAMERA_CAMERA_SYSTEMS_HPP

#include <entt/fwd.hpp>

#include <glm/fwd.hpp>

namespace graphics::camera
{

    auto compute_projection(const entt::registry& reg, entt::entity cam_ent, float aspect) -> glm::mat4;
    void update_camera_system(entt::registry& reg, float time_delta);

}

#endif // GRAPHICS_CAMERA_CAMERA_SYSTEMS_HPP