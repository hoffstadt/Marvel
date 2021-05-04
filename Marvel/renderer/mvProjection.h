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

		// called when viewport is resized
		void update(int width, int height);

	private:

		float m_width;
		float m_height;
		float m_nearZ;
		float m_farZ;

	};

}