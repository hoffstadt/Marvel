#include "mvIndexBuffer.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace Marvel {

	mvIndexBuffer::mvIndexBuffer(mvGraphics& graphics, const std::vector<unsigned short>& indices)
	{
		glGenBuffers(1, &m_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);
	}

	void mvIndexBuffer::bind(mvGraphics& graphics)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
	}

}