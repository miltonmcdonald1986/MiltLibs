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
# Creates GoogleTest executable milt_<name> and registers each TEST() with CTest.
# Does nothing when MILTLIBS_BUILD_TESTS is OFF, so call it unconditionally.
# Arguments are validated either way.
function(milt_add_test name)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "SOURCES;DEPENDS")
    _milt_check_args(milt_add_test ${name} REQUIRED SOURCES)

    if(NOT MILTLIBS_BUILD_TESTS)
        return()
    endif()

    set(target milt_${name})
    add_executable(${target} ${ARG_SOURCES})
    _milt_configure_executable(${target} DEPENDS ${ARG_DEPENDS} GTest::gtest_main GTest::gmock)
    gtest_discover_tests(${target})
endfunction()
