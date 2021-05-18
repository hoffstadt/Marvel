#include "mvDirectionLight.h"
#include <imgui.h>
#include "mvGraphics.h"
#include "mvMarvelUtils.h"

namespace Marvel {

	mvDirectionLight::mvDirectionLight(mvGraphics& graphics, glm::vec3 dir)
	{

		m_info.viewLightDir.x = dir.x;
		m_info.viewLightDir.y = dir.y;
		m_info.viewLightDir.z = dir.z;
		m_buffer = std::make_unique<mvPixelConstantBuffer>(graphics, 2, &m_info);
	}


	void mvDirectionLight::show_imgui_window()
	{
		static int id = 0;

		if (ImGui::Begin("Directional Light"))
		{

			ImGui::Text("Direction");
			ImGui::SliderFloat("X", &m_info.viewLightDir.x, -1.0f, 1.0f, "%.1f");
			ImGui::SliderFloat("Y", &m_info.viewLightDir.y, -1.0f, 1.0f, "%.1f");
			ImGui::SliderFloat("Z", &m_info.viewLightDir.z, -1.0f, 1.0f, "%.1f");

			ImGui::Text("Intensity/Color");
			ImGui::SliderFloat("Intensity", &m_info.diffuseIntensity, 0.01f, 2.0f, "%.2f");
			ImGui::ColorEdit3("Diffuse Color", &m_info.diffuseColor.x);

		}
		ImGui::End();

	}

	void mvDirectionLight::bind(mvGraphics& graphics, glm::mat4 view)
	{

		glm::vec3 posCopy = m_info.viewLightDir;

		glm::vec4 out = view * glm::vec4(m_info.viewLightDir, 0.0f);
		m_info.viewLightDir.x = out.x;
		m_info.viewLightDir.y = out.y;
		m_info.viewLightDir.z = out.z;

		m_buffer->update(graphics, m_info);
		m_buffer->bind(graphics);

		m_info.viewLightDir = posCopy;

	}

	void mvDirectionLight::setDirection(float x, float y, float z)
	{
		m_info.viewLightDir.x = x;
		m_info.viewLightDir.y = y;
		m_info.viewLightDir.z = z;
	}
}