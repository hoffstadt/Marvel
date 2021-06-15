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
	class mvCamera;

	//-----------------------------------------------------------------------------
	// mvTransformUniform
	//-----------------------------------------------------------------------------
	class mvShadowCameraConstantBuffer : public mvBuffer
	{

		struct ShadowCameraInfo
		{
			glm::mat4 pointShadowView = glm::identity<glm::mat4>();
			glm::mat4 directShadowView = glm::identity<glm::mat4>();
			glm::mat4 directShadowProjection = glm::identity<glm::mat4>();
		};

	public:

		mvShadowCameraConstantBuffer(mvGraphics& graphics);

		void bind(mvGraphics& graphics) override;
		void setCamera(const mvCamera* camera);

	private:

		std::unique_ptr<mvVertexConstantBuffer> m_buf;
		const mvCamera*                         m_camera = nullptr;
		ShadowCameraInfo                        m_buffer;

	};

}