#include "mvTechnique.h"
#include "mvRenderGraph.h"

namespace Marvel {

	void mvTechnique::addStep(mvStep step)
	{
		m_steps.push_back(step);
	}

	void mvTechnique::submit(const mvDrawable& drawable) const
	{
		for (auto& step : m_steps)
			step.submit(drawable);
	}

	void mvTechnique::setPass(mvRenderGraph& graph) const
	{
		for (auto& step : m_steps)
			step.setPass(&graph.m_passes[0]);
	}

}