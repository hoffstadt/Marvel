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
			glm::mat4 shadowPosition[3] =
			{ 
				glm::identity<glm::mat4>(),
				glm::identity<glm::mat4>(),
				glm::identity<glm::mat4>()
			};

		};

	public:

		mvShadowCameraConstantBuffer(mvGraphics& graphics);

		void bind(mvGraphics& graphics) override;
		void setCamera1(const mvCamera* camera);
		void setCamera2(const mvCamera* camera);
		void setCamera3(const mvCamera* camera);

	private:

		std::unique_ptr<mvVertexConstantBuffer> m_buf;
		const mvCamera*                         m_camera1 = nullptr;
		const mvCamera*                         m_camera2 = nullptr;
		const mvCamera*                         m_camera3 = nullptr;
		ShadowCameraInfo                        m_buffer;

	};

}