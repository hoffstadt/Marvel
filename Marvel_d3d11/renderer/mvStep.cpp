#include "mvStep.h"
#include "mvDrawable.h"

namespace Marvel {

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

}