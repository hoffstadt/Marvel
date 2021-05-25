#include "mvRenderGraph.h"
#include <vector>
#include <imgui.h>
#include <assert.h>
#include "mvPass.h"
#include "mvLambertianPass.h"
#include "mvSkyboxPass.h"
#include "mvPointShadowMappingPass.h"
#include "mvClearBufferPass.h"
#include "mvOverlayPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"

namespace Marvel {


	mvRenderGraph::mvRenderGraph(mvGraphics& graphics, const char* skybox)
		:
		m_depthStencil(std::move(std::make_shared<mvOutputDepthStencil>(graphics))),
		m_renderTarget(graphics.getTarget())
	{

		requestGlobalResource(std::make_unique<mvBufferPassResource<mvRenderTarget>>("backbuffer", m_renderTarget));
		requestGlobalResource(std::make_unique<mvBufferPassResource<mvDepthStencil>>("masterdepth", m_depthStencil));

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
			auto pass = std::make_unique<mvPointShadowMappingPass>(graphics, "shadow", 3);
			addPass(std::move(pass));
		}

		{
			auto pass = std::make_unique<mvLambertianPass>(graphics, "lambertian");
			pass->linkResourceToProduct("map", "shadow", "map");
			pass->linkResourceToProduct("render_target", "clear_target", "buffer");
			pass->linkResourceToProduct("depth_stencil", "clear_depth", "buffer");
			addPass(std::move(pass));
		}

		{
			auto pass = std::make_unique<mvSkyboxPass>(graphics, "skybox", skybox);
			pass->linkResourceToProduct("render_target", "lambertian", "render_target");
			pass->linkResourceToProduct("depth_stencil", "lambertian", "depth_stencil");
			addPass(std::move(pass));
		}

		{
			auto pass = std::make_unique<mvOverlayPass>(graphics, "overlay");
			pass->linkResourceToProduct("render_target", "skybox", "render_target");
			addPass(std::move(pass));
		}

		// ensure something outputs the backbuffer
		linkGlobalResourceToProduct("backbuffer", "overlay", "render_target");
		linkGlobalResourceToProduct("masterdepth", "skybox", "depth_stencil");

		bake();

		m_buffer = std::make_unique<mvPixelConstantBuffer>(graphics, 3, &m_globalSettings);
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
		for (auto& pass : m_passes)
			pass->releaseBuffers();

		m_renderTarget.reset();
		m_depthStencil.reset();
	}

	void mvRenderGraph::resize(mvGraphics& graphics)
	{
		m_renderTarget = graphics.getTarget();
		m_depthStencil = std::move(std::make_shared<mvOutputDepthStencil>(graphics));
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
		m_buffer->update(graphics, m_globalSettings);
		m_buffer->bind(graphics);
	}

	void mvRenderGraph::show_imgui_window()
	{

		if (ImGui::Begin("Scene"))
		{
			ImGui::Text("Environment");
			ImGui::ColorEdit3("Ambient Color", &m_globalSettings.ambientColor.x);

			ImGui::Text("Fog");
			ImGui::SliderFloat("Fog Start", &m_globalSettings.fogStart,  0.0f, 100.0f, "%.1f");
			ImGui::SliderFloat("Fog Range", &m_globalSettings.fogRange,  0.0f, 100.0f, "%.1f");
			ImGui::ColorEdit3("Fog Color", &m_globalSettings.fogColor.x);
		}
		ImGui::End();
	}

	void mvRenderGraph::requestGlobalResource(std::unique_ptr<mvPassResource> resource)
	{
		// ensure resource doesn't already exists
		for (const auto& existing : m_resources)
		{
			if (existing->getName() == resource->getName())
			{
				assert(false && "global resource already exists");
			}
		}

		m_resources.push_back(std::move(resource));
	}

	void mvRenderGraph::issueGlobalProduct(std::unique_ptr<mvPassProduct> product)
	{
		// ensure product doesn't already exists
		for (const auto& existing : m_products)
		{
			if (existing->getName() == product->getName())
			{
				assert(false && "global product already exists");
			}
		}

		m_products.push_back(std::move(product));
	}

	void mvRenderGraph::addPass(std::unique_ptr<mvPass> pass)
	{
		linkResourcesToProducts(*pass);

		// ensure pass doesn't already exists
		for (const auto& existing : m_passes)
		{
			if (existing->getName() == pass->getName())
			{
				assert(false && "pass already exists");
			}
		}

		m_passes.push_back(std::move(pass));
	}

	void mvRenderGraph::linkGlobalResourceToProduct(const std::string& resource, const std::string& pass, const std::string& product)
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
		// iterate through pass's unlinked resorces
		for (auto& unlinkedResource : pass.getPassResources())
		{
			// pass local resource name
			const std::string& passname = unlinkedResource->getPass();

			if (passname == "global")
			{
				for (auto& globalproduct : m_products)
				{
					if (globalproduct->getName() == unlinkedResource->getProduct())
					{
						unlinkedResource->bind(*globalproduct);
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
						auto& product = existingPass->getPassProduct(unlinkedResource->getProduct());
						unlinkedResource->bind(product);
						break;
					}
				}

			}
		}
	}

	void mvRenderGraph::bake()
	{
		linkGlobalResources();

		// ensure all global resources have been linked
		for (const auto& globalResource : m_resources)
		{
			assert(globalResource->isPreLinked());
		}

		// ensure all global products have been linked
		for (const auto& globalProduct : m_products)
		{
			assert(globalProduct->isLinked());
		}

		// ensure all pass resources have been linked
		for (const auto& pass : m_passes)
		{
			if (!pass->isLinked())
				break;
		}
	}
}
