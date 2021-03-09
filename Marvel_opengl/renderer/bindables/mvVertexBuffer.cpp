#include "mvVertexBuffer.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace Marvel
{

	mvVertexBuffer::mvVertexBuffer(mvGraphics& graphics, const std::vector<float>& vbuf)
	{

		glGenBuffers(1, &m_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
		glBufferData(GL_ARRAY_BUFFER, vbuf.size()*sizeof(float), vbuf.data(), GL_STATIC_DRAW);
	}

	void mvVertexBuffer::bind(mvGraphics& gfx)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	}

	void mvVertexBuffer::unbind(mvGraphics& gfx)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
