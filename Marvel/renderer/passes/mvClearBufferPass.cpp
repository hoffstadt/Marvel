#include "mvClearBufferPass.h"
#include "mvRenderTarget.h"
#include "mvDepthStencil.h"

namespace Marvel
{
	mvClearBufferPass::mvClearBufferPass(std::string name)
		:
		mvPass(std::move(name))
	{
	}

	void mvClearBufferPass::execute(mvGraphics& graphics) const
	{
		if (m_depthStencil)
			m_depthStencil->clear(graphics);
		else if (m_renderTarget)
			m_renderTarget->clear(graphics);
	}
}