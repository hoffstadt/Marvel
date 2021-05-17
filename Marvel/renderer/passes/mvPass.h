#pragma once

#include <vector>
#include <memory>
#include <string>
#include "mvJob.h"
#include "mvBindable.h"
#include "mvBuffer.h"
#include "mvPassResource.h"
#include "mvPassProduct.h"

namespace Marvel{

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvDepthStencil;
	class mvRenderTarget;

	//-----------------------------------------------------------------------------
	// mvPass
	// 	   * collection of jobs
	// 	   * pass-wide bindables
	//-----------------------------------------------------------------------------
	class mvPass
	{

		friend class mvRenderGraph;

	public:

		mvPass(const std::string& name);
		virtual ~mvPass() = default;

		virtual void execute(mvGraphics& graphics) const = 0;

		void               addJob     (mvJob job);
		void               reset      ();
		void               releaseBuffers();
		void               addBindable(std::shared_ptr<mvBindable> bindable);
		void               addBuffer(std::shared_ptr<mvBuffer> buffer);
		const std::string& getName    () const;
		bool               isLinked() const;

		template<class T>
		void addBindableResource(std::string name)
		{
			const auto index = m_bindables.size();
			m_bindables.emplace_back();
			requestResource(std::make_unique<mvBindPassResource<T>>(name, m_bindables.back()));
		}

		// resources/producsts
		const std::vector<std::unique_ptr<mvPassResource>>& getPassResources() const;
		const std::vector<std::unique_ptr<mvPassProduct>>& getPassProducts() const;

		mvPassResource& getPassResource(const std::string& name) const;
		mvPassProduct&  getPassProduct (const std::string& name) const;
		void            linkResourceToProduct(const std::string& name, const std::string& pass, const std::string& product);

	protected:

		// resource/products
		void requestResource(std::unique_ptr<mvPassResource> resource);
		void issueProduct(std::unique_ptr<mvPassProduct> product);

	protected:

		std::vector<mvJob>                       m_jobs;
		std::vector<std::shared_ptr<mvBindable>> m_bindables;
		std::vector<std::shared_ptr<mvBuffer>>   m_buffers;
		std::string                              m_name;
		std::shared_ptr<mvDepthStencil>          m_depthStencil;
		std::shared_ptr<mvRenderTarget>          m_renderTarget;

		std::vector<std::unique_ptr<mvPassResource>> m_resources;
		std::vector<std::unique_ptr<mvPassProduct>> m_products;
	};
}