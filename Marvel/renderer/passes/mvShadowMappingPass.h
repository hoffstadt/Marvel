#pragma once

#include "mvPass.h"
#include <glm/glm.hpp>

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvCamera;
	class mvCubeTargetTexture;

	//-----------------------------------------------------------------------------
	// mvLambertianPass
	//-----------------------------------------------------------------------------
	class mvShadowMappingPass : public mvPass
	{

	public:

		mvShadowMappingPass(mvGraphics& graphics, const std::string& name);

		void execute(mvGraphics& graphics) const override;

		void bindShadowCamera(const mvCamera& cam);

	private:

		const mvCamera* m_shadowCamera = nullptr;
		std::shared_ptr<mvCubeTargetTexture> m_depthCube = nullptr;
		std::vector<glm::vec3> m_cameraDirections;
		std::vector<glm::vec3> m_cameraUps;

	};

}