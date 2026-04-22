#ifndef GRAPHICS_APP_H
#define GRAPHICS_APP_H

#include <entt/entt.hpp>

#include "gl_config.h"
#include "gl_state.h"
#include "window_config.h"
#include "window_state.h"

namespace graphics::app
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

#endif // GRAPHICS_APP_H
