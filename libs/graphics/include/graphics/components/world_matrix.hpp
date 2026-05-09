#ifndef GRAPHICS_COMPONENTS_WORLD_MATRIX_HPP
#define GRAPHICS_COMPONENTS_WORLD_MATRIX_HPP

#include <glm/ext/matrix_float4x4.hpp>

namespace graphics::components
{

	struct WorldMatrix
	{
		glm::mat4 value{ 1.F };
	};

} // namespace graphics::components

#endif // GRAPHICS_COMPONENTS_WORLD_MATRIX_HPP