#include "mvVertexLayout.h"

namespace Marvel {

	mvVertexElement::mvVertexElement(ElementType type)
	{
		switch (type)
		{

		case ElementType::Position2D:
			m_format = DXGI_FORMAT_R32G32_FLOAT;
			m_itemCount = 2;
			m_normalize = false;
			m_size = sizeof(float) * 2;
			m_semantic = "Position";
			break;

		case ElementType::Position3D:
			m_format = DXGI_FORMAT_R32G32B32_FLOAT;
			m_itemCount = 3;
			m_normalize = false;
			m_size = sizeof(float) * 3;
			m_semantic = "Position";
			break;

		case ElementType::Texture2D:
			m_format = DXGI_FORMAT_R32G32_FLOAT;
			m_itemCount = 2;
			m_normalize = false;
			m_size = sizeof(float) * 2;
			m_semantic = "Texcoord";
			break;

		case ElementType::Color:
			m_format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			m_itemCount = 4;
			m_normalize = false;
			m_size = sizeof(float) * 4;
			m_semantic = "Color";
			break;

		case ElementType::Normal:
			m_format = DXGI_FORMAT_R32G32B32_FLOAT;
			m_itemCount = 3;
			m_normalize = false;
			m_size = sizeof(float) * 3;
			m_semantic = "Normal";
			break;

		case ElementType::Tangent:
			m_format = DXGI_FORMAT_R32G32B32_FLOAT;
			m_itemCount = 3;
			m_normalize = false;
			m_size = sizeof(float) * 3;
			m_semantic = "Tangent";
			break;

		case ElementType::Bitangent:
			m_format = DXGI_FORMAT_R32G32B32_FLOAT;
			m_itemCount = 3;
			m_normalize = false;
			m_size = sizeof(float) * 3;
			m_semantic = "Bitangent";
			break;

		}
	}

	DXGI_FORMAT mvVertexElement::getFormat() const
	{
		return m_format;
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

	const char* mvVertexElement::getSemantic() const
	{
		return m_semantic.c_str();
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> mvVertexLayout::getD3DLayout() const
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout;

		for (const auto& element : m_elements)
		{
			layout.push_back({
				element.getSemantic(),
				0,
				element.getFormat(),
				0, 
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
				});
		}

		return std::move(layout);
	}
}