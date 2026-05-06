#ifndef GRAPHICS_CAMERA_CAMERA_CONTROLLER_HPP
#define GRAPHICS_CAMERA_CAMERA_CONTROLLER_HPP

#include <graphics/input/key.h>

namespace graphics::camera
{

    namespace defaults
    {
        inline constexpr bool enable_look { true };
        inline constexpr bool enable_movement{ true };
        inline constexpr bool enable_zoom { true };
        inline constexpr float look_sensitivity { 0.002F };
        inline constexpr float move_speed { 5.F };
        inline constexpr float pitch_limit { 89.F };
        inline constexpr float zoom_speed { 0.1F };
        inline constexpr input::Key enable_movement_toggle { input::Key::Unknown };
        inline constexpr input::Key enable_zoom_toggle { input::Key::Unknown };
        inline constexpr input::Key backward { input::Key::S };
        inline constexpr input::Key down { input::Key::Q };
        inline constexpr input::Key forward { input::Key::W };
        inline constexpr input::Key left { input::Key::A };
        inline constexpr input::Key look_activation { input::Key::MouseRight };
        inline constexpr input::Key right { input::Key::D };
        inline constexpr input::Key up { input::Key::E };
    }

    struct CameraController 
    {
        // Move parameters
        bool enable_movement { defaults::enable_movement };
        float move_speed { defaults::move_speed };
        input::Key enable_movement_toggle { defaults::enable_movement_toggle };
        input::Key forward { defaults::forward };
        input::Key backward { defaults::backward };
        input::Key right { defaults::right };
        input::Key left { defaults::left };
        input::Key up { defaults::up };
        input::Key down { defaults::down };

        // Look parameters
        bool enable_look { defaults::enable_look };
        input::Key look_activation { defaults::look_activation };
        float look_sensitivity { defaults::look_sensitivity };
        float pitch_limit { defaults::pitch_limit };

        // Zoom parameters
        bool enable_zoom { defaults::enable_zoom };
        float zoom_speed { defaults::zoom_speed };
        input::Key enable_zoom_toggle { defaults::enable_zoom_toggle };
    };

} // namespace graphics::camera

#endif // GRAPHICS_CAMERA_CAMERA_CONTROLLER_HPP
