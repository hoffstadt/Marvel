#pragma once
#include "mvDrawable.h"
#include <memory>
#include "mvMaterial.h"

namespace Marvel {

	// forward declarations
	class mvGraphics;

	class mvPointCloud : public mvDrawable
	{


	public:

		mvPointCloud(mvGraphics& graphics, const std::string& name, glm::vec3 color);

		glm::mat4 getTransform() const override;

		void show_imgui_windows(const char* name);
		void setPosition(float x, float y, float z);
		void setRotation(float x, float y, float z);

		std::vector<float>& getVertices();
		std::vector<unsigned int>& getIndices();

	private:

		float m_x = 0.0f;
		float m_y = 0.0f;
		float m_z = 0.0f;
		float m_xangle = 0.0f;
		float m_yangle = 0.0f;
		float m_zangle = 0.0f;

	};

}