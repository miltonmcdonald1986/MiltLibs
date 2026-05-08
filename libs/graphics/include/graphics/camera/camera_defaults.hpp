#ifndef GRAPHICS_CAMERA_CAMERA_DEFAULTS_HPP
#define GRAPHICS_CAMERA_CAMERA_DEFAULTS_HPP

#include <graphics/camera/projection_type.hpp>
#include <graphics/components/transform.hpp>
#include <graphics/input/key.hpp>
#include <graphics/math/mat4.hpp>
#include <graphics/math/vec3.hpp>

namespace graphics::camera
{

	struct Defaults
	{
		/*--------*/
		/* Camera */
		/*--------*/
		static constexpr ProjectionType type { ProjectionType::Perspective };

		/*-------------------*/
		/* Camera Controller */
		/*-------------------*/
        static constexpr bool       enable_look             { true };
        static constexpr bool       enable_movement         { true };
        static constexpr bool       enable_zoom             { true };
        static constexpr float      look_sensitivity        { 0.002F };
        static constexpr float      move_speed              { 5.F };
        static constexpr float      pitch_limit             { 89.F };
        static constexpr float      zoom_speed              { 0.1F };
        static constexpr input::Key enable_movement_toggle  { input::Key::Unknown };
        static constexpr input::Key enable_zoom_toggle      { input::Key::Unknown };
        static constexpr input::Key move_backward           { input::Key::S };
        static constexpr input::Key move_down               { input::Key::Q };
        static constexpr input::Key move_forward            { input::Key::W };
        static constexpr input::Key move_left               { input::Key::A };
        static constexpr input::Key look_activation         { input::Key::MouseRight };
        static constexpr input::Key move_right              { input::Key::D };
        static constexpr input::Key move_up                 { input::Key::E };

        /*-------------*/
        /* Clip Planes */
        /*-------------*/
        static constexpr float z_near   { 0.1F };
        static constexpr float z_far    { 1000.F };

        /*-----------*/
        /* Transform */
        /*-----------*/
        static constexpr    math::Vec3              pos         { 0.F, 0.F, 5.F };
        static constexpr    math::Vec3              rot         { 0.F, 0.F, 0.F };
        static constexpr    math::Vec3              scl         { 1.F, 1.F, 1.F };
        inline static const components::Transform   transform   { pos, rot, scl };

        /*----------*/
        /* Matrices */
        /*----------*/
        inline static const math::Mat4 view{ math::create_mat4_identity() };
        inline static const math::Mat4 proj{ math::create_mat4_identity() };

        /*-------------*/
        /* Perspective */
        /*-------------*/
        static constexpr float fov{ 60.F };

        /*--------------*/
        /* Orthographic */
        /*--------------*/
        static constexpr float height{ 10.F };
	};

} // namespace graphics::camera

#endif // GRAPHICS_CAMERA_CAMERA_DEFAULTS_HPP