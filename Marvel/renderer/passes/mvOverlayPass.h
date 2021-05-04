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
	class mvOverlayPass : public mvPass
	{

	public:

		mvOverlayPass(mvGraphics& graphics);

		void execute(mvGraphics& graphics) const override;

		void bindMainCamera(const mvCamera& cam);

	private:

		const mvCamera* m_camera = nullptr;

	};

}