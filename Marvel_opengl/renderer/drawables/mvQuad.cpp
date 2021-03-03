#include "mvQuad.h"
#include "mvGraphics.h"
#include "mvIndexBuffer.h"
#include "mvVertexBuffer.h"
#include "mvVertexLayout.h"
#include "mvInputLayout.h"
#include "mvShader.h"
#include "mvTransformUniform.h"

namespace Marvel {

	mvQuad::mvQuad(mvGraphics& graphics)
	{

		mvVertexLayout vl;
		vl.Append(ElementType::Position2D);

		mvDynamicVertexBuffer dvertexBuffer(vl);

		dvertexBuffer.EmplaceBack(std::array{ -0.5f,  0.5f });
		dvertexBuffer.EmplaceBack(std::array{  0.5f,  0.5f });
		dvertexBuffer.EmplaceBack(std::array{  0.5f, -0.5f });
		dvertexBuffer.EmplaceBack(std::array{ -0.5f, -0.5f });

		auto shader = std::make_shared<mvShader>(graphics, "vs_simple.glsl", "ps_simple.glsl");

		// create input layout
		m_layout = std::make_unique<mvInputLayout>(graphics, vl);

		// create vertex buffer
		m_vertexBuffer = std::make_unique<mvVertexBuffer>(graphics, dvertexBuffer);

		// create index buffer
		m_indexBuffer = std::make_unique<mvIndexBuffer>(graphics,
			std::vector<unsigned short>{
			   2, 1, 0,
			   2, 0, 3
		});

		m_layout->init(shader.get());
		m_layout->unbind(graphics);
		m_indexBuffer->unbind(graphics);
		m_vertexBuffer->unbind(graphics);

		addBindable(graphics, shader);
		addBindable(graphics, std::make_shared<mvTransformUniform>(graphics));

	}

	glm::mat4 mvQuad::getTransform() const
	{
		return glm::rotate(m_xangle, glm::vec3{ 1.0f, 0.0f, 0.0f }) *
			glm::rotate(m_yangle, glm::vec3{ 0.0f, 1.0f, 0.0f }) *
			glm::rotate(m_zangle, glm::vec3{ 0.0f, 0.0f, 1.0f }) *
			glm::translate(glm::vec3{ m_x, m_y, m_z });
	}

	void mvQuad::setPosition(float x, float y, float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	void mvQuad::setRotation(float x, float y, float z)
	{
		m_xangle = x;
		m_yangle = y;
		m_zangle = z;
	}
}