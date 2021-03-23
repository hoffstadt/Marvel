#pragma once
#include <memory>
#include "mvBindable.h"
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
	class mvSkyBoxTransformConstantBuffer : public mvBindable
	{

	public:

		mvSkyBoxTransformConstantBuffer(mvGraphics& graphics);

		void bind(mvGraphics& graphics) override;

	private:

		std::unique_ptr<mvVertexConstantBuffer> m_buf;
		std::unique_ptr<mvBuffer>               m_bufferRaw;

	};

}