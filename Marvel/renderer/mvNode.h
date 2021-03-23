#pragma once
#include <vector>
#include <memory>
#include "mvMath.h"
#include "mvMesh.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvRenderGraph;

	//-----------------------------------------------------------------------------
	// mvNode
	//-----------------------------------------------------------------------------
	class mvNode
	{

	public:

		mvNode(std::vector<std::shared_ptr<mvMesh>> meshPtrs, const glm::mat4& transform);

		void             submit             (mvRenderGraph& graph, glm::mat4 accumulatedTransform) const;
		void             draw               (mvGraphics& graphics) const;
		void             setAppliedTransform(glm::mat4 transform);
		const glm::mat4& getAppliedTransform() const;
		void             addChild           (mvNode* child);


	private:

		std::vector<std::shared_ptr<mvNode>> m_children;
		std::vector<std::shared_ptr<mvMesh>> m_meshes;
		glm::mat4                            m_transform;
		glm::mat4                            m_appliedTransform;

	};

}