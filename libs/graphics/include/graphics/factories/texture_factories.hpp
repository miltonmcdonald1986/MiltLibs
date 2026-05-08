#ifndef GRAPHICS_FACTORIES_TEXTURE_FACTORIES_HPP
#define GRAPHICS_FACTORIES_TEXTURE_FACTORIES_HPP

#include <graphics/components/texture.hpp>
#include <graphics/engine/result.hpp>

namespace graphics::factories
{
	
	using TextureResult = engine::Result<components::Texture>;

	auto create_texture_from_file(const char* path) -> TextureResult;

} // namespace graphics::factories

#endif // GRAPHICS_FACTORIES_TEXTURE_FACTORIES_HPP