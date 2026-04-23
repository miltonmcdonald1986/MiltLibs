#ifndef GRAPHICS_SYSTEMS_TRANSFORM_H
#define GRAPHICS_SYSTEMS_TRANSFORM_H

#include <glm/glm.hpp>

#include <graphics/components/transform.h>

namespace graphics::systems::transform
{

    glm::mat4 compute_model_matrix(const components::transform::Transform& t);

}

#endif // GRAPHICS_SYSTEMS_TRANSFORM_H
