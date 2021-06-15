#pragma once

#include "mvPass.h"
#include <glm/glm.hpp>

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvCamera;
	class mvDepthTexture;

	//-----------------------------------------------------------------------------
	// mvPointShadowMappingPass
	//-----------------------------------------------------------------------------
	class mvDirectionalShadowMappingPass : public mvPass
	{

	public:

		mvDirectionalShadowMappingPass(mvGraphics& graphics, const std::string& name, int slot);

		void execute(mvGraphics& graphics) const override;

		void bindShadowCamera(const mvCamera& cam);

	private:

		const mvCamera* m_shadowCamera = nullptr;
		std::shared_ptr<mvDepthTexture> m_depthTexture;

	};

}