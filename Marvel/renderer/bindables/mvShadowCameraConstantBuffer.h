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
	class mvCamera;

	//-----------------------------------------------------------------------------
	// mvTransformUniform
	//-----------------------------------------------------------------------------
	class mvShadowCameraConstantBuffer : public mvBindable
	{

	public:

		mvShadowCameraConstantBuffer(mvGraphics& graphics);

		void bind(mvGraphics& graphics) override;
		void setCamera(const mvCamera* camera);

	private:

		std::unique_ptr<mvVertexConstantBuffer> m_buf;
		std::unique_ptr<mvBuffer>               m_bufferRaw;
		const mvCamera*                         m_camera = nullptr;

	};

}