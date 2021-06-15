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

		struct DirectionLightInfo
		{

			float diffuseIntensity = 1.0f;
			glm::vec3 viewLightDir = glm::vec3{ 0.0f, 1.0f, 0.0f};
			//-------------------------- ( 16 bytes )

			glm::vec3 diffuseColor = glm::vec3{ 1.0f, 1.0f, 1.0f };
			float padding = 0.0f;
			//-------------------------- ( 16 bytes )

			//-------------------------- ( 2*16 = 32 bytes )
		};

	public:

		mvDirectionLight(mvGraphics& graphics, glm::vec3 dir = { 0.0f,0.0f,0.5f });

		void show_imgui_window();

		void bind(mvGraphics& graphics, glm::mat4 view);
		void setDirection(float x, float y, float z);
		std::shared_ptr<mvOrthoCamera> getCamera() const;

	private:

		std::unique_ptr<mvPixelConstantBuffer> m_buffer;
		DirectionLightInfo                     m_info = {};
		std::shared_ptr<mvOrthoCamera>         m_camera;

	};

}