find_package(OpenGL REQUIRED)

set(GRAPHICS_HEADERS
    include/graphics/animation_systems.h
    include/graphics/app/app.h
    include/graphics/app/app_guard.h
    include/graphics/app/lifecycle.h
    include/graphics/components/color.h
    include/graphics/components/flash.h
    include/graphics/components/mesh_gl.h
    include/graphics/components/shader.h
    include/graphics/components/texture.h
    include/graphics/factories/mesh_factories.h
    include/graphics/factories/prefab_factories.h
    include/graphics/factories/texture_factories.h
    include/graphics/gl_config.h
    include/graphics/gl_state.h
    include/graphics/mesh/mesh_factory_backend.h
    include/graphics/mesh/vertex_attribute.h
    include/graphics/mesh/vertex_layout.h
    include/graphics/platform/glfw_callbacks.h
    include/graphics/shader_factory.h
    include/graphics/ui/widgets.h
    include/graphics/window_config.h
    include/graphics/window_state.h
)

set(GRAPHICS_SOURCES
    src/animations_systems.cpp
    src/app/app_guard.cpp
    src/app/lifecycle.cpp
    src/factories/mesh_factories.cpp
    src/factories/prefab_factories.cpp
    src/factories/texture_factories.cpp
    src/platform/glfw_callbacks.cpp
    src/shader_factory.cpp
    src/mesh/mesh_factory_backend.cpp
    src/ui/widgets.cpp
)

add_library(graphics STATIC)

target_sources(graphics
    PRIVATE
        ${GRAPHICS_SOURCES}
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES ${GRAPHICS_HEADERS}
)

target_include_directories(graphics
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${stb_SOURCE_DIR}
)

# Compiler warnings
if(MSVC)
    target_compile_options(graphics PRIVATE /W4 /WX)
else()
    target_compile_options(graphics PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Link OpenGL, gl3w/glad, GLFW, etc.
target_link_libraries(graphics
    PUBLIC
        EnTT::EnTT
	    glfw
	    gl3w
        imgui
	    OpenGL::GL
)
