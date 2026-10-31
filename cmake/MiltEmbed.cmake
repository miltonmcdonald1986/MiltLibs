include_guard(GLOBAL)

set(_milt_embed_script "${CMAKE_CURRENT_LIST_DIR}/embed_file.cmake")

# milt_embed_file(<target> FILE <file> NAME <name> [NAMESPACE <namespace>])
#
# Compiles <file>'s bytes into <target>, so it needs no file at run time, not
# even under Emscripten. The bytes are exposed as
#   extern const std::span<const unsigned char> <namespace>::<name>;
# which the target declares itself, in the source that uses it unless others
# need it too. <file> is relative
# to the current source directory. The source holding the bytes is generated
# at build time, regenerated when <file> changes, and skipped by clang-tidy.
function(milt_embed_file target)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "FILE;NAME;NAMESPACE" "")
    if(NOT ARG_FILE OR NOT ARG_NAME)
        message(FATAL_ERROR "milt_embed_file(${target}): FILE and NAME are required.")
    endif()
    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR
            "milt_embed_file(${target}): unknown arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()

    cmake_path(ABSOLUTE_PATH ARG_FILE OUTPUT_VARIABLE input)
    set(output "${CMAKE_CURRENT_BINARY_DIR}/embedded/${ARG_NAME}.cpp")

    add_custom_command(
        OUTPUT "${output}"
        COMMAND "${CMAKE_COMMAND}"
            "-DINPUT=${input}" "-DOUTPUT=${output}"
            "-DNAME=${ARG_NAME}" "-DNAMESPACE=${ARG_NAMESPACE}"
            -P "${_milt_embed_script}"
        DEPENDS "${input}" "${_milt_embed_script}"
        COMMENT "Embedding ${ARG_FILE}"
        VERBATIM
    )
    # A generated array of bytes has nothing for clang-tidy to find, and is
    # slow to lint.
    set_source_files_properties("${output}" PROPERTIES SKIP_LINTING ON)
    target_sources(${target} PRIVATE "${output}")
endfunction()
