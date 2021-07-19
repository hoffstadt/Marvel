#pragma once

#include "mvPass.h"
#include <glm/glm.hpp>

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvCamera;
	class mvCubeDepthTexture;

	//-----------------------------------------------------------------------------
	// mvPointShadowMappingPass
	//-----------------------------------------------------------------------------
	class mvPointShadowMappingPass : public mvPass
	{

	public:

		mvPointShadowMappingPass(mvGraphics& graphics, const std::string& name, int slot);

		void execute(mvGraphics& graphics) const override;
		void bindShadowCamera(const mvCamera& cam);
		std::shared_ptr<mvCubeDepthTexture> getDepthCube();

	private:

		const mvCamera* m_shadowCamera = nullptr;
		std::shared_ptr<mvCubeDepthTexture> m_depthCube;
		std::vector<glm::vec3> m_cameraDirections;
		std::vector<glm::vec3> m_cameraUps;

	};

}