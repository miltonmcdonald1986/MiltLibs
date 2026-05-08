#ifndef GRAPHICS_FACTORIES_SHADER_FACTOIES_HPP
#define GRAPHICS_FACTORIES_SHADER_FACTOIES_HPP

#include <filesystem>

#include <graphics/engine/result.hpp>
#include <graphics/platform/gl_includes.h> // IWYU pragma: keep

namespace graphics::factories
{

	using ShaderResult = engine::Result<GLuint>; // NOLINT(misc-include-cleaner)

	auto create_basic_shader() -> ShaderResult;
	auto create_color_shader() -> ShaderResult;
	auto create_shader_from_files(const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path) -> ShaderResult;
	auto create_textured_color_mvp_shader() -> ShaderResult;
	auto create_textured_mvp_shader() -> ShaderResult;
	auto create_textured_shader() -> ShaderResult;
	auto create_vertex_color_shader() -> ShaderResult;

} // namespace graphics::factories

#endif // GRAPHICS_FACTORIES_SHADER_FACTOIES_HPP