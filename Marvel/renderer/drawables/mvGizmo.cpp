#include "mvGizmo.h"
#include <imgui.h>
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvStep.h"
#include "mvMarvelUtils.h"

namespace Marvel {

	mvGizmo::mvGizmo(mvGraphics& graphics, const std::string& name)
	{

		std::vector<float> verticies;
		std::vector<unsigned int> indicies;
		std::vector<float> normals;

		mvStep step("overlay");

		//-----------------------------------------------------------------------------
		// additional buffers
		//-----------------------------------------------------------------------------
		step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));

		//-----------------------------------------------------------------------------
		// pipeline state setup
		//-----------------------------------------------------------------------------
		mvPipelineInfo pipeline;

		// input assembler stage
		pipeline.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		pipeline.vertexLayout.append(ElementType::Position3D);
		pipeline.vertexLayout.append(ElementType::Color);

		// vertex shader stage
		pipeline.vertexShader = graphics.getShaderRoot() + "Gizmo_vs.hlsl";

		// geometry shader stage
		pipeline.geometryShader = "";

		// rasterizer stage
		pipeline.viewportWidth = 0;  // use render target
		pipeline.viewportHeight = 0; // use render target
		pipeline.rasterizerStateCull = false;
		pipeline.rasterizerStateHwPCF = false;
		pipeline.rasterizerStateDepthBias = 0;    // not used
		pipeline.rasterizerStateSlopeBias = 0.0f; // not used
		pipeline.rasterizerStateClamp = 0.0f;	  // not used

		// pixel shader stage
		pipeline.pixelShader = graphics.getShaderRoot() + "Gizmo_ps.hlsl";
		pipeline.samplers.push_back({ mvSamplerStateTypeFlags::ANISOTROPIC, mvSamplerStateAddressingFlags::WRAP, 0u, false });
		pipeline.samplers.push_back({ mvSamplerStateTypeFlags::POINT, mvSamplerStateAddressingFlags::BORDER, 1u, true });

		// output merger stage
		pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::DEPTH_REVERSED;
		pipeline.blendStateFlags = mvBlendStateFlags::OFF;

		// registers required pipeline
		step.registerPipeline(graphics, pipeline);

		addStep(std::move(step));

		// create vertex buffer
		m_vertexBuffer = std::make_shared<mvVertexBuffer>(graphics, name, std::vector<float>{

			0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

				0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

				0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
				0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f

		}, pipeline.vertexLayout);

		// create index buffer
		m_indexBuffer = mvBufferRegistry::Request<mvIndexBuffer>(graphics, name, std::vector<unsigned int>{
			0, 1,
				2, 3,
				4, 5
		}, false);

	}

	void mvGizmo::setTransform(glm::mat4 transform)
	{
		m_transform = transform;
	}

	glm::mat4 mvGizmo::getTransform() const
	{
		return m_transform;
	}


}