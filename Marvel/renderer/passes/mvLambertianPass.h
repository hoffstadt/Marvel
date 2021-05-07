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
	class mvLambertianPass : public mvPass
	{

	public:

		mvLambertianPass(mvGraphics& graphics, const std::string& name);

		void execute(mvGraphics& graphics) const override;

		void bindMainCamera(const mvCamera& cam);

	private:

		const mvCamera* m_camera = nullptr;

	};

}