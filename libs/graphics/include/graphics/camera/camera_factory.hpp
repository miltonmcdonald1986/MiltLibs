#ifndef GRAPHICS_CAMERA_CAMERA_FACTORY_HPP
#define GRAPHICS_CAMERA_CAMERA_FACTORY_HPP

#include <entt/entity/fwd.hpp>

#include <graphics/camera/camera.hpp>
#include <graphics/camera/camera_controller.hpp>
#include <graphics/camera/camera_defaults.hpp>
#include <graphics/camera/orthographic_camera.hpp>
#include <graphics/camera/perspective_camera.hpp>
#include <graphics/components/transform.hpp>

namespace graphics::camera
{

	struct CameraConfig
	{
		Camera					camera			{};
		CameraController		controller		{};
		components::Transform	transform		{ Defaults::transform };
		OrthographicCamera		orthographic	{};
		PerspectiveCamera		perspective		{};
	};

	void add_camera(entt::registry& reg, entt::entity ent, const CameraConfig& config);

} // namespace graphics::camera

#endif // GRAPHICS_CAMERA_CAMERA_FACTORY_HPP
