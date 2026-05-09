#ifndef GRAPHICS_COMPONENTS_COMPONENT_DEFAULTS_HPP
#define GRAPHICS_COMPONENTS_COMPONENT_DEFAULTS_HPP

#include <glm/ext/vector_float4.hpp>

#include <graphics/platform/gl_includes.h>	// IWYU pragma: keep

namespace graphics::components
{

	struct ColorDefaults
	{
		static constexpr glm::vec4 rgba{ 1.F, 1.F, 1.F, 1.F };
	};

	struct FlashDefaults
	{
		static constexpr bool	enabled	{ false };
		static constexpr float	speed	{ 2.F };
	};

	struct MeshGLDefaults
	{
		static constexpr GLenum primitive{ GL_TRIANGLES };	// NOLINT(misc-include-cleaner)
	};

	struct ShakeDefaults
	{
		static constexpr float intensity{ 0.02F };
		static constexpr float speed{ 60.F };
	};

	struct ShakeOnceDefaults
	{
		static constexpr ShakeDefaults shake_defaults{};
		static constexpr float duration{ 0.15F };
	};

} // namespace graphics::components

#endif // GRAPHICS_COMPONENTS_COMPONENT_DEFAULTS_HPP