#ifndef GRAPHICS_MATH_MAT4_HPP
#define GRAPHICS_MATH_MAT4_HPP

#include <span>

#include <graphics/math/vec4.hpp>

namespace graphics::math
{

	struct Mat4
	{
		float m[16];
	};

	auto create_mat4_identity() -> Mat4;
	auto create_mat4_values(std::span<const float, 16> vals) -> Mat4;

}

#endif // GRAPHICS_MATH_MAT4_HPP