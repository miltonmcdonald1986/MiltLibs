#ifndef GRAPHICS_CAMERA_CAMERA_HPP
#define GRAPHICS_CAMERA_CAMERA_HPP

#include <graphics/camera/camera_defaults.hpp>
#include <graphics/camera/projection_type.hpp>

namespace graphics::camera
{

    struct Camera 
    {
        ProjectionType type{ Defaults::type };
    };

} // namespace graphics::camera

#endif // GRAPHICS_CAMERA_CAMERA_HPP