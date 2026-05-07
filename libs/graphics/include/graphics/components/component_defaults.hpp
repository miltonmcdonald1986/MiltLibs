#ifndef GRAPHICS_COMPONENTS_COMPONENT_DEFAULTS_HPP
#define GRAPHICS_COMPONENTS_COMPONENT_DEFAULTS_HPP

#include <entt/entity/entity.hpp>
#include <entt/entity/fwd.hpp>

#include <glm/ext/vector_float4.hpp>

#include <graphics/platform/gl_includes.h>	// IWYU pragma: keep

namespace graphics::components
{

	struct ColorDefaults
	{
		static constexpr glm::vec4 rgba{ 1.F };
	};

	struct FlashDefaults
	{
		static constexpr float speed{ 2.F };
	};

	struct MeshGLDefaults
	{
		static constexpr GLenum primitive{ GL_TRIANGLES };	// NOLINT(misc-include-cleaner)
	};

} // namespace graphics::components

#endif // GRAPHICS_COMPONENTS_COMPONENT_DEFAULTS_HPP