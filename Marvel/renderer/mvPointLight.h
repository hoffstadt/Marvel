#pragma once
#include "mvConstantBuffer.h"
#include "mvSolidSphere.h"

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

	public:

		mvPointLight(mvGraphics& graphics, glm::vec3 pos = { 0.0f,0.0f,0.5f });

		void show_imgui_windows();
		void bind          (mvGraphics& graphics, glm::mat4 view);
		void setPosition   (float x, float y, float z);
		void linkTechniques(mvRenderGraph& graph);
		void submit        (mvRenderGraph& graph);

	private:

		mvSolidSphere                          m_mesh;
		std::unique_ptr<mvPixelConstantBuffer> m_buffer;
		std::unique_ptr<mvBuffer>              m_bufferData;

	};

}