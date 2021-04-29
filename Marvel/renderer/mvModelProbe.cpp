#include "mvModelProbe.h"
#include "mvNode.h"
#include "mvModel.h"
#include "mvMarvelUtils.h"
#include <imgui.h>

namespace Marvel {

	mvModelProbe::mvModelProbe(const std::string& name)
	{
		m_name = name;
	}

	void mvModelProbe::spawnWindow(mvModel& model)
	{
		ImGui::Begin(m_name.c_str());
		ImGui::Columns(2, nullptr, true);
		model.accept(*this);

		ImGui::NextColumn();
		if (m_selectedNode != nullptr)
		{
			bool dirty = false;
			const auto dcheck = [&dirty](bool changed) {dirty = dirty || changed; };
			auto& tf = ResolveTransform();
			ImGui::TextColored({ 0.4f,1.0f,0.6f,1.0f }, "Translation");
			dcheck(ImGui::SliderFloat("X", &tf.x, -60.f, 60.f));
			dcheck(ImGui::SliderFloat("Y", &tf.y, -60.f, 60.f));
			dcheck(ImGui::SliderFloat("Z", &tf.z, -60.f, 60.f));
			ImGui::TextColored({ 0.4f,1.0f,0.6f,1.0f }, "Orientation");
			dcheck(ImGui::SliderAngle("X-rotation", &tf.xRot, -180.0f, 180.0f));
			dcheck(ImGui::SliderAngle("Y-rotation", &tf.yRot, -180.0f, 180.0f));
			dcheck(ImGui::SliderAngle("Z-rotation", &tf.zRot, -180.0f, 180.0f));
			if (dirty)
			{

				// create translation matrix
				glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(tf.x, tf.y, tf.z));

				// create rotation matrix
				glm::mat4 rotationx = glm::rotate(glm::identity<glm::mat4>(), tf.xRot, glm::vec3(1.0f, 0.0f, 0.0f));
				glm::mat4 rotationy = glm::rotate(glm::identity<glm::mat4>(), tf.yRot, glm::vec3(0.0f, 1.0f, 0.0f));
				glm::mat4 rotationz = glm::rotate(glm::identity<glm::mat4>(), tf.zRot, glm::vec3(0.0f, 0.0f, 1.0f));

				glm::mat4 transformation = translation * rotationz * rotationy * rotationx;
				//glm::mat4 transformation = rotationx * rotationy * rotationz * translation;

				m_selectedNode->setAppliedTransform(transformation);
			}

		}
		ImGui::End();
	}

	bool mvModelProbe::pushNode(mvNode& node)
	{
		// if there is no selected node, set selectedId to an impossible value
		const int selectedId = (m_selectedNode == nullptr) ? -1 : m_selectedNode->getID();

		// build up flags for current node
		const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
			| ((node.getID() == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
			| (node.hasChildren() ? 0 : ImGuiTreeNodeFlags_Leaf);
		
		// render this node
		const auto expanded = ImGui::TreeNodeEx(
			(void*)(intptr_t)node.getID(),
			node_flags, node.getName().c_str()
		);
		// processing for selecting node
		if (ImGui::IsItemClicked())
			m_selectedNode = &node;
	
		// signal if children should also be recursed
		return expanded;
	}

	void mvModelProbe::popNode(mvNode& node)
	{
		ImGui::TreePop();
	}


	mvModelProbe::TransformParameters& mvModelProbe::ResolveTransform()
	{
		const auto id = m_selectedNode->getID();
		auto i = m_transformParams.find(id);
		if (i == m_transformParams.end())
		{
			return LoadTransform(id);
		}
		return i->second;
	}

	mvModelProbe::TransformParameters& mvModelProbe::LoadTransform(int id)
	{
		const auto& applied = m_selectedNode->getAppliedTransform();
		const auto angles = ExtractEulerAngles(applied);
		const auto translation = ExtractTranslation(applied);
		TransformParameters tp;
		tp.zRot = angles.z;
		tp.xRot = angles.x;
		tp.yRot = angles.y;
		tp.x = translation.x;
		tp.y = translation.y;
		tp.z = translation.z;
		return m_transformParams.insert({ id,{ tp } }).first->second;
	}
}