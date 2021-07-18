#include "mvBaseRenderGraph.h"
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
#include "mvCamera.h"
#include "mvDirectionalShadowMappingPass.h"

namespace Marvel {


	mvBaseRenderGraph::mvBaseRenderGraph(mvGraphics& graphics)
		:
		m_depthStencil(std::move(std::make_shared<mvOutputDepthStencil>(graphics))),
		m_renderTarget(graphics.getTarget())
	{
	}

	void mvBaseRenderGraph::execute(mvGraphics& graphics) const
	{
		for (auto& pass : m_passes)
			pass->execute(graphics);
	}

	void mvBaseRenderGraph::reset()
	{
		for (auto& pass : m_passes)
			pass->reset();
	}

	void mvBaseRenderGraph::releaseBuffers()
	{
		for (auto& pass : m_passes)
			pass->releaseBuffers();

		m_renderTarget.reset();
		m_depthStencil.reset();
	}

	void mvBaseRenderGraph::resize(mvGraphics& graphics)
	{
		m_renderTarget = graphics.getTarget();
		m_depthStencil = std::move(std::make_shared<mvOutputDepthStencil>(graphics));
	}

	mvPass* mvBaseRenderGraph::getPass(const std::string& name)
	{
		for (auto& pass : m_passes)
		{
			if (pass->getName() == name)
				return pass.get();
		}

		assert(false);
	}

	void mvBaseRenderGraph::bindMainCamera(mvCamera& camera)
	{
		m_camera = &camera;
	}

	void mvBaseRenderGraph::requestGlobalResource(std::unique_ptr<mvPassResource> resource)
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

	void mvBaseRenderGraph::issueGlobalProduct(std::unique_ptr<mvPassProduct> product)
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

	void mvBaseRenderGraph::addPass(std::unique_ptr<mvPass> pass)
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

	void mvBaseRenderGraph::linkGlobalResourceToProduct(const std::string& resource, const std::string& pass, const std::string& product)
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

	void mvBaseRenderGraph::linkGlobalResources()
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

	void mvBaseRenderGraph::linkResourcesToProducts(mvPass& pass)
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

	void mvBaseRenderGraph::bake()
	{
		linkGlobalResources();

		// ensure all pass resources have been linked
		for (const auto& pass : m_passes)
		{
			if (!pass->isLinked())
				break;
		}
	}
}
