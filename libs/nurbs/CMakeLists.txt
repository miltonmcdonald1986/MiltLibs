set(NURBS_HEADERS
    include/nurbs/bernstein.h
    include/nurbs/bezier.h
    include/nurbs/bmp.h
    include/nurbs/horner.h
    include/nurbs/vector.h
)

# Create INTERFACE library target (header-only)
add_library(nurbs INTERFACE)

# Set include directories
target_include_directories(nurbs
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Compiler warnings
if(MSVC)
    target_compile_options(nurbs INTERFACE /W4 /WX)
else()
    target_compile_options(nurbs INTERFACE -Wall -Wextra -Wpedantic -Werror)
endif()

target_sources(nurbs INTERFACE FILE_SET HEADERS BASE_DIRS include FILES ${NURBS_HEADERS})
