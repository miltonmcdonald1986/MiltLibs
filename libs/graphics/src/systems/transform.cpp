#include <graphics/systems/transform.h>

#include <glm/gtc/matrix_transform.hpp>

using graphics::components::transform::Transform;

namespace graphics::systems::transform
{
	
    glm::mat4 compute_model_matrix(const Transform& t)
    {
        glm::mat4 m = glm::mat4(1.0f);

        m = glm::translate(m, t.position);

        m = glm::rotate(m, t.rotation.x, glm::vec3(1, 0, 0));
        m = glm::rotate(m, t.rotation.y, glm::vec3(0, 1, 0));
        m = glm::rotate(m, t.rotation.z, glm::vec3(0, 0, 1));

        m = glm::scale(m, t.scale);

        return m;
    }

}