#include "mvNode.h"
#include "mvMesh.h"

namespace Marvel {

	mvNode::mvNode(std::vector<std::shared_ptr<mvMesh>> meshPtrs, const glm::mat4& transform)
	{
		m_meshes = meshPtrs;
		m_transform = transform;
		m_appliedTransform = glm::identity<glm::mat4>();
	}

	void mvNode::submit(glm::mat4 accumulatedTransform) const
	{
		const auto built = accumulatedTransform * m_transform * m_appliedTransform;

		for (const auto pm : m_meshes)
			pm->submit(built);

		for (const auto& pc : m_children)
			pc->submit(built);
	}

	void mvNode::draw(mvGraphics& graphics) const
	{
		for (const auto& pm : m_meshes)
			pm->draw(graphics);

		for (const auto& pc : m_children)
			pc->draw(graphics);
	}

	void mvNode::setAppliedTransform(glm::mat4 transform)
	{
		m_appliedTransform = transform;
	}

	const glm::mat4& mvNode::getAppliedTransform() const
	{
		return m_appliedTransform;
	}

	void mvNode::addChild(mvNode* child)
	{
		m_children.push_back(std::shared_ptr<mvNode>(child));
	}

}