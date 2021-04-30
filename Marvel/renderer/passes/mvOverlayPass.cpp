#include "mvOverlayPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"

namespace Marvel {

	mvOverlayPass::mvOverlayPass(mvGraphics& graphics)
		:
		mvPass("Overlay")
	{
		addBindable(std::make_shared<mvStencil>(graphics, mvStencil::Mode::DepthOff));
	}

	void mvOverlayPass::execute(mvGraphics& graphics) const
	{
		for (auto& bind : m_bindables)
			bind->bind(graphics);

		for (const auto& j : m_jobs)
			j.execute(graphics);
	}

}