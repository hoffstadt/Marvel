#include "mvModel.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mvNode.h"
#include "drawables/mvMesh.h"

namespace Marvel {

	mvModel::mvModel(mvGraphics& graphics, const std::string& pathString, float scale)
	{
		Assimp::Importer imp;
		const auto pScene = imp.ReadFile(pathString.c_str(),
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_ConvertToLeftHanded |
			aiProcess_GenNormals |
			aiProcess_CalcTangentSpace
		);


		for (size_t i = 0; i < pScene->mNumMeshes; i++)
		{
			const auto& mesh = *pScene->mMeshes[i];
			m_meshes.push_back(std::make_shared<mvMesh>(
				graphics, mesh, *pScene->mMaterials[mesh.mMaterialIndex], pathString, scale));
		}

		m_root.reset(parseNode(*pScene->mRootNode));
	}

	void mvModel::submit(mvRenderGraph& graph) const
	{
		m_root->submit(graph, glm::identity<glm::mat4>());
	}

	void mvModel::draw(mvGraphics& graphics) const
	{
		m_root->draw(graphics);
	}

	void mvModel::setRootTransform(glm::mat4 tf)
	{
		m_root->setAppliedTransform(tf);
	}

	mvNode* mvModel::parseNode(const aiNode& node)
	{
		const auto transform = reinterpret_cast<const glm::mat4*>(&node.mTransformation);

		std::vector<std::shared_ptr<mvMesh>> curMeshPtrs;
		curMeshPtrs.reserve(node.mNumMeshes);
		for (size_t i = 0; i < node.mNumMeshes; i++)
		{
			const auto meshIdx = node.mMeshes[i];
			curMeshPtrs.push_back(m_meshes.at(meshIdx));
		}

		mvNode* pNode = new mvNode(curMeshPtrs, *transform);
		for (size_t i = 0; i < node.mNumChildren; i++)
			pNode->addChild(parseNode(*node.mChildren[i]));

		return pNode;
	}

}