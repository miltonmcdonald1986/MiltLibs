#ifndef GRAPHICS_CAMERA_ORTHOGRAPHIC_CAMERA_HPP
#define GRAPHICS_CAMERA_ORTHOGRAPHIC_CAMERA_HPP

#include <graphics/camera/clip_planes.hpp>
#include <graphics/camera/camera_defaults.hpp>

namespace graphics::camera
{

    struct OrthographicCamera
    {
        float       height      { Defaults::height };   // vertical size of view volume in world units
        ClipPlanes  clip_planes {};
    };

} // namespace graphics::camera

#endif // GRAPHICS_CAMERA_ORTHOGRAPHIC_CAMERA_HPP