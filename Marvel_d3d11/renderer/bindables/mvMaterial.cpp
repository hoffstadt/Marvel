#include "mvMaterial.h"
#include "mvGraphics.h"

namespace Marvel {

	mvMaterial::mvMaterial(mvGraphics& graphics, glm::vec3 materialColor)
	{
		mvBufferLayout layout(std::make_shared<mvBufferLayoutEntry>(Struct));
		auto& root = layout.getRoot();
		root->add(Float3, std::string("materialColor"));
		root->add(Float3, std::string("specularColor"));
		root->add(Float, std::string("specularWeight"));
		root->add(Float, std::string("speculargloss"));
		root->finalize(0);

		m_bufferRaw = std::make_unique<mvBuffer>(std::move(layout));
		m_bufferRaw->getElement("diffuseColor").set(materialColor);
		m_bufferRaw->getElement("specularColor").set(glm::vec3{ 1.0f, 1.0f, 1.0f });
		m_bufferRaw->getElement("specularWeight").set(1.0f);
		m_bufferRaw->getElement("speculargloss").set(15.0f);

		m_buf = std::make_unique<mvPixelConstantBuffer>(graphics, *root.get(), 2, m_bufferRaw.get());
	}


	void mvMaterial::bind(mvGraphics& graphics)
	{
		m_buf->update(graphics, *m_bufferRaw);
		m_buf->bind(graphics);
	}

}