#ifndef GRAPHICS_COMPONENTS_COLOR_HPP
#define GRAPHICS_COMPONENTS_COLOR_HPP

#include <glm/ext/vector_float4.hpp>

#include <graphics/components/component_defaults.hpp>

namespace graphics::components
{

    struct Color 
    {
        glm::vec4 rgba{ ColorDefaults::rgba };
    };

} // namespace graphics::components

#endif // GRAPHICS_COMPONENTS_COLOR_HPP