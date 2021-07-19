#pragma once

#include "mvPass.h"
#include "mvShadowCameraConstantBuffer.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvOrthoCamera;
	class mvCamera;
	class mvCubeDepthTexture;
	class mvDepthTexture;
	class mvPointShadowMappingPass;
	class mvDirectionalShadowMappingPass;

	//-----------------------------------------------------------------------------
	// mvLambertianPass
	//-----------------------------------------------------------------------------
	class mvLambertianPass : public mvPass
	{

	public:

		mvLambertianPass(mvGraphics& graphics, const std::string& name);

		void execute(mvGraphics& graphics) const override;

		void bindMainCamera(const mvCamera& cam);
		void bindShadowCamera(const mvCamera& cam);
		void bindDirectionalShadowCamera(const mvOrthoCamera& cam);
		void linkDepthCube(mvPointShadowMappingPass& pass);
		void linkDepthTexture(mvDirectionalShadowMappingPass& pass);

	public:

		const mvCamera* m_camera = nullptr;
		std::shared_ptr<mvShadowCameraConstantBuffer> m_shadowCBuf;
		std::shared_ptr<mvCubeDepthTexture> m_depthCube;
		std::shared_ptr<mvDepthTexture> m_depthTexture;

	};

}