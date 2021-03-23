#include "mvStep.h"
#include "mvDrawable.h"
#include "mvPass.h"
#include "mvJob.h"

namespace Marvel {

	void mvStep::setPass(mvPass* pass) const
	{
		m_pass = pass;
	}

	void mvStep::addBindable(std::shared_ptr<mvBindable> bindable)
	{
		m_bindables.push_back(bindable);
	}

	void mvStep::bind(mvGraphics& graphics, const mvDrawable* parent) const
	{
		for (auto& bindable : m_bindables)
		{
			bindable->setParent(parent);
			bindable->bind(graphics);
		}
	}

	void mvStep::submit(const mvDrawable& drawable) const
	{
		m_pass->addJob(mvJob(this, &drawable));
	}

}