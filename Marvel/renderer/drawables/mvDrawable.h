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
	class mvRenderGraph;

	//-----------------------------------------------------------------------------
	// mvDrawable
	//-----------------------------------------------------------------------------
	class mvDrawable
	{

	public:

		mvDrawable() = default;

		virtual void      draw(mvGraphics& graphics) const;
		virtual glm::mat4 getTransform() const = 0;

		void submit(mvRenderGraph& graph) const;

		void bind         (mvGraphics& graphics) const;
		UINT getIndexCount() const;
		void addTechnique (mvTechnique technique);
		void linkTechniques(mvRenderGraph& graph);

	protected:

		std::shared_ptr<mvIndexBuffer>  m_indexBuffer;
		std::shared_ptr<mvVertexBuffer> m_vertexBuffer;
		std::shared_ptr<mvTopology>     m_topology;
		std::vector<mvTechnique>        m_techniques;

	};

}