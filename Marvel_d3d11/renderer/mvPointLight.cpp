#include "mvPointLight.h"
#include <imgui.h>

namespace Marvel {

	mvPointLight::mvPointLight(mvGraphics& graphics, glm::vec3 pos)
		:
		m_sphere(graphics, 0.25, { 1.0f, 1.0f, 1.0f }, 1),
		m_cbuf(graphics, 1u)
	{
		m_cbData = {
			pos,
			{ 0.05f,0.05f,0.05f },
			{ 1.0f,1.0f,1.0f },
			1.0f,
			1.0f,
			0.045f,
			0.0075f,
				};
	}

	void mvPointLight::show_imgui_windows()
	{
		if (ImGui::Begin("Light"))
		{

			ImGui::Text("Position");
			ImGui::SliderFloat("X", &m_cbData.pos.x, -60.0f, 60.0f, "%.1f");
			ImGui::SliderFloat("Y", &m_cbData.pos.y, -60.0f, 60.0f, "%.1f");
			ImGui::SliderFloat("Z", &m_cbData.pos.z, -60.0f, 60.0f, "%.1f");


			ImGui::Text("Intensity/Color");
			ImGui::SliderFloat("Intensity", &m_cbData.diffuseIntensity, 0.01f, 2.0f, "%.2f");
			ImGui::ColorEdit3("Diffuse Color", &m_cbData.diffuseColor.x);
			ImGui::ColorEdit3("Ambient", &m_cbData.ambient.x);

			ImGui::Text("Falloff");
			ImGui::SliderFloat("Constant", &m_cbData.attConst, 0.05f, 10.0f, "%.2f");
			ImGui::SliderFloat("Linear", &m_cbData.attLin, 0.0001f, 4.0f, "%.4f");
			ImGui::SliderFloat("Quadratic", &m_cbData.attQuad, 0.0000001f, 10.0f, "%.7f");
		}
		ImGui::End();

		m_sphere.setPosition(m_cbData.pos.x, m_cbData.pos.y, m_cbData.pos.z);

	}

	mvSolidSphere* mvPointLight::getSphere()
	{
		return &m_sphere;
	}

	void mvPointLight::bind(mvGraphics& graphics, glm::mat4 view)
	{
		auto dataCopy = m_cbData;
		const glm::vec4 pos = glm::vec4(m_cbData.pos, 1.0f);
		dataCopy.pos = view * pos;
		m_cbuf.update(graphics, dataCopy);
		m_cbuf.bind(graphics);
	}

	void mvPointLight::setPosition(float x, float y, float z)
	{
		m_cbData.pos.x = x;
		m_cbData.pos.y = y;
		m_cbData.pos.z = z;
	}

}