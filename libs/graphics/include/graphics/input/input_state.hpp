#ifndef GRAPHICS_INPUT_INPUT_STATE_HPP
#define GRAPHICS_INPUT_INPUT_STATE_HPP

#include <graphics/input/key_state.hpp>
#include <graphics/input/mouse_state.hpp>
#include <graphics/input/scroll_state.hpp>

namespace graphics::input
{

	// NOLINTBEGIN(misc-non-private-member-variables-in-classes)

	struct InputState 
	{
		void reset_frame_accumulators();

		input::MouseState mouse;
		input::ScrollState scroll;
		input::KeyState keys;
	};

	// NOLINTEND(misc-non-private-member-variables-in-classes)

} // namespace graphics::input

#endif // GRAPHICS_INPUT_INPUT_STATE_HPP