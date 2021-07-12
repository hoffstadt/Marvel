#pragma once
#include <memory>
#include <vector>
#include <d3d11.h>
#include "mvMath.h"
#include "mvCommonBindables.h"
#include "mvCommonBuffers.h"
#include "mvStep.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvBaseRenderGraph;
	class mvMaterial;

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
		// drawable -> step -> pass
		void linkSteps(mvBaseRenderGraph& graph);

		// propagates through graph submitting jobs
		// drawable -> technique -> step -> pass
		void submit(mvBaseRenderGraph& graph) const;

		// binds topology, index buffer, and vertex buffer
		void bind(mvGraphics& graphics) const;

		void addStep(mvStep step);
		
	protected:

		std::shared_ptr<mvIndexBuffer>             m_indexBuffer;
		std::shared_ptr<mvVertexBuffer>            m_vertexBuffer;
		std::vector<mvStep>                        m_steps;

	};

}