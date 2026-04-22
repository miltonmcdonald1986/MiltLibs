#ifndef GRAPHICS_COMPONENTS_FLASH_H
#define GRAPHICS_COMPONENTS_FLASH_H

namespace graphics::components::flash
{

    struct Flash 
    {
        double speed = 2.;   // how fast it flashes
        double t = 0.;       // internal time accumulator
    };

}

#endif // GRAPHICS_COMPONENTS_FLASH_H
