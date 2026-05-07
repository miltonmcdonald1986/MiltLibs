#ifndef GRAPHICS_CAMERA_PROJECTION_TYPE_HPP
#define GRAPHICS_CAMERA_PROJECTION_TYPE_HPP

#include <cstdint>

namespace graphics::camera
{

	enum class ProjectionType : std::uint8_t
	{
		Orthographic,
		Perspective
	};

} // namespace graphics::camera

#endif // GRAPHICS_CAMERA_PROJECTION_TYPE_HPP