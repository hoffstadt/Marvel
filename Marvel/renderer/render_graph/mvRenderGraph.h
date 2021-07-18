#pragma once

#include <vector>
#include <memory>
#include "mvConstantBuffer.h"
#include "mvPass.h"
#include "mvVertexBuffer.h"
#include "mvIndexBuffer.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvCamera;
	class mvDepthStencil;
	class mvRenderTarget;

	//-----------------------------------------------------------------------------
	// mvRenderGraph
	//-----------------------------------------------------------------------------
	class mvRenderGraph
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

		mvRenderGraph(mvGraphics& graphics);

		void         execute(mvGraphics& graphics) const;
		void         reset();
		mvPass*      getPass(const std::string& name);
		virtual void bindMainCamera(mvCamera& camera);

		// binds global bindables
		void bind(mvGraphics& graphics);

		// clears render target/depth stencil
		void releaseBuffers();

		// reset depth/targets
		void resize(mvGraphics& graphics);

		void addPass(std::shared_ptr<mvPass> pass);

		void show_imgui_window();

	public:

		const mvCamera*                        m_camera = nullptr;
		std::shared_ptr<mvDepthStencil>        m_depthStencil; // master depth
		std::shared_ptr<mvRenderTarget>        m_renderTarget; // back buffer

	private:

		std::vector <std::shared_ptr<mvPass>>  m_passes;	

		GlobalSettings                         m_globalSettings = {};
		std::unique_ptr<mvPixelConstantBuffer> m_buffer;
	};

}
