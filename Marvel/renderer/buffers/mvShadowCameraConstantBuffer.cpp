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
		m_buffer.shadowPosition[0] = glm::transpose(m_camera1->getMatrix());
		m_buffer.shadowPosition[1] = glm::transpose(m_camera2->getMatrix());
		m_buffer.shadowPosition[2] = glm::transpose(m_camera3->getMatrix());

		m_buf->update(graphics, m_buffer);
		m_buf->bind(graphics);
	}

	void mvShadowCameraConstantBuffer::setCamera1(const mvCamera* camera)
	{
		m_camera1 = camera;
	}

	void mvShadowCameraConstantBuffer::setCamera2(const mvCamera* camera)
	{
		m_camera2 = camera;
	}

	void mvShadowCameraConstantBuffer::setCamera3(const mvCamera* camera)
	{
		m_camera3 = camera;
	}
}