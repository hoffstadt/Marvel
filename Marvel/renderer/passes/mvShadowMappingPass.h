#pragma once

#include "mvPass.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvCamera;

	//-----------------------------------------------------------------------------
	// mvLambertianPass
	//-----------------------------------------------------------------------------
	class mvShadowMappingPass : public mvPass
	{

	public:

		mvShadowMappingPass(mvGraphics& graphics);

		void execute(mvGraphics& graphics) const override;

		void bindShadowCamera(const mvCamera& cam);

	private:

		const mvCamera* m_shadowCamera = nullptr;

	};

}