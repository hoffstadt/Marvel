#include "mvTopologyState.h"
#include "mvGraphics.h"

namespace Marvel {

	mvTopologyState::mvTopologyState(D3D11_PRIMITIVE_TOPOLOGY type)
		:
		m_type(type)
	{

	}

	void mvTopologyState::set(mvGraphics& graphics)
	{
		graphics.getContext()->IASetPrimitiveTopology(m_type);
	}

}