#ifndef GRAPHICS_COMPONENTS_PARENT_HPP
#define GRAPHICS_COMPONENTS_PARENT_HPP

#include <entt/entity/entity.hpp>
#include <entt/entity/fwd.hpp>

namespace graphics::components
{

	struct Parent
	{
		entt::entity parent{ entt::null };
	};

} // namespace graphics::components

#endif // GRAPHICS_COMPONENTS_PARENT_HPP