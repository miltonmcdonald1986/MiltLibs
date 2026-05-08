#include <graphics/camera/camera_factory.hpp>

#include <entt/entity/registry.hpp>

#include <graphics/camera/camera_matrices.hpp>
#include <graphics/components/transform.hpp>

#include "camera_controller_state.hpp"

namespace graphics::camera
{

	void add_camera(entt::registry& reg, entt::entity ent, const CameraConfig& config)
	{
		reg.emplace<Camera>(ent, config.camera);
		reg.emplace<CameraController>(ent, config.controller);
		reg.emplace<components::Transform>(ent, config.transform);
		reg.emplace<CameraControllerState>(ent, CameraControllerState{});
		reg.emplace<CameraMatrices>(ent, CameraMatrices{});
		switch (config.camera.type)
		{
		case ProjectionType::Orthographic:
			reg.emplace<OrthographicCamera>(ent, config.orthographic);
			break;
		case ProjectionType::Perspective:
			reg.emplace<PerspectiveCamera>(ent, config.perspective);
			break;
		}
	}

}
