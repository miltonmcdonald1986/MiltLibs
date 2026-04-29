#ifndef GRAPHICS_COMPONENTS_CAMERA_H
#define GRAPHICS_COMPONENTS_CAMERA_H

#include <glm/glm.hpp>

namespace graphics::components::camera
{

    enum class ProjectionType 
    {
        Perspective,
        Orthographic
    };

    struct Camera 
    {
        ProjectionType type = ProjectionType::Perspective;

        // Perspective
        float fov = glm::radians(60.0f);

        // Orthographic
        float orthoHeight = 10.0f;

        // Shared
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;
        float aspect = 16.0f / 9.0f;

        bool primary = true;
    };

}

#endif // GRAPHICS_COMPONENTS_CAMERA_H
