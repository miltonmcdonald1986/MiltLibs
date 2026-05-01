#ifndef GRAPHICS_CAMERA_CAMERA_H
#define GRAPHICS_CAMERA_CAMERA_H

#include <graphics/camera/projection_type.h>

namespace graphics::camera
{

    struct Camera 
    {
        ProjectionType type{ ProjectionType::Perspective };
    };

}

#endif // GRAPHICS_COMPONENTS_CAMERA_H
