#include "mvCamera.h"
#include <algorithm>
#include "mvGraphics.h"

namespace Marvel {

	constexpr float PI = 3.14159265f;
	constexpr double PI_D = 3.1415926535897932;

	template<typename T>
	T wrap_angle(T theta) noexcept
	{
		constexpr T twoPi = (T)2 * (T)PI_D;
		const T mod = (T)fmod(theta, twoPi);
		if (mod > (T)PI_D)
		{
			return mod - twoPi;
		}
		else if (mod < -(T)PI_D)
		{
			return mod + twoPi;
		}
		return mod;
	}

	mvCamera::mvCamera(mvGraphics& graphics, glm::vec3 homePos, glm::vec3 homeRot)
		:
		m_pos(homePos),
		m_rot(homeRot),
		m_proj(graphics, 640.0f, 480.0f, 0.1f, 100.0f)
	{

	}

	glm::mat4 mvCamera::getMatrix() const
	{

		glm::mat4 xmatrix = glm::rotate(m_rot.x, glm::vec3{ 1.0f, 0.0f, 0.0f });
		glm::mat4 ymatrix = glm::rotate(m_rot.y, glm::vec3{ 0.0f, 1.0f, 0.0f });

		glm::mat4 roll_pitch_yaw = xmatrix * ymatrix;

		glm::vec4 forward_base_vector = { 0.0f, 0.0f, -1.0f, 0.0f };

		glm::vec4 look_vector = roll_pitch_yaw * forward_base_vector;

		glm::vec3 lpos = { look_vector.x, look_vector.y, look_vector.z};

		glm::vec3 look_target = m_pos + lpos;

		glm::mat4 camera_matrix = glm::lookAt(m_pos, look_target, glm::vec3{ 0.0f, 1.0f, 0.0f });

//		return glm::inverse(camera_matrix);
		return camera_matrix;
	}

	void mvCamera::bind(mvGraphics& graphics) const
	{
		graphics.setCamera(getMatrix());
		graphics.setProjection(m_proj.getMatrix());
	}

	void mvCamera::rotate(float dx, float dy)
	{
		m_rot.x = wrap_angle(m_rot.x + dx * s_rotationSpeed);
		m_rot.y = std::clamp(m_rot.y + dy * s_rotationSpeed, 0.995f * -PI / 2.0f, 0.995f * PI / 2.0f);
	}

	void mvCamera::setRotation(float x, float y, float z)
	{
		m_rot.x = x;
		m_rot.y = y;
		m_rot.z = z;
	}

	void mvCamera::setPosition(float x, float y, float z)
	{
		m_pos = glm::vec3{ x, y, z };
	}

	void mvCamera::translate(float dx, float dy, float dz)
	{

		glm::mat4 xmatrix = glm::rotate(m_rot.x, glm::vec3{ 1.0f, 0.0f, 0.0f });
		glm::mat4 ymatrix = glm::rotate(m_rot.y, glm::vec3{ 0.0f, 1.0f, 0.0f });
		glm::mat4 scale = glm::scale(glm::vec3{ s_travelSpeed, s_travelSpeed, s_travelSpeed });
		glm::vec4 translation = glm::vec4{ dx, dy, dz, 0.0f };

		translation = (xmatrix * ymatrix * scale) * translation;
		
		m_pos = {
			m_pos.x + translation.x,
			m_pos.y + translation.y,
			m_pos.z + translation.z
		};
	}

}