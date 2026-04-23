add_executable(horner1
	main.cpp
)

if (WIN32)
    set_target_properties(horner1 PROPERTIES WIN32_EXECUTABLE TRUE)
	target_link_options(horner1 PRIVATE "/ENTRY:mainCRTStartup")
endif()

target_include_directories(horner1
    PRIVATE
        ${stb_SOURCE_DIR}
)

target_link_libraries(horner1 PRIVATE
	graphics
	nurbs
	imgui
	glfw
	gl3w
	OpenGL::GL
	EnTT::EnTT
)
