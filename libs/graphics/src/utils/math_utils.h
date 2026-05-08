#ifndef GRAPHICS_UTILS_MATH_UTILS_H
#define GRAPHICS_UTILS_MATH_UTILS_H

#include <glm/glm.hpp>

namespace graphics::utils
{

	glm::mat4 remove_scale_from_matrix(const glm::mat4& m);

}

#endif // GRAPHICS_UTILS_MATH_UTILS_H
