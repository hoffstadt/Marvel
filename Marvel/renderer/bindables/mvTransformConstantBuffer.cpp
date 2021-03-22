#include "mvTransformConstantBuffer.h"
#include "drawables/mvDrawable.h"
#include "mvGraphics.h"
#include "mvDrawable.h"

namespace Marvel {

	mvTransformConstantBuffer::mvTransformConstantBuffer(mvGraphics& graphics)
	{
		mvBufferLayout layout(std::make_shared<mvBufferLayoutEntry>(Struct));
		auto& root = layout.getRoot();
		root->add(Matrix, std::string("model"));
		root->add(Matrix, std::string("modelView"));
		root->add(Matrix, std::string("modelViewProject"));
		root->finalize(0);

		m_bufferRaw = std::make_unique<mvBuffer>(std::move(layout));
		m_bufferRaw->getElement("model") = glm::identity<glm::mat4>();
		m_bufferRaw->getElement("modelView") = glm::identity<glm::mat4>();
		m_bufferRaw->getElement("modelViewProject") = glm::identity<glm::mat4>();

		m_buf = std::make_unique<mvVertexConstantBuffer>(graphics, *root.get(), 0, m_bufferRaw.get());
		
	}

	void mvTransformConstantBuffer::bind(mvGraphics& graphics)
	{
		auto model = m_parent->getTransform();
		auto modelView = graphics.getCamera() * model;
		auto modelViewProj = graphics.getProjection() * modelView;

		m_bufferRaw->getElement("model") = glm::transpose(model);
		m_bufferRaw->getElement("modelView") = glm::transpose(modelView);
		m_bufferRaw->getElement("modelViewProject") = glm::transpose(modelViewProj);

		m_buf->update(graphics, *m_bufferRaw);
		m_buf->bind(graphics);
	}

}