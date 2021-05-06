#include "mvClearBufferPass.h"
#include "mvRenderTarget.h"
#include "mvDepthStencil.h"

namespace Marvel
{
	mvClearBufferPass::mvClearBufferPass(std::string name)
		:
		mvPass(std::move(name))
	{
		requestResource(std::make_unique<mvBufferPassResource<mvBufferResource>>("buffer", m_buffer));
		issueProduct(std::make_unique<mvBufferPassProduct<mvBufferResource>>("buffer", m_buffer));
	}

	void mvClearBufferPass::execute(mvGraphics& graphics) const
	{
		m_buffer->clear(graphics);
	}
}