#ifndef GRAPHICS_FACTORIES_TEXTURE_FACTORIES_H
#define GRAPHICS_FACTORIES_TEXTURE_FACTORIES_H

#include <expected>
#include <string>

#include <graphics/components/texture.h>
#include <graphics/engine/result.h>

namespace graphics::factories
{
	
    engine::Result<components::Texture> create_texture_from_file(const char* path);

}

#endif // GRAPHICS_FACTORIES_TEXTURE_FACTORIES_H
