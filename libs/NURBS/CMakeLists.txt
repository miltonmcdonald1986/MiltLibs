milt_add_library(nurbs
    SOURCES src/nurbs.cpp
)

milt_add_test(nurbs_tests
    SOURCES tests/nurbs_tests.cpp
    DEPENDS Milt::nurbs
)

milt_add_runner(nurbs_runner
    SOURCES runner/main.cpp
    DEPENDS Milt::nurbs
)
