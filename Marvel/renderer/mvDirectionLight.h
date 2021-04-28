#pragma once
#include "mvConstantBuffer.h"
#include "mvSolidSphere.h"
#include "mvCamera.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvRenderGraph;

	//-----------------------------------------------------------------------------
	// mvDirectionLight
	//-----------------------------------------------------------------------------
	class mvDirectionLight
	{

		friend class mvDirectionLightManager;

	public:

		mvDirectionLight(mvGraphics& graphics, glm::vec3 dir = { 0.0f,0.0f,0.5f });

	};

	//-----------------------------------------------------------------------------
	// mvDirectionLightManager
	//-----------------------------------------------------------------------------
	class mvDirectionLightManager
	{

	public:

		static constexpr int MaxLights = 16;

		mvDirectionLightManager(mvGraphics& graphics);

		void show_imgui_windows();

		void bind(mvGraphics& graphics, glm::mat4 view);
		void setDirection(int i, float x, float y, float z);

		mvDirectionLight& addLight(mvGraphics& graphics, glm::vec3 dir = { 0.0f,-1.0f,0.0f });
		mvDirectionLight& getLight(int i);

	private:

		std::unique_ptr<mvPixelConstantBuffer> m_buffer;
		std::unique_ptr<mvBuffer>              m_bufferData;
		std::vector<mvDirectionLight>          m_lights;

	};

}