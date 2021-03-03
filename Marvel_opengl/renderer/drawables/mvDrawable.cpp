#include "mvDrawable.h"
#include "mvGraphics.h"
#include "mvIndexBuffer.h"
#include "mvVertexBuffer.h"
#include "mvInputLayout.h"

namespace Marvel {

	void mvDrawable::addBindable(mvGraphics& graphics, std::shared_ptr<mvBindable> bindable)
	{
		m_bindables.push_back(bindable);
	}

	void mvDrawable::bind(mvGraphics& graphics) const
	{

		m_layout->bind(graphics);

		for (auto& bindable : m_bindables)
		{
			bindable->setParent(this);
			bindable->bind(graphics);
		}

	}

	size_t mvDrawable::getIndexCount() const
	{
		return m_indexBuffer->getCount();
	}

	void mvDrawable::draw(mvGraphics& graphics) const
	{
		graphics.drawIndexed(getIndexCount());
	}

}