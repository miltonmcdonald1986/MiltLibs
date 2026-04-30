#ifndef GRAPHICS_APP_INPUT_STATE_H
#define GRAPHICS_APP_INPUT_STATE_H

#include <array>

#include <graphics/input/key.h>
#include <graphics/platform/gl_includes.h>

namespace graphics::app::input_state
{

	struct MouseState
	{
		double x = 0.;
		double y = 0.;
		double dx = 0.;
		double dy = 0.;
		bool first_time = true;
		bool left_is_down = false;
		bool right_is_down = false;
		bool middle_is_down = false;
	};

	struct ScrollState {
		float x = 0.0f;
		float y = 0.0f;
	};

	struct KeyState
	{
		std::array<bool, static_cast<size_t>(input::Key::COUNT)> down{};

		bool is_down(input::Key key) const
		{
			return down[static_cast<size_t>(key)];
		}

		void set(input::Key key, bool value)
		{
			down[static_cast<size_t>(key)] = value;
		}

		bool shift_is_down() const
		{
			return is_down(input::Key::LeftShift) || is_down(input::Key::RightShift);
		}

		bool ctrl_is_down() const
		{
			return is_down(input::Key::LeftControl) || is_down(input::Key::RightControl);
		}

		bool alt_is_down() const
		{
			return is_down(input::Key::LeftAlt) || is_down(input::Key::RightAlt);
		}
	};

	struct InputState 
	{
		MouseState mouse;
		ScrollState scroll;
		KeyState keys;

		// Called once per frame
		void reset_frame_accumulators() 
		{
			scroll.x = 0.0f;
			scroll.y = 0.0f;
			mouse.dx = 0.0;
			mouse.dy = 0.0;
		}
	};

}

#endif // GRAPHICS_APP_INPUT_STATE_H
