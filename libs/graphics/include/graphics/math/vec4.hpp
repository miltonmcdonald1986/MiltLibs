#ifndef GRAPHICS_MATH_VEC4_HPP
#define GRAPHICS_MATH_VEC4_HPP

namespace graphics::math
{

	struct Vec4
	{
		float v[4];
	};

	auto create_vec4_fill(float val) -> Vec4;
	auto create_vec4_values(float v0, float v1, float v2, float v3) -> Vec4;

}

#endif // GRAPHICS_MATH_VEC4_HPP