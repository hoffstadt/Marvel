#include "mvRenderGraph.h"
#include <vector>
#include <imgui.h>
#include <assert.h>
#include "mvPass.h"
#include "mvLambertianPass.h"
#include "mvSkyboxPass.h"
#include "mvPointShadowMappingPass.h"
#include "mvClearBufferPass.h"
#include "mvOverlayPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCamera.h"
#include "mvDirectionalShadowMappingPass.h"

namespace Marvel {


	mvRenderGraph::mvRenderGraph(mvGraphics& graphics, const char* skybox)
		:
		mvBaseRenderGraph(graphics)
	{
		auto clear_target = std::make_unique<mvClearBufferPass>("clear_target");
		clear_target->m_renderTarget = m_renderTarget;
		addPass(std::move(clear_target));

		auto clear_depth = std::make_unique<mvClearBufferPass>("clear_depth");
		clear_depth->m_depthStencil = m_depthStencil;
		addPass(std::move(clear_depth));

		auto point_shadow = std::make_unique<mvPointShadowMappingPass>(graphics, "shadow", 3);
		std::shared_ptr<mvCubeDepthTexture> depthCube = point_shadow->m_depthCube;
		addPass(std::move(point_shadow));

		auto direction_shadow = std::make_unique<mvDirectionalShadowMappingPass>(graphics, "directional_shadow", 4);
		std::shared_ptr<mvDepthTexture> depthTexture = direction_shadow->m_depthTexture;
		addPass(std::move(direction_shadow));

		auto lambertian = std::make_unique<mvLambertianPass>(graphics, "lambertian");
		lambertian->m_depthStencil = m_depthStencil;
		lambertian->m_renderTarget = m_renderTarget;
		lambertian->m_depthCube = depthCube;
		lambertian->m_depthTexture = depthTexture;
		addPass(std::move(lambertian));

		auto skybox_pass = std::make_unique<mvSkyboxPass>(graphics, "skybox", skybox);
		skybox_pass->m_depthStencil = m_depthStencil;
		skybox_pass->m_renderTarget = m_renderTarget;
		addPass(std::move(skybox_pass));

		auto overlay = std::make_unique<mvOverlayPass>(graphics, "overlay");
		overlay->m_renderTarget = m_renderTarget;
		addPass(std::move(overlay));

		bake();

		m_buffer = std::make_unique<mvPixelConstantBuffer>(graphics, 3, &m_globalSettings);
	}

	void mvRenderGraph::bindMainCamera(mvCamera& camera)
	{
		m_camera = &camera;
		static_cast<mvLambertianPass*>(getPass("lambertian"))->bindMainCamera(camera);
		static_cast<mvOverlayPass*>(getPass("overlay"))->bindMainCamera(camera);
	}

	void mvRenderGraph::bind(mvGraphics& graphics)
	{
		m_globalSettings.camPos = m_camera->getPos();
		m_buffer->update(graphics, m_globalSettings);
		m_buffer->bind(graphics);
	}

	void mvRenderGraph::show_imgui_window()
	{

		if (ImGui::Begin("Scene"))
		{
			ImGui::Text("Environment");
			ImGui::ColorEdit3("Ambient Color", &m_globalSettings.ambientColor.x);

			ImGui::Text("Fog");
			ImGui::SliderFloat("Fog Start", &m_globalSettings.fogStart,  0.0f, 100.0f, "%.1f");
			ImGui::SliderFloat("Fog Range", &m_globalSettings.fogRange,  0.0f, 100.0f, "%.1f");
			ImGui::ColorEdit3("Fog Color", &m_globalSettings.fogColor.x);
			ImGui::Checkbox("Use Shadows", (bool*)&m_globalSettings.useShadows);
			ImGui::Checkbox("Use Skybox", (bool*)&m_globalSettings.useSkybox);
		}
		ImGui::End();
	}

}
