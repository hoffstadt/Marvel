#include "mvFrustum.h"
#include <imgui.h>
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvTechnique.h"
#include "mvMarvelUtils.h"

namespace Marvel {

	mvFrustum::mvFrustum(mvGraphics& graphics, const std::string& name, float width, float height, float nearZ, float farZ, bool normalize)
	{

		// create vertex buffer
		mvVertexLayout vl;
		vl.append(ElementType::Position3D);

		if (normalize)
		{
			if (width > height)
			{
				height = height / width;
				width = 1.0f;
			}
			else
			{
				width = width / height;
				height = 1.0f;
			}
		}

		float zRatio = 0.0f;
		if (normalize)
			zRatio = farZ / nearZ;
		else
			zRatio = 1.0f;

		const float nearX = width / 2.0f;
		const float nearY = height / 2.0f;
		const float farX = nearX * zRatio;
		const float farY = nearY * zRatio;

		m_vertexBuffer = std::make_shared<mvVertexBuffer>(graphics, name+"frust", std::vector<float>{
			-nearX, nearY, nearZ,
				nearX, nearY, nearZ,
				nearX, -nearY, nearZ,
				-nearX, -nearY, nearZ,
				-farX, farY, farZ,
				farX, farY, farZ,
				farX, -farY, farZ,
				-farX, -farY, farZ

		}, vl);

		 //create index buffer
		m_indexBuffer = mvBufferRegistry::Request<mvIndexBuffer>(graphics, name + "frust", std::vector<unsigned int>{
			0u, 1u,
			1u, 2u,
			2u, 3u,
			3u, 0u,
			4u, 5u,
			5u, 6u,
			6u, 7u,
			7u, 4u,
			0u, 4u,
			1u, 5u,
			2u, 6u,
			3u, 7u,
		}, false);

		mvTechnique technique;

		{
			mvStep step("overlay");

			mvBufferLayout layout(std::make_shared<mvBufferLayoutEntry>(Struct));
			auto& root = layout.getRoot();
			root->add(Float3, std::string("materialColor"));
			root->finalize(0);

			std::unique_ptr<mvDynamicBuffer> bufferRaw = std::make_unique<mvDynamicBuffer>(std::move(layout));

			bufferRaw->getElement("materialColor").setIfExists(glm::vec3{ 1.0f,1.0f,0.2f });

			std::shared_ptr<mvPixelConstantBuffer> buf = std::make_shared<mvPixelConstantBuffer>(graphics, *root.get(), 1, bufferRaw.get());

			step.addBuffer(buf);
			step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));

			mvPipelineInfo pipeline;

			pipeline.vertexShader = graphics.getShaderRoot() + "Solid_VS.hlsl";
			pipeline.pixelShader = graphics.getShaderRoot() + "Solid_PS.hlsl";
			pipeline.geometryShader = "";
			pipeline.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			pipeline.vertexLayout = vl;
			pipeline.rasterizerStateCull = false;
			pipeline.rasterizerStateHwPCF = false;
			pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::DEPTH_REVERSED;
			pipeline.blendStateFlags = mvBlendStateFlags::OFF;

			step.registerPipeline(graphics, pipeline);

			technique.addStep(std::move(step));
		}

		addTechnique(technique);

	}

	void mvFrustum::setPos(float x, float y, float z)
	{
		m_pos.x = x;
		m_pos.y = y;
		m_pos.z = z;
	}

	void mvFrustum::setRotation(float x, float y, float z)
	{
		m_rot.x = x;
		m_rot.y = y;
		m_rot.z = z;
	}

	glm::mat4 mvFrustum::getTransform() const
	{
		return glm::translate(m_pos) *
			glm::rotate(m_rot.z, glm::vec3{ 0.0f, 0.0f, 1.0f }) *
			glm::rotate(m_rot.y, glm::vec3{ 0.0f, 1.0f, 0.0f }) *
			glm::rotate(m_rot.x, glm::vec3{ 1.0f, 0.0f, 0.0f });
	}


}