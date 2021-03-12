#include "mvMaterial.h"
#include "mvGraphics.h"

namespace Marvel {

	mvMaterial::mvMaterial(mvGraphics& graphics, glm::vec3 materialColor)
		:
		m_buf(graphics, 2u)
	{
		m_cbData =
		{
			materialColor,
			{1.0f, 1.0f, 1.0f},
			1.0f,
			15.0f
		};
	}


	void mvMaterial::bind(mvGraphics& graphics)
	{
		auto dataCopy = m_cbData;
		m_buf.update(graphics, dataCopy);
		m_buf.bind(graphics);
	}

}