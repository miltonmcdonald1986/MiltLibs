milt_add_library(nurbs HEADER_ONLY)

milt_add_test(nurbs_tests
    SOURCES tests/power_basis_tests.cpp
    DEPENDS Milt::nurbs glm::glm
)

# Tests for the runner's model code, which is header-only and doesn't use raylib, so they
# build even when runners are off.
milt_add_test(nurbs_runner_tests
    SOURCES tests/horner_model_tests.cpp tests/horner_bounds_tests.cpp
            tests/horner_presets_tests.cpp
    DEPENDS Milt::nurbs glm::glm
)
# milt_add_test does nothing when tests are off, so the target may not exist.
if(TARGET milt_nurbs_runner_tests)
    target_include_directories(milt_nurbs_runner_tests PRIVATE runner)
endif()

milt_add_runner(nurbs_runner
    SOURCES runner/main.cpp runner/triangle_view.cpp runner/framework/app_loop.cpp
    DEPENDS Milt::nurbs milt_raygui
)
