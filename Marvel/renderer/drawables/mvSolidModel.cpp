#include "mvSolidModel.h"
#include <imgui.h>
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "Sphere.h"
#include "mvTechnique.h"

namespace Marvel {

	mvSolidModel::mvSolidModel(mvGraphics& graphics, const std::string& name, std::vector<float> vertices, std::vector<unsigned int> indices, glm::vec3 color)
	{

		// create topology
		m_topology = std::make_shared<mvTopology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// create vertex layout
		mvVertexLayout vl;
		vl.append(ElementType::Position3D);
		vl.append(ElementType::Normal);

		// create vertex buffer
		m_vertexBuffer = std::make_shared<mvVertexBuffer>(graphics, name, vertices, vl, true);

		// create index buffer
		m_indexBuffer = mvBufferRegistry::Request<mvIndexBuffer>(graphics, name, indices, false);

		mvStep step("lambertian");

		mvBufferLayout layout(std::make_shared<mvBufferLayoutEntry>(Struct));
		auto& root = layout.getRoot();
		root->add(Float3, std::string("materialColor"));
		root->finalize(0);

		std::unique_ptr<mvDynamicBuffer> bufferRaw = std::make_unique<mvDynamicBuffer>(std::move(layout));

		bufferRaw->getElement("materialColor").setIfExists(color);

		std::shared_ptr<mvPixelConstantBuffer> buf = std::make_shared<mvPixelConstantBuffer>(graphics, *root.get(), 1, bufferRaw.get());

		step.addBuffer(buf);

		// create vertex shader
		auto vshader = mvBindableRegistry::Request<mvVertexShader>(graphics, graphics.getShaderRoot() + "Phong_VS.hlsl");
		step.addBindable(vshader);
		step.addBindable(mvBindableRegistry::Request<mvInputLayout>(graphics, vl, *vshader));
		step.addBindable(mvBindableRegistry::Request<mvPixelShader>(graphics, graphics.getShaderRoot() + "Phong_PS.hlsl"));
		step.addBindable(std::make_shared<mvNullGeometryShader>(graphics));
		step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));

		mvTechnique technique;
		technique.addStep(step);
		addTechnique(technique);

	}

	glm::mat4 mvSolidModel::getTransform() const
	{
		return glm::translate(glm::vec3{ m_x, m_y, m_z }) *
			glm::rotate(m_zangle, glm::vec3{ 0.0f, 0.0f, 1.0f }) *
			glm::rotate(m_yangle, glm::vec3{ 0.0f, 1.0f, 0.0f }) *
			glm::rotate(m_xangle, glm::vec3{ 1.0f, 0.0f, 0.0f });
	}

	void mvSolidModel::show_imgui_windows(const char* name)
	{
		if (ImGui::Begin(name))
		{
			ImGui::SliderFloat("X-Pos", &m_x, -50.0f, 50.0f);
			ImGui::SliderFloat("Y-Pos", &m_y, -50.0f, 50.0f);
			ImGui::SliderFloat("Z-Pos", &m_z, -50.0f, 50.0f);
			ImGui::SliderFloat("X-Angle", &m_xangle, -50.0f, 50.0f);
			ImGui::SliderFloat("Y-Angle", &m_yangle, -50.0f, 50.0f);
			ImGui::SliderFloat("Z-Angle", &m_zangle, -50.0f, 50.0f);
		}
		ImGui::End();
	}

	void mvSolidModel::setPosition(float x, float y, float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	void mvSolidModel::setRotation(float x, float y, float z)
	{
		m_xangle = x;
		m_yangle = y;
		m_zangle = z;
	}

}