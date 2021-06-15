#include "mvSkyBoxTransformConstantBuffer.h"
#include "drawables/mvDrawable.h"
#include "mvGraphics.h"
#include "mvDrawable.h"

namespace Marvel {

	mvSkyBoxTransformConstantBuffer::mvSkyBoxTransformConstantBuffer(mvGraphics& graphics)
	{
		mvBufferLayout layout(std::make_shared<mvBufferLayoutEntry>(Struct));
		auto& root = layout.getRoot();
		root->add(Matrix, std::string("viewProj"));
		root->finalize(0);

		m_bufferRaw = std::make_unique<mvDynamicBuffer>(std::move(layout));
		m_bufferRaw->getElement("viewProj") = glm::identity<glm::mat4>();

		m_buf = std::make_unique<mvVertexConstantBuffer>(graphics, *root.get(), 0, m_bufferRaw.get());
		
	}

	void mvSkyBoxTransformConstantBuffer::bind(mvGraphics& graphics)
	{
		auto viewProj = graphics.getProjection() * graphics.getCamera();

		m_bufferRaw->getElement("viewProj") = viewProj;

		m_buf->update(graphics, *m_bufferRaw);
		m_buf->bind(graphics);
	}

}