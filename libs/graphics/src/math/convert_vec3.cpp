#include "convert_vec3.hpp"

namespace graphics::math
{

	auto to_glm(const math::Vec3& vec) -> glm::vec3
	{
		return glm::vec3{ vec.v[0], vec.v[1], vec.v[2] };
	}

	auto from_glm(const glm::vec3& vec) -> math::Vec3
	{
		return math::Vec3{ vec[0], vec[1], vec[2] };
	}

}