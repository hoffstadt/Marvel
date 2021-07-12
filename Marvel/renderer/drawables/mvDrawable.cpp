#include "mvDrawable.h"
#include "mvGraphics.h"
#include "mvIndexBuffer.h"
#include "mvVertexBuffer.h"
#include "mvBaseRenderGraph.h"

namespace Marvel {

	void mvDrawable::bind(mvGraphics& graphics) const
	{
		m_indexBuffer->bind(graphics);
		m_vertexBuffer->bind(graphics);
	}

	void mvDrawable::addStep(mvStep step)
	{
		m_steps.push_back(std::move(step));
	}

	void mvDrawable::submit(mvBaseRenderGraph& graph) const
	{
		for (const auto& step : m_steps)
			step.submit(*this);
	}

	void mvDrawable::linkSteps(mvBaseRenderGraph& graph)
	{
		for (auto& step : m_steps)
			step.link(graph);
	}

}