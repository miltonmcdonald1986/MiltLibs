#ifndef GRAPHICS_FACTORIES_MESH_FACTORY_HPP
#define GRAPHICS_FACTORIES_MESH_FACTORY_HPP

#include <graphics/mesh/mesh_gl.hpp>

namespace graphics::mesh
{

	auto create_rainbow_triangle_mesh() -> mesh::MeshGLResult;
	auto create_textured_cube_mesh() -> mesh::MeshGLResult;
	auto create_textured_quad_mesh() -> mesh::MeshGLResult;
	auto create_textured_triangle_mesh() -> mesh::MeshGLResult;
	auto create_triangle_mesh() -> mesh::MeshGLResult;

} // namespace graphics::factories

#endif // GRAPHICS_FACTORIES_MESH_FACTORY_HPP