#pragma once
#include "mvGraphics.h"
#include <string>
#include <vector>
#include <memory>
#include "mvNode.h"
#include "mvMesh.h"
#include "mvGizmo.h"

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------
struct aiNode;

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvRenderGraph;
	class mvModelProbe;

	//-----------------------------------------------------------------------------
	// mvModel
	//-----------------------------------------------------------------------------
	class mvModel
	{

	public:

		mvModel(mvGraphics& graphics, const std::string& pathString, float scale = 1.0f);

		// propagates through graph linking steps to passes
		// model -> node -> mesh -> technique -> step -> pass
		void linkTechniques(mvRenderGraph& graph);

		// propagates through graph submitting jobs
		// model -> node -> mesh -> technique -> step -> pass
		void submit(mvRenderGraph& graph) const;

		// sets the root transform which will propagate
		// trhough the graph
		void setRootTransform(glm::mat4 tf);

		void     accept          (mvModelProbe& probe);
		mvNode*  getNode         (const std::string& name);
		mvGizmo& getGizmo        () { return m_mesh; }

	private:

		mvNode* parseNode(int& id, const aiNode& node, float scale);

	private:

		std::shared_ptr<mvNode>              m_root;
		std::vector<std::shared_ptr<mvMesh>> m_meshes;
		mvGizmo                              m_mesh;
	};
}