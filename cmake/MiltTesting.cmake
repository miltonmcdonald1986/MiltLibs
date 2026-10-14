include_guard(GLOBAL)

if(MILTLIBS_BUILD_TESTS)
    include(FetchContent)
    include(GoogleTest)

    # Use an installed GoogleTest if one is found, otherwise download it.
    FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
        GIT_SHALLOW    TRUE
        FIND_PACKAGE_ARGS NAMES GTest
    )

    # Match the MSVC runtime used by the rest of the project.
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)
endif()

# milt_add_test(<name> SOURCES <src>... [DEPENDS <target>...])
#
# Creates a GoogleTest executable and registers each TEST() with CTest.
# Does nothing when MILTLIBS_BUILD_TESTS is OFF, so call it unconditionally.
function(milt_add_test name)
    if(NOT MILTLIBS_BUILD_TESTS)
        return()
    endif()

    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "SOURCES;DEPENDS")

    add_executable(${name} ${ARG_SOURCES})
    _milt_configure_executable(${name} DEPENDS ${ARG_DEPENDS} GTest::gtest_main GTest::gmock)
    gtest_discover_tests(${name})
endfunction()
