#include "mvPointLight.h"
#include <imgui.h>

namespace Marvel {

	mvPointLight::mvPointLight(mvGraphics& graphics, glm::vec3 pos)
		:
		m_sphere(graphics, 0.25, { 1.0f, 1.0f, 1.0f }, 1)
	{

		mvBufferLayout layout(std::make_shared<mvBufferLayoutEntry>(Struct));
		auto& root = layout.getRoot();
		root->add(Float3, std::string("pos"));
		root->add(Float3, std::string("ambient"));
		root->add(Float3, std::string("diffuseColor"));
		root->add(Float, std::string("diffuseIntensity"));
		root->add(Float, std::string("attConst"));
		root->add(Float, std::string("attLin"));
		root->add(Float, std::string("attQuad"));
		root->finalize(0);

		m_bufferRaw = std::make_unique<mvBuffer>(std::move(layout));
		m_bufferRaw->getElement("pos").set(pos);
		m_bufferRaw->getElement("ambient").set(glm::vec3{0.05f, 0.05f, 0.05f});
		m_bufferRaw->getElement("diffuseColor").set(glm::vec3{1.0f, 1.0f, 1.0f});
		m_bufferRaw->getElement("diffuseIntensity").set(1.0f);
		m_bufferRaw->getElement("attConst").set(1.0f);
		m_bufferRaw->getElement("attLin").set(0.045f);
		m_bufferRaw->getElement("attQuad").set(0.0075f);

		m_buf = std::make_unique<mvPixelConstantBuffer>(graphics, *root.get(), 1, m_bufferRaw.get());
		m_sphere.setPosition(pos.x, pos.y, pos.z);

	}

	void mvPointLight::show_imgui_windows()
	{
		//if (ImGui::Begin("Light"))
		//{

		//	ImGui::Text("Position");
		//	ImGui::SliderFloat("X", &m_bufferRaw->getElement("pos").x, -60.0f, 60.0f, "%.1f");
		//	ImGui::SliderFloat("Y", &m_cbData.pos.y, -60.0f, 60.0f, "%.1f");
		//	ImGui::SliderFloat("Z", &m_cbData.pos.z, -60.0f, 60.0f, "%.1f");


		//	ImGui::Text("Intensity/Color");
		//	ImGui::SliderFloat("Intensity", &m_cbData.diffuseIntensity, 0.01f, 2.0f, "%.2f");
		//	ImGui::ColorEdit3("Diffuse Color", &m_cbData.diffuseColor.x);
		//	ImGui::ColorEdit3("Ambient", &m_cbData.ambient.x);

		//	ImGui::Text("Falloff");
		//	ImGui::SliderFloat("Constant", &m_cbData.attConst, 0.05f, 10.0f, "%.2f");
		//	ImGui::SliderFloat("Linear", &m_cbData.attLin, 0.0001f, 4.0f, "%.4f");
		//	ImGui::SliderFloat("Quadratic", &m_cbData.attQuad, 0.0000001f, 10.0f, "%.7f");
		//}
		//ImGui::End();

		//m_sphere.setPosition(m_cbData.pos.x, m_cbData.pos.y, m_cbData.pos.z);

	}

	mvSolidSphere* mvPointLight::getSphere()
	{
		return &m_sphere;
	}

	void mvPointLight::bind(mvGraphics& graphics, glm::mat4 view)
	{
		m_buf->update(graphics, *m_bufferRaw);
		m_buf->bind(graphics);
	}

	void mvPointLight::setPosition(float x, float y, float z)
	{
		//m_cbData.pos.x = x;
		//m_cbData.pos.y = y;
		//m_cbData.pos.z = z;
	}

}