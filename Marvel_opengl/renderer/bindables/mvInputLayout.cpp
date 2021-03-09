#include "mvInputLayout.h"
#include "mvVertexLayout.h"

namespace Marvel {

	mvInputLayout::mvInputLayout(mvGraphics& graphics, mvVertexLayout vertexLayout)
		:
		m_layout(std::move(vertexLayout))
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

	}

	void mvInputLayout::init(mvShader* shader)
	{

		for (size_t i = 0; i < m_layout.getElementCount(); i++)
		{
			const auto& element = m_layout.getElements()[i];

			glVertexAttribPointer(i, 
				element.getItemCount(), 
				element.getGLType(), 
				element.isNormalized(),
				m_layout.getSize(), 
				(void*)element.getOffset());
			glEnableVertexAttribArray(i);

		}
	}

	void mvInputLayout::bind(mvGraphics& graphics)
	{
		glBindVertexArray(m_vao);
	}

	void mvInputLayout::unbind(mvGraphics& graphics)
	{
		glBindVertexArray(0);
	}

}