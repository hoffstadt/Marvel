#include "mvSolidSphere.h"
#include <imgui.h>
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "Sphere.h"
#include "mvTechnique.h"
#include "mvMarvelUtils.h"

namespace Marvel {

	mvSolidSphere::mvSolidSphere(mvGraphics& graphics, const std::string& name, float radius, glm::vec3 color, int simple)
	{

		// create vertex layout
		mvVertexLayout vl;
		vl.append(ElementType::Position3D);
		vl.append(ElementType::Normal);
		vl.append(ElementType::Tangent);
		vl.append(ElementType::Bitangent);
		vl.append(ElementType::Texture2D);

		std::vector<float> verticies;
		std::vector<unsigned int> indicies;
		std::vector<float> normals;
		Sphere sphere(radius);

		verticies = sphere.vertices;
		indicies = sphere.indices;
		normals = sphere.normals;

		std::vector<float> nverticies;
		for (int i = 0; i < verticies.size(); i = i + 3)
		{
			nverticies.push_back(verticies[i]);
			nverticies.push_back(verticies[i+1]);
			nverticies.push_back(verticies[i+2]);
			nverticies.push_back(normals[i]);
			nverticies.push_back(normals[i+1]);
			nverticies.push_back(normals[i+2]);

			nverticies.push_back(0.0f);
			nverticies.push_back(0.0f);
			nverticies.push_back(0.0f);
			nverticies.push_back(0.0f);
			nverticies.push_back(0.0f);
			nverticies.push_back(0.0f);
			nverticies.push_back(0.0f);
			nverticies.push_back(0.0f);
		}

		// create vertex buffer
		m_vertexBuffer = std::make_shared<mvVertexBuffer>(graphics, name, nverticies, vl);

		// create index buffer
		m_indexBuffer = mvBufferRegistry::Request<mvIndexBuffer>(graphics, name, indicies, false);

		mvStep step("lambertian");

		// solid color
		if (simple == 1)
		{
			mvBufferLayout layout(std::make_shared<mvBufferLayoutEntry>(Struct));
			auto& root = layout.getRoot();
			root->add(Float3, std::string("materialColor"));
			root->finalize(0);

			std::unique_ptr<mvDynamicBuffer> bufferRaw = std::make_unique<mvDynamicBuffer>(std::move(layout));

			bufferRaw->getElement("materialColor").setIfExists(color);

			//-----------------------------------------------------------------------------
			// additional buffers
			//-----------------------------------------------------------------------------
			step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));
			step.addBuffer(std::make_shared<mvPixelConstantBuffer>(graphics, *root.get(), 1, bufferRaw.get()));

			//-----------------------------------------------------------------------------
			// pipeline state setup
			//-----------------------------------------------------------------------------
			mvPipelineInfo pipeline;

			// input assembler stage
			pipeline.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			pipeline.vertexLayout = vl;

			// vertex shader stage
			pipeline.vertexShader = graphics.getShaderRoot() + "Solid_VS.hlsl";

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
			pipeline.pixelShader = graphics.getShaderRoot() + "Solid_PS.hlsl";
			// * no samplers

			// output merger stage
			pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::OFF;
			pipeline.blendStateFlags = mvBlendStateFlags::OFF;

			// registers required pipeline
			step.registerPipeline(graphics, pipeline);
		}

		// flat shade
		else if(simple == 2)
		{
			//// create vertex shader
			//auto vshader = mvBindableRegistry::Request<mvVertexShader>(graphics, graphics.getShaderRoot() + "vs_flat.hlsl");
			//step.addBindable(vshader);
			//step.addBindable(mvBindableRegistry::Request<mvInputLayout>(graphics, vl, *vshader));
			//step.addBindable(mvBindableRegistry::Request<mvPixelShader>(graphics, graphics.getShaderRoot() + "ps_flat.hlsl"));
			//step.addBindable(std::make_shared<mvGeometryShader>(graphics, "../../Marvel/shaders/gs_flat.hlsl"));
			//step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));
		}

		// phong
		else
		{

			//-----------------------------------------------------------------------------
			// additional buffers
			//-----------------------------------------------------------------------------
			step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));
			step.addBuffer(std::make_shared<mvPhongMaterialCBuf>(graphics, 1));

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
			pipeline.pixelShader = graphics.getShaderRoot() + "PBRModel_PS.hlsl";
			pipeline.samplers.push_back({ mvSamplerStateTypeFlags::ANISOTROPIC, mvSamplerStateAddressingFlags::WRAP, 0u, false });
			pipeline.samplers.push_back({ mvSamplerStateTypeFlags::POINT, mvSamplerStateAddressingFlags::BORDER, 1u, true });

			// output merger stage
			pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::OFF;
			pipeline.blendStateFlags = mvBlendStateFlags::OFF;

			// registers required pipeline
			step.registerPipeline(graphics, pipeline);
		}


		mvTechnique technique;
		technique.addStep(step);
		addTechnique(technique);

	}

	void mvSolidSphere::setTransform(glm::mat4 transform)
	{
		const auto angles = ExtractEulerAngles(transform);
		const auto translation = ExtractTranslation(transform);
		m_x = translation.x;
		m_y = translation.y;
		m_z = translation.z;
		m_xangle = angles.x;
		m_yangle = angles.y;
		m_zangle = angles.z;
	}

	glm::mat4 mvSolidSphere::getTransform() const
	{
		return glm::translate(glm::vec3{ m_x, m_y, m_z }) *
			glm::rotate(m_zangle, glm::vec3{ 0.0f, 0.0f, 1.0f }) *
			glm::rotate(m_yangle, glm::vec3{ 0.0f, 1.0f, 0.0f }) *
			glm::rotate(m_xangle, glm::vec3{ 1.0f, 0.0f, 0.0f });
	}

	void mvSolidSphere::show_imgui_windows(const char* name)
	{
		if (ImGui::Begin(name))
		{
			ImGui::SliderFloat("X-Pos", &m_x, -50.0f, 50.0f);
			ImGui::SliderFloat("Y-Pos", &m_y, -50.0f, 50.0f);
			ImGui::SliderFloat("Z-Pos", &m_z, -50.0f, 50.0f);
			ImGui::SliderFloat("X-Angle", &m_xangle, -50.0f, 50.0f);
			ImGui::SliderFloat("Y-Angle", &m_yangle, -50.0f, 50.0f);
			ImGui::SliderFloat("Z-Angle", &m_zangle, -50.0f, 50.0f);
			//ImGui::ColorEdit3("Material Color", &m_material->m_cbData.materialColor.x);
			//ImGui::ColorEdit3("Specular Color", &m_material->m_cbData.specularColor.x);
			//ImGui::SliderFloat("Specular Weight", &m_material->m_cbData.specularWeight, 0.0f, 100.0f);
			//ImGui::SliderFloat("Specular Gloss", &m_material->m_cbData.specularGloss, 0.0f, 100.0f);
		}
		ImGui::End();
	}

	void mvSolidSphere::setPosition(float x, float y, float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	void mvSolidSphere::setRotation(float x, float y, float z)
	{
		m_xangle = x;
		m_yangle = y;
		m_zangle = z;
	}

}