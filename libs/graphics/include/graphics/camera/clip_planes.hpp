#ifndef GRAPHICS_CAMERA_CLIP_PLANES_HPP
#define GRAPHICS_CAMERA_CLIP_PLANES_HPP

#include <graphics/camera/camera_defaults.hpp>

namespace graphics::camera
{

	struct ClipPlanes
	{
		float z_near	{ Defaults::z_near };
		float z_far		{ Defaults::z_far };
	};

} // namespace graphics::camera

#endif // GRAPHICS_CAMERA_CLIP_PLANES_HPP