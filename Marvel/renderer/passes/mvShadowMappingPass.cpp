#include "mvShadowMappingPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCamera.h"

namespace Marvel {

	mvShadowMappingPass::mvShadowMappingPass(mvGraphics& graphics)
		:
		mvPass("shadow")
	{
		//m_depthStencil = std::make_shared<mvInputDepthStencil>(graphics, 3, mvDepthStencil::Usage::ShadowDepth);
		addBindable(mvBindableRegistry::GetBindable("Solid_VS"));
		addBindable(mvBindableRegistry::GetBindable("blender"));
		addBindable(mvBindableRegistry::GetBindable("null_ps"));
		addBindable(std::make_shared<mvStencil>(graphics, mvStencil::Mode::DepthOff));
	}

	void mvShadowMappingPass::execute(mvGraphics& graphics) const
	{
		//m_depthStencil->clear(graphics);
		m_shadowCamera->bind(graphics);

		for (auto& bind : m_bindables)
			bind->bind(graphics);

		for (const auto& j : m_jobs)
			j.execute(graphics);
	}

	void mvShadowMappingPass::bindShadowCamera(const mvCamera& cam)
	{
		m_shadowCamera = &cam;
	}

}