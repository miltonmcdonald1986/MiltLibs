add_executable(horner1
	main.cpp
)

if (WIN32)
	set_target_properties(horner1 PROPERTIES WIN32_EXECUTABLE TRUE)
	target_link_options(horner1 PRIVATE "/ENTRY:mainCRTStartup")
endif()

# stb as system include
target_include_directories(horner1
    SYSTEM PRIVATE
        ${stb_SOURCE_DIR}
        ${stb_BINARY_DIR}
)

# only link what horner1 actually needs
target_link_libraries(horner1 PRIVATE
    graphics
    nurbs
)