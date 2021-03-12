#include "mvTransformConstantBuffer.h"
#include "drawables/mvDrawable.h"
#include "mvGraphics.h"
#include "mvDrawable.h"

namespace Marvel {

	mvTransformConstantBuffer::mvTransformConstantBuffer(mvGraphics& graphics)
	{
		m_buf = new mvVertexConstantBuffer<Transforms>(graphics);
	}

	mvTransformConstantBuffer::~mvTransformConstantBuffer()
	{
		delete m_buf;
		m_buf = nullptr;
	}

	void mvTransformConstantBuffer::bind(mvGraphics& graphics)
	{
		m_buf->update(graphics, getTransforms(graphics));
		m_buf->bind(graphics);
	}

	mvTransformConstantBuffer::Transforms mvTransformConstantBuffer::getTransforms(mvGraphics& graphics)
	{
		auto model = m_parent->getTransform();
		auto modelView = graphics.getCamera() * model;
		auto modelViewProj = graphics.getProjection() * modelView;
		return{ glm::transpose(model), glm::transpose(modelView), glm::transpose(modelViewProj) };
	}

}