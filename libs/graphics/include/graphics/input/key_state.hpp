#ifndef GRAPHICS_INPUT_KEY_STATE_HPP
#define GRAPHICS_INPUT_KEY_STATE_HPP

#include <array>
#include <cstddef>

#include <graphics/input/key.hpp>

namespace graphics::input
{

	struct KeyState
	{
		[[nodiscard]]	auto shift()	const -> bool;
		[[nodiscard]]	auto ctrl()		const -> bool;
		[[nodiscard]]	auto alt()		const -> bool;
		[[nodiscard]]	auto super()	const -> bool;

		auto operator[](Key key)	const	-> bool;
		auto operator[](Key key)			-> bool&;

	private:
		
		enum class Mod 
		{ 
			Shift, 
			Control, 
			Alt, 
			Super 
		};

		[[nodiscard]] static constexpr	auto idx(Key key)					-> size_t;
		[[nodiscard]]					auto mod_is_down(Mod mod)	const	-> bool;
		
		static constexpr size_t key_count = static_cast<size_t>(Key::COUNT);
		std::array<bool, key_count> down{};
	};

} // namespace graphics::input

#endif // GRAPHICS_INPUT_KEY_STATE_HPP