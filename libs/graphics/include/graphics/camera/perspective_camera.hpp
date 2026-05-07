#ifndef GRAPHICS_CAMERA_PERSPECTIVE_CAMERA_HPP
#define GRAPHICS_CAMERA_PERSPECTIVE_CAMERA_HPP

#include <graphics/camera/clip_planes.hpp>
#include <graphics/camera/camera_defaults.hpp>

namespace graphics::camera
{

    struct PerspectiveCamera
    {
        float       fov         { Defaults::fov }; // vertical field of view in degrees
        ClipPlanes  clip_planes {};
    };

} // namespace graphics::camera

#endif // GRAPHICS_CAMERA_PERSPECTIVE_CAMERA_HPP