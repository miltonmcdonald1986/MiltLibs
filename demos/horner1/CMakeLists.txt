add_executable(horner1
	main.cpp
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
