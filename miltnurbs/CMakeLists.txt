set(MiltNURBS_HEADERS
    include/miltnurbs/bernstein.h
    include/miltnurbs/bezier.h
    include/miltnurbs/bmp.h
    include/miltnurbs/horner.h
    include/miltnurbs/vector.h
)

# Create INTERFACE library target (header-only)
add_library(miltnurbs INTERFACE)

# Set include directories
target_include_directories(miltnurbs
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Compiler warnings
if(MSVC)
    target_compile_options(miltnurbs INTERFACE /W4 /WX)
else()
    target_compile_options(miltnurbs INTERFACE -Wall -Wextra -Wpedantic -Werror)
endif()

target_sources(miltnurbs INTERFACE FILE_SET HEADERS BASE_DIRS include FILES ${MiltNURBS_HEADERS})
