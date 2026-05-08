#ifndef GRAPHICS_MATH_CONVERT_MAT4_HPP
#define GRAPHICS_MATH_CONVERT_MAT4_HPP

#include <glm/ext/matrix_float4x4.hpp>

#include <graphics/math/mat4.hpp>

namespace graphics::math
{

	auto to_glm(const math::Mat4& mat) -> glm::mat4;
	auto from_glm(const glm::mat4& mat) -> math::Mat4;

}

#endif // GRAPHICS_MATH_CONVERT_MAT4_HPP