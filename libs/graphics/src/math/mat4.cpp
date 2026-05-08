#include <graphics/math/mat4.hpp>

namespace graphics::math
{

	auto create_mat4_identity() -> Mat4
	{
		return Mat4
		{
			1.F, 0.F, 0.F, 0.F,
			0.F, 1.F, 0.F, 0.F,
			0.F, 0.F, 1.F, 0.F,
			0.F, 0.F, 0.F, 1.F
		};
	}

	auto create_mat4_values(std::span<const float, 16> vals) -> Mat4
	{
		return Mat4
		{
			vals[ 0], vals[ 1], vals[ 2], vals[ 3],
			vals[ 4], vals[ 5], vals[ 6], vals[ 7],
			vals[ 8], vals[ 9], vals[10], vals[11],
			vals[12], vals[13], vals[14], vals[15]
		};
	}

}