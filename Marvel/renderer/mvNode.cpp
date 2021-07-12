#include "mvNode.h"
#include "mvMesh.h"
#include "mvModel.h"
#include "mvModelProbe.h"

namespace Marvel {

	mvNode::mvNode(const std::string& name, int id, std::vector<std::shared_ptr<mvMesh>> meshPtrs, const glm::mat4& transform)
	{
		m_name = name;
		m_id = id;
		m_meshes = meshPtrs;
		m_transform = transform;
		m_appliedTransform = glm::identity<glm::mat4>();
	}

	int mvNode::getID() const
	{
		return m_id;
	}

	void mvNode::setModel(mvModel* model)
	{
		m_model = model;
	}

	void mvNode::submit(mvBaseRenderGraph& graph, glm::mat4 accumulatedTransform) const
	{
		const auto built = accumulatedTransform * m_transform * m_appliedTransform;

		m_fullTransform = built;

		if (m_selected)
			m_model->getGizmo().setTransform(built);

		for (const auto& pm : m_meshes)
			pm->submit(graph, built);

		for (const auto& pc : m_children)
			pc->submit(graph, built);
	}

	void mvNode::accept(mvModelProbe& probe)
	{
		if (probe.pushNode(*this))
		{
			for (auto& cp : m_children)
				cp->accept(probe);
			probe.popNode(*this);
		}
	}

	void mvNode::linkSteps(mvBaseRenderGraph& graph)
	{
		for (auto& mesh : m_meshes)
			mesh->linkSteps(graph);

		for (auto& node : m_children)
			node->linkSteps(graph);
	}

	void mvNode::setAppliedTransform(glm::mat4 transform)
	{
		m_appliedTransform = transform;
	}

	const glm::mat4& mvNode::getAppliedTransform() const
	{
		return m_appliedTransform;
	}

	const glm::mat4& mvNode::getFullTransform() const
	{
		return m_fullTransform;
	}

	const glm::mat4& mvNode::getTransform() const
	{
		return m_transform;
	}

	void mvNode::addChild(mvNode* child)
	{
		m_children.push_back(std::shared_ptr<mvNode>(child));
	}

	mvNode* mvNode::getNode(const std::string& name)
	{
		if (m_name == name)
			return this;

		for (const auto& child : m_children)
		{
			if (auto result = child->getNode(name))
				return result;
		}

		return nullptr;
	}

}