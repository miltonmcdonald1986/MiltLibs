cmake_minimum_required(VERSION 3.28)

project(MiltLibs
    VERSION 0.1.0
    DESCRIPTION "Milton's C++ libraries"
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Tests, runners and warnings-as-errors are for developing MiltLibs itself.
# When consumed via add_subdirectory/FetchContent, only the libraries are built.
option(MILTLIBS_BUILD_TESTS "Build MiltLibs unit tests" ${PROJECT_IS_TOP_LEVEL})
option(MILTLIBS_BUILD_RUNNERS "Build MiltLibs runner executables" ${PROJECT_IS_TOP_LEVEL})
option(MILTLIBS_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ${PROJECT_IS_TOP_LEVEL})
option(MILTLIBS_BUILD_DOCS "Build the Doxygen API reference (needs Doxygen)" OFF)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(MiltCompileOptions)
include(MiltDocs)
include(MiltEmbed)
include(MiltLibrary)
include(MiltRaylib)
include(MiltTesting)

if(MILTLIBS_BUILD_TESTS)
    enable_testing()
endif()

add_subdirectory(libs)
