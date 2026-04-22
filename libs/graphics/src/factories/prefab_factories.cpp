#include <graphics/factories/prefab_factories.h>

#include <graphics/components/shader.h>
#include <graphics/factories/mesh_factories.h>
#include <graphics/shader_factory.h>

using graphics::components::mesh_gl::MeshGL;
using graphics::components::shader::Shader;
using graphics::factories::mesh_factories::create_rainbow_triangle_mesh;
using graphics::shader_factory::create_vertex_color_shader;

namespace graphics::factories::prefab_factories
{

	std::expected<entt::entity, std::string> create_rainbow_triangle_ent(entt::registry& reg)
	{
        auto meshResult = create_rainbow_triangle_mesh();
        if (!meshResult)
            return std::unexpected(std::format("Failed to create mesh: {}\n", meshResult.error()));

        MeshGL mesh = *meshResult;

        auto vertex_color_shader_result = create_vertex_color_shader();
        if (!vertex_color_shader_result) 
            return std::unexpected(std::format("Failed to create shader: {}\n", vertex_color_shader_result.error()));
        
        GLuint vertex_color_shader = *vertex_color_shader_result;

        entt::entity ent_rainbow_triangle = reg.create();
        reg.emplace<Shader>(ent_rainbow_triangle, vertex_color_shader);
        reg.emplace<MeshGL>(ent_rainbow_triangle, *meshResult);

		return ent_rainbow_triangle;
	}

} // namespace graphics::factories::prefab_factories