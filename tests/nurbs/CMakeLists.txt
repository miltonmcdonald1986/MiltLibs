# Test executable
set(TEST_SOURCES
	test_bernstein.cpp
    test_bezier.cpp
    test_bmp.cpp
	test_horner.cpp
    test_main.cpp
    test_vector.cpp
)

add_executable(nurbs_tests ${TEST_SOURCES})

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(nurbs_tests PRIVATE --coverage -O0 -g)
    target_link_options(nurbs_tests PRIVATE --coverage)
endif()

target_link_libraries(nurbs_tests
    PRIVATE
        nurbs
        GTest::gtest
        GTest::gtest_main
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(nurbs_tests)
