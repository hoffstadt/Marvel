#include "mvCamera.h"
#include <algorithm>
#include <imgui.h>
#include "mvGraphics.h"

namespace Marvel {

	template<typename T>
	static T wrap_angle(T theta) noexcept
	{
		constexpr T twoPi = (T)2 * (T)PI_D;
		const T mod = (T)fmod(theta, twoPi);
		if (mod > (T)PI_D)
			return mod - twoPi;
		else if (mod < -(T)PI_D)
			return mod + twoPi;
		return mod;
	}

	mvCamera::mvCamera(mvGraphics& graphics, const std::string& name, glm::vec3 homePos, float homePitch, float homeYaw,
		float width, float height, float nearZ, float farZ, bool ortho)
		:
		m_pos(homePos),
		m_pitch(homePitch),
		m_yaw(homeYaw),
		m_proj(graphics, name, width, height, nearZ, farZ, ortho)
	{
		m_proj.setPos(homePos.x, homePos.y, homePos.z);
		m_proj.setRotation(homePitch, homeYaw, 0.0f);
	}

	void mvCamera::setPos(float x, float y, float z)
	{
		m_pos.x = x;
		m_pos.y = y;
		m_pos.z = z;
		m_proj.setPos(x, y, z);
	}

	void mvCamera::setRotation(float x, float y, float z)
	{
		m_pitch = x;
		m_yaw = y;
		m_proj.setRotation(x, y, z);
	}

	void mvCamera::updateProjection(int width, int height)
	{
		m_proj.update(width, height);
	}

	void mvCamera::linkTechniques(mvRenderGraph& graph)
	{
		m_proj.linkTechniques(graph);
	}

	void mvCamera::submit(mvRenderGraph& graph) const
	{
		m_proj.submit(graph);
	}

	glm::mat4 mvCamera::getMatrix() const
	{

		glm::mat4 roll_pitch_yaw = glm::yawPitchRoll(m_yaw, m_pitch, 0.0f);

		glm::vec4 forward_base_vector = { 0.0f, 0.0f, 1.0f, 0.0f };

		glm::vec4 look_vector = roll_pitch_yaw * forward_base_vector;

		glm::vec3 lpos = { look_vector.x, look_vector.y, look_vector.z };

		glm::vec3 look_target = m_pos + lpos;

		glm::mat4 camera_matrix = glm::lookAtLH(m_pos, look_target, glm::vec3{ 0.0f, 1.0f, 0.0f });

		return camera_matrix;
	}

	glm::mat4 mvCamera::getProjection() const
	{
		return m_proj.getMatrix();
	}

	void mvCamera::bind(mvGraphics& graphics) const
	{
		graphics.setCamera(getMatrix());
		graphics.setProjection(m_proj.getMatrix());
	}

	void mvCamera::rotate(float dx, float dy)
	{
		m_yaw = wrap_angle(m_yaw + dx * s_rotationSpeed);
		m_pitch = std::clamp(m_pitch + dy * s_rotationSpeed, 0.995f * -PI / 2.0f, 0.995f * PI / 2.0f);
	}

	void mvCamera::translate(float dx, float dy, float dz)
	{
		glm::mat4 roll_pitch_yaw = glm::yawPitchRoll(m_yaw, m_pitch, 0.0f);
		glm::mat4 scale = glm::scale(glm::vec3{ s_travelSpeed, s_travelSpeed, s_travelSpeed });
		glm::vec4 translation = glm::vec4{ dx, dy, dz, 0.0f };

		translation = (roll_pitch_yaw * scale) * translation;

		m_pos = {
			m_pos.x + translation.x,
			m_pos.y + translation.y,
			m_pos.z + translation.z
		};
	}

