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
	class mvLambertianPass : public mvPass
	{

	public:

		mvLambertianPass(mvGraphics& graphics);

		void execute(mvGraphics& graphics) const override;

	};

}