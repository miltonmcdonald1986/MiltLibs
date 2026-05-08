#include "convert_mat4.hpp"

namespace graphics::math
{

	auto to_glm(const math::Mat4& mat) -> glm::mat4
	{
		return glm::mat4
		{
			mat.m[0], mat.m[4], mat.m[ 8], mat.m[12], 
			mat.m[1], mat.m[5], mat.m[ 9], mat.m[13], 
			mat.m[2], mat.m[6], mat.m[10], mat.m[14], 
			mat.m[3], mat.m[7], mat.m[11], mat.m[15]
		};
	}

	auto from_glm(const glm::mat4& mat) -> math::Mat4
	{
		return math::Mat4
		{
			mat[0][0], mat[1][0], mat[2][0], mat[3][0],
			mat[0][1], mat[1][1], mat[2][1], mat[3][1],
			mat[0][2], mat[1][2], mat[2][2], mat[3][2],
			mat[0][3], mat[1][3], mat[2][3], mat[3][3]			
		};
	}

}