#ifndef GRAPHICS_PLATFORM_GLFW_CALLBACKS_H
#define GRAPHICS_PLATFORM_GLFW_CALLBACKS_H

#include <GLFW/glfw3.h>

namespace graphics::platform::glfw_callbacks
{

	/// <summary>
	/// Callback invoked by GLFW to report an error.
	/// </summary>
	/// <param name="error_code">Numeric code identifying the error.</param>
	/// <param name="description">Null-terminated string containing a human-readable description of the error.</param>
	void glfw_error_callback(int error_code, const char* description);

	/// <summary>
	/// Callback invoked when a GLFW window's framebuffer is resized; typically used to update the OpenGL viewport or other rendering state.
	/// </summary>
	/// <param name="pWindow">Pointer to the GLFWwindow whose framebuffer size changed.</param>
	/// <param name="w">New framebuffer width in pixels.</param>
	/// <param name="h">New framebuffer height in pixels.</param>
	void glfw_framebuffer_size_callback(GLFWwindow* pWindow, int w, int h);

}

#endif // GRAPHICS_PLATFORM_GLFW_CALLBACKS_H
