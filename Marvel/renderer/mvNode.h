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
	class mvModelProbe;
	class mvModel;

	//-----------------------------------------------------------------------------
	// mvNode
	//-----------------------------------------------------------------------------
	class mvNode
	{

	public:

		mvNode(const std::string& name, int id, std::vector<std::shared_ptr<mvMesh>> meshPtrs, const glm::mat4& transform);

		// propagates through node children submitting jobs
		// node -> mesh -> technique -> step -> pass
		void submit(mvRenderGraph& graph, glm::mat4 accumulatedTransform) const;
		void linkSteps(mvRenderGraph& graph);

		// transforms
		const glm::mat4& getTransform       () const; // internal transform
		const glm::mat4& getAppliedTransform() const; // transform set by user
		const glm::mat4& getFullTransform   () const; // final transform (accumulation included)
		void             setAppliedTransform(glm::mat4 transform);

		void addChild(mvNode* child);
		void accept  (mvModelProbe& probe);

		// setters
		void setModel     (mvModel* model);
		void setSelection (bool value) { m_selected = value; }

		// getters
		int                getID              () const;
		bool               hasChildren        () const { return !m_children.empty(); }
		const std::string& getName            () const { return m_name; }
		mvNode*            getNode            (const std::string& name);

		std::vector<std::shared_ptr<mvMesh>>& getMeshes() { return m_meshes; }


	private:

		std::vector<std::shared_ptr<mvNode>> m_children;
		std::vector<std::shared_ptr<mvMesh>> m_meshes;
		glm::mat4                            m_transform;
		glm::mat4                            m_appliedTransform;
		mutable glm::mat4                    m_fullTransform;
		int                                  m_id = 0;
		std::string                          m_name;
		bool                                 m_selected = false;
		mvModel*                             m_model;

	};

}