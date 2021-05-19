#include "mvDrawable.h"
#include "mvGraphics.h"
#include "mvIndexBuffer.h"
#include "mvVertexBuffer.h"
#include "mvRenderGraph.h"

namespace Marvel {

	void mvDrawable::bind(mvGraphics& graphics) const
	{
		m_indexBuffer->bind(graphics);
		m_vertexBuffer->bind(graphics);
	}

	void mvDrawable::addTechnique(mvTechnique technique)
	{
		m_techniques.push_back(std::move(technique));
	}

	void mvDrawable::submit(mvRenderGraph& graph) const
	{
		for (const auto& tech : m_techniques)
			tech.submit(*this);
	}

	void mvDrawable::linkTechniques(mvRenderGraph& graph)
	{
		for (auto& tech : m_techniques)
			tech.link(graph);
	}

}