#ifndef GRAPHICS_COMPONENTS_TRANSFORM_HPP
#define GRAPHICS_COMPONENTS_TRANSFORM_HPP

#include <glm/ext/vector_float3.hpp>

namespace graphics::components
{

    struct Transform
    {
        Transform() noexcept;
        Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl) noexcept;

        /*---------*/
        /* Getters */
        /*---------*/
        [[nodiscard]] auto get_dirty_flag() const -> bool;
        [[nodiscard]] auto get_position() const -> glm::vec3;
        [[nodiscard]] auto get_rotation() const -> glm::vec3;
        [[nodiscard]] auto get_scale() const -> glm::vec3;
        
        /*---------*/
        /* Setters */
        /*---------*/
        auto set_dirty_flag(bool is_dirty) -> void;
        auto set_position(const glm::vec3& pos) -> void;
        auto set_rotation(const glm::vec3& rot) -> void;
        auto set_scale(const glm::vec3& scl) -> void;

    private:
        bool        dirty       { false };
        glm::vec3   position    { 0.F, 0.F, 0.F };
        glm::vec3   rotation    { 0.F, 0.F, 0.F }; // Euler angles in radians
        glm::vec3   scale       { 1.F, 1.F, 1.F };
    };

} // namespace graphics::components

#endif // GRAPHICS_COMPONENTS_TRANSFORM_HPP