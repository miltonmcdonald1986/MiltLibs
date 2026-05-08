#ifndef GRAPHICS_ENGINE_ENGINE_HPP
#define GRAPHICS_ENGINE_ENGINE_HPP

#include <functional>

#include <graphics/engine/result.hpp>

namespace graphics::engine { struct AppData; }

namespace graphics::engine
{

	using InitFn = std::function<bool(engine::AppData*)>;
	using UpdateFn = std::function<bool(engine::AppData*)>;

	auto run(InitFn init_fn, UpdateFn update_fn) -> Status;

} // namespace graphics::engine

#endif // GRAPHICS_ENGINE_ENGINE_HPP