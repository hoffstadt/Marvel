#include "mvVertexLayout.h"

namespace Marvel {

	mvVertexElement::mvVertexElement(ElementType type)
	{
		switch (type) 
		{

		case ElementType::Position2D:
			m_gltype = GL_FLOAT;
			m_itemCount = 2;
			m_normalize = false;
			m_size = sizeof(float) * 2;
			break;

		case ElementType::Texture2D:
			m_gltype = GL_FLOAT;
			m_itemCount = 2;
			m_normalize = false;
			m_size = sizeof(float) * 2;
			break;

		}
	}

	GLenum mvVertexElement::getGLType() const
	{
		return m_gltype;
	}

	int mvVertexElement::getItemCount() const
	{
		return m_itemCount;
	}

	bool mvVertexElement::isNormalized() const
	{
		return m_normalize;
	}

	size_t mvVertexElement::getSize() const
	{
		return m_size;
	}

	size_t mvVertexElement::getOffset() const
	{
		return m_offset;
	}

	void mvVertexLayout::append(ElementType type)
	{
		m_elements.emplace_back(type);
		m_elements.back().m_offset = m_stride;
		m_stride += m_elements.back().getSize();
		m_size += m_elements.back().getSize();
	}

	const std::vector<mvVertexElement>& mvVertexLayout::getElements() const
	{
		return m_elements;
	}

	const size_t mvVertexLayout::getElementCount() const
	{
		return m_elements.size();
	}

	const size_t mvVertexLayout::getSize() const
	{
		return m_size;
	}
}