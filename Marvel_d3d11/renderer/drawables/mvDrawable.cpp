#include "mvDrawable.h"
#include "mvGraphics.h"
#include "mvIndexBuffer.h"
#include "mvVertexBuffer.h"
#include "mvTopology.h"

namespace Marvel {

	void mvDrawable::addBindable(std::shared_ptr<mvBindable> bindable)
	{
		m_bindables.push_back(bindable);
	}

	void mvDrawable::bind(mvGraphics& graphics) const
	{
		m_indexBuffer->bind(graphics);
		m_vertexBuffer->bind(graphics);
		m_topology->bind(graphics);

		for (auto& bindable : m_bindables)
		{
			bindable->setParent(this);
			bindable->bind(graphics);
		}
	}

	UINT mvDrawable::getIndexCount() const
	{
		return m_indexBuffer->getCount();
	}

	void mvDrawable::draw(mvGraphics& graphics) const
	{
		bind(graphics);
		graphics.drawIndexed(getIndexCount());
	}

}