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

		// for retrieving index count
		friend class mvJob;

	public:

		mvDrawable() = default;
		virtual ~mvDrawable() = default;

		virtual glm::mat4 getTransform() const = 0;

		// propagates through graph linking steps to passes
		// drawable -> technique -> step -> pass
		void linkTechniques(mvRenderGraph& graph);

		// propagates through graph submitting jobs
		// drawable -> technique -> step -> pass
		void submit(mvRenderGraph& graph) const;

		// binds topology, index buffer, and vertex buffer
		void bind(mvGraphics& graphics) const;

		void addTechnique (mvTechnique technique);
		
	protected:

		std::shared_ptr<mvIndexBuffer>  m_indexBuffer;
		std::shared_ptr<mvVertexBuffer> m_vertexBuffer;
		std::shared_ptr<mvTopology>     m_topology;
		std::vector<mvTechnique>        m_techniques;

	};

}