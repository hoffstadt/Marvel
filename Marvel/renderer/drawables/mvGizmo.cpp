#include "mvGizmo.h"
#include <imgui.h>
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvTechnique.h"
#include "mvMarvelUtils.h"

namespace Marvel {

	mvGizmo::mvGizmo(mvGraphics& graphics, const std::string& name)
	{

		// create vertex layout
		mvVertexLayout vl;
		vl.append(ElementType::Position3D);
		vl.append(ElementType::Color);

		std::vector<float> verticies;
		std::vector<unsigned int> indicies;
		std::vector<float> normals;

		// create vertex buffer
		m_vertexBuffer = std::make_shared<mvVertexBuffer>(graphics, name, std::vector<float>{
			    
				0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

				0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

				0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
				0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
			
		}, vl);

		// create index buffer
		m_indexBuffer = mvBufferRegistry::Request<mvIndexBuffer>(graphics, name, std::vector<unsigned int>{
			0, 1,
				2, 3,
				4, 5
		}, false);

		mvStep step("overlay");

		step.addBuffer(mvBufferRegistry::GetBuffer("transCBuf"));

		mvPipelineInfo pipeline;

		pipeline.vertexShader = graphics.getShaderRoot() + "Gizmo_vs.hlsl";
		pipeline.pixelShader = graphics.getShaderRoot() + "Gizmo_ps.hlsl";
		pipeline.geometryShader = "";
		pipeline.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::MV_DEPTH_STENCIL_STATE_DEPTH_REVERSED;
		pipeline.vertexLayout = vl;
		pipeline.rasterizerStateCull = false;
		pipeline.rasterizerStateHwPCF = false;
		pipeline.blendStateFlags = mvBlendStateFlags::MV_BLEND_STATE_BLEND_OFF;

		step.registerPipeline(graphics, pipeline);

		mvTechnique technique;
		technique.addStep(std::move(step));
		addTechnique(technique);

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