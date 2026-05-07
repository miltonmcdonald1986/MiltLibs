#ifndef GRAPHICS_CAMERA_CAMERA_CONTROLLER_HPP
#define GRAPHICS_CAMERA_CAMERA_CONTROLLER_HPP

#include <graphics/camera/camera_defaults.hpp>
#include <graphics/input/key.h>

namespace graphics::camera
{

    struct CameraController 
    {
        // Move parameters
        bool        enable_movement         { Defaults::enable_movement };
        float       move_speed              { Defaults::move_speed };
        input::Key  enable_movement_toggle  { Defaults::enable_movement_toggle };
        input::Key  forward                 { Defaults::move_forward };
        input::Key  backward                { Defaults::move_backward };
        input::Key  right                   { Defaults::move_right };
        input::Key  left                    { Defaults::move_left };
        input::Key  up                      { Defaults::move_up };
        input::Key  down                    { Defaults::move_down };

        // Look parameters
        bool        enable_look         { Defaults::enable_look };
        input::Key  look_activation     { Defaults::look_activation };
        float       look_sensitivity    { Defaults::look_sensitivity };
        float       pitch_limit         { Defaults::pitch_limit };

        // Zoom parameters
        bool        enable_zoom         { Defaults::enable_zoom };
        float       zoom_speed          { Defaults::zoom_speed };
        input::Key  enable_zoom_toggle  { Defaults::enable_zoom_toggle };
    };

} // namespace graphics::camera

#endif // GRAPHICS_CAMERA_CAMERA_CONTROLLER_HPP
