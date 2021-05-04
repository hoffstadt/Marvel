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

		// binds global bindables
		void bind(mvGraphics& graphics);

	private:

		std::vector <std::shared_ptr<mvPass>>  m_passes;
		std::unique_ptr<mvPixelConstantBuffer> m_buffer;
		std::unique_ptr<mvBuffer>              m_bufferData;

	};

}
