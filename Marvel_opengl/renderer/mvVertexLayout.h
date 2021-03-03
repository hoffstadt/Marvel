#pragma once
#include <vector>
#include <array>
#include <type_traits>
#include <utility>
#include <string>
#include "mvGraphics.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <assert.h>

namespace Marvel {

	struct BGRAColor
	{
		unsigned char a;
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};

	enum class ElementType
	{
		Position2D,
		Texture2D,
		Count
	};

	//-----------------------------------------------------------------------------
	// Forward Declarations
	//-----------------------------------------------------------------------------
	class mvVertexView;

	template<ElementType>
	struct mvMap;

	//-----------------------------------------------------------------------------
	// mvVertexElement
	//-----------------------------------------------------------------------------
	class mvVertexElement
	{

	public:

		static constexpr size_t SizeOf(ElementType type);

	public:

		mvVertexElement(ElementType type, size_t offset, GLenum glType, const char* semantic,
			int itemCount, bool normalize);

		size_t                   GetOffsetAfter() const;
		size_t                   GetOffset() const;
		size_t                   Size() const;
		ElementType              GetType() const;
		GLenum                   GetGLType() const;
		const char*              GetSemantic() const;
		int                      GetItemCount() const;
		bool                     Normalized() const { return m_normalize; }

	private:

		ElementType m_type;
		size_t      m_offset;
		GLenum      m_gltype;
		const char* m_semantic;
		int         m_itemCount;
		bool        m_normalize;
	};

	//-----------------------------------------------------------------------------
	// mvVertexLayout
	//   - represents the layout of a single vertex
	//-----------------------------------------------------------------------------
	class mvVertexLayout
	{

	public:

		template<template<ElementType> class F, typename... Args>
		static constexpr auto Bridge(ElementType type, Args&&... args)
		{
			switch (type)
			{

			case ElementType::Position2D:
				return F<ElementType::Position2D>::Exec(std::forward<Args>(args)...);

			case ElementType::Texture2D:
				return F<ElementType::Texture2D>::Exec(std::forward<Args>(args)...);

			case ElementType::Count:
				return F<ElementType::Count>::Exec(std::forward<Args>(args)...);
			}
			assert("Invalid element type" && false);
			return F<ElementType::Count>::Exec(std::forward<Args>(args)...);
		}

	public:

		template<ElementType Type>
		const mvVertexElement& Resolve() const
		{
			for (auto& e : m_elements)
			{
				if (e.GetType() == Type)
					return e;
			}
			assert("Could not resolve element type" && false);
			return m_elements.front();
		}

		const mvVertexElement& ResolveByIndex(size_t i) const;
		mvVertexLayout& Append(ElementType type);
		size_t                                Size() const;
		size_t                                GetElementCount() const;
		bool                                  Has(ElementType type) const;

	private:

		std::vector<mvVertexElement> m_elements;
	};

	//-----------------------------------------------------------------------------
	// AttributeSetting
	//   - necessary for Bridge to SetAttribute
	//-----------------------------------------------------------------------------
	template<ElementType type>
	struct mvAttributeSetting
	{
		template<typename T>
		static constexpr auto Exec(mvVertexView* pVertex, char* pAttribute, T&& val)
		{
			return pVertex->SetAttribute<type>(pAttribute, std::forward<T>(val));
		}
	};

	//-----------------------------------------------------------------------------
	// mvVertexView
	//-----------------------------------------------------------------------------
	class mvVertexView
	{

		friend class mvDynamicVertexBuffer;

	public:

		template<ElementType Type>
		auto& Attr()
		{
			auto pAttribute = m_pData + m_layout.Resolve<Type>().GetOffset();
			return *reinterpret_cast<typename mvVertexLayout::mvMap<Type>::SysType*>(pAttribute);
		}

		template<typename T>
		void SetAttributeByIndex(size_t i, T&& val)
		{
			const auto& element = m_layout.ResolveByIndex(i);
			auto pAttribute = m_pData + element.GetOffset();
			mvVertexLayout::Bridge<mvAttributeSetting>(
				element.GetType(), this, pAttribute, std::forward<T>(val)
				);
		}
	protected:

		mvVertexView(char* pData, const mvVertexLayout& layout);


	public:

		// enables parameter pack setting of multiple parameters by element index
		template<typename First, typename ...Rest>
		void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest)
		{
			SetAttributeByIndex(i, std::forward<First>(first));
			SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
		}

		// helper to reduce code duplication in SetAttributeByIndex
		template<ElementType DestLayoutType, typename SrcType>
		void SetAttribute(char* pAttribute, SrcType&& val)
		{
			using Dest = typename mvMap<DestLayoutType>::SysType;
			if constexpr (std::is_assignable<Dest, SrcType>::value)
				*reinterpret_cast<Dest*>(pAttribute) = val;
			else
				assert("Parameter attribute type mismatch" && false);
		}

	private:

		char* m_pData = nullptr;
		const mvVertexLayout& m_layout;
	};

	//-----------------------------------------------------------------------------
	// mvConstVertexView
	//-----------------------------------------------------------------------------
	class mvConstVertexView
	{
	public:

		mvConstVertexView(const mvVertexView& v);

		template<ElementType Type>
		const auto& Attr() const
		{
			return const_cast<mvVertexView&>(m_vertex).Attr<Type>();
		}

	private:

		mvVertexView m_vertex;
	};

	//-----------------------------------------------------------------------------
	// mvDynamicVertexBuffer
	//-----------------------------------------------------------------------------
	class mvDynamicVertexBuffer
	{
	public:
		mvDynamicVertexBuffer(mvVertexLayout layout, size_t size = 0u);

		const char* GetData() const;
		const mvVertexLayout& GetLayout() const;
		void                  Resize(size_t newSize);
		size_t                Size() const;
		size_t                SizeBytes() const;

		template<typename ...Params>
		void EmplaceBack(Params&&... params)
		{
			assert(sizeof...(params) == m_layout.GetElementCount() && "Param count doesn't match number of vertex elements");
			m_buffer.resize(m_buffer.size() + m_layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}

		mvVertexView Back();
		mvVertexView Front();
		mvVertexView operator[](size_t i);
	private:
		std::vector<char> m_buffer;
		mvVertexLayout    m_layout;
	};

	template<> struct mvMap<ElementType::Position2D>
	{
		using SysType = std::array<float, 2>;
		static constexpr GLenum GlType = GL_FLOAT;
		static constexpr int ItemCount = 2;
		static constexpr bool Normalize = false;
		static constexpr const char* semantic = "Position";
	};

	template<> struct mvMap<ElementType::Texture2D>
	{
		using SysType = std::array<float, 2>;
		static constexpr GLenum GlType = GL_FLOAT;
		static constexpr int ItemCount = 2;
		static constexpr bool Normalize = false;
		static constexpr const char* semantic = "Texture";
	};

	template<> struct mvMap<ElementType::Count>
	{
		using SysType = std::array<float, 2>;
		static constexpr GLenum GlType = GL_FLOAT;
		static constexpr int ItemCount = 2;
		static constexpr bool Normalize = false;
		static constexpr const char* semantic = "!INVALID!";
	};
}
