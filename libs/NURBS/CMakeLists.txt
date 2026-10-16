milt_add_library(nurbs HEADER_ONLY)

milt_add_test(nurbs_tests
    SOURCES tests/power_basis_tests.cpp
    DEPENDS Milt::nurbs glm::glm
)

milt_add_runner(nurbs_runner
    SOURCES runner/main.cpp runner/framework/app_loop.cpp
    DEPENDS Milt::nurbs milt_raygui
)
