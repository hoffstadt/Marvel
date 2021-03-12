#pragma once
#include <memory>
#include <vector>
#include <d3d11.h>
#include "mvMath.h"

namespace Marvel {

	// forward declarations
	class mvIndexBuffer;
	class mvVertexBuffer;
	class mvTopology;
	class mvBindable;
	class mvGraphics;

	class mvDrawable
	{

	public:

		mvDrawable() = default;
		virtual ~mvDrawable();

		virtual void      draw(mvGraphics& graphics) const;
		virtual glm::mat4 getTransform() const = 0;

		void addBindable(mvBindable* bindable);
		void bind(mvGraphics& graphics) const;
		UINT getIndexCount() const;

	protected:

		mvIndexBuffer*           m_indexBuffer;
		mvVertexBuffer*          m_vertexBuffer;
		mvTopology*              m_topology;
		std::vector<mvBindable*> m_bindables;

	};

}