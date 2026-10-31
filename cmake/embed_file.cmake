# Run by milt_embed_file (MiltEmbed.cmake) at build time:
#   cmake -DINPUT=<file> -DOUTPUT=<cpp> -DNAME=<name> [-DNAMESPACE=<ns>] -P embed_file.cmake
# Writes a C++ source defining <ns>::<name>, a span over INPUT's bytes.

file(READ "${INPUT}" hex HEX)
string(LENGTH "${hex}" hex_length)
math(EXPR size "${hex_length} / 2")

# 16 bytes per line, each as 0xNN. CMake regexes have no {n}, so spell out a line's worth.
string(REPEAT "[0-9a-f]" 32 line_of_hex)
string(REGEX REPLACE "(${line_of_hex})" "\\1\n" bytes "${hex}")
string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," bytes "${bytes}")

cmake_path(GET INPUT FILENAME input_name)
if(NAMESPACE)
    set(open_namespace "namespace ${NAMESPACE} {\n")
    set(close_namespace "} // namespace ${NAMESPACE}\n")
endif()

file(WRITE "${OUTPUT}.tmp"
"// Generated from ${input_name} by embed_file.cmake. Do not edit.

#include <array>
#include <span>

namespace {

constexpr std::array<unsigned char, ${size}> bytes{
${bytes}
};

} // namespace

${open_namespace}
extern const std::span<const unsigned char> ${NAME};
const std::span<const unsigned char> ${NAME}{bytes};

${close_namespace}")

# Only touch OUTPUT when it changes, so an unchanged file doesn't trigger a recompile.
file(COPY_FILE "${OUTPUT}.tmp" "${OUTPUT}" ONLY_IF_DIFFERENT)
file(REMOVE "${OUTPUT}.tmp")
