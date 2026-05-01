#ifndef GRAPHICS_CAMERA_CAMERA_MATRICES_H
#define GRAPHICS_CAMERA_CAMERA_MATRICES_H

#include <glm/glm.hpp>

namespace graphics::camera
{

	struct CameraMatrices
	{
		glm::mat4 view{ glm::mat4(1.f) };
		glm::mat4 projection{ glm::mat4(1.f) };
	};

}

#endif // GRAPHICS_CAMERA_CAMERA_MATRICES_H
