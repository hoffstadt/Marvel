#pragma once
#include "mvDrawable.h"
#include <memory>
#include "mvMaterial.h"

namespace Marvel {

	// forward declarations
	class mvGraphics;

	class mvFrustum : public mvDrawable
	{


	public:

		mvFrustum(mvGraphics& graphics, const std::string& name, float width, float height, float nearZ, float farZ);

		void setPos(float x, float y, float z);
		void setRotation(float x, float y, float z);
		glm::mat4 getTransform() const override;

	private:

		glm::vec3 m_pos = {0.0f, 0.0f, 0.0f};
		glm::vec3 m_rot = {0.0f, 0.0f, 0.0f};

	};

}