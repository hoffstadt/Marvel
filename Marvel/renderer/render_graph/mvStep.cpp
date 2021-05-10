#include "mvStep.h"
#include "mvDrawable.h"
#include "mvPass.h"
#include "mvJob.h"
#include "mvRenderGraph.h"

namespace Marvel {

	mvStep::mvStep(const std::string& targetPass)
	{
		m_targetPass = targetPass;
	}

	void mvStep::link(mvRenderGraph& graph)
	{
		m_pass = graph.getPass(m_targetPass);
	}

	void mvStep::addBindable(std::shared_ptr<mvBindable> bindable)
	{
		m_bindables.push_back(std::move(bindable));
	}

	void mvStep::bind(mvGraphics& graphics, const mvDrawable* parent) const
	{
		for (auto& bindable : m_bindables)
		{
			bindable->m_parent = parent;
			bindable->bind(graphics);
		}
	}

	void mvStep::submit(const mvDrawable& drawable) const
	{
		assert(m_pass != nullptr);
		m_pass->addJob(mvJob(this, &drawable));
	}

}