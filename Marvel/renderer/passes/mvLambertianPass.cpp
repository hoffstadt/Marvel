#include "mvLambertianPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCamera.h"
#include "mvPointShadowMappingPass.h"
#include "mvDirectionalShadowMappingPass.h"

namespace Marvel {

	mvLambertianPass::mvLambertianPass(mvGraphics& graphics, const std::string& name)
		:
		mvPass(name)
	{
		m_shadowCBuf = std::make_shared<mvShadowCameraConstantBuffer>(graphics);
		addBuffer(m_shadowCBuf);	
	}

	void mvLambertianPass::execute(mvGraphics& graphics) const
	{

		m_shadowCBuf->bind(graphics);
		m_camera->bind(graphics);
		m_depthCube->bind(graphics);
		m_depthTexture->bind(graphics);

		if (m_renderTarget)
			m_renderTarget->bindAsBuffer(graphics, m_depthStencil.get());
		else
			m_depthStencil->bindAsBuffer(graphics);

		for (auto& bind : m_bindables)
			bind->bind(graphics);

		for (const auto& j : m_jobs)
			j.execute(graphics);
	}

	void mvLambertianPass::bindMainCamera(const mvCamera& cam)
	{
		m_camera = &cam;
	}

	void mvLambertianPass::bindShadowCamera(const mvCamera& cam)
	{
		m_shadowCBuf->setCamera(&cam);
	}

	void mvLambertianPass::bindDirectionalShadowCamera(const mvOrthoCamera& cam)
	{
		m_shadowCBuf->setOrthoCamera(&cam);
	}

	void mvLambertianPass::linkDepthCube(mvPointShadowMappingPass& pass)
	{
		m_depthCube = pass.getDepthCube();
	}

	void mvLambertianPass::linkDepthTexture(mvDirectionalShadowMappingPass& pass)
	{
		m_depthTexture = pass.getDepthTexture();
	}
}