#include "mvMesh.h"
#include "mvMarvelUtils.h"
#include <imgui.h>
#include "mvGraphics.h"
#include "mvMaterial.h"
#include "mvCommonBindables.h"
#include "assimp/Importer.hpp"
#include "assimp/Scene.h"
#include "assimp/postprocess.h"
#include "mvTechnique.h"

namespace Marvel {

	struct TransformParameters
	{
		float xRot = 0.0f;
		float yRot = 0.0f;
		float zRot = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};

	mvMesh::mvMesh(mvGraphics& graphics, const std::string& name, const aiMesh& mesh, const aiMaterial& material, const std::filesystem::path& path, float scale)
	{

		const auto rootPath = path.parent_path().string() + "\\";

		m_transform = glm::identity<glm::mat4>();

		mvMaterial mat = mvMaterial(graphics, material, rootPath);

		// create vertex layout
		const Marvel::mvVertexLayout& vertexLayout = mat.getLayout();
		
		std::vector<float> verticies;
		verticies.reserve(mesh.mNumVertices * 14);
		std::vector<unsigned int> indicies;
		indicies.reserve(mesh.mNumFaces * 3);


		// this is nasty and requires the layout order to be correct. 
		// a more robust system is needed.
		for (size_t i = 0; i < mesh.mNumVertices; i++)
		{
			verticies.push_back(mesh.mVertices[i].x * scale);
			verticies.push_back(mesh.mVertices[i].y * scale);
			verticies.push_back(mesh.mVertices[i].z * scale);

			verticies.push_back(mesh.mNormals[i].x);
			verticies.push_back(mesh.mNormals[i].y);
			verticies.push_back(mesh.mNormals[i].z);

			verticies.push_back(mesh.mTangents[i].x);
			verticies.push_back(mesh.mTangents[i].y);
			verticies.push_back(mesh.mTangents[i].z);

			verticies.push_back(mesh.mBitangents[i].x);
			verticies.push_back(mesh.mBitangents[i].y);
			verticies.push_back(mesh.mBitangents[i].z);

			verticies.push_back(mesh.mTextureCoords[0][i].x);
			verticies.push_back(mesh.mTextureCoords[0][i].y);
		}

		for (int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indicies.push_back(face.mIndices[0]);
			indicies.push_back(face.mIndices[1]);
			indicies.push_back(face.mIndices[2]);
		}


		// create vertex buffer
		m_vertexBuffer = mvBufferRegistry::Request<mvVertexBuffer>(graphics, name, verticies, vertexLayout, false);

		// create index buffer
		m_indexBuffer = mvBufferRegistry::Request<mvIndexBuffer>(graphics, name, indicies, false);

		auto techniques = mat.getTechniques();

		for (auto& tech : techniques)
			addTechnique(tech);

	}

	void mvMesh::submit(mvRenderGraph& graph, glm::mat4 accumulatedTranform) const
	{
		m_transform = accumulatedTranform;
		mvDrawable::submit(graph);
	}

	glm::mat4 mvMesh::getTransform() const
	{
		return m_transform;
	}

}