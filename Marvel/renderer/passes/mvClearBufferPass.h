#pragma once
#include "mvPass.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvBufferResource;

	//-----------------------------------------------------------------------------
	// mvClearBufferPass
	//-----------------------------------------------------------------------------
	class mvClearBufferPass : public mvPass
	{

	public:

		mvClearBufferPass(std::string name);

		void execute(mvGraphics& graphics) const override;

	private:

		std::shared_ptr<mvBufferResource> m_buffer;
	
	};


}