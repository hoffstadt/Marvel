#pragma once
#include "mvDrawable.h"
#include <memory>
#include "mvMaterial.h"

namespace Marvel {

	// forward declarations
	class mvGraphics;

	class mvSolidSphere : public mvDrawable
	{


	public:

		mvSolidSphere(mvGraphics& graphics, float radius, glm::vec3 color, int simple);

		glm::mat4 getTransform() const override;
		void setTransform(glm::mat4 transform);

		void show_imgui_windows(const char* name);
		void setPosition(float x, float y, float z);
		void setRotation(float x, float y, float z);

	private:

		float m_x = 0.0f;
		float m_y = 0.0f;
		float m_z = 0.0f;
		float m_xangle = 0.0f;
		float m_yangle = 0.0f;
		float m_zangle = 0.0f;
		glm::mat4 m_transform;

	};

}