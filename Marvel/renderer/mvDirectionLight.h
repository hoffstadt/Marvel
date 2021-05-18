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

		struct DirectionLightInfo
		{

			int lightCount = 0;
			//-------------------------- ( 16 bytes )

			alignas(16) glm::vec4 viewLightDir[2] = {
				glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f},
				glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f} };
			//-------------------------- ( 32 bytes )

			glm::vec4 diffuseColor[2] = {
				glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f},
				glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f}};
			//-------------------------- ( 32 bytes )

			float diffuseIntensity[2 * 4] = {
				1.0f, 0.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f, 0.0f};
			//-------------------------- ( 32 bytes )

			//-------------------------- ( 3*32 + 16 = 112 bytes )
		};

	public:

		static constexpr int MaxLights = 2;

		mvDirectionLightManager(mvGraphics& graphics);

		void show_imgui_windows();

		void bind(mvGraphics& graphics, glm::mat4 view);
		void setDirection(int i, float x, float y, float z);

		mvDirectionLight& addLight(mvGraphics& graphics, glm::vec3 dir = { 0.0f,-1.0f,0.0f });
		mvDirectionLight& getLight(int i);

	private:

		std::unique_ptr<mvPixelConstantBuffer> m_buffer;
		std::vector<mvDirectionLight>          m_lights;
		DirectionLightInfo                     m_info = {};

	};

}