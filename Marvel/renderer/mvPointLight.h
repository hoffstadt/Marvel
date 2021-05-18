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

		friend class mvPointLightManager;

	public:

		mvPointLight(mvGraphics& graphics, const std::string& name, glm::vec3 pos = { 0.0f,0.0f,0.5f });

		void linkTechniques(mvRenderGraph& graph);
		void submit        (mvRenderGraph& graph);
		std::shared_ptr<mvCamera> getCamera() const;

	private:

		mvSolidSphere                          m_mesh;
		std::shared_ptr<mvCamera>              m_camera;

	};

	//-----------------------------------------------------------------------------
	// mvPointLightManager
	//-----------------------------------------------------------------------------
	class mvPointLightManager
	{

		struct PointLightInfo
		{

			int lightCount = 0;
			//-------------------------- ( 16 bytes )

			alignas(16) glm::vec4 viewLightPos[3] = {
				glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f}, 
				glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f} , 
				glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f} };
			//-------------------------- ( 48 bytes )

			glm::vec4 diffuseColor[3] = { 
				glm::vec4{ 1.0f, 1.0f, 1.0f, 0.0f}, 
				glm::vec4{ 1.0f, 1.0f, 1.0f, 0.0f} , 
				glm::vec4{ 1.0f, 1.0f, 1.0f, 0.0f } };
			//-------------------------- ( 48 bytes )

			float diffuseIntensity[3*4] = { 
				1.0f, 0.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f, 0.0f,
			};
			//-------------------------- ( 48 bytes )

			float attConst[3*4] = { 
				1.0f, 0.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f, 0.0f,
			};
			//-------------------------- ( 48 bytes )

			float attLin[3*4] = {
				0.045f, 0.0f, 0.0f, 0.0f,
				0.045f, 0.0f, 0.0f, 0.0f,
				0.045f, 0.0f, 0.0f, 0.0f,
			};
			//-------------------------- ( 48 bytes )

			float attQuad[3*4] = {
				0.0075f, 0.0f, 0.0f, 0.0f,
				0.0075f, 0.0f, 0.0f, 0.0f,
				0.0075f, 0.0f, 0.0f, 0.0f,
			};
			//-------------------------- ( 48 bytes )

			//-------------------------- ( 6*48 + 16 = 304 bytes )
		};

	public:

		static constexpr int MaxLights = 3;

		mvPointLightManager(mvGraphics& graphics);

		void show_imgui_windows();

		void bind(mvGraphics& graphics, glm::mat4 view);
		void linkTechniques(mvRenderGraph& graph);
		void submit(mvRenderGraph& graph);
		void setPosition(int i, float x, float y, float z);

		mvPointLight& addLight(mvGraphics& graphics, const std::string& name, glm::vec3 pos = { 0.0f,0.0f,0.0f });
		mvPointLight& getLight(int i);

	private:

		std::unique_ptr<mvPixelConstantBuffer> m_buffer;
		std::vector<mvPointLight>              m_lights;
		PointLightInfo                         m_info = {};

	};

}