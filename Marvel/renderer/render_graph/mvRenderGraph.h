#pragma once

#include <vector>
#include <memory>
#include "mvConstantBuffer.h"
#include "mvPass.h"
#include "mvVertexBuffer.h"
#include "mvIndexBuffer.h"
#include "mvBaseRenderGraph.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvRenderGraph
	//-----------------------------------------------------------------------------
	class mvRenderGraph : public mvBaseRenderGraph
	{

		struct GlobalSettings
		{
			
			float fogStart = 10.0f;
			glm::vec3 fogColor = { 1.0f, 1.0f, 1.0f };
			//-------------------------- ( 16 bytes )

			float fogRange = 100.0f;
			glm::vec3 ambientColor = { 0.05f, 0.05f, 0.05f };
			//-------------------------- ( 16 bytes )

			glm::vec3 camPos = { 0.0f, 0.0f, 0.0f };
			int useShadows = true;
			//-------------------------- ( 16 bytes )

			int useSkybox = true;
			char _pad0[12];
			//-------------------------- ( 16 bytes )

			//-------------------------- ( 4*16 = 64 bytes )
		};

	public:

		mvRenderGraph(mvGraphics& graphics, const char* skybox);

		void    show_imgui_window();

		void bind(mvGraphics& graphics) override;
		void bindMainCamera(mvCamera& camera) override;



	private:

		GlobalSettings                         m_globalSettings = {};
		std::unique_ptr<mvPixelConstantBuffer> m_buffer;

	};

}
