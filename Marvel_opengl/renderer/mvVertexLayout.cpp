#include "mvVertexLayout.h"

namespace Marvel {

	const mvVertexElement& mvVertexLayout::ResolveByIndex(size_t i) const
	{
		return m_elements[i];
	}

	template<ElementType type>
	struct GLTypeLookup
	{
		static constexpr auto Exec() noexcept
		{
			return mvMap<type>::GlType;
		}
	};

	template<ElementType type>
	struct SemanticLookup
	{
		static constexpr auto Exec() noexcept
		{
			return mvMap<type>::semantic;
		}
	};

	template<ElementType type>
	struct ItemCountLookup
	{
		static constexpr auto Exec() noexcept
		{
			return mvMap<type>::ItemCount;
		}
	};

	template<ElementType type>
	struct NormalizeLookup
	{
		static constexpr auto Exec() noexcept
		{
			return mvMap<type>::Normalize;
		}
	};

	mvVertexLayout& mvVertexLayout::Append(ElementType type)
	{
		if (!Has(type))
		{
			m_elements.emplace_back(type, Size(), 
				mvVertexLayout::Bridge<GLTypeLookup>(type),
				mvVertexLayout::Bridge<SemanticLookup>(type),
				mvVertexLayout::Bridge<ItemCountLookup>(type),
				mvVertexLayout::Bridge<NormalizeLookup>(type)
				);
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

	mvVertexElement::mvVertexElement(ElementType type, size_t offset, GLenum glType, const char* semantic,
		int itemCount, bool normalize)
		:
		m_type(type),
		m_offset(offset),
		m_gltype(glType),
		m_semantic(semantic),
		m_itemCount(itemCount),
		m_normalize(normalize)
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

	GLenum mvVertexElement::GetGLType() const
	{
		return m_gltype;
	}

	const char* mvVertexElement::GetSemantic() const
	{
		return m_semantic;
	}

	int mvVertexElement::GetItemCount() const
	{
		return m_itemCount;
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


	mvVertexView::mvVertexView(char* pData, const mvVertexLayout& layout)
		: m_pData(pData), m_layout(layout)
	{
		//assert(pData != nullptr);
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
		//assert(m_buffer.size() != 0u);
		return mvVertexView{ m_buffer.data() + m_buffer.size() - m_layout.Size(), m_layout };
	}

	mvVertexView mvDynamicVertexBuffer::Front()
	{
		//assert(m_buffer.size() != 0u);
		return mvVertexView{ m_buffer.data(), m_layout };
	}

	mvVertexView mvDynamicVertexBuffer::operator[](size_t i)
	{
		//assert(i < Size());
		return mvVertexView{ m_buffer.data() + m_layout.Size() * i,m_layout };
	}
}