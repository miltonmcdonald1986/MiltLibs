#ifndef GRAPHICS_CAMERA_CAMERA_SYSTEMS_HPP
#define GRAPHICS_CAMERA_CAMERA_SYSTEMS_HPP

#include <entt/fwd.hpp>

namespace graphics::camera
{

    void update_camera_system(entt::registry& reg, float time_delta);

}

#endif // GRAPHICS_CAMERA_CAMERA_SYSTEMS_HPP