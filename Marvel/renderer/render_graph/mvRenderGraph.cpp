#include "mvRenderGraph.h"
#include <vector>
#include <imgui.h>
#include <assert.h>
#include "mvPass.h"
#include "mvLambertianPass.h"
#include "mvSkyboxPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"

namespace Marvel {


	mvRenderGraph::mvRenderGraph(mvGraphics& graphics, const char* skybox)
	{
		m_passes.push_back(std::make_shared<mvLambertianPass>(graphics));
		m_passes.push_back(std::make_shared<mvSkyboxPass>(graphics, skybox));

		mvBufferLayout layout(std::make_shared<mvBufferLayoutEntry>(Struct));
		auto& root = layout.getRoot();
		root->add(Float, "FogStart");
		root->add(Float, "FogRange");
		root->add(Float3, "FogColor");
		root->finalize(0);

		m_bufferData = std::make_unique<mvBuffer>(std::move(layout));
		m_bufferData->getElement("FogColor") = glm::vec3{ 0.05f, 0.05f, 0.05f };
		m_bufferData->getElement("FogRange") = 100.0f;
		m_bufferData->getElement("FogStart") = 10.0f;
		m_buffer = std::make_unique<mvPixelConstantBuffer>(graphics, *root.get(), 3, m_bufferData.get());
	}

	void mvRenderGraph::addJob(mvJob job, size_t target)
	{
		m_passes[target]->addJob(job);
	}

	void mvRenderGraph::execute(mvGraphics& graphics) const
	{
		for (auto& pass : m_passes)
			pass->execute(graphics);
	}

	void mvRenderGraph::reset()
	{
		for (auto& pass : m_passes)
			pass->reset();
	}

	mvPass* mvRenderGraph::getPass(const std::string& name)
	{
		for (auto& pass : m_passes)
		{
			if (pass->getName() == name)
				return pass.get();
		}

		assert(false);
	}

	void mvRenderGraph::bind(mvGraphics& graphics)
	{
		m_buffer->update(graphics, *m_bufferData);
		m_buffer->bind(graphics);
	}

	void mvRenderGraph::show_imgui_window()
	{
		float& FogRange = m_bufferData->getElement("FogRange");
		float& FogStart = m_bufferData->getElement("FogStart");
		glm::vec3& FogColor = m_bufferData->getElement("FogColor");

		if (ImGui::Begin("Render Graph"))
		{
			ImGui::Text("Fog");
			ImGui::SliderFloat("Fog Start", &FogStart,  0.0f, 100.0f, "%.1f");
			ImGui::SliderFloat("Fog Range", &FogRange,  0.0f, 100.0f, "%.1f");
			ImGui::ColorEdit3("Fog Color", &FogColor.x);
		}
		ImGui::End();
	}
}
