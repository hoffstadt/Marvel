#include "mvTexturedQuad.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"

namespace Marvel {

	mvTexturedQuad::mvTexturedQuad(mvGraphics& graphics, const std::string& path)
	{



		// create topology
		m_topology = new mvTopology(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		// create vertex layout
		mvVertexLayout vl;
		vl.append(ElementType::Position3D);
		vl.append(ElementType::Normal);
		vl.append(ElementType::Texture2D);

		// create vertex buffer
		m_vertexBuffer = new mvVertexBuffer(graphics, std::vector<float>{
			-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.f, 0.f,
			 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 1.f, 1.f,
			-0.5f, -0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.f, 1.f,
			 0.5f,  0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 1.f, 0.f
		}, vl);

		// create index buffer
		m_indexBuffer = new mvIndexBuffer(graphics,
			std::vector<unsigned short>{
				0, 1, 2,
				0, 3, 1
			});


		// create vertex shader
		auto vshader = new mvVertexShader(graphics, "../../../Marvel_d3d11/shaders/vs_texture.hlsl");
		addBindable(vshader);
		addBindable(new mvInputLayout(graphics, vl, *vshader));
		addBindable(new mvPixelShader(graphics, "../../../Marvel_d3d11/shaders/ps_texture.hlsl"));
		addBindable(new mvTransformConstantBuffer(graphics));
		addBindable(new mvSampler(graphics));
		addBindable(new mvTexture(graphics, path));

	}

	glm::mat4 mvTexturedQuad::getTransform() const
	{
		return glm::rotate(m_xangle, glm::vec3{ 1.0f, 0.0f, 0.0f }) *
			glm::rotate(m_yangle, glm::vec3{ 0.0f, 1.0f, 0.0f }) *
			glm::rotate(m_zangle, glm::vec3{ 0.0f, 0.0f, 1.0f }) *
			glm::translate(glm::vec3{ m_x, m_y, m_z });
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

}