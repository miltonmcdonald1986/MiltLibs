#ifndef GRAPHICS_MESH_VERTEX_ATTRIBUTE_H
#define GRAPHICS_MESH_VERTEX_ATTRIBUTE_H

#include <graphics/platform/gl_includes.h>

namespace graphics::mesh
{

    struct VertexAttribute
    {
        GLuint index;   // attribute location (0, 1, 2, ...)
        GLint components;   // number of floats (e.g., 3 for vec3)
        size_t offset;  // byte offset from start of vertex
    };

} // namespace graphics::mesh

#endif // GRAPHICS_MESH_VERTEX_ATTRIBUTE_H
