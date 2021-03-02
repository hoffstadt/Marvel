cmake_minimum_required (VERSION 3.13)

add_executable (marvel_opengl)

set_property(TARGET marvel_opengl PROPERTY CXX_STANDARD 17)

target_sources(marvel_opengl

	PRIVATE

		"main.cpp"

		"core/mvWindow.cpp"
		"renderer/mvGraphics.cpp"
		"renderer/mvVertexLayout.cpp"

		"renderer/bindables/mvShader.cpp"
		"renderer/bindables/mvIndexBuffer.cpp"
		"renderer/bindables/mvVertexBuffer.cpp"
		"renderer/bindables/mvInputLayout.cpp"
		
		"renderer/drawables/mvDrawable.cpp"
		"renderer/drawables/mvQuad.cpp"

		"../Dependencies/imgui/examples/libs/gl3w/GL/gl3w.c"

)

target_include_directories(marvel_opengl

	PRIVATE

		"core/"
		"renderer/"
		"renderer/bindables"
		"renderer/drawables"

		"../Dependencies/imgui/examples/libs/gl3w/"
		"../Dependencies/glfw/include/"
		"../Dependencies/glm/"
)

target_link_libraries(marvel_opengl

	PRIVATE
		glfw
		opengl32
)
