#include "mvProjection.h"

namespace Marvel {

	mvProjection::mvProjection(mvGraphics& graphics, float width, float height, float nearZ, float farZ)
		:
		m_width(width),
		m_height(height),
		m_nearZ(nearZ),
		m_farZ(farZ)
	{

	}


	void mvProjection::update(int width, int height)
	{
		m_width = width;
		m_height = height;
	}

	glm::mat4 mvProjection::getMatrix() const
	{

		float fieldOfView = 45 * PI / 180.0f;
		float aspect = m_width / m_height;

		return glm::perspectiveLH(fieldOfView, aspect, m_nearZ, m_farZ);
	}

}