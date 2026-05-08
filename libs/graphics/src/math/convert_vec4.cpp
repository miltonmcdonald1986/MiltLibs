#include "convert_vec4.hpp"

namespace graphics::math
{

	auto to_glm(const math::Vec4& vec) -> glm::vec4
	{
		return glm::vec4{ vec.v[0], vec.v[1], vec.v[2], vec.v[3] };
	}

	auto from_glm(const glm::vec4& vec) -> math::Vec4
	{
		return math::Vec4{ vec[0], vec[1], vec[2], vec[3] };
	}

}