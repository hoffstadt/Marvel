#include "mvTransformConstantBuffer.h"
#include "drawables/mvDrawable.h"
#include "mvGraphics.h"
#include "mvDrawable.h"

namespace Marvel {

	mvTransformConstantBuffer::mvTransformConstantBuffer(mvGraphics& graphics)
	{

		m_buf = std::make_unique<mvVertexConstantBuffer>(graphics, 0, &m_transforms);
		
	}

	void mvTransformConstantBuffer::bind(mvGraphics& graphics)
	{
		auto model = m_parent->getTransform();
		auto modelView = graphics.getCamera() * m_parent->getTransform();
		auto modelViewProj = graphics.getProjection() * graphics.getCamera() * m_parent->getTransform();

		m_transforms.model = model;
		m_transforms.modelView = modelView;
		m_transforms.modelViewProjection = modelViewProj;

		m_buf->update(graphics, m_transforms);
		m_buf->bind(graphics);
	}

}