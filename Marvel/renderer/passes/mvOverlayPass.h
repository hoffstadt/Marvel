#pragma once

#include "mvPass.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvLambertianPass
	//-----------------------------------------------------------------------------
	class mvOverlayPass : public mvPass
	{

	public:

		mvOverlayPass(mvGraphics& graphics);

		void execute(mvGraphics& graphics) const override;

	};

}