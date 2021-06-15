#include "mvShadowCameraConstantBuffer.h"
#include "drawables/mvDrawable.h"
#include "mvGraphics.h"
#include "mvDrawable.h"
#include "mvCamera.h"

namespace Marvel {

	mvShadowCameraConstantBuffer::mvShadowCameraConstantBuffer(mvGraphics& graphics)
	{
		m_buf = std::make_unique<mvVertexConstantBuffer>(graphics, 1, &m_buffer);
	}

	void mvShadowCameraConstantBuffer::bind(mvGraphics& graphics)
	{
		m_buffer.pointShadowView = m_camera->getMatrix();
		m_buffer.directShadowView = glm::lookAtLH(glm::vec3{ 0.0f, 80.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f });
		m_buffer.directShadowProjection = glm::orthoLH(-100.0f, 100.0f, -100.0f, 100.0f, -101.0f, 101.0f);

		m_buf->update(graphics, m_buffer);
		m_buf->bind(graphics);
	}

	void mvShadowCameraConstantBuffer::setCamera(const mvCamera* camera)
	{
		m_camera = camera;
	}
}