#ifndef GRAPHICS_SYSTEMS_CAMERA_CONTROLLER_STATE_H
#define GRAPHICS_SYSTEMS_CAMERA_CONTROLLER_STATE_H

#include <tuple>

namespace graphics::systems::camera_controller_state
{

    struct CameraControllerState 
    {
        bool firstMouse = true;
        double lastX = 0.0;
        double lastY = 0.0;

        // returns dx, dy
        std::pair<float, float> compute_mouse_delta(double mouseX, double mouseY);
    };

}

#endif // GRAPHICS_SYSTEMS_CAMERA_CONTROLLER_STATE_H
