#include "mvDirectionalShadowMappingPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCamera.h"

namespace Marvel {

	mvDirectionalShadowMappingPass::mvDirectionalShadowMappingPass(mvGraphics& graphics, const std::string& name, int slot)
		:
		mvPass(name)
	{

		UINT size = 4000;
		m_depthTexture = std::make_shared<mvDepthTexture>(graphics, size, slot);
		issueProduct(std::make_unique<mvBindPassProduct<mvDepthTexture>>("map", m_depthTexture));

		m_depthStencil = m_depthTexture->getDepthBuffer();
	}

	void mvDirectionalShadowMappingPass::execute(mvGraphics& graphics) const
	{
		m_depthStencil->clear(graphics);

		graphics.setProjection(glm::orthoLH(-100.0f, 100.0f, -100.0f, 100.0f, -101.0f, 101.0f));
		glm::mat4 camera_matrix = glm::lookAtLH(glm::vec3{ 0.0f, 80.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f });

		graphics.setCamera(camera_matrix);

		m_depthStencil->bindAsBuffer(graphics);

		for (auto& bind : m_bindables)
			bind->bind(graphics);

		for (const auto& j : m_jobs)
			j.execute(graphics);
	}

	void mvDirectionalShadowMappingPass::bindShadowCamera(const mvCamera& cam)
	{
		m_shadowCamera = &cam;
	}

}