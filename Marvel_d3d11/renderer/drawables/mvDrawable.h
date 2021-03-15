#pragma once
#include <memory>
#include <vector>
#include <d3d11.h>
#include "mvMath.h"
#include "mvCommonBindables.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvDrawable
	//-----------------------------------------------------------------------------
	class mvDrawable
	{

	public:

		mvDrawable() = default;

		virtual void      draw(mvGraphics& graphics) const;
		virtual glm::mat4 getTransform() const = 0;

		void addBindable(std::shared_ptr<mvBindable> bindable);
		void bind(mvGraphics& graphics) const;
		UINT getIndexCount() const;

	protected:

		std::shared_ptr<mvIndexBuffer>           m_indexBuffer;
		std::shared_ptr<mvVertexBuffer>          m_vertexBuffer;
		std::shared_ptr<mvTopology>              m_topology;
		std::vector<std::shared_ptr<mvBindable>> m_bindables;

	};

}