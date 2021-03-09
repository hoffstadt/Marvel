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

	enum class ElementType
	{
		Position2D,
		Texture2D
	};

	//-----------------------------------------------------------------------------
	// mvVertexElement
	//-----------------------------------------------------------------------------
	class mvVertexElement
	{

		friend class mvVertexLayout;

	public:

		mvVertexElement(ElementType type);

		GLenum getGLType   () const;
		int    getItemCount() const;
		bool   isNormalized() const;
		size_t getSize     () const;
		size_t getOffset   () const;

	private:

		GLenum      m_gltype;
		int         m_itemCount;
		bool        m_normalize;
		size_t      m_size;
		size_t      m_offset;

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

	private:

		size_t m_stride = 0;
		size_t m_size = 0;
		std::vector<mvVertexElement> m_elements;

	};

}
