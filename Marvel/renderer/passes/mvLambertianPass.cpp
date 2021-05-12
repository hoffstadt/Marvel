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
		addBindable(std::make_shared<mvStencil>(graphics, mvStencil::Mode::Off));
		addBindable(std::make_shared<mvShadowSampler>(graphics));
		addBindableResource<mvBindable>("map");
		issueProduct(std::make_unique<mvBufferPassProduct<mvRenderTarget>>("render_target", m_renderTarget));
		issueProduct(std::make_unique<mvBufferPassProduct<mvDepthStencil>>("depth_stencil", m_depthStencil));

	}

	void mvLambertianPass::execute(mvGraphics& graphics) const
	{

		if (m_renderTarget)
			m_renderTarget->bindAsBuffer(graphics, m_depthStencil.get());
		else
			m_depthStencil->bindAsBuffer(graphics);

		m_shadowCBuf->bind(graphics);
		m_camera->bind(graphics);

		for (auto& bind : m_bindables)
			bind->bind(graphics);

		for (const auto& j : m_jobs)
			j.execute(graphics);

		// unbind shadow map
		ID3D11ShaderResourceView* const pSRV[6] = { NULL };
		graphics.getContext()->PSSetShaderResources(3, 6, pSRV);
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