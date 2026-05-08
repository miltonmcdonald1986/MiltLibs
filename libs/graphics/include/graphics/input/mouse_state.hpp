#ifndef GRAPHICS_INPUT_MOUSE_STATE_HPP
#define GRAPHICS_INPUT_MOUSE_STATE_HPP

namespace graphics::input
{

	struct MouseState
	{
		double	x				{ 0. };
		double	y				{ 0. };
		double	dx				{ 0. };
		double	dy				{ 0. };
		bool	left_is_down	{ false };
		bool	right_is_down	{ false };
		bool	middle_is_down	{ false };
	};

} // namespace graphics::input

#endif // GRAPHICS_INPUT_MOUSE_STATE_HPP