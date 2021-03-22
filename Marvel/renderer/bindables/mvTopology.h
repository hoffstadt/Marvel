#pragma once
#include <d3d11.h>
#include "mvBindable.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvTopology
	//-----------------------------------------------------------------------------
	class mvTopology : public mvBindable
	{

	public:

		mvTopology(mvGraphics& graphics, D3D11_PRIMITIVE_TOPOLOGY type);

		void bind(mvGraphics& graphics) override;

	private:

		D3D11_PRIMITIVE_TOPOLOGY m_type;

	};

}