#ifndef GRAPHICS_COMPONENTS_FLASH_H
#define GRAPHICS_COMPONENTS_FLASH_H

namespace graphics::components
{

    struct Flash 
    {
        float speed = 2.F;   // how fast it flashes
        float t = 0.F;       // internal time accumulator
    };

}

#endif // GRAPHICS_COMPONENTS_FLASH_H
