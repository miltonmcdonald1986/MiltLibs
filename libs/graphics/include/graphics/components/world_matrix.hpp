#ifndef GRAPHICS_COMPONENTS_WORLD_MATRIX_HPP
#define GRAPHICS_COMPONENTS_WORLD_MATRIX_HPP

#include <graphics/math/mat4.hpp>

namespace graphics::components
{

	struct WorldMatrix
	{
		math::Mat4 value{ math::create_mat4_identity() };
	};

} // namespace graphics::components

#endif // GRAPHICS_COMPONENTS_WORLD_MATRIX_HPP