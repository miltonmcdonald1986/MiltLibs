#include <graphics/factories/prefab_factories.hpp>

#include <entt/entity/registry.hpp>

#include <graphics/components/shader.hpp>
#include <graphics/mesh/mesh_factory.hpp>
#include <graphics/factories/shader_factories.hpp>

namespace graphics::factories
{

    auto create_solid_color_triangle_ent(entt::registry& reg, const components::Color& color) -> EntityResult
    {
        auto mesh_result = mesh::create_triangle_mesh();
		if (!mesh_result)
            return std::unexpected(mesh_result.error());

        auto color_shader_result = create_color_shader();
        if (!color_shader_result) 
            return std::unexpected(color_shader_result.error());
        
        GLuint color_shader = *color_shader_result;
        entt::entity ent_triangle = reg.create();
        reg.emplace<components::Color>(ent_triangle, color);
        reg.emplace<components::Shader>(ent_triangle, color_shader);
		reg.emplace<mesh::MeshGL>(ent_triangle, *mesh_result);

		return ent_triangle;
    }

	auto create_rainbow_triangle_ent(entt::registry& reg) -> EntityResult
	{
        auto mesh_result = mesh::create_rainbow_triangle_mesh();
        if (!mesh_result)
            return std::unexpected(mesh_result.error());

        auto vertex_color_shader_result = create_vertex_color_shader();
        if (!vertex_color_shader_result) 
            return std::unexpected(vertex_color_shader_result.error());
        
        GLuint vertex_color_shader = *vertex_color_shader_result;

        entt::entity ent_rainbow_triangle = reg.create();
        reg.emplace<components::Shader>(ent_rainbow_triangle, vertex_color_shader);
        reg.emplace<mesh::MeshGL>(ent_rainbow_triangle, *mesh_result);

		return ent_rainbow_triangle;
	}

}
