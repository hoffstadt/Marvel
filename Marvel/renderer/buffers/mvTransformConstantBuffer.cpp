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
		auto modelView = graphics.getCamera() * model;
		auto modelViewProj = graphics.getProjection() * modelView;

		m_transforms.model = glm::transpose(model);
		m_transforms.modelView = glm::transpose(modelView);
		m_transforms.modelViewProjection = glm::transpose(modelViewProj);

		m_buf->update(graphics, m_transforms);
		m_buf->bind(graphics);
	}

}