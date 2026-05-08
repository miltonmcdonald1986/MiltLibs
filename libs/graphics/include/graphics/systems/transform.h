#ifndef GRAPHICS_SYSTEMS_TRANSFORM_H
#define GRAPHICS_SYSTEMS_TRANSFORM_H

#include <entt/entt.hpp>

namespace graphics::systems
{

    void update_transform_dependents(entt::registry& reg);
    void update_transform_system(entt::registry& reg);

}

#endif // GRAPHICS_SYSTEMS_TRANSFORM_H
