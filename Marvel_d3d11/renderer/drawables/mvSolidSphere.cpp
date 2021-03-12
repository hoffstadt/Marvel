#include "mvSolidSphere.h"
#include <imgui.h>
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "Sphere.h"

namespace Marvel {

	mvSolidSphere::mvSolidSphere(mvGraphics& graphics, float radius, glm::vec3 color, int simple)
	{

		// create topology
		m_topology = new mvTopology(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// create vertex layout
		mvVertexLayout vl;
		vl.append(ElementType::Position3D);
		vl.append(ElementType::Normal);

		std::vector<float> verticies;
		std::vector<unsigned short> indicies;
		std::vector<float> normals;
		Sphere sphere(radius);

		verticies = sphere.vertices;
		indicies = sphere.indices;
		normals = sphere.normals;

		std::vector<float> nverticies;
		for (int i = 0; i < verticies.size(); i = i + 3)
		{
			nverticies.push_back(verticies[i]);
			nverticies.push_back(verticies[i+1]);
			nverticies.push_back(verticies[i+2]);
			nverticies.push_back(normals[i]);
			nverticies.push_back(normals[i+1]);
			nverticies.push_back(normals[i+2]);
		}

		// create vertex buffer
		m_vertexBuffer = new mvVertexBuffer(graphics, nverticies, vl);

		// create index buffer
		m_indexBuffer = new mvIndexBuffer(graphics, indicies);

		if (simple == 1)
		{
			// create vertex shader
			auto vshader = new mvVertexShader(graphics, "../../../Marvel_d3d11/shaders/vs_shader.hlsl");
			addBindable(vshader);
			addBindable(new mvInputLayout(graphics, vl, *vshader));
			addBindable(new mvPixelShader(graphics, "../../../Marvel_d3d11/shaders/ps_shader.hlsl"));
			addBindable(new mvNullGeometryShader(graphics));
			addBindable(new mvTransformConstantBuffer(graphics));
		}
		else if(simple == 2)
		{
			// create vertex shader
			auto vshader = new mvVertexShader(graphics, "../../../Marvel_d3d11/shaders/vs_flat.hlsl");
			addBindable(vshader);
			addBindable(new mvInputLayout(graphics, vl, *vshader));
			addBindable(new mvPixelShader(graphics, "../../../Marvel_d3d11/shaders/ps_flat.hlsl"));
			addBindable(new mvGeometryShader(graphics, "../../../Marvel_d3d11/shaders/gs_flat.hlsl"));
			addBindable(new mvTransformConstantBuffer(graphics));
			m_material = new mvMaterial(graphics, color);
			addBindable(m_material);
		}
		else
		{
			// create vertex shader
			auto vshader = new mvVertexShader(graphics, "../../../Marvel_d3d11/shaders/vs_phong.hlsl");
			addBindable(vshader);
			addBindable(new mvInputLayout(graphics, vl, *vshader));
			addBindable(new mvPixelShader(graphics, "../../../Marvel_d3d11/shaders/ps_phong.hlsl"));
			addBindable(new mvNullGeometryShader(graphics));
			addBindable(new mvTransformConstantBuffer(graphics));
			m_material = new mvMaterial(graphics, color);
			addBindable(m_material);
		}

	}

	glm::mat4 mvSolidSphere::getTransform() const
	{
		return glm::translate(glm::vec3{ m_x, m_y, m_z }) *
			glm::rotate(m_zangle, glm::vec3{ 0.0f, 0.0f, 1.0f }) *
			glm::rotate(m_yangle, glm::vec3{ 0.0f, 1.0f, 0.0f }) *
			glm::rotate(m_xangle, glm::vec3{ 1.0f, 0.0f, 0.0f });
	}

	void mvSolidSphere::show_imgui_windows(const char* name)
	{
		if (ImGui::Begin(name))
		{
			ImGui::SliderFloat("X-Pos", &m_x, -50.0f, 50.0f);
			ImGui::SliderFloat("Y-Pos", &m_y, -50.0f, 50.0f);
			ImGui::SliderFloat("Z-Pos", &m_z, -50.0f, 50.0f);
			ImGui::SliderFloat("X-Angle", &m_xangle, -50.0f, 50.0f);
			ImGui::SliderFloat("Y-Angle", &m_yangle, -50.0f, 50.0f);
			ImGui::SliderFloat("Z-Angle", &m_zangle, -50.0f, 50.0f);
			ImGui::ColorEdit3("Material Color", &m_material->m_cbData.materialColor.x);
			ImGui::ColorEdit3("Specular Color", &m_material->m_cbData.specularColor.x);
			ImGui::SliderFloat("Specular Weight", &m_material->m_cbData.specularWeight, 0.0f, 100.0f);
			ImGui::SliderFloat("Specular Gloss", &m_material->m_cbData.specularGloss, 0.0f, 100.0f);
		}
		ImGui::End();
	}

	void mvSolidSphere::setPosition(float x, float y, float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	void mvSolidSphere::setRotation(float x, float y, float z)
	{
		m_xangle = x;
		m_yangle = y;
		m_zangle = z;
	}

}