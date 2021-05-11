#include "mvFrustum.h"
#include <imgui.h>
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvTechnique.h"
#include "mvMarvelUtils.h"

namespace Marvel {

	mvFrustum::mvFrustum(mvGraphics& graphics, const std::string& name, float width, float height, float nearZ, float farZ)
	{

		// create topology
		m_topology = std::make_shared<mvTopology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		// create vertex buffer
		setVertices(graphics, name, width, height, nearZ, farZ);

		// create index buffer
		m_indexBuffer = mvBindableRegistry::Request<mvIndexBuffer>(graphics, name, std::vector<unsigned int>{
			0, 1,
			1, 2,
			2, 3,
			3, 0,
			4, 5,
			5, 6,
			6, 7,
			7, 4,
			0, 4,
			1, 5,
			2, 6,
			3, 7,
		}, false);

		mvTechnique technique;
		{
			mvStep step("lambertian");

			mvBufferLayout layout(std::make_shared<mvBufferLayoutEntry>(Struct));
			auto& root = layout.getRoot();
			root->add(Float3, std::string("materialColor"));
			root->finalize(0);

			std::unique_ptr<mvBuffer> bufferRaw = std::make_unique<mvBuffer>(std::move(layout));

			bufferRaw->getElement("materialColor").setIfExists(glm::vec3{ 1.0f,0.2f,0.2f });

			std::shared_ptr<mvPixelConstantBuffer> buf = std::make_shared<mvPixelConstantBuffer>(graphics, *root.get(), 1, bufferRaw.get());

			step.addBindable(buf);

			// create vertex shader
			auto vshader = mvBindableRegistry::Request<mvVertexShader>(graphics, graphics.getShaderRoot() + "Solid_VS.hlsl");
			step.addBindable(vshader);

			mvVertexLayout vl;
			vl.append(ElementType::Position3D);
			step.addBindable(mvBindableRegistry::Request<mvInputLayout>(graphics, vl, *vshader));
			step.addBindable(mvBindableRegistry::Request<mvPixelShader>(graphics, graphics.getShaderRoot() + "Solid_PS.hlsl"));
			step.addBindable(std::make_shared<mvNullGeometryShader>(graphics));
			step.addBindable(std::make_shared<mvTransformConstantBuffer>(graphics));
			step.addBindable(mvBindableRegistry::Request<mvRasterizer>(graphics, false));
			//step.addBindable(std::make_shared<mvStencil>(graphics, mvStencil::Mode::DepthReversed));

			
			technique.addStep(std::move(step));
			
		}

		{
			mvStep step("overlay");

			mvBufferLayout layout(std::make_shared<mvBufferLayoutEntry>(Struct));
			auto& root = layout.getRoot();
			root->add(Float3, std::string("materialColor"));
			root->finalize(0);

			std::unique_ptr<mvBuffer> bufferRaw = std::make_unique<mvBuffer>(std::move(layout));

			bufferRaw->getElement("materialColor").setIfExists(glm::vec3{ 0.0f,1.0f,0.2f });

			std::shared_ptr<mvPixelConstantBuffer> buf = std::make_shared<mvPixelConstantBuffer>(graphics, *root.get(), 1, bufferRaw.get());

			step.addBindable(buf);

			// create vertex shader
			auto vshader = mvBindableRegistry::Request<mvVertexShader>(graphics, graphics.getShaderRoot() + "Solid_VS.hlsl");
			step.addBindable(vshader);

			mvVertexLayout vl;
			vl.append(ElementType::Position3D);
			step.addBindable(mvBindableRegistry::Request<mvInputLayout>(graphics, vl, *vshader));
			step.addBindable(mvBindableRegistry::Request<mvPixelShader>(graphics, graphics.getShaderRoot() + "Solid_PS.hlsl"));
			step.addBindable(std::make_shared<mvNullGeometryShader>(graphics));
			step.addBindable(std::make_shared<mvTransformConstantBuffer>(graphics));
			step.addBindable(mvBindableRegistry::Request<mvRasterizer>(graphics, false));
			step.addBindable(std::make_shared<mvStencil>(graphics, mvStencil::Mode::DepthReversed));

			technique.addStep(std::move(step));
		}

		addTechnique(technique);

	}

	void mvFrustum::setVertices(mvGraphics& graphics, const std::string& name, float width, float height, float nearZ, float farZ)
	{
		mvVertexLayout vl;
		vl.append(ElementType::Position3D);

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

		const float zRatio = farZ / nearZ;
		const float nearX = width / 2.0f;
		const float nearY = height / 2.0f;
		const float farX = nearX * zRatio;
		const float farY = nearY * zRatio;

		m_vertexBuffer = std::make_shared<mvVertexBuffer>(graphics, name, std::vector<float>{
			-nearX,  nearY, nearZ,
			 nearX,  nearY, nearZ,
			 nearX, -nearY, nearZ,
			-nearX, -nearY, nearZ,
			 -farX,   farY,  farZ,
			  farX,   farY,  farZ,
			  farX,  -farY,  farZ,
			 -farX,  -farY,  farZ

		}, vl);
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