cmake_minimum_required (VERSION 3.13)

add_library(marvel)
set_property(TARGET marvel PROPERTY CXX_STANDARD 17)

target_compile_definitions(marvel

	PRIVATE
		$<$<CONFIG:Debug>:MV_DEBUG>
		$<$<CONFIG:Release>:MV_RELEASE>
)

target_sources(marvel

	PRIVATE

		"core/mvWindow.cpp"
		"core/mvMarvelUtils.cpp"
		"core/mvKeyboard.cpp"
		"core/mvMouse.cpp"
		"core/mvTimer.cpp"

		"geometry/Sphere.cpp"

		"renderer/mvGraphics.cpp"
		"renderer/mvImGuiManager.cpp"
		"renderer/mvProjection.cpp"
		"renderer/mvCamera.cpp"
		"renderer/mvDirectionLight.cpp"
		"renderer/mvPointLight.cpp"
		"renderer/mvNode.cpp"
		"renderer/mvModel.cpp"
		"renderer/mvModelProbe.cpp"

		"renderer/render_graph/mvMaterial.cpp"
		"renderer/render_graph/mvStep.cpp"
		"renderer/render_graph/mvTechnique.cpp"
		"renderer/render_graph/mvJob.cpp"
		"renderer/render_graph/mvRenderGraph.cpp"

		"renderer/buffers/mvBufferRegistry.cpp"
		"renderer/buffers/mvShadowCameraConstantBuffer.cpp"
		"renderer/buffers/mvSkyBoxTransformConstantBuffer.cpp"
		"renderer/buffers/mvTransformConstantBuffer.cpp"
		"renderer/buffers/mvIndexBuffer.cpp"
		"renderer/buffers/mvVertexBuffer.cpp"
		"renderer/buffers/mvDynamicBuffer.cpp"
		"renderer/buffers/mvVertexLayout.cpp"
		"renderer/buffers/mvPBRMaterialCBuf.cpp"
		"renderer/buffers/mvPhongMaterialCBuf.cpp"
		"renderer/buffers/mvComputeBuffer.cpp"

		"renderer/bindables/mvBindableRegistry.cpp"
		"renderer/bindables/mvDepthStencil.cpp"
		"renderer/bindables/mvTexture.cpp"
		"renderer/bindables/mvRenderTarget.cpp"
		"renderer/bindables/mvCubeTexture.cpp"

		"renderer/pipeline/mvPipeline.cpp"
		"renderer/pipeline/mvBlendState.cpp"
		"renderer/pipeline/mvDepthStencilState.cpp"
		"renderer/pipeline/mvTopologyState.cpp"
		"renderer/pipeline/mvRasterizerState.cpp"
		"renderer/pipeline/mvPixelShader.cpp"
		"renderer/pipeline/mvVertexShader.cpp"
		"renderer/pipeline/mvComputeShader.cpp"
		"renderer/pipeline/mvGeometryShader.cpp"
		"renderer/pipeline/mvInputLayout.cpp"
		"renderer/pipeline/mvSamplerState.cpp"

		"renderer/drawables/mvDrawable.cpp"
		"renderer/drawables/mvTexturedQuad.cpp"
		"renderer/drawables/mvSolidSphere.cpp"
		"renderer/drawables/mvPointCloud.cpp"
		"renderer/drawables/mvMesh.cpp"
		"renderer/drawables/mvGizmo.cpp"
		"renderer/drawables/mvSolidModel.cpp"
		"renderer/drawables/mvCube.cpp"
		"renderer/drawables/mvFrustum.cpp"
		
		"renderer/passes/mvPass.cpp"
		"renderer/passes/mvPassResource.cpp"
		"renderer/passes/mvLambertianPass.cpp"
		"renderer/passes/mvSkyboxPass.cpp"
		"renderer/passes/mvOverlayPass.cpp"
		"renderer/passes/mvPointShadowMappingPass.cpp"
		"renderer/passes/mvClearBufferPass.cpp"

		#implot
		"vendor/implot/implot.cpp"
		"vendor/implot/implot_demo.cpp"
		"vendor/implot/implot_items.cpp"

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
		"shaders/common"
		"renderer"
		"geometry"
		"renderer/pipeline"
		"renderer/buffers"
		"renderer/bindables"
		"renderer/drawables"
		"renderer/passes"
		"renderer/render_graph"
		"vendor/implot"

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