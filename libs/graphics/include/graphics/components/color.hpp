#ifndef GRAPHICS_COMPONENTS_COLOR_HPP
#define GRAPHICS_COMPONENTS_COLOR_HPP

#include <graphics/components/component_defaults.hpp>
#include <graphics/math/vec4.hpp>

namespace graphics::components
{

    struct Color 
    {
        math::Vec4 rgba{ ColorDefaults::rgba };
    };

} // namespace graphics::components

#endif // GRAPHICS_COMPONENTS_COLOR_HPP