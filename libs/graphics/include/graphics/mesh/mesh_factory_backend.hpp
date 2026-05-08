#ifndef GRAPHICS_MESH_MESH_FACTORY_BACKEND_HPP
#define GRAPHICS_MESH_MESH_FACTORY_BACKEND_HPP

#include <span>

#include <graphics/mesh/mesh_gl.hpp>
#include <graphics/mesh/vertex_layout.h>

namespace graphics::mesh
{

	using Indices = std::span<const unsigned int>;
	using Vertices = std::span<const float>;

	auto create_indexed_mesh_gl_layout(Vertices vertices, Indices indices, const VertexLayout& layout, GLenum primitive = GL_TRIANGLES) -> MeshGLResult;
    auto create_indexed_mesh_gl_pos_only(Vertices vertices, Indices indices, GLint componentsPerVertex, GLenum primitive) -> MeshGLResult;
	auto create_mesh_gl_layout(Vertices vertices, const VertexLayout& layout, GLenum primitive = GL_TRIANGLES) -> MeshGLResult;
	auto create_mesh_gl_pos_only(Vertices vertices, GLint componentsPerVertex = 3, GLenum primitive = GL_TRIANGLES) -> MeshGLResult;

} // namespace graphics::mesh

#endif // GRAPHICS_MESH_MESH_FACTORY_BACKEND_HPP