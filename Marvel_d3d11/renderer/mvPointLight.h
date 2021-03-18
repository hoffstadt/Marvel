#pragma once
#include "mvGraphics.h"
#include "mvConstantBuffer.h"
#include "mvSolidSphere.h"

namespace Marvel {

	class mvPointLight
	{

	public:

		mvPointLight(mvGraphics& graphics, glm::vec3 pos = { 0.0f,0.0f,0.5f });

		void show_imgui_windows();
		void bind(mvGraphics& graphics, glm::mat4 view);
		void setPosition(float x, float y, float z);

		mvSolidSphere* getSphere();

	private:

		mvSolidSphere                          m_sphere;
		std::unique_ptr<mvPixelConstantBuffer> m_buffer;
		std::unique_ptr<mvBuffer>              m_bufferData;

	};

}