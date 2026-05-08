#ifndef GRAPHICS_PLATFORM_GLFW_CALLBACKS_H
#define GRAPHICS_PLATFORM_GLFW_CALLBACKS_H

#include <graphics/platform/gl_includes.h>

namespace graphics::platform
{

	void glfw_char_callback(GLFWwindow* window, unsigned int codepoint);
	void glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
	void glfw_error_callback(int error_code, const char* description);
	void glfw_framebuffer_size_callback(GLFWwindow* pWindow, int w, int h);
	void glfw_key_callback(GLFWwindow* win, int key, int scancode, int action, int mods);
	void glfw_mouse_button_callback(GLFWwindow* win, int button, int action, int mods);
	void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

}

#endif // GRAPHICS_PLATFORM_GLFW_CALLBACKS_H
