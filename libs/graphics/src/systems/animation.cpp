#include <graphics/systems/animation.h>

#include <glm/gtc/matrix_transform.hpp>

#include <graphics/components/color.hpp>
#include <graphics/components/flash.hpp>
#include <graphics/components/shake.hpp>
#include <graphics/components/transform.hpp>
#include <graphics/components/world_matrix.hpp>

namespace graphics::systems
{

    void update_flash(entt::registry& reg, float dt)
    {
        auto view = reg.view<components::Flash>();
        for (auto [e, flash] : view.each())
        {
            if (flash.enabled)
                flash.t += dt * flash.speed;
            else
                flash.t = 0.F;
        }
    }

    void update_shake(entt::registry& reg, float dt)
    {
        auto view = reg.view<components::WorldMatrix, components::Shake>();
        for (auto [e, wm, shake] : view.each())
        {
            shake.time += dt * shake.speed;

            float dx = std::sin(shake.time * 1.3f) * shake.intensity;
            float dy = std::sin(shake.time * 2.1f) * shake.intensity;
            float dz = std::sin(shake.time * 3.7f) * shake.intensity;

            glm::vec3 offset(dx, dy, dz);
            wm.value = shake.base_world * glm::translate(glm::mat4(1.F), offset);
        }
    }

    void update_shake_base_world(entt::registry& reg, entt::entity e, const glm::mat4& world_matrix)
    {
        if (components::Shake* shake = reg.try_get<components::Shake>(e))
            shake->base_world = world_matrix;
        else if (components::ShakeOnce* shake_once = reg.try_get<components::ShakeOnce>(e))
            shake_once->base_world = world_matrix;
    }

    void update_shake_once(entt::registry& reg, float dt)
    {
        auto view = reg.view<components::WorldMatrix, components::ShakeOnce>();
        for (auto [e, wm, shake] : view.each())
        {
            // Countdown
            shake.time_left -= dt;
            if (shake.time_left <= 0.F)
            {
                // Restore original position
                reg.remove<components::ShakeOnce>(e);
                continue;
            }

            // Compute shake intensity falloff (optional)
            float alpha = shake.time_left / shake.duration;
            float current_intensity = shake.intensity * alpha;

            float dx = std::sin(shake.time_left * shake.speed * 1.3f) * current_intensity;
            float dy = std::sin(shake.time_left * shake.speed * 2.1f) * current_intensity;
            float dz = std::sin(shake.time_left * shake.speed * 3.7f) * current_intensity;

            glm::vec3 offset(dx, dy, dz);
            wm.value = shake.base_world * glm::translate(glm::mat4(1.F), offset);
        }
    }


}
