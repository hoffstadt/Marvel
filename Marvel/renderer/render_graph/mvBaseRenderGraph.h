#pragma once

#include <vector>
#include <memory>
#include "mvConstantBuffer.h"
#include "mvPass.h"
#include "mvVertexBuffer.h"
#include "mvIndexBuffer.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvCamera;
	class mvDepthStencil;
	class mvRenderTarget;

	//-----------------------------------------------------------------------------
	// mvRenderGraph
	//-----------------------------------------------------------------------------
	class mvBaseRenderGraph
	{

	public:

		mvBaseRenderGraph(mvGraphics& graphics);

		void         execute(mvGraphics& graphics) const;
		void         reset();
		mvPass*      getPass(const std::string& name);
		virtual void bindMainCamera(mvCamera& camera);

		// binds global bindables
		virtual void bind(mvGraphics& graphics) {}

		// clears render target/depth stencil
		void releaseBuffers();

		// reset depth/targets
		void resize(mvGraphics& graphics);

	protected:

		void addPass(std::unique_ptr<mvPass> pass);

		// preps passes
		void linkGlobalResourceToProduct(const std::string& resource, const std::string& pass, const std::string& product);

		// performs actual linking
		void linkResourcesToProducts(mvPass& pass);

		void linkGlobalResources();
		void bake();

		// resource/products
		void requestGlobalResource(std::unique_ptr<mvPassResource> resource);
		void issueGlobalProduct(std::unique_ptr<mvPassProduct> product);

	protected:

		const mvCamera*                        m_camera = nullptr;
		std::shared_ptr<mvDepthStencil>        m_depthStencil; // master depth
		std::shared_ptr<mvRenderTarget>        m_renderTarget; // back buffer

	private:

		std::vector <std::unique_ptr<mvPass>>  m_passes;

		std::vector<std::unique_ptr<mvPassResource>> m_resources;
		std::vector<std::unique_ptr<mvPassProduct>> m_products;

		
	};

}
