#include "mvShadowMappingPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCamera.h"

namespace Marvel {

	mvShadowMappingPass::mvShadowMappingPass(mvGraphics& graphics, const std::string& name)
		:
		mvPass(name)
	{

		UINT size = 1000;
		//m_depthStencil = std::make_shared<mvOutputDepthStencil>(graphics, 1000, 1000);
		m_depthCube = std::make_shared<mvCubeDepthTexture>(graphics, size, 3);

		addBindable(mvBindableRegistry::Request<mvVertexShader>(graphics, graphics.getShaderRoot() + "Shadow_VS.hlsl"));
		addBindable(mvBindableRegistry::GetBindable("null_ps"));
		addBindable(std::make_shared<mvStencil>(graphics, mvStencil::Mode::Off));
		addBindable(mvBindableRegistry::Request<mvBlender>(graphics, false));
		addBindable(std::make_shared<mvViewport>(graphics, size, size));
		addBindable(std::make_shared<mvShadowRasterizer>(graphics, 50, 2.0f, 0.1f));

		issueProduct(std::make_unique<mvBindPassProduct<mvCubeDepthTexture>>("map", m_depthCube));
		
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

	void mvShadowMappingPass::execute(mvGraphics& graphics) const
	{

		// unbind shadow map
		ID3D11ShaderResourceView* const pSRV[6] = { NULL };
		graphics.getContext()->PSSetShaderResources(0, 1, pSRV);
		graphics.getContext()->PSSetShaderResources(3, 6, pSRV);

		graphics.setProjection(glm::perspectiveLH(PI / 2.0f, 1.0f, 0.5f, 100.0f));
		for (size_t i = 0; i < 6; i++)
		{

			glm::vec3 look_target = m_shadowCamera->getPos() + m_cameraDirections[i];

			glm::mat4 camera_matrix = glm::lookAtLH(m_shadowCamera->getPos(), look_target, m_cameraUps[i]);

			graphics.setCamera(camera_matrix);

			auto d = m_depthCube->getDepthBuffer(i);

			d->clear(graphics);

			const_cast<mvShadowMappingPass*>(this)->m_depthStencil = std::move(d);

			m_depthStencil->bindAsBuffer(graphics);

			for (auto& bind : m_bindables)
				bind->bind(graphics);

			for (const auto& j : m_jobs)
				j.execute(graphics);
		}

		// unbind shadow map
		graphics.getContext()->PSSetShaderResources(0, 1, pSRV);
		graphics.getContext()->PSSetShaderResources(3, 6, pSRV);

	}

	void mvShadowMappingPass::bindShadowCamera(const mvCamera& cam)
	{
		m_shadowCamera = &cam;
	}

}