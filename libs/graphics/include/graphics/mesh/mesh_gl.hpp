#ifndef GRAPHICS_MESH_MESH_GL_HPP
#define GRAPHICS_MESH_MESH_GL_HPP

#include <graphics/components/component_defaults.hpp>
#include <graphics/engine/result.hpp>
#include <graphics/platform/gl_includes.h>              // IWYU pragma: keep

namespace graphics::mesh
{
    
    // NOLINTBEGIN(misc-include-cleaner)

    struct MeshGL 
    {
        GLenum  primitive   { components::MeshGLDefaults::primitive };
        GLuint  vao         { 0 };
        GLuint  vbo         { 0 };
        GLuint  ebo         { 0 };
        GLsizei vertexCount { 0 };
        GLsizei indexCount  { 0 };
    };

    // NOLINTEND(misc-include-cleaner)

    using MeshGLResult = engine::Result<MeshGL>;

} // namespace graphics::mesh

#endif // GRAPHICS_MESH_MESH_GL_HPP