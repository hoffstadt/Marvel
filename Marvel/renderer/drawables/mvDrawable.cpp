#include "mvDrawable.h"
#include "mvGraphics.h"
#include "mvIndexBuffer.h"
#include "mvVertexBuffer.h"
#include "mvTopology.h"

namespace Marvel {

	void mvDrawable::bind(mvGraphics& graphics) const
	{
		m_indexBuffer->bind(graphics);
		m_vertexBuffer->bind(graphics);
		m_topology->bind(graphics);
	}

	void mvDrawable::addTechnique(mvTechnique technique)
	{
		m_techniques.push_back(std::move(technique));
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