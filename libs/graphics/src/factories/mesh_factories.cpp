#include <graphics/factories/mesh_factories.h>

#include <format>

#include <graphics/mesh/vertex_layout.h>
#include <graphics/mesh/mesh_factory_backend.h>

using graphics::components::mesh_gl::MeshGL;
using graphics::mesh::vertex_layout::VertexLayout;
using graphics::mesh::mesh_factory_backend::create_indexed_mesh_gl_layout;

namespace graphics::factories::mesh_factories
{

	std::expected<MeshGL, std::string> create_rainbow_triangle_mesh()
	{
        VertexLayout PosColorLayout{
            .stride = 6 * sizeof(float), // 3 pos + 3 color
            .attributes = {
                { 0, 3, 0 },                        // position at offset 0
                { 1, 3, 3 * sizeof(float) }         // color after 3 floats
            }
        };

        float vertices[] = {
            // pos              // color
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // red
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // green
             0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // blue
        };

        unsigned int indices[] = {
            0, 1, 2
        };

        auto meshResult = create_indexed_mesh_gl_layout(vertices, indices, PosColorLayout);
        if (!meshResult)
            return std::unexpected(std::format("Failed to create mesh: {}\n", meshResult.error()));

		return *meshResult;
	}

} // namespace graphics::factories::mesh_factories
