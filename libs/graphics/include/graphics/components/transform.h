#ifndef GRAPHICS_COMPONENTS_TRANSFORM_H
#define GRAPHICS_COMPONENTS_TRANSFORM_H

#include <glm/glm.hpp>

namespace graphics::components
{

    struct Transform
    {
        Transform() noexcept;
        Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl) noexcept;

        auto get_position() const -> glm::vec3;
        auto set_position(const glm::vec3& pos) -> void;

        auto get_rotation() const -> glm::vec3;
        auto set_rotation(const glm::vec3& rot) -> void;

        auto get_scale() const -> glm::vec3;
        auto set_scale(const glm::vec3& scl) -> void;

        bool dirty{ false };

    private:
        glm::vec3 position  { 0.F, 0.F, 0.F };
        glm::vec3 rotation  { 0.F, 0.F, 0.F }; // Euler angles in radians
        glm::vec3 scale     { 1.F, 1.F, 1.F };
    };

}

#endif // GRAPHICS_COMPONENTS_TRANSFORM_H
