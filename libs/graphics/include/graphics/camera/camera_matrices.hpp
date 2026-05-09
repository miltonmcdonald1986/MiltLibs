#ifndef GRAPHICS_CAMERA_CAMERA_MATRICES_HPP
#define GRAPHICS_CAMERA_CAMERA_MATRICES_HPP

#include <glm/ext/matrix_float4x4.hpp>

#include <graphics/camera/camera_defaults.hpp>

namespace graphics::camera
{

	struct CameraMatrices
	{
		glm::mat4 view			{ Defaults::view };
		glm::mat4 projection	{ Defaults::proj };
	};

}

#endif // GRAPHICS_CAMERA_CAMERA_MATRICES_HPP