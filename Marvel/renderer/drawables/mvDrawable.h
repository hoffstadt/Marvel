#pragma once
#include <memory>
#include <vector>
#include <d3d11.h>
#include "mvMath.h"
#include "mvCommonBindables.h"
#include "mvTechnique.h"

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

		void bind         (mvGraphics& graphics) const;
		UINT getIndexCount() const;
		void addTechnique (mvTechnique technique);

	protected:

		std::shared_ptr<mvIndexBuffer>  m_indexBuffer;
		std::shared_ptr<mvVertexBuffer> m_vertexBuffer;
		std::shared_ptr<mvTopology>     m_topology;
		std::vector<mvTechnique>        m_techniques;

	};

}