#include "mvGizmo.h"
#include <imgui.h>
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvTechnique.h"
#include "mvMarvelUtils.h"

namespace Marvel {

	mvGizmo::mvGizmo(mvGraphics& graphics, const std::string& name)
	{

		// create topology
		m_topology = std::make_shared<mvTopology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

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
		m_indexBuffer = mvBindableRegistry::Request<mvIndexBuffer>(graphics, name, std::vector<unsigned int>{
			0, 1,
				2, 3,
				4, 5
		}, false);

		mvStep step("overlay");

		// create vertex shader
		auto vshader = mvBindableRegistry::Request<mvVertexShader>(graphics, graphics.getShaderRoot() + "Gizmo_vs.hlsl");
		step.addBindable(vshader);
		step.addBindable(mvBindableRegistry::Request<mvInputLayout>(graphics, vl, *vshader));
		step.addBindable(mvBindableRegistry::Request<mvPixelShader>(graphics, graphics.getShaderRoot() + "Gizmo_ps.hlsl"));
		step.addBindable(std::make_shared<mvNullGeometryShader>(graphics));
		step.addBindable(std::make_shared<mvTransformConstantBuffer>(graphics));

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