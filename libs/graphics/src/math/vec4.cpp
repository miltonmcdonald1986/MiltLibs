#include <graphics/math/vec4.hpp>

namespace graphics::math
{

	auto create_vec4_fill(float val) -> Vec4
	{
		return Vec4{ val, val, val, val };
	}

	auto create_vec4_values(float v0, float v1, float v2, float v3) -> Vec4
	{
		return Vec4{ v0, v1, v2, v3 };
	}

}