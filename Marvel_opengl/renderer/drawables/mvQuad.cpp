#include "mvQuad.h"
#include "mvGraphics.h"
#include "mvIndexBuffer.h"
#include "mvVertexBuffer.h"
#include "mvVertexLayout.h"
#include "mvInputLayout.h"
#include "mvShader.h"

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


		auto shader = new mvShader(graphics, "vs_simple.glsl", "ps_simple.glsl");

		// create input layout
		m_layout = new mvInputLayout(graphics, vl);

		// create vertex buffer
		m_vertexBuffer = new mvVertexBuffer(graphics, dvertexBuffer);

		// create index buffer
		m_indexBuffer = new mvIndexBuffer(graphics,
			std::vector<unsigned short>{
			   2, 1, 0,
			   2, 0, 3
		});

		m_layout->init(*shader);
		m_layout->unbind(graphics);
		m_indexBuffer->unbind(graphics);
		m_vertexBuffer->unbind(graphics);

		addBindable(graphics, shader);

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