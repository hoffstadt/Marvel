#include "mvPointShadowMappingPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCamera.h"

namespace Marvel {

	mvPointShadowMappingPass::mvPointShadowMappingPass(mvGraphics& graphics, const std::string& name, int slot)
		:
		mvPass(name)
	{

		UINT size = 2000;
		m_depthCube = std::make_shared<mvCubeDepthTexture>(graphics, size, slot);
		//issueProduct(std::make_unique<mvBindPassProduct<mvCubeDepthTexture>>("map", m_depthCube));
		
		m_cameraDirections.push_back(glm::vec3{ 1.0f, 0.0f, 0.0f });
		m_cameraDirections.push_back(glm::vec3{ -1.0f, 0.0f, 0.0f });
		m_cameraDirections.push_back(glm::vec3{ 0.0f, 1.0f, 0.0f });
		m_cameraDirections.push_back(glm::vec3{ 0.0f, -1.0f, 0.0f });
		m_cameraDirections.push_back(glm::vec3{ 0.0f, 0.0f, 1.0f });
		m_cameraDirections.push_back(glm::vec3{ 0.0f, 0.0f, -1.0f });

		m_cameraUps.push_back(glm::vec3{ 0.0f, 1.0f, 0.0f});
		m_cameraUps.push_back(glm::vec3{ 0.0f, 1.0f, 0.0f});
		m_cameraUps.push_back(glm::vec3{ 0.0f, 0.0f, -1.0f });
		m_cameraUps.push_back(glm::vec3{ 0.0f, 0.0f, 1.0f});
		m_cameraUps.push_back(glm::vec3{ 0.0f, 1.0f, 0.0f});
		m_cameraUps.push_back(glm::vec3{ 0.0f, 1.0f, 0.0f});

		m_depthStencil = m_depthCube->getDepthBuffer(0);
	}

	void mvPointShadowMappingPass::execute(mvGraphics& graphics) const
	{
		graphics.setProjection(glm::perspectiveLH(PI / 2.0f, 1.0f, 0.5f, 100.0f));
		for (size_t i = 0; i < 6; i++)
		{

			glm::vec3 look_target = m_shadowCamera->getPos() + m_cameraDirections[i];

			glm::mat4 camera_matrix = glm::lookAtLH(m_shadowCamera->getPos(), look_target, m_cameraUps[i]);

			graphics.setCamera(camera_matrix);

			auto d = m_depthCube->getDepthBuffer(i);

			d->clear(graphics);

			const_cast<mvPointShadowMappingPass*>(this)->m_depthStencil = std::move(d);
			m_depthStencil->bindAsBuffer(graphics);

			for (auto& bind : m_bindables)
				bind->bind(graphics);

			for (const auto& j : m_jobs)
				j.execute(graphics);
		}
	}

	void mvPointShadowMappingPass::bindShadowCamera(const mvCamera& cam)
	{
		m_shadowCamera = &cam;
	}

	std::shared_ptr<mvCubeDepthTexture> mvPointShadowMappingPass::getDepthCube()
	{
		return m_depthCube;
	}

}