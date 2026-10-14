include_guard(GLOBAL)

# milt_warnings: carries the project's warning flags. Link it PRIVATE so the
# flags apply to MiltLibs code only and never leak to consumers.
add_library(milt_warnings INTERFACE)

set(_milt_msvc "$<CXX_COMPILER_ID:MSVC>")
set(_milt_gnu_like "$<CXX_COMPILER_ID:GNU,Clang,AppleClang>")

target_compile_options(milt_warnings INTERFACE
    $<${_milt_msvc}:/W4 /permissive- /utf-8 /Zc:__cplusplus>
    $<${_milt_gnu_like}:-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion>
)

if(MILTLIBS_WARNINGS_AS_ERRORS)
    target_compile_options(milt_warnings INTERFACE
        $<${_milt_msvc}:/WX>
        $<${_milt_gnu_like}:-Werror>
    )
endif()

unset(_milt_msvc)
unset(_milt_gnu_like)
