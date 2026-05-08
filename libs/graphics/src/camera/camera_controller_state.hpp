#ifndef GRAPHICS_CAMERA_CAMERA_CONTROLLER_STATE_HPP
#define GRAPHICS_CAMERA_CAMERA_CONTROLLER_STATE_HPP

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>

namespace graphics::camera
{

    struct CameraControllerState 
    {
        glm::vec3   move_delta  { 0.F };
        glm::vec2   look_delta  { 0.F };
        bool        look_active { false };
        float       zoom_delta  { 0.F };
    };

}

#endif // GRAPHICS_CAMERA_CAMERA_CONTROLLER_STATE_HPP
