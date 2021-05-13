#include "mvLambertianPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCamera.h"

namespace Marvel {

	mvLambertianPass::mvLambertianPass(mvGraphics& graphics, const std::string& name)
		:
		mvPass(name)
	{
		m_shadowCBuf = std::make_shared<mvShadowCameraConstantBuffer>(graphics);
		addBindable(m_shadowCBuf);

		requestResource(std::make_unique<mvBufferPassResource<mvRenderTarget>>("render_target", m_renderTarget));
		requestResource(std::make_unique<mvBufferPassResource<mvDepthStencil>>("depth_stencil", m_depthStencil));
		
		addBindable(std::make_shared<mvShadowSampler>(graphics));
		addBindable(std::make_shared<mvStencil>(graphics, mvStencil::Mode::Off));

		// for not this need to be after other bindables (reference to array junk, needs to be fixed)
		addBindableResource<mvBindable>("map");
		
		issueProduct(std::make_unique<mvBufferPassProduct<mvRenderTarget>>("render_target", m_renderTarget));
		issueProduct(std::make_unique<mvBufferPassProduct<mvDepthStencil>>("depth_stencil", m_depthStencil));

	}

	void mvLambertianPass::execute(mvGraphics& graphics) const
	{

		m_shadowCBuf->bind(graphics);
		m_camera->bind(graphics);

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

}