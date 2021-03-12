#pragma once
#include <vector>
#include <type_traits>
#include <utility>
#include <DirectXMath.h>
#include <string>
#include "mvGraphics.h"

namespace Marvel {

	enum class ElementType
	{
		Position2D,
		Position3D,
		Texture2D,
		Color,
		Normal
	};

	//-----------------------------------------------------------------------------
	// mvVertexElement
	//-----------------------------------------------------------------------------
	class mvVertexElement
	{

		friend class mvVertexLayout;

	public:

		mvVertexElement(ElementType type);

		DXGI_FORMAT getFormat   () const;
		int         getItemCount() const;
		bool        isNormalized() const;
		size_t      getSize     () const;
		size_t      getOffset   () const;
		const char* getSemantic () const;

	private:

		int         m_itemCount = 0;
		bool        m_normalize = false;
		size_t      m_size = 0;
		size_t      m_offset = 0;
		DXGI_FORMAT m_format = DXGI_FORMAT_R32G32_FLOAT;
		std::string m_semantic;

	};

	//-----------------------------------------------------------------------------
	// mvVertexLayout
	//-----------------------------------------------------------------------------
	class mvVertexLayout
	{

	public:

		void append(ElementType type);

		const size_t getElementCount() const;
		const size_t getSize() const;
		const std::vector<mvVertexElement>& getElements() const;
		std::vector<D3D11_INPUT_ELEMENT_DESC> getD3DLayout() const;

	private:

		size_t m_stride = 0;
		size_t m_size = 0;
		std::vector<mvVertexElement> m_elements;

	};

}
