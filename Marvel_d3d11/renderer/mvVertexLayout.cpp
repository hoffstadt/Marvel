#include "mvVertexLayout.h"

namespace Marvel{

	const mvVertexElement& mvVertexLayout::ResolveByIndex(size_t i) const
	{
		return m_elements[i];
	}
	
	mvVertexLayout& mvVertexLayout::Append(ElementType type)
	{
		if (!Has(type))
		{
			m_elements.emplace_back(type, Size());
		}
		return *this;
	}
	
	bool mvVertexLayout::Has(ElementType type) const
	{
		for (auto& e : m_elements)
		{
			if (e.GetType() == type)
			{
				return true;
			}
		}
		return false;
	}
	
	size_t mvVertexLayout::Size() const
	{
		return m_elements.empty() ? 0u : m_elements.back().GetOffsetAfter();
	}
	
	size_t mvVertexLayout::GetElementCount() const
	{
		return m_elements.size();
	}
	
	std::vector<D3D11_INPUT_ELEMENT_DESC> mvVertexLayout::GetD3DLayout() const
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
		desc.reserve(GetElementCount());
		for (const auto& e : m_elements)
		{
			desc.push_back(e.GetDesc());
		}
		return desc;
	}

	mvVertexElement::mvVertexElement(ElementType type, size_t offset)
		:
		m_type(type),
		m_offset(offset)
	{}

	size_t mvVertexElement::GetOffsetAfter() const
	{
		return m_offset + Size();
	}

	size_t mvVertexElement::GetOffset() const
	{
		return m_offset;
	}

	size_t mvVertexElement::Size() const
	{
		return SizeOf(m_type);
	}

	ElementType mvVertexElement::GetType() const
	{
		return m_type;
	}

	template<ElementType type>
	struct SysSizeLookup
	{
		static constexpr auto Exec() noexcept
		{
			return sizeof(mvMap<type>::SysType);
		}
	};
	constexpr size_t mvVertexElement::SizeOf(ElementType type)
	{
		return mvVertexLayout::Bridge<SysSizeLookup>(type);
	}

	template<ElementType type> struct DescGenerate {
		static constexpr D3D11_INPUT_ELEMENT_DESC Exec(std::size_t offset) {
			return {
				mvMap<type>::semantic,0,
				mvMap<type>::dxgiFormat,
				0,(UINT)offset,D3D11_INPUT_PER_VERTEX_DATA,0
			};
		}
	};
	D3D11_INPUT_ELEMENT_DESC mvVertexElement::GetDesc() const
	{
		return mvVertexLayout::Bridge<DescGenerate>(m_type, GetOffset());
	}

	mvVertexView::mvVertexView(char* pData, const mvVertexLayout& layout)
		: m_pData(pData), m_layout(layout)
	{
		assert(pData != nullptr);
	}

	mvConstVertexView::mvConstVertexView(const mvVertexView& v)
		:
		m_vertex(v)
	{}

	mvDynamicVertexBuffer::mvDynamicVertexBuffer(mvVertexLayout layout, std::size_t size)
		:
		m_layout(std::move(layout))
	{
		Resize(size);
	}
	
	void mvDynamicVertexBuffer::Resize(size_t newSize)
	{
		const auto size = Size();
		if (size < newSize)
			m_buffer.resize(m_buffer.size() + m_layout.Size() * (newSize - size));
	}
	
	const char* mvDynamicVertexBuffer::GetData() const
	{
		return m_buffer.data();
	}

	const mvVertexLayout& mvDynamicVertexBuffer::GetLayout() const
	{
		return m_layout;
	}
	
	size_t mvDynamicVertexBuffer::Size() const
	{
		return m_buffer.size() / m_layout.Size();
	}
	
	size_t mvDynamicVertexBuffer::SizeBytes() const
	{
		return m_buffer.size();
	}
	
	mvVertexView mvDynamicVertexBuffer::Back()
	{
		assert(m_buffer.size() != 0u);
		return mvVertexView{ m_buffer.data() + m_buffer.size() - m_layout.Size(), m_layout };
	}
	
	mvVertexView mvDynamicVertexBuffer::Front()
	{
		assert(m_buffer.size() != 0u);
		return mvVertexView{ m_buffer.data(), m_layout };
	}
	
	mvVertexView mvDynamicVertexBuffer::operator[](size_t i)
	{
		assert(i < Size());
		return mvVertexView{ m_buffer.data() + m_layout.Size() * i,m_layout };
	}
}