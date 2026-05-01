#include <graphics/utils/math_utils.h>

namespace graphics::utils
{

    glm::mat4 remove_scale_from_matrix(const glm::mat4& m)
    {
        // Remove any scaling from the camera's world matrix.
        // Extract basis
        glm::vec3 x = glm::vec3(m[0]);
        glm::vec3 y = glm::vec3(m[1]);
        glm::vec3 z = glm::vec3(m[2]);

        // Gram–Schmidt orthonormalization
        x = glm::normalize(x);

        y = y - x * glm::dot(x, y);
        y = glm::normalize(y);

        z = z - x * glm::dot(x, z) - y * glm::dot(y, z);
        z = glm::normalize(z);

        // Rebuild TR matrix
        glm::mat4 m_no_scale(1.f);
        m_no_scale[0] = glm::vec4(x, 0.f);
        m_no_scale[1] = glm::vec4(y, 0.f);
        m_no_scale[2] = glm::vec4(z, 0.f);
        m_no_scale[3] = m[3]; // translation

        return m_no_scale;
    }

}
