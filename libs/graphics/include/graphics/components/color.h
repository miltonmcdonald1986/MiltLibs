#ifndef GRAPHICS_COMPONENTS_COLOR_H
#define GRAPHICS_COMPONENTS_COLOR_H

#include <glm/glm.hpp>

namespace graphics::components
{

    struct Color 
    {
        glm::vec4 rgba{ glm::vec4(1.f) };
    };

}

#endif // GRAPHICS_COMPONENTS_COLOR_H
