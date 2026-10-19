include_guard(GLOBAL)

# raylib renders the graphical runners and raygui supplies their controls.
# Runners list milt_raygui in DEPENDS, which also links raylib.
# Both are always built from source, so desktop and Emscripten builds each get
# raylib for their own platform (raylib picks Web under Emscripten itself).
# They're fetched as release tarballs, not git clones, to skip raylib's history.
# SYSTEM keeps the project's warnings-as-errors from applying to their headers.
if(MILTLIBS_BUILD_RUNNERS)
    # raylib and raygui are C; the root project only enables CXX.
    enable_language(C)

    include(FetchContent)

    FetchContent_Declare(raylib
        URL      https://github.com/raysan5/raylib/archive/refs/tags/6.2.tar.gz
        URL_HASH SHA256=df7a13129b1bec9e73a6962efff170beba310ee6eab8f94cd000b7316b4db5ee
        SYSTEM
    )

    # raygui is a single header with no top-level CMakeLists.txt, so
    # FetchContent only downloads it.
    FetchContent_Declare(raygui
        URL      https://github.com/raysan5/raygui/archive/refs/tags/5.0.tar.gz
        URL_HASH SHA256=0f194c4a5e837c0930aca0b6315db45d00f76fa0052d841eea94598d390c39d6
    )

    FetchContent_MakeAvailable(raylib raygui)

    # Under Emscripten, raylib's target exports -sEXPORTED_RUNTIME_METHODS=ccall.
    # The default shell page's Fullscreen button (and raylib's ToggleFullscreen)
    # call Module.requestFullscreen, which Emscripten 4.0.9+ no longer exports
    # by default. Emscripten keeps only the last value of a -s setting, and
    # raylib's lands after any the runner adds, so extend raylib's instead.
    if(EMSCRIPTEN)
        set(raylib_exports "-sEXPORTED_RUNTIME_METHODS=ccall")
        get_target_property(raylib_link_options raylib INTERFACE_LINK_OPTIONS)
        if(NOT raylib_exports IN_LIST raylib_link_options)
            message(FATAL_ERROR "raylib no longer links with ${raylib_exports}; "
                "update the requestFullscreen export in ${CMAKE_CURRENT_LIST_FILE}.")
        endif()
        list(TRANSFORM raylib_link_options
            REPLACE "^${raylib_exports}$" "${raylib_exports},requestFullscreen")
        set_target_properties(raylib PROPERTIES INTERFACE_LINK_OPTIONS "${raylib_link_options}")
    endif()

    # milt_raygui: raygui's implementation, compiled once. Third-party code, so
    # it doesn't link milt_warnings. Under Emscripten, raylib's target already
    # exports -sUSE_GLFW=3, so it isn't repeated here.
    add_library(milt_raygui STATIC "${CMAKE_CURRENT_LIST_DIR}/raygui_impl.c")
    target_include_directories(milt_raygui SYSTEM PUBLIC "${raygui_SOURCE_DIR}/src")
    target_link_libraries(milt_raygui PUBLIC raylib)
endif()
