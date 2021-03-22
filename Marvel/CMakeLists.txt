cmake_minimum_required (VERSION 3.13)

add_executable(marvel)
set_property(TARGET marvel PROPERTY CXX_STANDARD 17)

target_sources(marvel

	PRIVATE

		"main.cpp"

		"core/mvWindow.cpp"
		"core/mvMarvelUtils.cpp"
		"core/mvKeyboard.cpp"
		"core/mvMouse.cpp"
		"core/mvTimer.cpp"

		"geometry/Sphere.cpp"

		"renderer/mvGraphics.cpp"
		"renderer/mvVertexLayout.cpp"
		"renderer/mvImGuiManager.cpp"
		"renderer/mvProjection.cpp"
		"renderer/mvCamera.cpp"
		"renderer/mvPointLight.cpp"
		"renderer/mvNode.cpp"
		"renderer/mvModel.cpp"
		"renderer/mvBuffer.cpp"
		"renderer/mvStep.cpp"
		"renderer/mvMaterial.cpp"

		"renderer/bindables/mvBindableRegistry.cpp"
		"renderer/bindables/mvDepthStencil.cpp"
		"renderer/bindables/mvTopology.cpp"
		"renderer/bindables/mvGeometryShader.cpp"
		"renderer/bindables/mvPixelShader.cpp"
		"renderer/bindables/mvVertexShader.cpp"
		"renderer/bindables/mvIndexBuffer.cpp"
		"renderer/bindables/mvVertexBuffer.cpp"
		"renderer/bindables/mvInputLayout.cpp"
		"renderer/bindables/mvTexture.cpp"
		"renderer/bindables/mvSampler.cpp"
		"renderer/bindables/mvRenderTarget.cpp"
		"renderer/bindables/mvTransformConstantBuffer.cpp"
		"renderer/bindables/mvNullGeometryShader.cpp"
		"renderer/bindables/mvBlender.cpp"
		"renderer/bindables/mvRasterizer.cpp"
		"renderer/bindables/mvCubeTexture.cpp"

		"renderer/drawables/mvDrawable.cpp"
		"renderer/drawables/mvTexturedQuad.cpp"
		"renderer/drawables/mvSolidSphere.cpp"
		"renderer/drawables/mvMesh.cpp"

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

target_include_directories(marvel

	PRIVATE

		"core"
		"shaders"
		"renderer"
		"geometry"
		"renderer/bindables"
		"renderer/drawables"

		"../Dependencies/imgui/"
		"../Dependencies/imgui/backends/"
		"../Dependencies/assimp/include/"
		"../Dependencies/stb/"
		"../Dependencies/glm/"

)

target_link_libraries(marvel
	PRIVATE
		d3d11
		d3dcompiler
		assimp
)