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

		requestGlobalResource(std::make_unique<mvBufferPassResource<mvRenderTarget>>("backbuffer", m_renderTarget));
		requestGlobalResource(std::make_unique<mvBufferPassResource<mvDepthStencil>>("masterdepth", m_depthStencil));

		issueGlobalProduct(std::make_unique<mvBufferPassProduct<mvRenderTarget>>("backbuffer", m_renderTarget));
		issueGlobalProduct(std::make_unique<mvBufferPassProduct<mvDepthStencil>>("masterdepth", m_depthStencil));

		{
			auto pass = std::make_unique<mvClearBufferPass>("clear_target");
			pass->linkResourceToProduct("buffer", "global", "backbuffer");
			addPass(std::move(pass));
		}

		{
			auto pass = std::make_unique<mvClearBufferPass>("clear_depth");
			pass->linkResourceToProduct("buffer", "global", "masterdepth");
			addPass(std::move(pass));
		}

		{
			auto pass = std::make_unique<mvPointShadowMappingPass>(graphics, "shadow", 3);
			addPass(std::move(pass));
		}

		{
			auto pass = std::make_unique<mvDirectionalShadowMappingPass>(graphics, "directional_shadow", 4);
			addPass(std::move(pass));
		}

		{
			auto pass = std::make_unique<mvLambertianPass>(graphics, "lambertian");
			pass->linkResourceToProduct("map", "shadow", "map");
			pass->linkResourceToProduct("directional_map", "directional_shadow", "map");
			pass->linkResourceToProduct("render_target", "clear_target", "buffer");
			pass->linkResourceToProduct("depth_stencil", "clear_depth", "buffer");
			addPass(std::move(pass));
		}

		{
			auto pass = std::make_unique<mvSkyboxPass>(graphics, "skybox", skybox);
			pass->linkResourceToProduct("render_target", "lambertian", "render_target");
			pass->linkResourceToProduct("depth_stencil", "lambertian", "depth_stencil");
			addPass(std::move(pass));
		}

		{
			auto pass = std::make_unique<mvOverlayPass>(graphics, "overlay");
			pass->linkResourceToProduct("render_target", "skybox", "render_target");
			addPass(std::move(pass));
		}

		// ensure something outputs the backbuffer
		linkGlobalResourceToProduct("backbuffer", "overlay", "render_target");
		linkGlobalResourceToProduct("masterdepth", "skybox", "depth_stencil");

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
