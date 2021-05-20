cmake_minimum_required (VERSION 3.13)

add_executable(sandbox)
set_property(TARGET sandbox PROPERTY CXX_STANDARD 17)

target_sources(sandbox

	PRIVATE

		"main.cpp"
)

target_include_directories(sandbox

	PRIVATE

		"../Marvel/core"
		"../Marvel/shaders"
		"../Marvel/renderer"
		"../Marvel/geometry"
		"../Marvel/renderer/pipeline"
		"../Marvel/renderer/bindables"
		"../Marvel/renderer/buffers"
		"../Marvel/renderer/drawables"
		"../Marvel/renderer/passes"
		"../Marvel/renderer/render_graph"

		"../Dependencies/imgui/"
		"../Dependencies/imgui/backends/"
		"../Dependencies/assimp/include/"
		"../Dependencies/stb/"
		"../Dependencies/glm/"

)

target_link_libraries(sandbox
	PRIVATE
		marvel
)