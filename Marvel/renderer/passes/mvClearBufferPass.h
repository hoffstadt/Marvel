#pragma once
#include "mvPass.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mv;

	//-----------------------------------------------------------------------------
	// mvClearBufferPass
	//-----------------------------------------------------------------------------
	class mvClearBufferPass : public mvPass
	{

	public:

		mvClearBufferPass(std::string name);

		void execute(mvGraphics& graphics) const override;
	
	};


}