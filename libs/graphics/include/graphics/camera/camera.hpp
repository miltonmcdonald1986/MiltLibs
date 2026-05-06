#ifndef GRAPHICS_CAMERA_CAMERA_HPP
#define GRAPHICS_CAMERA_CAMERA_HPP

#include <graphics/camera/projection_type.h>

namespace graphics::camera
{

    struct Camera 
    {
        ProjectionType type{ ProjectionType::Perspective };
    };

} // namespace graphics::camera

#endif // GRAPHICS_CAMERA_CAMERA_HPP
