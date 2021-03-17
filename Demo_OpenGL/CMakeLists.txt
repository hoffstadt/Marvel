cmake_minimum_required (VERSION 3.13)

add_executable (demo_opengl)

set_property(TARGET demo_opengl PROPERTY CXX_STANDARD 17)

target_sources(demo_opengl

	PRIVATE

		"main.cpp"
		"../Dependencies/imgui/examples/libs/gl3w/GL/gl3w.c"

)

target_include_directories(demo_opengl

	PRIVATE
		"../Dependencies/imgui/examples/libs/gl3w/"
		"../Dependencies/glfw/include/"
		"../Dependencies/glm/"
		"../Dependencies/stb/"
)

target_link_libraries(demo_opengl

	PRIVATE
		glfw
		GL
		$<$<PLATFORM_ID:Linux>:GL>
		$<$<PLATFORM_ID:Windows>:opengl32>
)
