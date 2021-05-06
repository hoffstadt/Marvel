#include "mvOverlayPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCamera.h"

namespace Marvel {

	mvOverlayPass::mvOverlayPass(mvGraphics& graphics)
		:
		mvPass("overlay")
	{
		requestResource(std::make_unique<mvBufferPassResource<mvRenderTarget>>("render_target", m_renderTarget));
		requestResource(std::make_unique<mvBufferPassResource<mvDepthStencil>>("depth_stencil", m_depthStencil));
		addBindable(std::make_shared<mvStencil>(graphics, mvStencil::Mode::Off));
		issueProduct(std::make_unique<mvBufferPassProduct<mvRenderTarget>>("render_target", m_renderTarget));
		issueProduct(std::make_unique<mvBufferPassProduct<mvDepthStencil>>("depth_stencil", m_depthStencil));
	}

	void mvOverlayPass::execute(mvGraphics& graphics) const
	{
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