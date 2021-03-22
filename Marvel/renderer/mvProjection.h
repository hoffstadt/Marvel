#pragma once
#include "mvMath.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvProjection
	//-----------------------------------------------------------------------------
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