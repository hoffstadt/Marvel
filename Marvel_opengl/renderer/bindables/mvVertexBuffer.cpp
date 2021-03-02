#include "mvVertexBuffer.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace Marvel
{

	mvVertexBuffer::mvVertexBuffer(mvGraphics& graphics, const mvDynamicVertexBuffer& vbuf)
	{

		glGenBuffers(1, &m_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
		glBufferData(GL_ARRAY_BUFFER, vbuf.SizeBytes(), vbuf.GetData(), GL_STATIC_DRAW);
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
