#pragma once
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp> 

namespace Marvel {

	// forward declarations
	class mvGraphics;

	class mvProjection
	{
	public:

		mvProjection(mvGraphics& graphics, float width, float height,
			float nearZ, float farZ);

		glm::mat4 getMatrix() const;

	private:

		float m_width;
		float m_height;
		float m_nearZ;
		float m_farZ;

	};

}