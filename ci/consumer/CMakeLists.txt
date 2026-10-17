cmake_minimum_required(VERSION 3.28)

# A stand-in for a project that consumes MiltLibs via add_subdirectory with
# default options. CI builds it so breakage that only consumers see, like #2
# and #4, fails a PR.
project(MiltLibsConsumer LANGUAGES CXX)

add_subdirectory(../.. MiltLibs)

# With default options, MiltLibs builds only the libraries, so none of its
# development dependencies should be pulled in.
foreach(dev_target IN ITEMS glm::glm GTest::gtest raylib)
    if(TARGET ${dev_target})
        message(FATAL_ERROR "MiltLibs pulled in ${dev_target} as a subproject")
    endif()
endforeach()

add_executable(consumer main.cpp)
target_link_libraries(consumer PRIVATE Milt::nurbs)
