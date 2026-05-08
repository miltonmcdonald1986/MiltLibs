#ifndef GRAPHICS_COMPONENTS_TEXTURE_HPP
#define GRAPHICS_COMPONENTS_TEXTURE_HPP

#include <graphics/platform/gl_includes.h>	// IWYU pragma: keep

namespace graphics::components
{

    struct Texture 
    {
        GLuint id{ 0 };	// NOLINT(misc-include-cleaner)
        int width{ 0 };
        int height{ 0 };
    };
	
} // namespace graphics::components

#endif // GRAPHICS_COMPONENTS_TEXTURE_HPP