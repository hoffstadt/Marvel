#pragma once

#include <memory>
#include "mvPass.h"
#include "mvVertexBuffer.h"
#include "mvIndexBuffer.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvPipeline;

	//-----------------------------------------------------------------------------
	// mvLambertianPass
	//-----------------------------------------------------------------------------
	class mvSkyboxPass : public mvPass
	{

	public:

		mvSkyboxPass(mvGraphics& graphics, const std::string& name, const char* skybox);

		void execute(mvGraphics& graphics) const override;

	private:

		std::shared_ptr<mvVertexBuffer> m_vertexBuffer;
		std::shared_ptr<mvIndexBuffer>  m_indexBuffer;
		mvPipeline*                     m_pipeline;

	};

}