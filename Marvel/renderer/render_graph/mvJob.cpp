#include "mvJob.h"
#include "mvStep.h"
#include "mvDrawable.h"

namespace Marvel
{
	mvJob::mvJob(const mvStep* step, const mvDrawable* drawable)
		:
		m_drawable(drawable),
		m_step(step)
	{}

	void mvJob::execute(mvGraphics& graphics) const
	{
		m_drawable->bind(graphics);
		m_step->bind(graphics, m_drawable);
		graphics.drawIndexed(m_drawable->m_indexBuffer->getCount());
	}
}