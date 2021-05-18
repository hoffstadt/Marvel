#include "mvPointLight.h"
#include <imgui.h>
#include "mvGraphics.h"

namespace Marvel {

	mvPointLight::mvPointLight(mvGraphics& graphics, const std::string& name, glm::vec3 pos)
		:
		m_mesh(graphics, name,  0.5f, { 1.0f, 1.0f, 1.0f }, 1)
	{
		m_mesh.setPosition(pos.x, pos.y, pos.z);
		m_camera = std::make_shared<mvCamera>(graphics, name, pos, 0.0f, 0.0f, 1000, 1000, 0.5, 100.0f);
	}

	void mvPointLight::submit(mvRenderGraph& graph)
	{
		m_mesh.submit(graph);
	}

	void mvPointLight::linkTechniques(mvRenderGraph& graph)
	{
		m_mesh.linkTechniques(graph);
	}

	std::shared_ptr<mvCamera> mvPointLight::getCamera() const
	{
		return m_camera;
	}

	mvPointLightManager::mvPointLightManager(mvGraphics& graphics)
	{
		m_buffer = std::make_unique<mvPixelConstantBuffer>(graphics, 0, &m_info);
	}

	void mvPointLightManager::show_imgui_windows()
	{

		static int id = 0;

		if (ImGui::Begin("Point Lights"))
		{
			
			ImGui::SliderInt("Light", &id, 0, m_lights.size()-1);

			ImGui::Text("Position");
			ImGui::SliderFloat("X", &m_info.viewLightPos[id].x, -60.0f, 60.0f, "%.1f");
			ImGui::SliderFloat("Y", &m_info.viewLightPos[id].y, -60.0f, 60.0f, "%.1f");
			ImGui::SliderFloat("Z", &m_info.viewLightPos[id].z, -60.0f, 60.0f, "%.1f");

			ImGui::Text("Intensity/Color");
			ImGui::SliderFloat("Intensity", &m_info.diffuseIntensity[id*4], 0.01f, 2.0f, "%.2f");
			ImGui::ColorEdit3("Diffuse Color", &m_info.diffuseColor[id].x);

			ImGui::Text("Falloff");
			ImGui::SliderFloat("Constant", &m_info.attConst[id * 4], 0.05f, 10.0f, "%.2f");
			ImGui::SliderFloat("Linear", &m_info.attLin[id * 4], 0.0001f, 4.0f, "%.4f");
			ImGui::SliderFloat("Quadratic", &m_info.attQuad[id * 4], 0.0000001f, 10.0f, "%.7f");
		}
		ImGui::End();

		m_lights[id].m_mesh.setPosition(m_info.viewLightPos[id].x, m_info.viewLightPos[id].y, m_info.viewLightPos[id].z);
		m_lights[id].m_camera->setPos(m_info.viewLightPos[id].x, m_info.viewLightPos[id].y, m_info.viewLightPos[id].z);
	}

	void mvPointLightManager::bind(mvGraphics& graphics, glm::mat4 view)
	{
		
		glm::vec4 posCopy[3];
		posCopy[0] = m_info.viewLightPos[0];
		posCopy[1] = m_info.viewLightPos[1];
		posCopy[2] = m_info.viewLightPos[2];

		for (int i = 0; i<m_lights.size(); i++)
		{
			glm::vec4 out = view * m_info.viewLightPos[i];
			m_info.viewLightPos[i].x = out.x;
			m_info.viewLightPos[i].y = out.y;
			m_info.viewLightPos[i].z = out.z;
		}

		m_info.lightCount = m_lights.size();
		m_buffer->update(graphics, m_info);
		m_buffer->bind(graphics);

		m_info.viewLightPos[0] = posCopy[0];
		m_info.viewLightPos[1] = posCopy[1];
		m_info.viewLightPos[2] = posCopy[2];

	}

	void mvPointLightManager::linkTechniques(mvRenderGraph& graph)
	{
		for (auto& light : m_lights)
			light.linkTechniques(graph);
	}

	void mvPointLightManager::submit(mvRenderGraph& graph)
	{
		for (auto& light : m_lights)
			light.submit(graph);
	}

	void mvPointLightManager::setPosition(int i, float x, float y, float z)
	{
		m_info.viewLightPos[i].x = x;
		m_info.viewLightPos[i].y = y;
		m_info.viewLightPos[i].z = z;
	}

	mvPointLight& mvPointLightManager::addLight(mvGraphics& graphics, const std::string& name, glm::vec3 pos)
	{

		m_lights.emplace_back(graphics, name, pos);
		m_info.viewLightPos[m_lights.size()-1].x = pos.x;
		m_info.viewLightPos[m_lights.size()-1].y = pos.y;
		m_info.viewLightPos[m_lights.size()-1].z = pos.z;

		return m_lights.back();
	}

	mvPointLight& mvPointLightManager::getLight(int i)
	{
		return m_lights[i];
	}
}