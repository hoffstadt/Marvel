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

		// create vertex layout
		mvVertexLayout vl;
		vl.append(ElementType::Position3D);

		mvPipelineInfo pipeline;

		pipeline.vertexShader = graphics.getShaderRoot() + "Skybox_VS.hlsl";
		pipeline.pixelShader = graphics.getShaderRoot() + "Skybox_PS.hlsl";
		pipeline.geometryShader = "";
		pipeline.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		pipeline.vertexLayout = vl;
		pipeline.rasterizerStateCull = false;
		pipeline.rasterizerStateHwPCF = false;
		pipeline.blendStateFlags = mvBlendStateFlags::MV_BLEND_STATE_BLEND_OFF;
		pipeline.depthStencilStateFlags = mvDepthStencilStateFlags::MV_DEPTH_STENCIL_STATE_DEPTH_FIRST;

		mvSamplerStateInfo sampler
		{
			mvSamplerStateTypeFlags::MV_SAMPLER_STATE_TYPE_BILINEAR,
			mvSamplerStateAddressingFlags::MV_SAMPLER_STATE_ADDRESS_BORDER,
			0u,
			false
		};

		pipeline.samplers.push_back(sampler);

		m_pipeline = mvPipeline::Request(graphics, pipeline);

		addBindable(std::make_shared<mvCubeTexture>(graphics, skybox));
		addBuffer(std::make_shared<mvSkyBoxTransformConstantBuffer>(graphics));

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
		}, vl);

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