#include <graphics/math/vec3.hpp>

namespace graphics::math
{

	auto create_vec3_fill(float val) -> Vec3
	{
		return Vec3{ val, val, val };
	}

	auto create_vec3_values(float v0, float v1, float v2) -> Vec3
	{
		return Vec3{ v0, v1, v2 };
	}

}