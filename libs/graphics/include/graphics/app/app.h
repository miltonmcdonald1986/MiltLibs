#ifndef GRAPHICS_APP_APP_H
#define GRAPHICS_APP_APP_H

#define NOMINMAX

#include <entt/entt.hpp>

// Include these at the topmost level to avoid forcing downstream code to include them.
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <graphics/app/input_state.h>
#include <graphics/app/gl_config.h>
#include <graphics/app/window_config.h>
#include <graphics/components/transform.h>
#include <graphics/platform/window_state.h>
#include <graphics/systems/camera_controller_state.h>
#include <graphics/systems/gl_state.h>

namespace graphics::app::app
{

	struct App
	{
		double delta_time{ 0. };
		double last_time{ 0. };
		entt::registry reg{};
		input_state::InputState input{};
		gl_config::GLConfig glConfig{};
		systems::gl_state::GLState glState{};
		systems::camera_controller_state::CameraControllerState cam_state{};
		window_config::WindowConfig winConfig{};
		platform::window_state::WindowState winState{};
		std::unordered_map<entt::entity, graphics::components::transform::Transform> initialTransforms{};
	};

}

#endif // GRAPHICS_APP_APP_H
