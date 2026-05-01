#ifndef GRAPHICS_ENGINE_ENGINE_H
#define GRAPHICS_ENGINE_ENGINE_H

#include <expected>
#include <functional>
#include <string>

namespace graphics::engine { struct AppData; }

namespace graphics::engine
{

	using InitFn = std::function<std::expected<void, std::string>(engine::AppData*)>;
	using UpdateFn = std::function<std::expected<void, std::string>(engine::AppData*)>;

	std::expected<void, std::string> run(InitFn init_fn, UpdateFn update_fn);

}

#endif // GRAPHICS_ENGINE_ENGINE_H
