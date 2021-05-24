#include "mvTexturedQuad.h"
#include "mvGraphics.h"
#include "mvTechnique.h"
#include "mvCommonBindables.h"
#include <imgui.h>
#include "Sphere.h"

namespace Marvel {

	mvTexturedQuad::mvTexturedQuad(mvGraphics& graphics, const std::string& name, const std::string& path)
	{

		// create vertex layout
		mvVertexLayout vl;
		vl.append(ElementType::Position3D);
		vl.append(ElementType::Normal);
		vl.append(ElementType::Tangent);
		vl.append(ElementType::Bitangent);
		vl.append(ElementType::Texture2D);

		auto phongMaterial = std::make_shared<mvPhongMaterialCBuf>(graphics, 1);

		phongMaterial->material.materialColor = { 0.45f, 0.45f, 0.85f };
		phongMaterial->material.specularColor = { 0.18f, 0.18f, 0.18f };
		phongMaterial->material.specularGloss = 8.0f;
		phongMaterial->material.normalMapWeight = 1.0f;
		phongMaterial->material.useTextureMap = true;
		phongMaterial->material.useNormalMap = false;
		phongMaterial->material.useSpecularMap = false;
		phongMaterial->material.useGlossAlpha = false;
		phongMaterial->material.hasAlpha = false;


		// initialize vertices
		auto vertices = std::vector<float>
		{
			-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		};

		// initialize indexes
		auto indices = std::vector<unsigned int>{
			1,2,0,
				3, 1, 0
		};

		for (size_t i = 0; i < indices.size(); i += 3)
		{
			auto v0 = vertices[14 * indices[i]];
			auto v1 = vertices[14 * indices[i + 1]];
			auto v2 = vertices[14 * indices[i + 2]];
			const auto p0 = glm::vec3{ v0, vertices[14 * indices[i] + 1], vertices[14 * indices[i] + 2] };
			const auto p1 = glm::vec3{ v1, vertices[14 * indices[i + 1] + 1], vertices[14 * indices[i + 1] + 2] };
			const auto p2 = glm::vec3{ v2, vertices[14 * indices[i + 2] + 1], vertices[14 * indices[i + 2] + 2] };

			const auto n = glm::normalize(glm::cross(p1 - p0, p2 - p0));
			vertices[14 * indices[i] + 3] = n[0];
			vertices[14 * indices[i] + 4] = n[1];
			vertices[14 * indices[i] + 5] = n[2];
			vertices[14 * indices[i + 1] + 3] = n[0];
			vertices[14 * indices[i + 1] + 4] = n[1];
			vertices[14 * indices[i + 1] + 5] = n[2];
			vertices[14 * indices[i + 2] + 3] = n[0];
			vertices[14 * indices[i + 2] + 4] = n[1];
			vertices[14 * indices[i + 2] + 5] = n[2];
		}

		// create vertex buffer
		m_vertexBuffer = mvBufferRegistry::Request<mvVertexBuffer>(graphics, name, vertices, vl, false);

		// create index buffer
		m_indexBuffer = mvBufferRegistry::Request<mvIndexBuffer>(graphics, name, indices, false);

		mvTechnique phong;
		{
			mvStep step("lambertian");

			//-----------------------------------------------------------------------------
			// additional buffers
			//-----------------------------------------------------------------------------
			step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));
			step.addBuffer(phongMaterial);
			step.addBindable(mvBindableRegistry::Request<mvTexture>(graphics, path, 0u));

			//-----------------------------------------------------------------------------
			// pipeline state setup
			//-----------------------------------------------------------------------------
			mvPipelineInfo pipeline;

			// input assembler stage
			pipeline.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			pipeline.vertexLayout = vl;

			// vertex shader stage
			pipeline.vertexShader = graphics.getShaderRoot() + "PhongModel_VS.hlsl";

			// geometry shader stage
			pipeline.geometryShader = "";

			// rasterizer stage
			pipeline.viewportWidth = 0;  // use render target
			pipeline.viewportHeight = 0; // use render target
			pipeline.rasterizerStateCull = true;
			pipeline.rasterizerStateHwPCF = false;
			pipeline.rasterizerStateDepthBias = 0;    // not used
			pipeline.rasterizerStateSlopeBias = 0.0f; // not used
			pipeline.rasterizerStateClamp = 0.0f;	  // not used

			// pixel shader stage
			pipeline.pixelShader = graphics.getShaderRoot() + "PhongModel_PS.hlsl";
			pipeline.samplers.push_back({ mvSamplerStateTypeFlags::ANISOTROPIC, mvSamplerStateAddressingFlags::WRAP, 0u, false });
			pipeline.samplers.push_back({ mvSamplerStateTypeFlags::POINT, mvSamplerStateAddressingFlags::BORDER, 1u, true });

			// output merger stage
			pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::OFF;
			pipeline.blendStateFlags = mvBlendStateFlags::OFF;

			// registers required pipeline
			step.registerPipeline(graphics, pipeline);

			phong.addStep(step);
		}

		mvTechnique map;
		{
			//-----------------------------------------------------------------------------
			// shadow mapping pipeline state setup
			//-----------------------------------------------------------------------------
			mvPipelineInfo pipeline;

			// input assembler stage
			pipeline.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			pipeline.vertexLayout = vl;

			// vertex shader stage
			pipeline.vertexShader = graphics.getShaderRoot() + "PhongShadow_VS.hlsl";

			// geometry shader stage
			pipeline.geometryShader = "";

			// rasterizer stage
			pipeline.viewportWidth = 1000;
			pipeline.viewportHeight = 1000;
			pipeline.rasterizerStateCull = false;
			pipeline.rasterizerStateHwPCF = true;
			pipeline.rasterizerStateDepthBias = 50;
			pipeline.rasterizerStateSlopeBias = 2.0f;
			pipeline.rasterizerStateClamp = 0.1f;

			// pixel shader stage
			pipeline.pixelShader = "";
			// * no samplers

			// output merger stage
			pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::OFF;
			pipeline.blendStateFlags = mvBlendStateFlags::OFF;

			for (int i = 1; i < 4; i++)
			{
				mvStep step("shadow" + std::to_string(i));

				//-----------------------------------------------------------------------------
				// additional buffers
				//-----------------------------------------------------------------------------
				step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));

				// registers required pipeline
				step.registerPipeline(graphics, pipeline);

				map.addStep(step);
			}
		}

		addTechnique(phong);
		addTechnique(map);
	}

	glm::mat4 mvTexturedQuad::getTransform() const
	{

		return glm::translate(glm::vec3{ m_x, m_y, m_z }) *
			glm::rotate(m_zangle, glm::vec3{ 0.0f, 0.0f, 1.0f }) *
			glm::rotate(m_yangle, glm::vec3{ 0.0f, 1.0f, 0.0f }) *
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