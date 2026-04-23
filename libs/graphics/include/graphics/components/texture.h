#ifndef GRAPHICS_COMPONENTS_TEXTURE_H
#define GRAPHICS_COMPONENTS_TEXTURE_H

#include <GL/gl3w.h>

namespace graphics::components::texture
{

    struct Texture {
        GLuint id = 0;
        int width = 0;
        int height = 0;
    };
	
}

#endif // GRAPHICS_COMPONENTS_TEXTURE_H
