#ifndef GRAPHICS_COMPONENTS_TRANSFORM_HPP
#define GRAPHICS_COMPONENTS_TRANSFORM_HPP

#include <graphics/math/vec3.hpp>

namespace graphics::components
{

    struct Transform
    {
        Transform() noexcept;
        Transform(const math::Vec3& pos, const math::Vec3& rot, const math::Vec3& scl) noexcept;

        /*---------*/
        /* Getters */
        /*---------*/
        [[nodiscard]] auto get_dirty_flag() const -> bool;
        [[nodiscard]] auto get_position() const -> math::Vec3;
        [[nodiscard]] auto get_rotation() const -> math::Vec3;
        [[nodiscard]] auto get_scale() const -> math::Vec3;
        
        /*---------*/
        /* Setters */
        /*---------*/
        auto set_dirty_flag(bool is_dirty) -> void;
        auto set_position(const math::Vec3& pos) -> void;
        auto set_rotation(const math::Vec3& rot) -> void;
        auto set_scale(const math::Vec3& scl) -> void;

    private:
        bool        dirty       { false };
        math::Vec3   position    { 0.F, 0.F, 0.F };
        math::Vec3   rotation    { 0.F, 0.F, 0.F }; // Euler angles in radians
        math::Vec3   scale       { 1.F, 1.F, 1.F };
    };

} // namespace graphics::components

#endif // GRAPHICS_COMPONENTS_TRANSFORM_HPP