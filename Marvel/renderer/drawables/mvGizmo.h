#pragma once
#include "mvDrawable.h"
#include <memory>
#include "mvMaterial.h"

namespace Marvel {

	// forward declarations
	class mvGraphics;

	class mvGizmo : public mvDrawable
	{


	public:

		mvGizmo(mvGraphics& graphics);

		glm::mat4 getTransform() const override;
		void setTransform(glm::mat4 transform);

	private:

		glm::mat4 m_transform;

	};

}