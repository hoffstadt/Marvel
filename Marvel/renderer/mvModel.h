#pragma once
#include "mvGraphics.h"
#include <string>
#include <vector>
#include <memory>
#include "mvNode.h"
#include "mvMesh.h"

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------
struct aiNode;

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvModel
	//-----------------------------------------------------------------------------
	class mvModel
	{
	public:
		mvModel(mvGraphics& graphics, const std::string& pathString, float scale = 1.0f);

		void submit          () const;
		void draw            (mvGraphics& graphics) const;
		void setRootTransform(glm::mat4 tf);

	private:

		mvNode* parseNode(const aiNode& node);

	private:

		std::shared_ptr<mvNode> m_root;
		std::vector<std::shared_ptr<mvMesh>> m_meshes;
	};
}