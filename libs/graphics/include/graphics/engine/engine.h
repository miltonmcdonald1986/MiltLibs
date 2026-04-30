#ifndef GRAPHICS_ENGINE_ENGINE_H
#define GRAPHICS_ENGINE_ENGINE_H

#include <expected>
#include <functional>
#include <string>

namespace graphics::engine { struct AppData; }

namespace graphics::engine
{

	using GraphicsInitFn = std::function<std::expected<void, std::string>(engine::AppData*)>;
	using GraphicsUpdateFn = std::function<std::expected<void, std::string>(engine::AppData*)>;

	std::expected<void, std::string> run(GraphicsInitFn init_fn, GraphicsUpdateFn update_fn);

}

#endif // GRAPHICS_ENGINE_ENGINE_H