	void mvCamera::show_imgui_windows()
	{
		bool rotDirty = false;
		bool posDirty = false;
		const auto dcheck = [](bool d, bool& carry) { carry = carry || d; };

		ImGui::Begin("Camera");

		ImGui::Text("Position");
		dcheck(ImGui::SliderFloat("X", &m_pos.x, -80.0f, 80.0f, "%.1f"), posDirty);
		dcheck(ImGui::SliderFloat("Y", &m_pos.y, -80.0f, 80.0f, "%.1f"), posDirty);
		dcheck(ImGui::SliderFloat("Z", &m_pos.z, -80.0f, 80.0f, "%.1f"), posDirty);

		ImGui::Text("Orientation");
		dcheck(ImGui::SliderAngle("Pitch", &m_pitch, 0.995f * -90.0f, 0.995f * 90.0f), rotDirty);
		dcheck(ImGui::SliderAngle("Yaw", &m_yaw, -180.0f, 180.0f), rotDirty);

		if (rotDirty)
		{
			const glm::vec3 angles = { m_pitch,m_yaw,0.0f };
			m_proj.setRotation(m_pitch, m_yaw, 0.0f);
		}
		if (posDirty)
		{
			m_proj.setPos(m_pos.x, m_pos.y, m_pos.z);
		}

		ImGui::End();
	}

	mvOrthoCamera::mvOrthoCamera(mvGraphics& graphics, const std::string& name, glm::vec3 homePos, float left, float right, float bottom, float top,
		float nearZ, float farZ)
		:
		m_pos(homePos),
		m_projection(glm::orthoLH(left, right, bottom, top, nearZ, farZ)),
		m_left(left),
		m_right(right),
		m_top(top),
		m_bottom(bottom),
		m_near(nearZ),
		m_far(farZ),
		m_viewLightDir(glm::vec3{ 0.0f, 1.0f, 0.0f })
	{
	}

	glm::mat4 mvOrthoCamera::getMatrix() const
	{

		//glm::mat4 roll_pitch_yaw = glm::yawPitchRoll(m_yaw, m_pitch, 0.0f);

		//glm::vec4 forward_base_vector = { 0.0f, 0.0f, 1.0f, 0.0f };

		//glm::vec4 look_vector = roll_pitch_yaw * forward_base_vector;

		//glm::vec3 lpos = { look_vector.x, look_vector.y, look_vector.z };

		//glm::vec3 look_target = m_pos + lpos;

		//glm::mat4 camera_matrix = glm::lookAtLH(m_pos, look_target, glm::vec3{ 0.0f, 1.0f, 0.0f });

		//return camera_matrix;

		//auto result = glm::lookAtLH(m_pos, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f });
		auto result = glm::lookAtLH(m_pos, m_pos + m_viewLightDir, glm::vec3{ 0.0f, 0.0f, 1.0f });
		return result;
	}

	glm::mat4 mvOrthoCamera::getProjection() const
	{
		return m_projection;
	}

	void mvOrthoCamera::bind(mvGraphics& graphics) const
	{
		graphics.setCamera(getMatrix());
		graphics.setProjection(m_projection);
	}

	void mvOrthoCamera::show_imgui_windows()
	{
		bool projDirty = false;
		bool posDirty = false;
		const auto dcheck = [](bool d, bool& carry) { carry = carry || d; };

		ImGui::Begin("Directional Shadow Camera");

		ImGui::Text("Position");
		dcheck(ImGui::SliderFloat("X", &m_pos.x, -80.0f, 80.0f, "%.1f"), posDirty);
		dcheck(ImGui::SliderFloat("Y", &m_pos.y, -80.0f, 80.0f, "%.1f"), posDirty);
		dcheck(ImGui::SliderFloat("Z", &m_pos.z, -80.0f, 80.0f, "%.1f"), posDirty);

		ImGui::Text("Orientation");
		dcheck(ImGui::SliderFloat("left", &m_left, -500.0f, 0.0f, "%.0f"), projDirty);
		dcheck(ImGui::SliderFloat("right", &m_right,  0.0f, 500.0f, "%.0f"), projDirty);
		dcheck(ImGui::SliderFloat("top", &m_top,  0.0f, 500.0f, "%.0f"), projDirty);
		dcheck(ImGui::SliderFloat("bottom", &m_bottom,  -500.0f, 0.0f, "%.0f"), projDirty);
		dcheck(ImGui::SliderFloat("near", &m_near,  -500.0f, 0.0f, "%.0f"), projDirty);
		dcheck(ImGui::SliderFloat("far", &m_far,  0.0f, 500.0f, "%.0f"), projDirty);

		if (projDirty)
			m_projection = glm::orthoLH(m_left, m_right, m_bottom, m_top, m_near, m_far);

		ImGui::End();
	}
}