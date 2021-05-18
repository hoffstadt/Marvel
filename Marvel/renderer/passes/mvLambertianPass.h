#pragma once

#include "mvPass.h"
#include "mvShadowCameraConstantBuffer.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvCamera;

	//-----------------------------------------------------------------------------
	// mvLambertianPass
	//-----------------------------------------------------------------------------
	class mvLambertianPass : public mvPass
	{

	public:

		mvLambertianPass(mvGraphics& graphics, const std::string& name);

		void execute(mvGraphics& graphics) const override;

		void bindMainCamera(const mvCamera& cam);
		void bindShadowCamera1(const mvCamera& cam);
		void bindShadowCamera2(const mvCamera& cam);
		void bindShadowCamera3(const mvCamera& cam);

	private:

		const mvCamera* m_camera = nullptr;
		std::shared_ptr<mvShadowCameraConstantBuffer> m_shadowCBuf;

	};

}