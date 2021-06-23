#include "mvMesh.h"
#include "mvMarvelUtils.h"
#include <imgui.h>
#include "mvGraphics.h"
#include "mvMaterial.h"
#include "mvPBRMaterial.h"
#include "mvPhongMaterial.h"
#include "mvCommonBindables.h"
#include "mvStep.h"
#include "mvObjMaterial.h"

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

	mvMesh::mvMesh(mvGraphics& graphics, const std::string& name, const mvObjMesh& mesh,
		const mvObjMaterial& material, const std::filesystem::path& path, float scale)
	{

		const auto rootPath = path.parent_path().string() + "\\";

		if(material.pbr)
			m_material = std::make_shared<mvPBRMaterial>(graphics, rootPath, material);
		else
			m_material = std::make_shared<mvPhongMaterial>(graphics, rootPath, material);

		// create vertex layout
		const Marvel::mvVertexLayout& vertexLayout = m_material->getLayout();
		
		std::vector<float> verticies;
		std::vector<unsigned int> indicies = mesh.indicies;

		for (size_t i = 0; i < mesh.vertexCount; i++)
		{
			verticies.push_back(mesh.averticies[i].position.x * scale);
			verticies.push_back(mesh.averticies[i].position.y * scale);
			verticies.push_back(mesh.averticies[i].position.z * scale);

			verticies.push_back(mesh.averticies[i].normal.x);
			verticies.push_back(mesh.averticies[i].normal.y);
			verticies.push_back(mesh.averticies[i].normal.z);

			verticies.push_back(mesh.averticies[i].tangent.x);
			verticies.push_back(mesh.averticies[i].tangent.y);
			verticies.push_back(mesh.averticies[i].tangent.z);

			verticies.push_back(mesh.averticies[i].bitangent.x);
			verticies.push_back(mesh.averticies[i].bitangent.y);
			verticies.push_back(mesh.averticies[i].bitangent.z);

			verticies.push_back(mesh.averticies[i].uv.x);
			verticies.push_back(mesh.averticies[i].uv.y);
		}

		// create vertex buffer
		m_vertexBuffer = mvBufferRegistry::Request<mvVertexBuffer>(graphics, name, verticies, vertexLayout, false);

		// create index buffer
		m_indexBuffer = mvBufferRegistry::Request<mvIndexBuffer>(graphics, name, indicies, false);

		auto steps = m_material->getSteps();

		for (auto& tech : steps)
			addStep(tech);

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

	void mvMesh::show_imgui_controls()
	{
		if (m_material)
			m_material->showControls();

	}

}