#include <graphics/systems/camera_controller_state.h>

namespace graphics::systems::camera_controller_state
{

    std::pair<float, float> CameraControllerState::compute_mouse_delta(double mouseX, double mouseY) {
        if (firstMouse) {
            lastX = mouseX;
            lastY = mouseY;
            firstMouse = false;
            return { 0.f, 0.f };
        }

        float dx = static_cast<float>(mouseX - lastX);
        float dy = static_cast<float>(lastY - mouseY); // inverted Y

        lastX = mouseX;
        lastY = mouseY;

        return { dx, dy };
    }

}
