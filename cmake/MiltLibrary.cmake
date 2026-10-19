include_guard(GLOBAL)

# GLM supplies real-world point types (glm::dvec3, ...) to runners, which link it
# automatically, and to tests, which list glm::glm in DEPENDS.
# Use an installed GLM if one is found, otherwise download it. SYSTEM keeps the
# project's warnings-as-errors from applying to GLM's headers.
if(MILTLIBS_BUILD_TESTS OR MILTLIBS_BUILD_RUNNERS)
    include(FetchContent)

    FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        1.0.3
        GIT_SHALLOW    TRUE
        SYSTEM
        FIND_PACKAGE_ARGS
    )

    # Header-only: don't build GLM's optional compiled library.
    set(GLM_BUILD_LIBRARY OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(glm)
endif()

# _milt_check_args(<helper> <target> [REQUIRED <keyword>...])
#
# Validates the ARG_* results of the caller's cmake_parse_arguments call.
# Fails on unknown arguments, keywords given without values, and empty
# REQUIRED keywords, naming <helper>(<target>) so the error points at the call.
function(_milt_check_args helper target)
    cmake_parse_arguments(PARSE_ARGV 2 CHECK "" "" "REQUIRED")

    if(ARG_UNPARSED_ARGUMENTS)
        list(JOIN ARG_UNPARSED_ARGUMENTS " " unparsed)
        message(FATAL_ERROR "${helper}(${target}): unknown arguments: ${unparsed}")
    endif()
    if(ARG_KEYWORDS_MISSING_VALUES)
        list(JOIN ARG_KEYWORDS_MISSING_VALUES " " missing)
        message(FATAL_ERROR "${helper}(${target}): keywords given without values: ${missing}")
    endif()
    foreach(keyword IN LISTS CHECK_REQUIRED)
        if(NOT ARG_${keyword})
            message(FATAL_ERROR "${helper}(${target}): ${keyword} is required")
        endif()
    endforeach()
endfunction()

# Applies the settings every compiled MiltLibs executable shares:
# C++23, project warnings, and the given dependencies.
function(_milt_configure_executable target)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "DEPENDS")
    _milt_check_args(_milt_configure_executable ${target})

    target_compile_features(${target} PRIVATE cxx_std_23)
    target_link_libraries(${target} PRIVATE milt_warnings ${ARG_DEPENDS})
endfunction()

# milt_add_library(<name> {STATIC SOURCES <src>... | HEADER_ONLY} [DEPENDS <target>...])
#
# Creates library target milt_<name> and its alias Milt::<name>.
# Consumers and other MiltLibs targets link only Milt::<name>; the milt_
# prefix keeps the real target from colliding with names in consumer builds.
# Exactly one of STATIC or HEADER_ONLY is required. STATIC needs SOURCES;
# HEADER_ONLY creates an INTERFACE library and takes no SOURCES.
# Public headers live in <lib>/include, private code in <lib>/src,
# tests in <lib>/tests.
# DEPENDS are linked PUBLIC, since a library's headers usually expose them.
function(milt_add_library name)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "STATIC;HEADER_ONLY" "" "SOURCES;DEPENDS")
    _milt_check_args(milt_add_library ${name})

    if(ARG_STATIC STREQUAL ARG_HEADER_ONLY)
        message(FATAL_ERROR "milt_add_library(${name}): specify exactly one of STATIC or HEADER_ONLY")
    endif()
    if(ARG_STATIC AND NOT ARG_SOURCES)
        message(FATAL_ERROR "milt_add_library(${name}): STATIC requires SOURCES")
    endif()
    if(ARG_HEADER_ONLY AND ARG_SOURCES)
        message(FATAL_ERROR "milt_add_library(${name}): HEADER_ONLY takes no SOURCES")
    endif()

    set(target milt_${name})
    set(include_dir "${CMAKE_CURRENT_SOURCE_DIR}/include")

    if(ARG_STATIC)
        add_library(${target} STATIC ${ARG_SOURCES})
        target_compile_features(${target} PUBLIC cxx_std_23)
        target_include_directories(${target}
            PUBLIC
                $<BUILD_INTERFACE:${include_dir}>
                $<INSTALL_INTERFACE:include>
            PRIVATE
                "${CMAKE_CURRENT_SOURCE_DIR}/src"
        )
        target_link_libraries(${target}
            PUBLIC ${ARG_DEPENDS}
            PRIVATE milt_warnings
        )
    else()
        add_library(${target} INTERFACE)
        target_compile_features(${target} INTERFACE cxx_std_23)
        target_include_directories(${target} INTERFACE
            $<BUILD_INTERFACE:${include_dir}>
            $<INSTALL_INTERFACE:include>
        )
        target_link_libraries(${target} INTERFACE ${ARG_DEPENDS})
    endif()

    set_target_properties(${target} PROPERTIES EXPORT_NAME ${name})
    add_library(Milt::${name} ALIAS ${target})
endfunction()

# milt_add_runner(<name> SOURCES <src>... [DEPENDS <target>...])
#
# Creates executable milt_<name> for trying a library by hand. Links glm::glm.
# Does nothing when MILTLIBS_BUILD_RUNNERS is OFF, so call it unconditionally.
# Arguments are validated either way.
#
# Under Emscripten (the web-debug/web-release presets), it builds
# milt_<name>.html, using Emscripten's default shell page, with
# milt_<name>.js and milt_<name>.wasm beside it. Browsers won't load these
# from file://, so serve them, for example with
#   emrun build/web-debug/libs/<Lib>/milt_<name>.html
function(milt_add_runner name)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "SOURCES;DEPENDS")
    _milt_check_args(milt_add_runner ${name} REQUIRED SOURCES)

    if(NOT MILTLIBS_BUILD_RUNNERS)
        return()
    endif()

    set(target milt_${name})
    add_executable(${target} ${ARG_SOURCES})
    _milt_configure_executable(${target} DEPENDS ${ARG_DEPENDS} glm::glm)

    if(EMSCRIPTEN)
        set_target_properties(${target} PROPERTIES SUFFIX ".html")
        target_link_options(${target} PRIVATE -sALLOW_MEMORY_GROWTH=1)
    endif()
endfunction()
