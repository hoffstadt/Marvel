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

	mvMesh::mvMesh(mvGraphics& graphics, const aiMesh& mesh, const aiMaterial& material, const std::filesystem::path& path, float scale)
	{

		const auto rootPath = path.parent_path().string() + "\\";

		m_transform = glm::identity<glm::mat4>();

		// create topology
		m_topology = std::make_shared<mvTopology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mvMaterial mat = mvMaterial(graphics, material, rootPath);

		// create vertex layout
		const Marvel::mvVertexLayout& vertexLayout = mat.getLayout();
		
		std::vector<float> verticies;
		verticies.reserve(mesh.mNumVertices * 14);
		std::vector<unsigned short> indicies;
		indicies.reserve(mesh.mNumFaces * 3);


		// this is nasty and requires the layout order to be correct. 
		// a more robust system is needed.
		for (int i = 0; i < mesh.mNumVertices; i++)
		{
			if (vertexLayout.hasElement(ElementType::Position3D))
			{
				verticies.push_back(mesh.mVertices[i].x * scale);
				verticies.push_back(mesh.mVertices[i].y * scale);
				verticies.push_back(mesh.mVertices[i].z * scale);
			}

			if (vertexLayout.hasElement(ElementType::Normal))
			{
				verticies.push_back(mesh.mNormals[i].x);
				verticies.push_back(mesh.mNormals[i].y);
				verticies.push_back(mesh.mNormals[i].z);
			}

			if (vertexLayout.hasElement(ElementType::Texture2D))
			{
				verticies.push_back(mesh.mTextureCoords[0][i].x);
				verticies.push_back(mesh.mTextureCoords[0][i].y);
			}

			if (vertexLayout.hasElement(ElementType::Tangent))
			{
				verticies.push_back(mesh.mTangents[i].x);
				verticies.push_back(mesh.mTangents[i].y);
				verticies.push_back(mesh.mTangents[i].z);
			}

			if (vertexLayout.hasElement(ElementType::Bitangent))
			{
				verticies.push_back(mesh.mBitangents[i].x);
				verticies.push_back(mesh.mBitangents[i].y);
				verticies.push_back(mesh.mBitangents[i].z);
			}
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
		m_vertexBuffer = std::make_shared<mvVertexBuffer>(graphics, verticies, vertexLayout);

		// create index buffer
		m_indexBuffer = std::make_shared<mvIndexBuffer>(graphics, indicies);

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


	void mvMesh::show_imgui_windows(const char* name)
	{

		auto angles = ExtractEulerAngles(m_transform);
		auto trans = ExtractTranslation(m_transform);

		static TransformParameters tf = {
		angles.x,
		angles.y,
		angles.z,
		trans.x,
		trans.y,
		trans.z
		};

		if (ImGui::Begin(name))
		{
			bool dirty = false;
			const auto dcheck = [&dirty](bool changed) {dirty = dirty || changed; };
			ImGui::TextColored({ 0.4f,1.0f,0.6f,1.0f }, "Translation");
			dcheck(ImGui::SliderFloat("X", &tf.x, -60.f, 60.f));
			dcheck(ImGui::SliderFloat("Y", &tf.y, -60.f, 60.f));
			dcheck(ImGui::SliderFloat("Z", &tf.z, -60.f, 60.f));
			ImGui::TextColored({ 0.4f,1.0f,0.6f,1.0f }, "Orientation");
			dcheck(ImGui::SliderAngle("X-rotation", &tf.xRot, -180.0f, 180.0f));
			dcheck(ImGui::SliderAngle("Y-rotation", &tf.yRot, -180.0f, 180.0f));
			dcheck(ImGui::SliderAngle("Z-rotation", &tf.zRot, -180.0f, 180.0f));
			//ImGui::ColorEdit3("Material Color", &m_material->m_cbData.materialColor.x);
			//ImGui::ColorEdit3("Specular Color", &m_material->m_cbData.specularColor.x);
			//ImGui::SliderFloat("Specular Weight", &m_material->m_cbData.specularWeight, 0.0f, 100.0f);
			//ImGui::SliderFloat("Specular Gloss", &m_material->m_cbData.specularGloss, 0.0f, 100.0f);

			if (dirty)
			{
				m_transform = glm::translate(trans) *
					glm::rotate(angles[0], glm::vec3{ 0.0f, 0.0f, 1.0f }) *
					glm::rotate(angles[1], glm::vec3{ 0.0f, 1.0f, 0.0f }) *
					glm::rotate(angles[2], glm::vec3{ 1.0f, 0.0f, 0.0f });
			}
		}
		ImGui::End();


	}

}