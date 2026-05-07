#ifndef GRAPHICS_COMPONENTS_SHADER_HPP
#define GRAPHICS_COMPONENTS_SHADER_HPP

#include <graphics/platform/gl_includes.h> // IWYU pragma: keep

namespace graphics::components
{

	struct Shader
	{
		GLuint id{ 0 }; // NOLINT(misc-include-cleaner)
	};

} // namespace graphics::components

#endif // GRAPHICS_COMPONENTS_SHADER_HPP