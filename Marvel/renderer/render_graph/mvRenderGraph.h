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

	public:

		mvRenderGraph(mvGraphics& graphics, const char* skybox);

		void    addJob (mvJob job, size_t target);
		void    execute(mvGraphics& graphics) const;
		void    reset();
		mvPass* getPass(const std::string& name);
		void    show_imgui_window();
		void    bindMainCamera(mvCamera& camera);

		// binds global bindables
		void bind(mvGraphics& graphics);

		// clears render target/depth stencil
		void releaseBuffers();

		// reset depth/targets
		void resize(mvGraphics& graphics);

	private:

		std::shared_ptr<mvDepthStencil>        m_depthStencil; // master depth
		std::shared_ptr<mvRenderTarget>        m_renderTarget; // back buffer

		std::vector <std::shared_ptr<mvPass>>  m_passes;
		std::unique_ptr<mvPixelConstantBuffer> m_buffer;
		std::unique_ptr<mvBuffer>              m_bufferData;



	};

}
