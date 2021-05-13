#include "mvOverlayPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCamera.h"

namespace Marvel {

	mvOverlayPass::mvOverlayPass(mvGraphics& graphics, const std::string& name)
		:
		mvPass(name)
	{
		requestResource(std::make_unique<mvBufferPassResource<mvRenderTarget>>("render_target", m_renderTarget));
		//addBindable(std::make_shared<mvStencil>(graphics, mvStencil::Mode::Off));
		issueProduct(std::make_unique<mvBufferPassProduct<mvRenderTarget>>("render_target", m_renderTarget));
	}

	void mvOverlayPass::execute(mvGraphics& graphics) const
	{
		if (m_renderTarget)
			m_renderTarget->bindAsBuffer(graphics, m_depthStencil.get());
		else
			m_depthStencil->bindAsBuffer(graphics);

		m_camera->bind(graphics);

		for (auto& bind : m_bindables)
			bind->bind(graphics);

		for (const auto& j : m_jobs)
			j.execute(graphics);
	}

	void mvOverlayPass::bindMainCamera(const mvCamera& cam)
	{
		m_camera = &cam;
	}

}