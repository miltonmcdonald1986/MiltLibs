#ifndef GRAPHICS_MATH_CONVERT_VEC4_HPP
#define GRAPHICS_MATH_CONVERT_VEC4_HPP

#include <glm/ext/vector_float4.hpp>

#include <graphics/math/vec4.hpp>

namespace graphics::math
{

	auto to_glm(const math::Vec4& vec) -> glm::vec4;
	auto from_glm(const glm::vec4& vec) -> math::Vec4;

}

#endif // GRAPHICS_MATH_CONVERT_VEC4_HPP