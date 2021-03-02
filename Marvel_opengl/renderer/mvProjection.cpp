#include "mvProjection.h"
#include <cmath>

namespace Marvel {

	mvProjection::mvProjection(mvGraphics& graphics, float width, float height, float nearZ, float farZ)
		:
		m_width(width),
		m_height(height),
		m_nearZ(nearZ),
		m_farZ(farZ)
	{

	}

	glm::mat4 mvProjection::getMatrix() const
	{

		float fieldOfView = 45 *  M_PI / 180.0f;
		float aspect = m_width / m_height;

		return glm::perspectiveRH(fieldOfView, aspect, m_nearZ, m_farZ);
		//return glm::identity<glm::mat4>();
	}

}