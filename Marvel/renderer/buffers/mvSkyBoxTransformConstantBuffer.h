#pragma once
#include <memory>
#include "mvBuffer.h"
#include "mvConstantBuffer.h"
#include "mvMath.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvDrawable;
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvTransformUniform
	//-----------------------------------------------------------------------------
	class mvSkyBoxTransformConstantBuffer : public mvBuffer
	{

	public:

		mvSkyBoxTransformConstantBuffer(mvGraphics& graphics);

		void bind(mvGraphics& graphics) override;

	private:

		std::unique_ptr<mvVertexConstantBuffer> m_buf;
		std::unique_ptr<mvDynamicBuffer>        m_bufferRaw;

	};

}