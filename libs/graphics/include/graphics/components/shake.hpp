#ifndef GRAPHICS_COMPONENTS_SHAKE_HPP
#define GRAPHICS_COMPONENTS_SHAKE_HPP

#include <graphics/components/component_defaults.hpp>
#include <graphics/math/mat4.hpp>

namespace graphics::components
{

    struct Shake
    {
        float       time        { 0.F };
        float       intensity   { ShakeDefaults::intensity };
        float       speed       { ShakeDefaults::speed };		
        math::Mat4  base_world  { 1.F };
    };

    struct ShakeOnce
    {
        float       duration    { ShakeOnceDefaults::duration };
        float       time_left   { ShakeOnceDefaults::duration };
        float       intensity   { ShakeDefaults::intensity };
        float       speed       { ShakeDefaults::speed };
		math::Mat4  base_world  { 1.F };
    };

} // namespace graphics::components

#endif // GRAPHICS_COMPONENTS_SHAKE_HPP