include_guard(GLOBAL)

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
# Creates library <name> and its alias Milt::<name>.
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

    set(include_dir "${CMAKE_CURRENT_SOURCE_DIR}/include")

    if(ARG_STATIC)
        add_library(${name} STATIC ${ARG_SOURCES})
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
# Does nothing when MILTLIBS_BUILD_RUNNERS is OFF, so call it unconditionally.
# Arguments are validated either way.
function(milt_add_runner name)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "SOURCES;DEPENDS")
    _milt_check_args(milt_add_runner ${name} REQUIRED SOURCES)

    if(NOT MILTLIBS_BUILD_RUNNERS)
        return()
    endif()

    add_executable(${name} ${ARG_SOURCES})
    _milt_configure_executable(${name} DEPENDS ${ARG_DEPENDS})
endfunction()
