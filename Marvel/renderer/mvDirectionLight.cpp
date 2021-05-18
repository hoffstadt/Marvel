#include "mvDirectionLight.h"
#include <imgui.h>
#include "mvGraphics.h"
#include "mvMarvelUtils.h"

namespace Marvel {

	mvDirectionLight::mvDirectionLight(mvGraphics& graphics, glm::vec3 dir)
	{
		//m_camera = std::make_shared<mvCamera>(graphics, pos, 0.0f, 0.0f, 300, 300);
	}


	mvDirectionLightManager::mvDirectionLightManager(mvGraphics& graphics)
	{
		m_buffer = std::make_unique<mvPixelConstantBuffer>(graphics, 2, &m_info);
	}

	void mvDirectionLightManager::show_imgui_windows()
	{
		static int id = 0;

		if (ImGui::Begin("Directional Lights"))
		{
			
			ImGui::SliderInt("Light", &id, 0, m_lights.size()-1);

			ImGui::Text("Direction");
			ImGui::SliderFloat("X", &m_info.viewLightDir[id].x, -1.0f, 1.0f, "%.1f");
			ImGui::SliderFloat("Y", &m_info.viewLightDir[id].y, -1.0f, 1.0f, "%.1f");
			ImGui::SliderFloat("Z", &m_info.viewLightDir[id].z, -1.0f, 1.0f, "%.1f");

			ImGui::Text("Intensity/Color");
			ImGui::SliderFloat("Intensity", &m_info.diffuseIntensity[id*4], 0.01f, 2.0f, "%.2f");
			ImGui::ColorEdit3("Diffuse Color", &m_info.diffuseColor[id*4].x);

		}
		ImGui::End();

	}

	void mvDirectionLightManager::bind(mvGraphics& graphics, glm::mat4 view)
	{

		glm::vec4 posCopy[2];
		posCopy[0] = m_info.viewLightDir[0];
		posCopy[1] = m_info.viewLightDir[1];

		for (int i = 0; i<m_lights.size(); i++)
		{
			glm::vec4 out = view * m_info.viewLightDir[i];
			m_info.viewLightDir[i].x = out.x;
			m_info.viewLightDir[i].y = out.y;
			m_info.viewLightDir[i].z = out.z;
		}

		m_info.lightCount = m_lights.size();
		m_buffer->update(graphics, m_info);
		m_buffer->bind(graphics);

		m_info.viewLightDir[0] = posCopy[0];
		m_info.viewLightDir[1] = posCopy[1];

	}

	void mvDirectionLightManager::setDirection(int i, float x, float y, float z)
	{
		m_info.viewLightDir[i].x = x;
		m_info.viewLightDir[i].y = y;
		m_info.viewLightDir[i].z = z;
	}

	mvDirectionLight& mvDirectionLightManager::addLight(mvGraphics& graphics, glm::vec3 dir)
	{

		m_lights.emplace_back(graphics, dir);

		m_info.viewLightDir[m_lights.size()-1].x = dir.x;
		m_info.viewLightDir[m_lights.size()-1].y = dir.y;
		m_info.viewLightDir[m_lights.size()-1].z = dir.z;

		return m_lights.back();
	}

	mvDirectionLight& mvDirectionLightManager::getLight(int i)
	{
		return m_lights[i];
	}
}