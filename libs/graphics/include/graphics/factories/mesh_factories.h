#ifndef GRAPHICS_FACTORIES_MESH_FACTORIES_H
#define GRAPHICS_FACTORIES_MESH_FACTORIES_H

#include <expected>
#include <string>

#include <graphics/components/mesh_gl.h>

namespace graphics::factories::mesh_factories
{

	/// <summary>
	/// Creates a MeshGL representing a single triangle with interleaved
	/// position and color attributes.
	///
	/// The generated mesh contains:
	///   • 3 vertices
	///   • position (vec3) at attribute location 0
	///   • color    (vec3) at attribute location 1
	///
	/// Vertex data is tightly packed (stride = 6 floats), and the mesh
	/// is uploaded using create_indexed_mesh_gl_layout(), which allocates
	/// a VAO, VBO, and EBO and configures the provided VertexLayout.
	///
	/// On success, returns a fully initialized MeshGL ready for rendering.
	/// On failure, returns an error message describing the OpenGL upload issue.
	/// </summary>
	/// <returns>
	/// std::expected containing:
	///   • MeshGL on success
	///   • std::string error message on failure
	/// </returns>
	std::expected<components::mesh_gl::MeshGL, std::string> create_rainbow_triangle_mesh();

}

#endif // GRAPHICS_FACTORIES_MESH_FACTORIES_H
