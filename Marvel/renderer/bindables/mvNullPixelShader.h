#pragma once
#include <d3d11.h>
#include "mvComPtr.h"
#include "mvBindable.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvVertexShader
	//-----------------------------------------------------------------------------
	class mvNullPixelShader : public mvBindable
	{

	public:

		mvNullPixelShader(mvGraphics& graphics);

		void bind(mvGraphics& graphics) override;

	};

}