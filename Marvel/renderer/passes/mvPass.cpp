#include "mvPass.h"
#include "mvGraphics.h"
#include "mvRenderTarget.h"
#include "mvDepthStencil.h"

namespace Marvel {

	mvPass::mvPass(const std::string& name)
	{
		m_name = name;
		m_bindables.reserve(12);
	}

	void mvPass::addJob(mvJob job)
	{
		m_jobs.push_back(job);
	}


	void mvPass::reset()
	{
		m_jobs.clear();
	}

	void mvPass::addBindable(std::shared_ptr<mvBindable> bindable)
	{
		m_bindables.push_back(std::move(bindable));
	}

	void mvPass::addBuffer(std::shared_ptr<mvBuffer> buffer)
	{
		m_buffers.push_back(std::move(buffer));
	}

	const std::string& mvPass::getName() const
	{
		return m_name;
	}

	void mvPass::requestResource(std::unique_ptr<mvPassResource> resource)
	{
		
		m_resources.push_back(std::move(resource));
	}

	void mvPass::issueProduct(std::unique_ptr<mvPassProduct> product)
	{
		m_products.push_back(std::move(product));
	}

	mvPassResource& mvPass::getPassResource(const std::string& name) const
	{
		for (const auto& resource : m_resources)
		{
			if (resource->getName() == name)
				return *resource;
		}

		assert(false && "pass resource not found");
	}

	mvPassProduct& mvPass::getPassProduct(const std::string& name) const
	{
		for (const auto& product : m_products)
		{
			if (product->getName() == name)
				return *product;
		}

		assert(false && "pass product not found");
	}

	void mvPass::linkResourceToProduct(const std::string& name, const std::string& pass, const std::string& product)
	{
		auto& resource = getPassResource(name);
		resource.setTarget(pass, product);
	}

	const std::vector<std::unique_ptr<mvPassResource>>& mvPass::getPassResources() const
	{
		return m_resources;
	}

	const std::vector<std::unique_ptr<mvPassProduct>>& mvPass::getPassProducts() const
	{
		return m_products;
	}

	void mvPass::releaseBuffers()
	{
		m_renderTarget->reset();
		m_renderTarget.reset();
		m_depthStencil->reset();
		m_depthStencil.reset();
	}

	bool mvPass::isLinked() const
	{
		for (const auto& resource : m_resources)
		{
			if (!resource->isPreLinked())
			{
				assert(false);
				return false;
			}

			if (!resource->isLinked())
			{
				assert(false);
				return false;
			}
		}

		for (const auto& product : m_products)
		{
			if (!product->isLinked())
			{
				assert(false);
				return false;
			}
		}

		return true;
	}
}