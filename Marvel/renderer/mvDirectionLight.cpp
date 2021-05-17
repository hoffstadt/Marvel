#include "mvDirectionLight.h"
#include <imgui.h>
#include "mvGraphics.h"
#include "mvMarvelUtils.h"

namespace Marvel {

	struct ViewLightDir { glm::vec4 values[mvDirectionLightManager::MaxLights]; };
	struct Diffuse { glm::vec4 values[mvDirectionLightManager::MaxLights]; };
	struct DiffuseIntensity{ float values[mvDirectionLightManager::MaxLights];};
	struct AttConst { float values[mvDirectionLightManager::MaxLights];};
	struct AttLin { float values[mvDirectionLightManager::MaxLights];};
	struct AttQuad { float values[mvDirectionLightManager::MaxLights];};

	mvDirectionLight::mvDirectionLight(mvGraphics& graphics, glm::vec3 dir)
	{
		//m_camera = std::make_shared<mvCamera>(graphics, pos, 0.0f, 0.0f, 300, 300);
	}


	mvDirectionLightManager::mvDirectionLightManager(mvGraphics& graphics)
	{
		mvBufferLayout layout(std::make_shared<mvBufferLayoutEntry>(Struct));
		auto& root = layout.getRoot();
		root->add(Integer, "dLightCount");
		root->add(Float3, std::string("dambient"));

		root->add(Array, "viewLightDir");
		root->getEntry("viewLightDir").set(Float3, MaxLights);

		root->add(Array, "ddiffuseColor");
		root->getEntry("ddiffuseColor").set(Float3, MaxLights);

		root->add(Array, "ddiffuseIntensity");
		root->getEntry("ddiffuseIntensity").set(Float, MaxLights);

		root->finalize(0);

		m_bufferData = std::make_unique<mvDynamicBuffer>(std::move(layout));
		m_bufferData->getElement("viewLightDir") = ViewLightDir();
		m_bufferData->getElement("dambient") = glm::vec3{ 0.05f, 0.05f, 0.05f };
		m_bufferData->getElement("ddiffuseColor") = Diffuse();
		m_bufferData->getElement("ddiffuseIntensity") = DiffuseIntensity();

		ViewLightDir& pos = m_bufferData->getElement("viewLightDir");
		Diffuse& diffuseColor = m_bufferData->getElement("ddiffuseColor");
		DiffuseIntensity& diffuseIntensity = m_bufferData->getElement("ddiffuseIntensity");

		glm::vec3& ambient = m_bufferData->getElement("dambient");

		for (int i = 0; i < MaxLights; i++)
		{
			pos.values[i] = { 0.0f, 0.0f, 0.0f, 0.0f };
			diffuseColor.values[i] = { 1.0f, 1.0f, 1.0f, 1.0f };
			diffuseIntensity.values[i] = 1.0f;
		}

		m_buffer = std::make_unique<mvPixelConstantBuffer>(graphics, *root.get(), 2, m_bufferData.get());
	}

	void mvDirectionLightManager::show_imgui_windows()
	{
		ViewLightDir& pos = m_bufferData->getElement("viewLightDir");
		glm::vec3& ambient = m_bufferData->getElement("dambient");
		Diffuse& diffuseColor = m_bufferData->getElement("ddiffuseColor");
		DiffuseIntensity& diffuseIntensity = m_bufferData->getElement("ddiffuseIntensity");

		static int id = 0;

		if (ImGui::Begin("Directional Lights"))
		{
			
			ImGui::SliderInt("Light", &id, 0, m_lights.size()-1);

			ImGui::Text("Direction");
			ImGui::SliderFloat("X", &pos.values[id].x, -1.0f, 1.0f, "%.1f");
			ImGui::SliderFloat("Y", &pos.values[id].y, -1.0f, 1.0f, "%.1f");
			ImGui::SliderFloat("Z", &pos.values[id].z, -1.0f, 1.0f, "%.1f");

			ImGui::Text("Intensity/Color");
			ImGui::SliderFloat("Intensity", &diffuseIntensity.values[id], 0.01f, 2.0f, "%.2f");
			ImGui::ColorEdit3("Diffuse Color", &diffuseColor.values[id].x);
			ImGui::ColorEdit3("Ambient", &ambient.x);

		}
		ImGui::End();

	}

	void mvDirectionLightManager::bind(mvGraphics& graphics, glm::mat4 view)
	{

		m_bufferData->getElement("dLightCount") = (int)m_lights.size();
		ViewLightDir& pos = m_bufferData->getElement("viewLightDir");
		ViewLightDir posCopy = pos;

		for (int i = 0; i<m_lights.size(); i++)
		{
			glm::vec4 out = view * pos.values[i];
			//glm::vec4 out = pos.values[i];
			pos.values[i].x = out.x;
			pos.values[i].y = out.y;
			pos.values[i].z = out.z;
		}

		m_buffer->update(graphics, *m_bufferData);
		m_buffer->bind(graphics);

		pos = posCopy;

	}

	void mvDirectionLightManager::setDirection(int i, float x, float y, float z)
	{
		ViewLightDir& pos = m_bufferData->getElement("viewLightDir");
		pos.values[i].x = x;
		pos.values[i].y = y;
		pos.values[i].z = z;
	}

	mvDirectionLight& mvDirectionLightManager::addLight(mvGraphics& graphics, glm::vec3 dir)
	{

		m_lights.emplace_back(graphics, dir);

		ViewLightDir& opos = m_bufferData->getElement("viewLightDir");
		opos.values[m_lights.size()-1].x = dir.x;
		opos.values[m_lights.size()-1].y = dir.y;
		opos.values[m_lights.size()-1].z = dir.z;

		return m_lights.back();
	}

	mvDirectionLight& mvDirectionLightManager::getLight(int i)
	{
		return m_lights[i];
	}
}