#include "mvCamera.h"
#include <algorithm>
#include "mvGraphics.h"

namespace Marvel {

	template<typename T>
	T wrap_angle(T theta) noexcept
	{
		constexpr T twoPi = (T)2 * (T)PI_D;
		const T mod = (T)fmod(theta, twoPi);
		if (mod > (T)PI_D)
			return mod - twoPi;
		else if (mod < -(T)PI_D)
			return mod + twoPi;
		return mod;
	}

	mvCamera::mvCamera(mvGraphics& graphics, glm::vec3 homePos, float homePitch, float homeYaw,
		float width, float height)
		:
		m_pos(homePos),
		m_pitch(homePitch),
		m_yaw(homeYaw),
		m_proj(graphics, width, height, 0.5f, 400.0f)
	{

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

}