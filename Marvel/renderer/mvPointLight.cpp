#include "mvPointLight.h"
#include <imgui.h>
#include "mvGraphics.h"

namespace Marvel {

	mvPointLight::mvPointLight(mvGraphics& graphics, glm::vec3 pos)
		:
		m_mesh(graphics, 0.5f, { 1.0f, 1.0f, 1.0f }, 1)
	{

		mvBufferLayout layout(std::make_shared<mvBufferLayoutEntry>(Struct));
		auto& root = layout.getRoot();
		root->add(Float3, std::string("viewLightPos"));
		root->add(Float3, std::string("ambient"));
		root->add(Float3, std::string("diffuseColor"));
		root->add(Float, std::string("diffuseIntensity"));
		root->add(Float, std::string("attConst"));
		root->add(Float, std::string("attLin"));
		root->add(Float, std::string("attQuad"));
		root->finalize(0);

		m_bufferData = std::make_unique<mvBuffer>(std::move(layout));
		m_bufferData->getElement("viewLightPos") = pos;
		m_bufferData->getElement("ambient") = glm::vec3{0.05f, 0.05f, 0.05f};
		m_bufferData->getElement("diffuseColor") = glm::vec3{1.0f, 1.0f, 1.0f};
		m_bufferData->getElement("diffuseIntensity") = 1.0f;
		m_bufferData->getElement("attConst") = 1.0f;
		m_bufferData->getElement("attLin") = 0.045f;
		m_bufferData->getElement("attQuad") = 0.0075f;

		m_buffer = std::make_unique<mvPixelConstantBuffer>(graphics, *root.get(), 0, m_bufferData.get());
		m_mesh.setPosition(pos.x, pos.y, pos.z);

	}

	void mvPointLight::show_imgui_windows()
	{

		glm::vec3& pos = m_bufferData->getElement("viewLightPos");
		glm::vec3& ambient = m_bufferData->getElement("ambient");
		glm::vec3& diffuseColor = m_bufferData->getElement("diffuseColor");
		float& diffuseIntensity = m_bufferData->getElement("diffuseIntensity");
		float& attConst = m_bufferData->getElement("attConst");
		float& attLin = m_bufferData->getElement("attLin");
		float& attQuad = m_bufferData->getElement("attQuad");

		if (ImGui::Begin("Light"))
		{

			ImGui::Text("Position");
			ImGui::SliderFloat("X", &pos.x, -60.0f, 60.0f, "%.1f");
			ImGui::SliderFloat("Y", &pos.y, -60.0f, 60.0f, "%.1f");
			ImGui::SliderFloat("Z", &pos.z, -60.0f, 60.0f, "%.1f");

			ImGui::Text("Intensity/Color");
			ImGui::SliderFloat("Intensity", &diffuseIntensity, 0.01f, 2.0f, "%.2f");
			ImGui::ColorEdit3("Diffuse Color", &diffuseColor.x);
			ImGui::ColorEdit3("Ambient", &ambient.x);

			ImGui::Text("Falloff");
			ImGui::SliderFloat("Constant", &attConst, 0.05f, 10.0f, "%.2f");
			ImGui::SliderFloat("Linear", &attLin, 0.0001f, 4.0f, "%.4f");
			ImGui::SliderFloat("Quadratic", &attQuad, 0.0000001f, 10.0f, "%.7f");
		}
		ImGui::End();

		m_mesh.setPosition(pos.x, pos.y, pos.z);

	}

	void mvPointLight::submit(mvRenderGraph& graph)
	{
		m_mesh.submit(graph);
	}

	void mvPointLight::bind(mvGraphics& graphics, glm::mat4 view)
	{
		glm::vec3& pos = m_bufferData->getElement("viewLightPos");
		glm::vec3 posCopy = pos;
		glm::vec4 out = view * glm::vec4(pos, 1.0f);
		pos.x = out.x;
		pos.y = out.y;
		pos.z = out.z;

		m_buffer->update(graphics, *m_bufferData);
		m_buffer->bind(graphics);

		pos = posCopy;
	}

	void mvPointLight::setPosition(float x, float y, float z)
	{
		glm::vec3& pos = m_bufferData->getElement("viewLightPos");
		pos.x = x;
		pos.y = y;
		pos.z = z;
	}

	void mvPointLight::linkTechniques(mvRenderGraph& graph)
	{
		m_mesh.linkTechniques(graph);
	}

}