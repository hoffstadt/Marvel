#pragma once
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp> 
#include "mvProjection.h"

namespace Marvel {

	// forward declarations
	class mvGraphics;

	class mvCamera
	{

	public:

		mvCamera(mvGraphics& graphics, glm::vec3 homePos = { 0.0f,0.0f, 0.0f },
			glm::vec3 homeRot = { 0.0f,0.0f, 0.0f });

		glm::mat4 getMatrix() const;

		void bind(mvGraphics& graphics) const;
		void rotate(float dx, float dy);
		void translate(float dx, float dy, float dz);
		void setRotation(float x, float y, float z);
		void setPosition(float x, float y, float z);

	private:

		glm::vec3    m_pos;
		glm::vec3    m_rot;
		mvProjection m_proj;

		static constexpr float s_travelSpeed = 12.0f;
		static constexpr float s_rotationSpeed = 0.004f;

	};

}