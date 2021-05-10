#include "mvTexturedQuad.h"
#include "mvGraphics.h"
#include "mvTechnique.h"
#include "mvCommonBindables.h"
#include <imgui.h>

namespace Marvel {

	mvTexturedQuad::mvTexturedQuad(mvGraphics& graphics, const std::string& name, const std::string& path)
	{

		// create topology
		m_topology = std::make_shared<mvTopology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		// create vertex layout
		mvVertexLayout vl;
		vl.append(ElementType::Position3D);
		vl.append(ElementType::Normal);
		vl.append(ElementType::Texture2D);

		// create vertex buffer
		m_vertexBuffer = std::make_shared<mvVertexBuffer>(graphics, name, std::vector<float>{
			-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.f, 0.f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 1.f, 1.f,
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.f, 1.f,
			 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 1.f, 0.f
		}, vl);

		// create index buffer
		m_indexBuffer = mvBindableRegistry::Request<mvIndexBuffer>(graphics, name, std::vector<unsigned int>{
			0, 1, 2,
				0, 3, 1
		}, false);

		mvStep step("lambertian");

		// create vertex shader
		auto vshader = mvBindableRegistry::Request<mvVertexShader>(graphics, graphics.getShaderRoot() + "PhongDif_VS.hlsl");
		step.addBindable(vshader);
		step.addBindable(mvBindableRegistry::Request<mvInputLayout>(graphics, vl, *vshader));
		step.addBindable(mvBindableRegistry::Request<mvPixelShader>(graphics, graphics.getShaderRoot() + "PhongDif_PS.hlsl"));
		step.addBindable(std::make_shared<mvTransformConstantBuffer>(graphics));
		step.addBindable(mvBindableRegistry::Request<mvSampler>(graphics, mvSampler::Type::Anisotropic, false, 0u));
		step.addBindable(mvBindableRegistry::Request<mvTexture>(graphics, path, 0u));

		mvTechnique technique;
		technique.addStep(step);
		addTechnique(technique);
	}

	glm::mat4 mvTexturedQuad::getTransform() const
	{

		return glm::translate(glm::vec3{ m_x, m_y, m_z }) * 
			glm::rotate(m_zangle, glm::vec3{ 0.0f, 0.0f, 1.0f }) *
			glm::rotate(m_yangle, glm::vec3{ 0.0f, 1.0f, 0.0f })*
			glm::rotate(m_xangle, glm::vec3{ 1.0f, 0.0f, 0.0f });
	}

	void mvTexturedQuad::setPosition(float x, float y, float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	void mvTexturedQuad::setRotation(float x, float y, float z)
	{
		m_xangle = x;
		m_yangle = y;
		m_zangle = z;
	}

	void mvTexturedQuad::show_imgui_windows(const char* name)
	{
		if (ImGui::Begin(name))
		{
			ImGui::SliderFloat("X-Pos", &m_x, -50.0f, 50.0f);
			ImGui::SliderFloat("Y-Pos", &m_y, -50.0f, 50.0f);
			ImGui::SliderFloat("Z-Pos", &m_z, -50.0f, 50.0f);
			ImGui::SliderAngle("X-Angle", &m_xangle, -180.0f, 180.0f);
			ImGui::SliderAngle("Y-Angle", &m_yangle, -180.0f, 180.0f);
			ImGui::SliderAngle("Z-Angle", &m_zangle, -180.0f, 180.0f);
			//ImGui::ColorEdit3("Material Color", &m_material->m_cbData.materialColor.x);
			//ImGui::ColorEdit3("Specular Color", &m_material->m_cbData.specularColor.x);
			//ImGui::SliderFloat("Specular Weight", &m_material->m_cbData.specularWeight, 0.0f, 100.0f);
			//ImGui::SliderFloat("Specular Gloss", &m_material->m_cbData.specularGloss, 0.0f, 100.0f);
		}
		ImGui::End();
	}

}