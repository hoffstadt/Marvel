#include "mvRenderGraph.h"
#include <vector>
#include <imgui.h>
#include <assert.h>
#include "mvPass.h"
#include "mvLambertianPass.h"
#include "mvSkyboxPass.h"
#include "mvShadowMappingPass.h"
#include "mvClearBufferPass.h"
#include "mvOverlayPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"

namespace Marvel {


	mvRenderGraph::mvRenderGraph(mvGraphics& graphics, const char* skybox)
		:
		m_depthStencil(std::make_shared<mvOutputDepthStencil>(graphics)),
		m_renderTarget(graphics.getTarget())
	{

		requestGlobalResource(std::make_unique<mvBufferPassResource<mvRenderTarget>>("backbuffer", m_renderTarget));

		issueGlobalProduct(std::make_unique<mvBufferPassProduct<mvRenderTarget>>("backbuffer", m_renderTarget));
		issueGlobalProduct(std::make_unique<mvBufferPassProduct<mvDepthStencil>>("masterdepth", m_depthStencil));

		{
			auto pass = std::make_unique<mvClearBufferPass>("clear_target");
			pass->linkResourceToProduct("buffer", "global", "backbuffer");
			addPass(std::move(pass));
		}

		{
			auto pass = std::make_unique<mvClearBufferPass>("clear_depth");
			pass->linkResourceToProduct("buffer", "global", "masterdepth");
			addPass(std::move(pass));
		}

		{
			auto pass = std::make_unique<mvLambertianPass>(graphics);
			pass->linkResourceToProduct("render_target", "clear_target", "buffer");
			pass->linkResourceToProduct("depth_stencil", "clear_depth", "buffer");
			addPass(std::move(pass));
		}

		{
			auto pass = std::make_unique<mvSkyboxPass>(graphics, skybox);
			pass->linkResourceToProduct("render_target", "lambertian", "render_target");
			pass->linkResourceToProduct("depth_stencil", "lambertian", "depth_stencil");
			addPass(std::move(pass));
		}

		{
			auto pass = std::make_unique<mvOverlayPass>(graphics);
			pass->linkResourceToProduct("render_target", "skybox", "render_target");
			pass->linkResourceToProduct("depth_stencil", "skybox", "depth_stencil");
			addPass(std::move(pass));
		}
		linkResourceProduct("backbuffer", "overlay", "render_target");

		bake();

		mvBufferLayout layout(std::make_shared<mvBufferLayoutEntry>(Struct));
		auto& root = layout.getRoot();
		root->add(Float, "FogStart");
		root->add(Float, "FogRange");
		root->add(Float3, "FogColor");
		root->finalize(0);

		m_bufferData = std::make_unique<mvBuffer>(std::move(layout));
		m_bufferData->getElement("FogColor") = glm::vec3{ 1.0f, 1.0f, 1.0f };
		m_bufferData->getElement("FogRange") = 100.0f;
		m_bufferData->getElement("FogStart") = 10.0f;
		m_buffer = std::make_unique<mvPixelConstantBuffer>(graphics, *root.get(), 3, m_bufferData.get());
	}

	void mvRenderGraph::addJob(mvJob job, size_t target)
	{
		m_passes[target]->addJob(job);
	}

	void mvRenderGraph::execute(mvGraphics& graphics) const
	{
		for (auto& pass : m_passes)
			pass->execute(graphics);
	}

	void mvRenderGraph::reset()
	{
		for (auto& pass : m_passes)
			pass->reset();
	}

	void mvRenderGraph::releaseBuffers()
	{
		m_renderTarget.reset();
		m_depthStencil.reset();
	}

	void mvRenderGraph::resize(mvGraphics& graphics)
	{
		m_renderTarget = graphics.getTarget();
		//m_depthStencil = graphics.getDepthBuffer();
	}

	mvPass* mvRenderGraph::getPass(const std::string& name)
	{
		for (auto& pass : m_passes)
		{
			if (pass->getName() == name)
				return pass.get();
		}

		assert(false);
	}

	void mvRenderGraph::bindMainCamera(mvCamera& camera)
	{
		static_cast<mvLambertianPass*>(getPass("lambertian"))->bindMainCamera(camera);
		static_cast<mvOverlayPass*>(getPass("overlay"))->bindMainCamera(camera);
	}

	void mvRenderGraph::bind(mvGraphics& graphics)
	{
		m_buffer->update(graphics, *m_bufferData);
		m_buffer->bind(graphics);
	}

	void mvRenderGraph::show_imgui_window()
	{
		float& FogRange = m_bufferData->getElement("FogRange");
		float& FogStart = m_bufferData->getElement("FogStart");
		glm::vec3& FogColor = m_bufferData->getElement("FogColor");

		if (ImGui::Begin("Render Graph"))
		{
			ImGui::Text("Fog");
			ImGui::SliderFloat("Fog Start", &FogStart,  0.0f, 100.0f, "%.1f");
			ImGui::SliderFloat("Fog Range", &FogRange,  0.0f, 100.0f, "%.1f");
			ImGui::ColorEdit3("Fog Color", &FogColor.x);
		}
		ImGui::End();
	}

	void mvRenderGraph::requestGlobalResource(std::unique_ptr<mvPassResource> resource)
	{
		m_resources.push_back(std::move(resource));
	}

	void mvRenderGraph::issueGlobalProduct(std::unique_ptr<mvPassProduct> product)
	{
		m_products.push_back(std::move(product));
	}

	void mvRenderGraph::addPass(std::unique_ptr<mvPass> pass)
	{
		linkResourcesToProducts(*pass);
		m_passes.push_back(std::move(pass));
	}

	void mvRenderGraph::linkResourceProduct(const std::string& resource, const std::string& pass, const std::string& product)
	{
		for (auto& global_resource : m_resources)
		{
			if (global_resource->getName() == resource)
			{
				global_resource->setTarget(pass, product);
				break;
			}
		}
	}

	void mvRenderGraph::linkGlobalResources()
	{
		for (auto& resource : m_resources)
		{
			const std::string& pass = resource->getPass();
			for (auto& existingPass : m_passes)
			{
				if (existingPass->getName() == pass)
				{
					auto& product = existingPass->getPassProduct(resource->getProduct());
					resource->bind(product);
					break;
				}
			}
		}
	}

	void mvRenderGraph::linkResourcesToProducts(mvPass& pass)
	{
		for (auto& resource : pass.getPassResources())
		{
			// local name
			const std::string& passname = resource->getPass();

			if (passname == "global")
			{
				for (auto& globalproduct : m_products)
				{
					if (globalproduct->getName() == resource->getProduct())
					{
						resource->bind(*globalproduct);
						break;
					}
				}
			}

			else
			{
				for (auto& existingPass : m_passes)
				{
					if (existingPass->getName() == passname)
					{
						resource->bind(existingPass->getPassProduct(resource->getProduct()));
						break;
					}
				}

			}
		}
	}

	void mvRenderGraph::bake()
	{
		linkGlobalResources();
	}
}
