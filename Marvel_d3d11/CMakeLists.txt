cmake_minimum_required (VERSION 3.13)

add_executable(marvel_d3d11)
set_property(TARGET marvel_d3d11 PROPERTY CXX_STANDARD 17)

target_sources(marvel_d3d11

	PRIVATE

		"main.cpp"
		"core/mvWindow.cpp"
		"core/mvMarvelUtils.cpp"
		"renderer/mvGraphics.cpp"
		"renderer/mvVertexLayout.cpp"
		"renderer/mvImGuiManager.cpp"

		"renderer/bindables/mvTopology.cpp"
		"renderer/bindables/mvPixelShader.cpp"
		"renderer/bindables/mvVertexShader.cpp"
		"renderer/bindables/mvIndexBuffer.cpp"
		"renderer/bindables/mvVertexBuffer.cpp"
		"renderer/bindables/mvInputLayout.cpp"
		"renderer/bindables/mvTexture.cpp"
		"renderer/bindables/mvSampler.cpp"
		"renderer/bindables/mvRenderTarget.cpp"

		# imgui
		"../Dependencies/imgui/misc/cpp/imgui_stdlib.cpp"
		"../Dependencies/imgui/imgui.cpp"
		"../Dependencies/imgui/imgui_demo.cpp"
		"../Dependencies/imgui/imgui_draw.cpp"
		"../Dependencies/imgui/imgui_widgets.cpp"
		"../Dependencies/imgui/imgui_tables.cpp"
		"../Dependencies/imgui/backends/imgui_impl_win32.cpp"
		"../Dependencies/imgui/backends/imgui_impl_dx11.cpp"
)

target_include_directories(marvel_d3d11

	PRIVATE

		"core"
		"shaders"
		"renderer"
		"renderer/bindables"

		"../Dependencies/imgui/"
		"../Dependencies/imgui/backends/"
		"../Dependencies/stb/"
		"../Dependencies/glfw/include/"

)

target_link_libraries(marvel_d3d11
	PRIVATE
		d3d11
		d3dcompiler
		glfw
)