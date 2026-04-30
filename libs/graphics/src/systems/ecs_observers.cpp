#include <graphics/systems/ecs_observers.h>

#include <entt/entt.hpp>

#include <graphics/components/world_matrix.h>
#include <graphics/engine/app_data.h>
#include <graphics/scene/scene.h>

using graphics::components::transform::Transform;
using graphics::components::world_matrix::WorldMatrix;
using graphics::engine::AppData;
using graphics::scene::Scene;

namespace graphics::systems::ecs_observers
{

    AppData* get_app(const entt::registry& reg)
    {
        return reg.ctx().get<AppData*>();
    }

    void on_transform_constructed(entt::registry& reg, entt::entity e)
    {
        AppData* p_data = get_app(reg);
        if (!p_data)
            return;

        Scene* scene = p_data->p_active_scene;
        if (!scene)
            return;

        // 1. Record initial transform (your existing behavior)
        const Transform& t = reg.get<Transform>(e);
        scene->initial_transforms[e] = t;

        // 2. Auto-add WorldMatrix if missing
        if (!reg.any_of<WorldMatrix>(e))
            reg.emplace<WorldMatrix>(e);

        // 4. Mark dirty so the TransformSystem recomputes world matrix
        reg.get<Transform>(e).dirty = true;
    }


    void on_transform_updated(entt::registry& reg, entt::entity e) 
    {
        AppData* p_data = get_app(reg);
        if (!p_data)
            return;

        Scene* scene = p_data->p_active_scene;
        if (!scene)
            return;

        auto& initial_transforms = scene->initial_transforms;

        if (!initial_transforms.contains(e)) {
            const Transform& t = reg.get<Transform>(e);
            initial_transforms[e] = t;
        }
    }

    void on_transform_destroyed(entt::registry& reg, entt::entity e) 
    {
        AppData* p_data = get_app(reg);

        Scene* scene = p_data->p_active_scene;
        if (!scene)
            return;

        scene->initial_transforms.erase(e);
    }

    void register_transform_observers(AppData* p_data) 
    {
        // TODO: apply this to all scenes once we have a mechanism for more than one scene???

        if (!p_data)
            return;

        Scene* scene = p_data->p_active_scene;
        if (!scene)
            return;

        scene->reg.ctx().emplace<AppData*>(p_data);
        
        scene->reg.on_construct<Transform>().connect<&on_transform_constructed>();
        scene->reg.on_update<Transform>().connect<&on_transform_updated>();
        scene->reg.on_destroy<Transform>().connect<&on_transform_destroyed>();
    }

}
