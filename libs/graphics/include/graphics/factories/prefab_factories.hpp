#ifndef GRAPHICS_FACTORIES_PREFAB_FACTORIES_HPP
#define GRAPHICS_FACTORIES_PREFAB_FACTORIES_HPP

#include <entt/entity/fwd.hpp>

#include <graphics/components/color.hpp>
#include <graphics/engine/result.hpp>

namespace graphics::factories
{

	using EntityResult = engine::Result<entt::entity>;

	auto create_solid_color_triangle_ent(entt::registry& reg, const components::Color& color) -> EntityResult;
	auto create_rainbow_triangle_ent(entt::registry& reg) -> EntityResult;

} // namespace graphics::factories

#endif // GRAPHICS_FACTORIES_PREFAB_FACTORIES_HPP