#include "mvTopology.h"
#include "mvGraphics.h"

namespace Marvel {

	mvTopology::mvTopology(mvGraphics& graphics, D3D11_PRIMITIVE_TOPOLOGY type)
		:
		m_type(type)
	{
	}

	void mvTopology::bind(mvGraphics& graphics)
	{
		graphics.getContext()->IASetPrimitiveTopology(m_type);
	}

}