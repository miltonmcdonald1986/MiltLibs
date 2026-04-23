#ifndef GRAPHICS_APP_APP_H
#define GRAPHICS_APP_APP_H

#include <entt/entt.hpp>

// Include these at the topmost level to avoid forcing downstream code to include them.
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <graphics/gl_config.h>
#include <graphics/gl_state.h>
#include <graphics/window_config.h>
#include <graphics/window_state.h>

namespace graphics::app::app
{

	struct App
	{
		double									delta_time{ 0. };
		entt::registry							reg{};
		graphics::gl_config::GLConfig			glConfig{};
		graphics::gl_state::GLState				glState{};
		graphics::window_config::WindowConfig	winConfig{};
		graphics::window_state::WindowState		winState{};
	};

}

#endif // GRAPHICS_APP_APP_H
