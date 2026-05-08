#include <graphics/components/transform.hpp>

namespace graphics::components
{

    Transform::Transform() noexcept
        : position(math::Vec3{ 0.F, 0.F, 0.F })
        , rotation(math::Vec3{ 0.F, 0.F, 0.F })
        , scale(math::Vec3{ 1.F, 1.F, 1.F })
    {
    }

    Transform::Transform(const math::Vec3& pos, const math::Vec3& rot, const math::Vec3& scl) noexcept
        : position(pos)
        , rotation(rot)
        , scale(scl)
    {
    }

    math::Vec3 Transform::get_position() const
    {
        return position;
    }

    void Transform::set_position(const math::Vec3& pos)
    {
        position = pos;
        dirty = true;
    }

    math::Vec3 Transform::get_rotation() const
    {
        return rotation;
    }

    void Transform::set_rotation(const math::Vec3& rot)
    {
        rotation = rot;
        dirty = true;
    }

    math::Vec3 Transform::get_scale() const
    {
        return scale;
    }

    void Transform::set_scale(const math::Vec3& scl)
    {
        scale = scl;
        dirty = true;
    }

    auto Transform::get_dirty_flag() const -> bool
    {
        return dirty;
    }

    auto Transform::set_dirty_flag(bool is_dirty) -> void
    {
        dirty = is_dirty;
    }

}
