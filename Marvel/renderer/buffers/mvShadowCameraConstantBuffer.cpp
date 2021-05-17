#include "mvShadowCameraConstantBuffer.h"
#include "drawables/mvDrawable.h"
#include "mvGraphics.h"
#include "mvDrawable.h"
#include "mvCamera.h"

namespace Marvel {

	mvShadowCameraConstantBuffer::mvShadowCameraConstantBuffer(mvGraphics& graphics)
	{
		mvBufferLayout layout(std::make_shared<mvBufferLayoutEntry>(Struct));
		auto& root = layout.getRoot();
		root->add(Matrix, std::string("shadowPosition"));
		root->finalize(0);

		m_bufferRaw = std::make_unique<mvDynamicBuffer>(std::move(layout));
		m_bufferRaw->getElement("shadowPosition") = glm::identity<glm::mat4>();

		m_buf = std::make_unique<mvVertexConstantBuffer>(graphics, *root.get(), 1, m_bufferRaw.get());
		
	}

	void mvShadowCameraConstantBuffer::bind(mvGraphics& graphics)
	{
		auto model = m_camera->getMatrix();

		m_bufferRaw->getElement("shadowPosition") = glm::transpose(model);

		m_buf->update(graphics, *m_bufferRaw);
		m_buf->bind(graphics);
	}

	void mvShadowCameraConstantBuffer::setCamera(const mvCamera* camera)
	{
		m_camera = camera;
	}

}