#include "mvModel.h"
#include "mvNode.h"
#include "drawables/mvMesh.h"
#include "mvObjMaterial.h"

namespace Marvel {

	mvModel::mvModel(mvGraphics& graphics, const std::string& pathString, float scale)
		:
		m_mesh(graphics, pathString+"sphere")
	{

		mvObjMaterialParser mat(pathString + ".mtl");
		mvObjParser objmesh(pathString + ".obj");

		int id = 0;
		m_root.reset(parseNode(graphics, objmesh, mat, pathString, id, objmesh.getRootNode(), scale));
	}

	void mvModel::submit(mvRenderGraph& graph) const
	{
		m_mesh.submit(graph);
		m_root->submit(graph, glm::mat4(1.0f));
	}

	void mvModel::accept(mvModelProbe& probe)
	{
		m_root->accept(probe);
	}

	void mvModel::linkTechniques(mvRenderGraph& graph)
	{
		m_mesh.linkTechniques(graph);
		for (auto& mesh : m_meshes)
			mesh->linkTechniques(graph);
		m_root->linkTechniques(graph);
	}

	mvNode* mvModel::getNode(const std::string& name)
	{
		return m_root->getNode(name);
	}

	void mvModel::setRootTransform(glm::mat4 tf)
	{
		m_root->setAppliedTransform(tf);
	}

	mvNode* mvModel::parseNode(mvGraphics& graphics, mvObjParser& parser, mvObjMaterialParser& mat, const std::string& pathString, int& id, const mvObjNode& node, float scale)
	{
		//const auto transform = glm::transpose(*reinterpret_cast<const glm::mat4*>(&node.mTransformation)) * glm::scale(glm::vec3(scale, scale, scale));
		const auto transform = glm::scale(glm::vec3(scale, scale, scale));

		std::vector<std::shared_ptr<mvMesh>> curMeshPtrs;
		curMeshPtrs.reserve(node.meshes.size());
		for (size_t i = 0; i < node.meshes.size(); i++)
		{
			const auto meshIdx = node.meshes[i];
			mvObjMesh* mesh = parser.getMeshes()[meshIdx];

				bool materialFound = false;
				for (const auto& material : mat.getMaterials())
				{
					if (material.name == mesh->material)
					{
						curMeshPtrs.push_back(std::make_shared<mvMesh>(graphics, mesh->name, *mesh, material, pathString, scale));
						materialFound = true;
						break;
					}
				}

				assert(materialFound);

			
		}

		mvNode* pNode = new mvNode(node.name, id++, curMeshPtrs, transform);
		pNode->setModel(this);
		for (size_t i = 0; i < node.children.size(); i++)
			pNode->addChild(parseNode(graphics, parser, mat, pathString, id, node.children[i], scale));

		return pNode;
	}

}