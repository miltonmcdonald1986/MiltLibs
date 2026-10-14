include_guard(GLOBAL)

# Applies the settings every compiled MiltLibs executable shares:
# C++23, project warnings, and the given dependencies.
function(_milt_configure_executable target)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "DEPENDS")

    target_compile_features(${target} PRIVATE cxx_std_23)
    target_link_libraries(${target} PRIVATE milt_warnings ${ARG_DEPENDS})
endfunction()

# milt_add_library(<name> [SOURCES <src>...] [DEPENDS <target>...])
#
# Creates library <name> and its alias Milt::<name>.
# Without SOURCES the library is header-only (INTERFACE).
# Public headers live in <lib>/include, private code in <lib>/src.
# DEPENDS are linked PUBLIC, since a library's headers usually expose them.
function(milt_add_library name)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "SOURCES;DEPENDS")

    set(include_dir "${CMAKE_CURRENT_SOURCE_DIR}/include")

    if(ARG_SOURCES)
        add_library(${name} ${ARG_SOURCES})
        target_compile_features(${name} PUBLIC cxx_std_23)
        target_include_directories(${name}
            PUBLIC
                $<BUILD_INTERFACE:${include_dir}>
                $<INSTALL_INTERFACE:include>
            PRIVATE
                "${CMAKE_CURRENT_SOURCE_DIR}/src"
        )
        target_link_libraries(${name}
            PUBLIC ${ARG_DEPENDS}
            PRIVATE milt_warnings
        )
    else()
        add_library(${name} INTERFACE)
        target_compile_features(${name} INTERFACE cxx_std_23)
        target_include_directories(${name} INTERFACE
            $<BUILD_INTERFACE:${include_dir}>
            $<INSTALL_INTERFACE:include>
        )
        target_link_libraries(${name} INTERFACE ${ARG_DEPENDS})
    endif()

    add_library(Milt::${name} ALIAS ${name})
endfunction()

# milt_add_runner(<name> SOURCES <src>... [DEPENDS <target>...])
#
# Creates a small executable for trying a library by hand.
function(milt_add_runner name)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "SOURCES;DEPENDS")

    add_executable(${name} ${ARG_SOURCES})
    _milt_configure_executable(${name} DEPENDS ${ARG_DEPENDS})
endfunction()
