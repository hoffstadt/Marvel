#pragma once
#include <vector>

namespace Marvel {

	// forward declarations
	class mvIndexBuffer;
	class mvVertexBuffer;
	class mvInputLayout;
	class mvBindable;
	class mvGraphics;

	class mvDrawable
	{

	public:

		mvDrawable() = default;
		virtual ~mvDrawable();

		virtual void draw(mvGraphics& graphics) const;

		void addBindable(mvGraphics& graphics, mvBindable* bindable);

		void bind(mvGraphics& graphics) const;

		size_t getIndexCount() const;

	protected:

		mvIndexBuffer*           m_indexBuffer;
		mvVertexBuffer*          m_vertexBuffer;
		mvInputLayout*           m_layout; // for vao
		std::vector<mvBindable*> m_bindables;

	};

}