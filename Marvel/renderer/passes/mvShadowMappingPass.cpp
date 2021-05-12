#include "mvShadowMappingPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCamera.h"

namespace Marvel {

	mvShadowMappingPass::mvShadowMappingPass(mvGraphics& graphics, const std::string& name)
		:
		mvPass(name)
	{

		m_depthStencil = std::make_shared<mvOutputDepthStencil>(graphics, 1000, 1000);
		m_depthCube = std::make_shared<mvCubeTargetTexture>(graphics, 1000, 1000, 3, DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT);

		addBindable(std::make_shared<mvViewport>(graphics, 1000.0f, 1000.0f));
		//addBindable(mvBindableRegistry::GetBindable("null_ps"));
		addBindable(mvBindableRegistry::Request<mvPixelShader>(graphics, graphics.getShaderRoot() + "Shadow_PS.hlsl"));
		addBindable(mvBindableRegistry::Request<mvVertexShader>(graphics, graphics.getShaderRoot() + "Shadow_VS.hlsl"));
		addBindable(std::make_shared<mvStencil>(graphics, mvStencil::Mode::Off));
		addBindable(mvBindableRegistry::Request<mvRasterizer>(graphics, false));
		addBindable(mvBindableRegistry::Request<mvBlender>(graphics, false));
		
		issueProduct(std::make_unique<mvBindPassProduct<mvCubeTargetTexture>>("map", m_depthCube));

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
	}

	void mvShadowMappingPass::execute(mvGraphics& graphics) const
	{

		graphics.setProjection(glm::perspectiveLH(PI / 2.0f, 1.0f, 0.5f, 100.0f));
		for (size_t i = 0; i < 6; i++)
		{

			
			glm::vec3 look_target = m_shadowCamera->getPos() + m_cameraDirections[i];

			glm::mat4 camera_matrix = glm::lookAtLH(m_shadowCamera->getPos(), look_target, m_cameraUps[i]);

			graphics.setCamera(camera_matrix);

			auto rt = m_depthCube->getRenderTarget(i);

			rt->clear(graphics);

			m_depthStencil->clear(graphics);

			if (rt)
				rt->bindAsBuffer(graphics, m_depthStencil.get());

			else
				m_depthStencil->bindAsBuffer(graphics);

			for (auto& bind : m_bindables)
				bind->bind(graphics);

			for (const auto& j : m_jobs)
				j.execute(graphics);
		}

	}

	void mvShadowMappingPass::bindShadowCamera(const mvCamera& cam)
	{
		m_shadowCamera = &cam;
	}

}