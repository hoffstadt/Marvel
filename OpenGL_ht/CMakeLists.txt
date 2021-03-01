cmake_minimum_required (VERSION 3.13)

add_executable (opengl_ht)

set_property(TARGET opengl_ht PROPERTY CXX_STANDARD 17)

target_sources(opengl_ht

	PRIVATE

		"main.cpp"
		"../Dependencies/imgui/examples/libs/gl3w/GL/gl3w.c"

)

target_include_directories(opengl_ht

	PRIVATE
		"../Dependencies/imgui/examples/libs/gl3w/"
		"../Dependencies/glfw/include/"
		"../Dependencies/glm/"
)

target_link_directories(opengl_ht

	PRIVATE

		"C:/VulkanSDK/1.2.170.0/Lib"

)

target_link_libraries(opengl_ht

	PRIVATE
		glfw
		opengl32
)
