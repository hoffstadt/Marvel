#pragma once
#include "mvDrawable.h"
#include <memory>
#include "mvMaterial.h"

namespace Marvel {

	// forward declarations
	class mvGraphics;

	class mvSolidModel : public mvDrawable
	{


	public:

		mvSolidModel(mvGraphics& graphics, std::vector<float> vertices, std::vector<unsigned int> indices, glm::vec3 color);

		glm::mat4 getTransform() const override;

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

	};

}