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
	// mvPointLight
	//-----------------------------------------------------------------------------
	class mvPointLight
	{

		struct PointLightInfo
		{

			glm::vec4 viewLightPos = { 0.0f, 0.0f, 0.0f, 1.0f};
			//-------------------------- ( 16 bytes )

			glm::vec3 diffuseColor = { 1.0f, 1.0f, 1.0f};
			float diffuseIntensity = 1.0f;
			//-------------------------- ( 16 bytes )

			float attConst = 1.0f;
			float attLin= 0.045f;
			float attQuad = 0.0075f;
			char _pad1[4];
			//-------------------------- ( 16 bytes )

			//-------------------------- ( 4*16 = 64 bytes )
		};

	public:

		mvPointLight(mvGraphics& graphics, const std::string& name, glm::vec3 pos = { 0.0f,0.0f,0.5f });

		void bind(mvGraphics& graphics, glm::mat4 view);
		void linkTechniques(mvRenderGraph& graph);
		void submit        (mvRenderGraph& graph);
		std::shared_ptr<mvCamera> getCamera() const;
		void setPosition(float x, float y, float z);
		void show_imgui_window();

	private:

		mvSolidSphere                          m_mesh;
		std::shared_ptr<mvCamera>              m_camera;
		std::unique_ptr<mvPixelConstantBuffer> m_buffer;
		PointLightInfo                         m_info = {};

	};

}