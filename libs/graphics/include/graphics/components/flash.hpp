#ifndef GRAPHICS_COMPONENTS_FLASH_HPP
#define GRAPHICS_COMPONENTS_FLASH_HPP

#include <graphics/components/component_defaults.hpp>

namespace graphics::components
{

    struct Flash 
    {
        bool  enabled   { FlashDefaults::enabled };
        float speed     { FlashDefaults::speed };
        float t         { 0.F }; // internal time accumulator
    };

} // namespace graphics::components

#endif // GRAPHICS_COMPONENTS_FLASH_HPP