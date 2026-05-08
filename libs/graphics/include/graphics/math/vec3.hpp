#ifndef GRAPHICS_MATH_VEC3_HPP
#define GRAPHICS_MATH_VEC3_HPP

namespace graphics::math
{

	struct Vec3
	{
		float v[3];
	};

	auto create_vec3_fill(float val) -> Vec3;
	auto create_vec3_values(float v0, float v1, float v2) -> Vec3;

}

#endif // GRAPHICS_MATH_VEC3_HPP