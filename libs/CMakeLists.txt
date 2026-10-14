# Every immediate subdirectory with a CMakeLists.txt is a library.
# Re-run CMake configure after adding a new library folder.
file(GLOB lib_dirs LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/*")
list(SORT lib_dirs)

foreach(lib_dir IN LISTS lib_dirs)
    if(IS_DIRECTORY "${lib_dir}" AND EXISTS "${lib_dir}/CMakeLists.txt")
        add_subdirectory("${lib_dir}")
    endif()
endforeach()
