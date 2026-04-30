#ifndef GRAPHICS_PLATFORM_GL_CONFIG_H
#define GRAPHICS_PLATFORM_GL_CONFIG_H

#include <graphics/platform/gl_includes.h>

namespace graphics::platform
{

    struct GLConfig
    {
        int profile{ GLFW_OPENGL_CORE_PROFILE };
        int version_major{ 3 };
        int version_minor{ 3 };
    };

}

#endif // GRAPHICS_PLATFORM_GL_CONFIG_H
