#pragma once

#include <d3d11_1.h>

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvTopologyState
	//-----------------------------------------------------------------------------
	class mvTopologyState
	{

	public:

		mvTopologyState(D3D11_PRIMITIVE_TOPOLOGY type);

		void set(mvGraphics& graphics);

	private:

		D3D11_PRIMITIVE_TOPOLOGY m_type;
	};

}