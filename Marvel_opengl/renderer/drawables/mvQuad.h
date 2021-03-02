#pragma once
#include "mvDrawable.h"

namespace Marvel {

	// forward declarations
	class mvGraphics;
	class mvMaterial;

	class mvQuad : public mvDrawable
	{


	public:

		mvQuad(mvGraphics& graphics);

		void setPosition(float x, float y, float z);
		void setRotation(float x, float y, float z);

	private:

		float m_x = 0.0f;
		float m_y = 0.0f;
		float m_z = 0.0f;
		float m_xangle = 0.0f;
		float m_yangle = 0.0f;
		float m_zangle = 0.0f;

	};

}