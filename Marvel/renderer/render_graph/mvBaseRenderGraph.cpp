#include "mvBaseRenderGraph.h"
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


	mvBaseRenderGraph::mvBaseRenderGraph(mvGraphics& graphics)
		:
		m_depthStencil(std::move(std::make_shared<mvOutputDepthStencil>(graphics))),
		m_renderTarget(graphics.getTarget())
	{
		m_buffer = std::make_unique<mvPixelConstantBuffer>(graphics, 3, &m_globalSettings);
	}

	void mvBaseRenderGraph::execute(mvGraphics& graphics) const
	{
		for (auto& pass : m_passes)
			pass->execute(graphics);
	}

	void mvBaseRenderGraph::reset()
	{
		for (auto& pass : m_passes)
			pass->reset();
	}

	void mvBaseRenderGraph::releaseBuffers()
	{
		for (auto& pass : m_passes)
			pass->releaseBuffers();

		m_renderTarget.reset();
		m_depthStencil.reset();
	}

	void mvBaseRenderGraph::resize(mvGraphics& graphics)
	{
		m_renderTarget = graphics.getTarget();
		m_depthStencil = std::move(std::make_shared<mvOutputDepthStencil>(graphics));
	}

	mvPass* mvBaseRenderGraph::getPass(const std::string& name)
	{
		for (auto& pass : m_passes)
		{
			if (pass->getName() == name)
				return pass.get();
		}

		assert(false);
	}

	void mvBaseRenderGraph::bindMainCamera(mvCamera& camera)
	{
		m_camera = &camera;
	}

	void mvBaseRenderGraph::bind(mvGraphics& graphics)
	{
		m_globalSettings.camPos = m_camera->getPos();
		m_buffer->update(graphics, m_globalSettings);
		m_buffer->bind(graphics);
	}

	void mvBaseRenderGraph::addPass(std::shared_ptr<mvPass> pass)
	{

		// ensure pass doesn't already exists
		for (const auto& existing : m_passes)
		{
			if (existing->getName() == pass->getName())
			{
				assert(false && "pass already exists");
			}
		}

		m_passes.push_back(std::move(pass));
	}

	void mvBaseRenderGraph::show_imgui_window()
	{

		if (ImGui::Begin("Scene"))
		{
			ImGui::Text("Environment");
			ImGui::ColorEdit3("Ambient Color", &m_globalSettings.ambientColor.x);

			ImGui::Text("Fog");
			ImGui::SliderFloat("Fog Start", &m_globalSettings.fogStart, 0.0f, 100.0f, "%.1f");
			ImGui::SliderFloat("Fog Range", &m_globalSettings.fogRange, 0.0f, 100.0f, "%.1f");
			ImGui::ColorEdit3("Fog Color", &m_globalSettings.fogColor.x);
			ImGui::Checkbox("Use Shadows", (bool*)&m_globalSettings.useShadows);
			ImGui::Checkbox("Use Skybox", (bool*)&m_globalSettings.useSkybox);
		}
		ImGui::End();
	}
}