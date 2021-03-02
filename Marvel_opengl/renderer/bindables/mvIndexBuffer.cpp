#include "mvIndexBuffer.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace Marvel {

	mvIndexBuffer::mvIndexBuffer(mvGraphics& graphics, const std::vector<unsigned short>& indices)
	{
		glGenBuffers(1, &m_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);

		m_count = indices.size();

	}

	void mvIndexBuffer::bind(mvGraphics& graphics)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
	}

	void mvIndexBuffer::unbind(mvGraphics& gfx)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	size_t mvIndexBuffer::getCount() const
	{
		return m_count;
	}

}