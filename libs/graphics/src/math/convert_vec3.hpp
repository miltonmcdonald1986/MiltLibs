#ifndef GRAPHICS_MATH_CONVERT_VEC3_HPP
#define GRAPHICS_MATH_CONVERT_VEC3_HPP

#include <glm/ext/vector_float3.hpp>

#include <graphics/math/vec3.hpp>

namespace graphics::math
{

	auto to_glm(const math::Vec3& vec) -> glm::vec3;
	auto from_glm(const glm::vec3& vec) -> math::Vec3;

}

#endif // GRAPHICS_MATH_CONVERT_VEC3_HPP