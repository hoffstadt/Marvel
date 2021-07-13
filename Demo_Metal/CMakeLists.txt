cmake_minimum_required (VERSION 3.13)

add_executable (demo_metal)

set_property(TARGET demo_metal PROPERTY CXX_STANDARD 17)

target_sources(demo_metal

	PRIVATE

		"main.mm"
		"MetalAdder.mm"
		"../Dependencies/imgui/examples/libs/gl3w/GL/gl3w.c"

)

target_include_directories(demo_metal

	PRIVATE
		"."
		"../Dependencies/imgui/examples/libs/gl3w/"
		"../Dependencies/glfw/include/"
		"../Dependencies/glm/"
		"../Dependencies/stb/"
)

target_link_libraries(demo_metal

	PRIVATE
		glfw
)

SET_PROPERTY(TARGET demo_metal APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc")

set(CMAKE_EXE_LINKER_FLAGS "\
		-framework Metal \
		-framework MetalKit \
		-framework Cocoa \
		-framework IOKit \
		-framework CoreVideo \
		-framework QuartzCore")
