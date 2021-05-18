#include "mvCube.h"
#include <imgui.h>
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "Sphere.h"
#include "mvTechnique.h"

namespace Marvel {

	mvCube::mvCube(mvGraphics& graphics, const std::string& name, glm::vec3 color)
	{

		// create topology
		m_topology = std::make_shared<mvTopology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// create vertex layout
		mvVertexLayout vl;
		vl.append(ElementType::Position3D);
		vl.append(ElementType::Normal);
		vl.append(ElementType::Tangent);
		vl.append(ElementType::Bitangent);
		vl.append(ElementType::Texture2D);

		static const float side = 1.0f;
		auto vertices = std::vector<float>{
			-side, -side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 0 near side
			 side, -side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 1
			-side,  side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 2
			 side,  side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 3
			-side, -side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 4 far side
			 side, -side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 5
			-side,  side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 6
			 side,  side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 7
			-side, -side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 8 left side
			-side,  side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 9
			-side, -side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 10
			-side,  side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 11
			 side, -side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 12 right side
			 side,  side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 13
			 side, -side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 14
			 side,  side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 15
			-side, -side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 16 bottom side
			 side, -side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 17
			-side, -side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 18
			 side, -side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 19
			-side,  side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 20 top side
			 side,  side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 21
			-side,  side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 22
			 side,  side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f   // 23
		};

		auto indices = std::vector<unsigned int>{
			0,2, 1,    2,3,1,
			4,5, 7,    4,7,6,
			8,10, 9,  10,11,9,
			12,13,15, 12,15,14,
			16,17,18, 18,17,19,
			20,23,21, 20,22,23
		};

		for (size_t i = 0; i < indices.size(); i+=3)
		{
			auto v0 = vertices[14*indices[i]];
			auto v1 = vertices[14*indices[i + 1]];
			auto v2 = vertices[14*indices[i + 2]];
			const auto p0 = glm::vec3{ v0, vertices[14*indices[i] + 1], vertices[14*indices[i] + 2] };
			const auto p1 = glm::vec3{ v1, vertices[14*indices[i+1] + 1], vertices[14*indices[i+1] + 2] };
			const auto p2 = glm::vec3{ v2, vertices[14*indices[i+2] + 1], vertices[14*indices[i+2]+ 2] };

			const auto n = glm::normalize(glm::cross(p1 - p0, p2 - p0));
			vertices[14*indices[i] + 3] = n[0];
			vertices[14*indices[i] + 4] = n[1];
			vertices[14*indices[i] + 5] = n[2];
			vertices[14*indices[i+1]+ 3] = n[0];
			vertices[14*indices[i+1]+ 4] = n[1];
			vertices[14*indices[i+1]+ 5] = n[2];
			vertices[14*indices[i + 2]+ 3] = n[0];
			vertices[14*indices[i + 2]+ 4] = n[1];
			vertices[14*indices[i + 2]+ 5] = n[2];
		}

		// create vertex buffer
		m_vertexBuffer = mvBufferRegistry::Request<mvVertexBuffer>(graphics, name, vertices, vl, false);

		// create index buffer
		m_indexBuffer = mvBufferRegistry::Request<mvIndexBuffer>(graphics, name, indices, false);

		mvTechnique technique;
		{
			mvStep step("lambertian");

			std::shared_ptr<mvPixelConstantBuffer> buf = std::make_shared<mvPixelConstantBuffer>(graphics, 1, &m_materialBuffer);
			step.addBuffer(buf);

			// create vertex shader
			auto vshader = mvBindableRegistry::Request<mvVertexShader>(graphics, graphics.getShaderRoot() + "PhongModel_VS.hlsl");
			step.addBindable(vshader);
			step.addBindable(mvBindableRegistry::Request<mvInputLayout>(graphics, vl, *vshader));
			step.addBindable(mvBindableRegistry::Request<mvPixelShader>(graphics, graphics.getShaderRoot() + "PhongModel_PS.hlsl"));
			step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));
			step.addBindable(std::make_shared<mvNullGeometryShader>(graphics));
			step.addBindable(mvBindableRegistry::Request<mvSampler>(graphics, mvSampler::Type::Anisotropic, false, 0u));
			step.addBindable(mvBindableRegistry::Request<mvTexture>(graphics, "../../Resources/brickwall.jpg", 0u));
			step.addBindable(mvBindableRegistry::Request<mvRasterizer>(graphics, false));
			step.addBindable(mvBindableRegistry::Request<mvBlender>(graphics, false));


			technique.addStep(step);
		}

		{
			mvStep step("shadow1");

			// create vertex shader
			auto vshader = mvBindableRegistry::Request<mvVertexShader>(graphics, graphics.getShaderRoot() + "PhongShadow_VS.hlsl");
			step.addBindable(mvBindableRegistry::GetBindable("null_ps"));
			step.addBindable(vshader);
			step.addBindable(mvBindableRegistry::Request<mvInputLayout>(graphics, vl, *vshader));
			step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));
			technique.addStep(step);
		}
		{
			mvStep step("shadow2");

			// create vertex shader
			auto vshader = mvBindableRegistry::Request<mvVertexShader>(graphics, graphics.getShaderRoot() + "PhongShadow_VS.hlsl");
			step.addBindable(mvBindableRegistry::GetBindable("null_ps"));
			step.addBindable(vshader);
			step.addBindable(mvBindableRegistry::Request<mvInputLayout>(graphics, vl, *vshader));
			step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));
			technique.addStep(step);
		}

		{
			mvStep step("shadow3");

			// create vertex shader
			auto vshader = mvBindableRegistry::Request<mvVertexShader>(graphics, graphics.getShaderRoot() + "PhongShadow_VS.hlsl");
			step.addBindable(mvBindableRegistry::GetBindable("null_ps"));
			step.addBindable(vshader);
			step.addBindable(mvBindableRegistry::Request<mvInputLayout>(graphics, vl, *vshader));
			step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));
			technique.addStep(step);
		}
		addTechnique(technique);

	}

	glm::mat4 mvCube::getTransform() const
	{
		return glm::translate(glm::vec3{ m_x, m_y, m_z }) *
			glm::rotate(m_zangle, glm::vec3{ 0.0f, 0.0f, 1.0f }) *
			glm::rotate(m_yangle, glm::vec3{ 0.0f, 1.0f, 0.0f }) *
			glm::rotate(m_xangle, glm::vec3{ 1.0f, 0.0f, 0.0f });
	}

	void mvCube::show_imgui_windows(const char* name)
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

	void mvCube::setPosition(float x, float y, float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	void mvCube::setRotation(float x, float y, float z)
	{
		m_xangle = x;
		m_yangle = y;
		m_zangle = z;
	}

}