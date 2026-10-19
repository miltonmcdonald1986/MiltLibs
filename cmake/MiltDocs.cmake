include_guard(GLOBAL)

# milt_docs: HTML API reference for every library's public headers, written to
# <build>/html. It isn't part of ALL, so build it by name, e.g. with the docs
# preset:
#   cmake --preset docs
#   cmake --build --preset docs
# Any Doxygen warning, such as an undocumented entity or parameter, fails the
# build. Formulas render with MathJax, so LaTeX isn't needed.
# Re-run CMake configure after adding a new library folder.
if(MILTLIBS_BUILD_DOCS)
    find_package(Doxygen REQUIRED)

    file(GLOB include_dirs LIST_DIRECTORIES true "${PROJECT_SOURCE_DIR}/libs/*/include")
    list(SORT include_dirs)

    set(DOXYGEN_PROJECT_NAME "${PROJECT_NAME}")
    set(DOXYGEN_PROJECT_BRIEF "${PROJECT_DESCRIPTION}")
    # Show paths and #include lines as consumers write them: <milt/nurbs/...>.
    set(DOXYGEN_STRIP_FROM_PATH ${include_dirs})
    set(DOXYGEN_STRIP_FROM_INC_PATH ${include_dirs})
    set(DOXYGEN_USE_MATHJAX YES)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
    set(DOXYGEN_WARN_IF_INCOMPLETE_DOC YES)
    set(DOXYGEN_WARN_NO_PARAMDOC YES)
    set(DOXYGEN_WARN_AS_ERROR FAIL_ON_WARNINGS)

    doxygen_add_docs(milt_docs ${include_dirs}
        COMMENT "Generating API documentation with Doxygen"
    )
endif()
