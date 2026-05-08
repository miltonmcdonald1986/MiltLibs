#ifndef GRAPHICS_INPUT_GLFW_KEY_HPP
#define GRAPHICS_INPUT_GLFW_KEY_HPP

#include <graphics/input/key.hpp>

namespace graphics::input 
{

    auto translate_glfw_key(int key) -> Key;
    auto translate_glfw_mouse_button(int button) -> Key;

} // namespace graphics::input

#endif // GRAPHICS_INPUT_GLFW_KEY_HPP