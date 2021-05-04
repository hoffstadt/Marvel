#include "mvModel.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mvNode.h"
#include "drawables/mvMesh.h"


namespace Marvel {

	mvModel::mvModel(mvGraphics& graphics, const std::string& pathString, float scale)
		:
		m_mesh(graphics)
	{

		Assimp::Importer imp;
		const auto pScene = imp.ReadFile(pathString.c_str(),
			aiProcess_ConvertToLeftHanded |
			aiProcess_SplitLargeMeshes |
			aiProcess_JoinIdenticalVertices |
			aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
			aiProcess_Triangulate |             // Make sure we're triangles
			//aiProcess_SortByPType |             // Split meshes by primitive type
			aiProcess_GenNormals |              // Make sure we have legit normals
			aiProcess_GenUVCoords |             // Convert UVs if required 
			aiProcess_OptimizeMeshes |          // Batch draws where possible
			aiProcess_ValidateDataStructure    // Validation

		);

		m_meshes.reserve(pScene->mNumMeshes);
		for (size_t i = 0; i < pScene->mNumMeshes; i++)
		{
			const auto& mesh = *pScene->mMeshes[i];
			m_meshes.push_back(std::make_shared<mvMesh>(
				graphics, mesh, *pScene->mMaterials[mesh.mMaterialIndex], pathString, scale));
		}

		int id = 0;
		m_root.reset(parseNode(id, *pScene->mRootNode, scale));
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
	}

	mvNode* mvModel::getNode(const std::string& name)
	{
		return m_root->getNode(name);
	}

	void mvModel::setRootTransform(glm::mat4 tf)
	{
		m_root->setAppliedTransform(tf);
	}

	mvNode* mvModel::parseNode(int& id, const aiNode& node, float scale)
	{
		const auto transform = glm::transpose(*reinterpret_cast<const glm::mat4*>(&node.mTransformation)) * glm::scale(glm::vec3(scale, scale, scale));

		std::vector<std::shared_ptr<mvMesh>> curMeshPtrs;
		curMeshPtrs.reserve(node.mNumMeshes);
		for (size_t i = 0; i < node.mNumMeshes; i++)
		{
			const auto meshIdx = node.mMeshes[i];
			curMeshPtrs.push_back(m_meshes.at(meshIdx));
		}

		mvNode* pNode = new mvNode(node.mName.C_Str(), id++, curMeshPtrs, transform);
		pNode->setModel(this);
		for (size_t i = 0; i < node.mNumChildren; i++)
			pNode->addChild(parseNode(id, *node.mChildren[i], scale));

		return pNode;
	}

}