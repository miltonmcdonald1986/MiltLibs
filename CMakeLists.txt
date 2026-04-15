cmake_minimum_required(VERSION 3.25)

project(MiltNURBS
    VERSION 0.1.0
    DESCRIPTION "A modern C++ NURBS library"
    LANGUAGES CXX
)

# Set C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(MiltNURBS_BUILD_TESTS "Build unit tests" ON)
option(MiltNURBS_BUILD_DEMOS "Build demo applications" ON)

# Build the library
add_subdirectory(miltnurbs)

# Unit tests
if(MiltNURBS_BUILD_TESTS)
    enable_testing()
    
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    add_subdirectory(tests)
endif()

# Demos
if(MiltNURBS_BUILD_DEMOS)
    add_subdirectory(demos)
endif()
