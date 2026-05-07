#ifndef GRAPHICS_COMPONENTS_MESH_GL_HPP
#define GRAPHICS_COMPONENTS_MESH_GL_HPP

#include <graphics/components/component_defaults.hpp>
#include <graphics/platform/gl_includes.h>              // IWYU pragma: keep

namespace graphics::components
{
    
    // NOLINTBEGIN(misc-include-cleaner)
    struct MeshGL 
    {
        GLenum  primitive   { MeshGLDefaults::primitive };
        GLuint  vao         { 0 };
        GLuint  vbo         { 0 };
        GLuint  ebo         { 0 };
        GLsizei vertexCount { 0 };
        GLsizei indexCount  { 0 };
    };
    // NOLINTEND(misc-include-cleaner)

} // namespace graphics::components

#endif // GRAPHICS_COMPONENTS_MESH_GL_HPP