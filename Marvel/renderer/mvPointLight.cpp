#include "mvPointLight.h"
#include <imgui.h>
#include "mvGraphics.h"

namespace Marvel {

	struct ViewLightPos { glm::vec4 values[mvPointLightManager::MaxLights]; };
	struct Diffuse { glm::vec4 values[mvPointLightManager::MaxLights]; };
	struct DiffuseIntensity{ float values[mvPointLightManager::MaxLights];};
	struct AttConst { float values[mvPointLightManager::MaxLights];};
	struct AttLin { float values[mvPointLightManager::MaxLights];};
	struct AttQuad { float values[mvPointLightManager::MaxLights];};

	mvPointLight::mvPointLight(mvGraphics& graphics, glm::vec3 pos)
		:
		m_mesh(graphics, 0.5f, { 1.0f, 1.0f, 1.0f }, 1)
	{
		m_mesh.setPosition(pos.x, pos.y, pos.z);
		m_camera = std::make_shared<mvCamera>(graphics, pos, 0.0f, 0.0f, 300, 300);
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
		mvBufferLayout layout(std::make_shared<mvBufferLayoutEntry>(Struct));
		auto& root = layout.getRoot();
		root->add(Integer, "LightCount");
		root->add(Float3, std::string("ambient"));

		root->add(Array, "viewLightPos");
		root->getEntry("viewLightPos").set(Float3, MaxLights);

		root->add(Array, "diffuseColor");
		root->getEntry("diffuseColor").set(Float3, MaxLights);

		root->add(Array, "diffuseIntensity");
		root->getEntry("diffuseIntensity").set(Float, MaxLights);

		root->add(Array, "attConst");
		root->getEntry("attConst").set(Float, MaxLights);

		root->add(Array, "attLin");
		root->getEntry("attLin").set(Float, MaxLights);

		root->add(Array, "attQuad");
		root->getEntry("attQuad").set(Float, MaxLights);

		root->finalize(0);

		m_bufferData = std::make_unique<mvBuffer>(std::move(layout));
		m_bufferData->getElement("viewLightPos") = ViewLightPos();
		m_bufferData->getElement("ambient") = glm::vec3{ 0.05f, 0.05f, 0.05f };
		m_bufferData->getElement("diffuseColor") = Diffuse();
		m_bufferData->getElement("diffuseIntensity") = DiffuseIntensity();
		m_bufferData->getElement("attConst") = AttConst();
		m_bufferData->getElement("attLin") = AttLin();
		m_bufferData->getElement("attQuad") = AttQuad();

		ViewLightPos& pos = m_bufferData->getElement("viewLightPos");
		Diffuse& diffuseColor = m_bufferData->getElement("diffuseColor");
		DiffuseIntensity& diffuseIntensity = m_bufferData->getElement("diffuseIntensity");
		AttConst& attConst = m_bufferData->getElement("attConst");
		AttLin& attLin = m_bufferData->getElement("attLin");
		AttQuad& attQuad = m_bufferData->getElement("attQuad");

		glm::vec3& ambient = m_bufferData->getElement("ambient");

		for (int i = 0; i < MaxLights; i++)
		{
			pos.values[i] = { 0.0f, 0.0f, 0.0f, 1.0f };
			diffuseColor.values[i] = { 1.0f, 1.0f, 1.0f, 1.0f };
			diffuseIntensity.values[i] = 1.0f;
			attConst.values[i] = 1.0f;
			attLin.values[i] = 0.045f;
			attQuad.values[i] = 0.0075f;
		}

		m_buffer = std::make_unique<mvPixelConstantBuffer>(graphics, *root.get(), 0, m_bufferData.get());
	}

	void mvPointLightManager::show_imgui_windows()
	{
		ViewLightPos& pos = m_bufferData->getElement("viewLightPos");
		glm::vec3& ambient = m_bufferData->getElement("ambient");
		Diffuse& diffuseColor = m_bufferData->getElement("diffuseColor");
		DiffuseIntensity& diffuseIntensity = m_bufferData->getElement("diffuseIntensity");
		AttConst& attConst = m_bufferData->getElement("attConst");
		AttLin& attLin = m_bufferData->getElement("attLin");
		AttQuad& attQuad = m_bufferData->getElement("attQuad");

		static int id = 0;

		if (ImGui::Begin("Point Lights"))
		{
			
			ImGui::SliderInt("Light", &id, 0, m_lights.size()-1);

			ImGui::Text("Position");
			ImGui::SliderFloat("X", &pos.values[id].x, -60.0f, 60.0f, "%.1f");
			ImGui::SliderFloat("Y", &pos.values[id].y, -60.0f, 60.0f, "%.1f");
			ImGui::SliderFloat("Z", &pos.values[id].z, -60.0f, 60.0f, "%.1f");

			ImGui::Text("Intensity/Color");
			ImGui::SliderFloat("Intensity", &diffuseIntensity.values[id], 0.01f, 2.0f, "%.2f");
			ImGui::ColorEdit3("Diffuse Color", &diffuseColor.values[id].x);
			ImGui::ColorEdit3("Ambient", &ambient.x);

			ImGui::Text("Falloff");
			ImGui::SliderFloat("Constant", &attConst.values[id], 0.05f, 10.0f, "%.2f");
			ImGui::SliderFloat("Linear", &attLin.values[id], 0.0001f, 4.0f, "%.4f");
			ImGui::SliderFloat("Quadratic", &attQuad.values[id], 0.0000001f, 10.0f, "%.7f");
		}
		ImGui::End();

		m_lights[id].m_mesh.setPosition(pos.values[id].x, pos.values[id].y, pos.values[id].z);
		m_lights[id].m_camera->setPos(pos.values[id].x, pos.values[id].y, pos.values[id].z);
	}

	void mvPointLightManager::bind(mvGraphics& graphics, glm::mat4 view)
	{

		m_bufferData->getElement("LightCount") = (int)m_lights.size();
		ViewLightPos& pos = m_bufferData->getElement("viewLightPos");
		ViewLightPos posCopy = pos;

		for (int i = 0; i<m_lights.size(); i++)
		{
			glm::vec4 out = view * pos.values[i];
			pos.values[i].x = out.x;
			pos.values[i].y = out.y;
			pos.values[i].z = out.z;
		}

		m_buffer->update(graphics, *m_bufferData);
		m_buffer->bind(graphics);

		pos = posCopy;

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
		ViewLightPos& pos = m_bufferData->getElement("viewLightPos");
		pos.values[i].x = x;
		pos.values[i].y = y;
		pos.values[i].z = z;
	}

	mvPointLight& mvPointLightManager::addLight(mvGraphics& graphics, glm::vec3 pos)
	{

		m_lights.emplace_back(graphics, pos);

		ViewLightPos& opos = m_bufferData->getElement("viewLightPos");
		opos.values[m_lights.size()-1].x = pos.x;
		opos.values[m_lights.size()-1].y = pos.y;
		opos.values[m_lights.size()-1].z = pos.z;

		return m_lights.back();
	}

	mvPointLight& mvPointLightManager::getLight(int i)
	{
		return m_lights[i];
	}
}