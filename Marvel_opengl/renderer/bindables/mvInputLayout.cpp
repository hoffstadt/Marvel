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
		int offset = 0;
		for (size_t i = 0; i < m_layout.GetElementCount(); i++)
		{
			const auto& element = m_layout.ResolveByIndex(i);

			glEnableVertexAttribArray(i);

			glVertexAttribPointer(i, element.GetItemCount(), element.GetGLType(), element.Normalized(),
				mvVertexElement::SizeOf(element.GetType()), (void*)offset);

			glBindAttribLocation(shader->getProgram(), i, element.GetSemantic());

			offset += mvVertexElement::SizeOf(element.GetType());

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