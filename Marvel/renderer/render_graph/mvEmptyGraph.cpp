#include "mvEmptyGraph.h"
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


	mvEmptyGraph::mvEmptyGraph(mvGraphics& graphics)
		:
		mvBaseRenderGraph(graphics)
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
			auto pass = std::make_unique<mvOverlayPass>(graphics, "overlay");
			pass->linkResourceToProduct("render_target", "clear_target", "buffer");
			addPass(std::move(pass));
		}


		// ensure something outputs the backbuffer
		linkGlobalResourceToProduct("backbuffer", "overlay", "render_target");
		linkGlobalResourceToProduct("masterdepth", "clear_depth", "buffer");

		bake();
	}

	void mvEmptyGraph::bindMainCamera(mvCamera& camera)
	{
		m_camera = &camera;
		static_cast<mvOverlayPass*>(getPass("overlay"))->bindMainCamera(camera);
	}

}
