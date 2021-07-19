#include "mvDirectionalShadowMappingPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCamera.h"

namespace Marvel {

	mvDirectionalShadowMappingPass::mvDirectionalShadowMappingPass(mvGraphics& graphics, const std::string& name, int slot)
		:
		mvPass(name)
	{

		UINT size = 4000;
		m_depthTexture = std::make_shared<mvDepthTexture>(graphics, size, slot);
		//issueProduct(std::make_unique<mvBindPassProduct<mvDepthTexture>>("map", m_depthTexture));

		m_depthStencil = m_depthTexture->getDepthBuffer();
	}

	void mvDirectionalShadowMappingPass::execute(mvGraphics& graphics) const
	{
		m_depthStencil->clear(graphics);

		m_shadowCamera->bind(graphics);

		m_depthStencil->bindAsBuffer(graphics);

		for (auto& bind : m_bindables)
			bind->bind(graphics);

		for (const auto& j : m_jobs)
			j.execute(graphics);

		graphics.getContext()->OMSetRenderTargets(0, nullptr, nullptr);
	}

	void mvDirectionalShadowMappingPass::bindShadowCamera(const mvOrthoCamera& cam)
	{
		m_shadowCamera = &cam;
	}

	std::shared_ptr<mvDepthTexture> mvDirectionalShadowMappingPass::getDepthTexture()
	{
		return m_depthTexture;
	}

}