#include "mvSkyboxPass.h"
#include "mvGraphics.h"
#include "mvCommonBindables.h"
#include "mvCommonBuffers.h"
#include "mvPipeline.h"

namespace Marvel {

	mvSkyboxPass::mvSkyboxPass(mvGraphics& graphics, const std::string& name, const char* skybox)
		:
		mvPass(name)
	{

		//-----------------------------------------------------------------------------
		// additional buffers
		//-----------------------------------------------------------------------------
		addBuffer(std::make_shared<mvSkyBoxTransformConstantBuffer>(graphics));
		addBindable(std::make_shared<mvCubeTexture>(graphics, skybox));

		//-----------------------------------------------------------------------------
		// pipeline state setup
		//-----------------------------------------------------------------------------
		mvPipelineInfo pipeline;

		// input assembler stage
		pipeline.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		pipeline.vertexLayout.append(ElementType::Position3D);

		// vertex shader stage
		pipeline.vertexShader = graphics.getShaderRoot() + "Skybox_VS.hlsl";

		// geometry shader stage
		pipeline.geometryShader = "";

		// rasterizer stage
		pipeline.viewportWidth = 0;  // use render target
		pipeline.viewportHeight = 0; // use render target
		pipeline.rasterizerStateCull = false;
		pipeline.rasterizerStateHwPCF = false;
		pipeline.rasterizerStateDepthBias = 0;    // not used
		pipeline.rasterizerStateSlopeBias = 0.0f; // not used
		pipeline.rasterizerStateClamp = 0.0f;	  // not used

		// pixel shader stage
		pipeline.pixelShader = graphics.getShaderRoot() + "Skybox_PS.hlsl";
		pipeline.samplers.push_back({ mvSamplerStateTypeFlags::BILINEAR, mvSamplerStateAddressingFlags::BORDER, 0u, false });

		// output merger stage
		pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::DEPTH_FIRST;
		pipeline.blendStateFlags = mvBlendStateFlags::OFF;

		m_pipeline = mvPipeline::Request(graphics, pipeline);

		requestResource(std::make_unique<mvBufferPassResource<mvRenderTarget>>("render_target", m_renderTarget));
		requestResource(std::make_unique<mvBufferPassResource<mvDepthStencil>>("depth_stencil", m_depthStencil));
		issueProduct(std::make_unique<mvBufferPassProduct<mvRenderTarget>>("render_target", m_renderTarget));
		issueProduct(std::make_unique<mvBufferPassProduct<mvDepthStencil>>("depth_stencil", m_depthStencil));

		constexpr float side = 1.0f / 2.0f;

		// create vertex buffer
		m_vertexBuffer = std::make_shared<mvVertexBuffer>(graphics, name, std::vector<float>{
			-side, -side, -side,
				side, -side, -side,
				-side, side, -side,
				side, side, -side,
				-side, -side, side,
				side, -side, side,
				-side, side, side,
				side, side, side
		}, pipeline.vertexLayout);

		// create index buffer
		m_indexBuffer = mvBufferRegistry::Request<mvIndexBuffer>(graphics, name, std::vector<unsigned int>{
			0, 2, 1, 2, 3, 1,
				1, 3, 5, 3, 7, 5,
				2, 6, 3, 3, 6, 7,
				4, 5, 7, 4, 7, 6,
				0, 4, 2, 2, 4, 6,
				0, 1, 4, 1, 5, 4
		}, false);

	}

	void mvSkyboxPass::execute(mvGraphics& graphics) const
	{

		m_pipeline->set(graphics);

		if (m_renderTarget)
			m_renderTarget->bindAsBuffer(graphics, m_depthStencil.get());
		else
			m_depthStencil->bindAsBuffer(graphics);

		m_vertexBuffer->bind(graphics);
		m_indexBuffer->bind(graphics);

		for (auto& buffer : m_buffers)
			buffer->bind(graphics);

		//m_passes[1].execute(graphics);
		for (auto& bind : m_bindables)
			bind->bind(graphics);
		graphics.drawIndexed(m_indexBuffer->getCount());
	}

}