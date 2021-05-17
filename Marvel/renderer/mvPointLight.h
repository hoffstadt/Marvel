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
			int lightCount = 3;
			glm::vec3 ambient = {0.05f, 0.05f, 0.05f};
			//-------------------------- ( 16 bytes )

			glm::vec3 viewLightPos[3] = { glm::vec3{ 0.0f, 0.0f, 0.0f}, glm::vec3{ 0.0f, 0.0f, 0.0f} , glm::vec3{ 0.0f, 0.0f, 0.0f } };
			//-------------------------- ( 64 bytes )

			glm::vec3 diffuseColor[3] = { glm::vec3{ 1.0f, 1.0f, 1.0f}, glm::vec3{ 1.0f, 1.0f, 1.0f} , glm::vec3{ 1.0f, 1.0f, 1.0f } };
			//-------------------------- ( 64 bytes )

			alignas(16) float diffuseIntensity[3] = {1.0f, 1.0f, 1.0f};
			//-------------------------- ( 64 bytes )

			alignas(16) float attConst[3] = { 1.0f, 1.0f, 1.0f};
			//-------------------------- ( 64 bytes )

			alignas(16) float attLin[3] = {0.045f, 0.045f, 0.045f};
			//-------------------------- ( 64 bytes )

			alignas(16) float attQuad[3] = {0.0075f, 0.0075f, 0.0075f};
			//-------------------------- ( 64 bytes )

			//-------------------------- ( 400 bytes )
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