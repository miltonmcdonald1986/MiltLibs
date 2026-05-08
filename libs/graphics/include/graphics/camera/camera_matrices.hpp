#ifndef GRAPHICS_CAMERA_CAMERA_MATRICES_HPP
#define GRAPHICS_CAMERA_CAMERA_MATRICES_HPP

#include <graphics/camera/camera_defaults.hpp>
#include <graphics/math/mat4.hpp>

namespace graphics::camera
{

	struct CameraMatrices
	{
		math::Mat4 view			{ Defaults::view };
		math::Mat4 projection	{ Defaults::proj };
	};

}

#endif // GRAPHICS_CAMERA_CAMERA_MATRICES_HPP
