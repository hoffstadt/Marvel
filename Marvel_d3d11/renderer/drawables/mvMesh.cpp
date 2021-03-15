#include "mvMesh.h"
#include "mvMarvelUtils.h"
#include <imgui.h>
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "assimp/Importer.hpp"
#include "assimp/Scene.h"
#include "assimp/postprocess.h"

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

		// create vertex layout
		Marvel::mvVertexLayout vertexLayout;
		vertexLayout.append(ElementType::Position3D);
		vertexLayout.append(ElementType::Normal);
		vertexLayout.append(ElementType::Texture2D);

		aiString texFileName;

		if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
		{

			m_material = std::make_shared<mvMaterial>(graphics, glm::vec3{ 1.0f, 1.0f, 1.0f });
			m_material->m_cbData.specularWeight = 0.0f;
			m_material->m_cbData.specularGloss = 75.0f;
			addBindable(m_material);
			addBindable(mvBindableRegistry::GetBindable("sampler"));
			addBindable(std::make_shared<mvTexture>(graphics, rootPath + texFileName.C_Str()));
		}
		else
		{
			m_material = std::make_shared<mvMaterial>(graphics, glm::vec3{ 1.0f, 1.0f, 1.0f });
			addBindable(m_material);
		}

		std::vector<float> verticies;
		verticies.reserve(mesh.mNumVertices * 8);
		std::vector<unsigned short> indicies;
		indicies.reserve(mesh.mNumFaces * 3);

		for (int i = 0; i < mesh.mNumVertices; i++)
		{
			verticies.push_back(mesh.mVertices[i].x * scale);
			verticies.push_back(mesh.mVertices[i].y * scale);
			verticies.push_back(mesh.mVertices[i].z * scale);
			verticies.push_back(mesh.mNormals[i].x);
			verticies.push_back(mesh.mNormals[i].y);
			verticies.push_back(mesh.mNormals[i].z);
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
		m_vertexBuffer = std::make_shared<mvVertexBuffer>(graphics, verticies, vertexLayout);

		// create index buffer
		m_indexBuffer = std::make_shared<mvIndexBuffer>(graphics, indicies);

		// create vertex shader
		auto vshader = mvBindableRegistry::GetBindable("vs_texture");

		addBindable(vshader);
		addBindable(std::make_shared<mvInputLayout>(graphics, vertexLayout, 
			static_cast<mvVertexShader*>(vshader.get())));
		addBindable(mvBindableRegistry::GetBindable("ps_texture"));
		addBindable(mvBindableRegistry::GetBindable("gs_null"));
		addBindable(mvBindableRegistry::GetBindable("transCBuf"));

	}

	void mvMesh::submit(glm::mat4 accumulatedTranform) const
	{
		m_transform = accumulatedTranform;
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
			ImGui::ColorEdit3("Material Color", &m_material->m_cbData.materialColor.x);
			ImGui::ColorEdit3("Specular Color", &m_material->m_cbData.specularColor.x);
			ImGui::SliderFloat("Specular Weight", &m_material->m_cbData.specularWeight, 0.0f, 100.0f);
			ImGui::SliderFloat("Specular Gloss", &m_material->m_cbData.specularGloss, 0.0f, 100.0f);

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