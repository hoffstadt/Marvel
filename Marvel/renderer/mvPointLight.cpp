#include "mvPointLight.h"
#include <imgui.h>
#include "mvGraphics.h"

namespace Marvel {

	mvPointLight::mvPointLight(mvGraphics& graphics, const std::string& name, glm::vec3 pos)
		:
		m_mesh(graphics, name,  0.5f, { 1.0f, 1.0f, 1.0f }, 1)
	{
		m_info.viewLightPos = glm::vec4{ pos.x, pos.y, pos.z, 1.0f };
		m_mesh.setPosition(pos.x, pos.y, pos.z);
		m_camera = std::make_shared<mvCamera>(graphics, name, pos, 0.0f, 0.0f, 1000, 1000, 0.5, 100.0f);
		m_buffer = std::make_unique<mvPixelConstantBuffer>(graphics, 0, &m_info);
	}

	void mvPointLight::submit(mvBaseRenderGraph& graph)
	{
		m_mesh.submit(graph);
	}

	void mvPointLight::linkSteps(mvBaseRenderGraph& graph)
	{
		m_mesh.linkSteps(graph);
	}

	std::shared_ptr<mvCamera> mvPointLight::getCamera() const
	{
		return m_camera;
	}

	void mvPointLight::show_imgui_window()
	{

		if (ImGui::Begin("Point Light"))
		{

			ImGui::Text("Position");
			ImGui::SliderFloat("X", &m_info.viewLightPos.x, -60.0f, 60.0f, "%.1f");
			ImGui::SliderFloat("Y", &m_info.viewLightPos.y, -60.0f, 60.0f, "%.1f");
			ImGui::SliderFloat("Z", &m_info.viewLightPos.z, -60.0f, 60.0f, "%.1f");

			ImGui::Text("Intensity/Color");
			ImGui::SliderFloat("Intensity", &m_info.diffuseIntensity, 0.01f, 2.0f, "%.2f");
			ImGui::ColorEdit3("Diffuse Color", &m_info.diffuseColor.x);

			ImGui::Text("Falloff");
			ImGui::SliderFloat("Constant", &m_info.attConst, 0.05f, 10.0f, "%.2f");
			ImGui::SliderFloat("Linear", &m_info.attLin, 0.0001f, 4.0f, "%.4f");
			ImGui::SliderFloat("Quadratic", &m_info.attQuad, 0.0000001f, 10.0f, "%.7f");
		}
		ImGui::End();

		m_mesh.setPosition(m_info.viewLightPos.x, m_info.viewLightPos.y, m_info.viewLightPos.z);
		m_camera->setPos(m_info.viewLightPos.x, m_info.viewLightPos.y, m_info.viewLightPos.z);
	}

	void mvPointLight::bind(mvGraphics& graphics, glm::mat4 view)
	{
		
		glm::vec4 posCopy = m_info.viewLightPos;


		glm::vec3 out = view * m_info.viewLightPos;
		m_info.viewLightPos.x = out.x;
		m_info.viewLightPos.y = out.y;
		m_info.viewLightPos.z = out.z;

		m_buffer->update(graphics, m_info);
		m_buffer->bind(graphics);

		m_info.viewLightPos = posCopy;

	}

	void mvPointLight::setPosition(float x, float y, float z)
	{
		m_info.viewLightPos.x = x;
		m_info.viewLightPos.y = y;
		m_info.viewLightPos.z = z;
	}

}