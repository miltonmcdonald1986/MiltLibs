#ifndef GRAPHICS_COMPONENTS_COLOR_H
#define GRAPHICS_COMPONENTS_COLOR_H

namespace graphics::components::color
{

    struct Color 
    {
        float base[4];   // original color
        float rgba[4];   // final color used for rendering

        Color(float r, float g, float b, float a = 1.0f)
            : base{ r, g, b, a }
            , rgba{ r, g, b, a }
        {}
    };

}

#endif // GRAPHICS_COMPONENTS_COLOR_H
