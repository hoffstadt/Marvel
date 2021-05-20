#include "mvPipeline.h"
#include "mvGraphics.h"
#include "mvBlendState.h"
#include "mvRasterizerState.h"
#include "mvTopologyState.h"
#include "mvDepthStencilState.h"
#include "mvVertexShader.h"
#include "mvPixelShader.h"
#include "mvGeometryShader.h"
#include "mvInputLayout.h"
#include "mvSamplerState.h"
#include "mvViewport.h"

namespace Marvel {

	std::string mvPipeline::GenerateUniqueIdentifier(const mvPipelineInfo& info)
	{
		std::string id;

		id.append("$pipeline");

		id.append("$vertexShader_");
		id.append(info.vertexShader);

		id.append("$geometryShader_");
		id.append(info.geometryShader);

		id.append("$pixelShader_");
		id.append(info.pixelShader);

		id.append("$topology_");
		id.append(std::to_string(info.topology));

		id.append("$blendState_");
		id.append(std::to_string((int)info.blendStateFlags));

		id.append("$depthStencilState_");
		id.append(std::to_string((int)info.depthStencilStateFlags));

		id.append("$rasterizerStateCull_");
		id.append(std::to_string(info.rasterizerStateCull));

		id.append("$rasterizerStateHwPCF_");
		id.append(std::to_string(info.rasterizerStateHwPCF));

		id.append("$rasterizerDepthBias_");
		id.append(std::to_string(info.rasterizerStateDepthBias));

		id.append("$rasterizerSlopeBias_");
		id.append(std::to_string(info.rasterizerStateSlopeBias));

		id.append("$rasterizerClamp_");
		id.append(std::to_string(info.rasterizerStateClamp));

		id.append("$viewportWidth_");
		id.append(std::to_string(info.viewportWidth));

		id.append("$viewportHeight_");
		id.append(std::to_string(info.viewportHeight));

		for (const auto& sampler : info.samplers)
		{
			id.append("$samplerType_");
			id.append(std::to_string((int)sampler.type));
			id.append("$samplerAddressing_");
			id.append(std::to_string((int)sampler.addressing));
			id.append("$samplerSlot_");
			id.append(std::to_string(sampler.slot));
			id.append("$samplerPCF_");
			id.append(std::to_string(sampler.hardwarePCF));
		}

		return id;
	}

	mvPipeline* mvPipeline::Request(mvGraphics& graphics, const mvPipelineInfo& info)
	{
		static std::vector<std::unique_ptr<mvPipeline>> pipelines;

		std::string ID = GenerateUniqueIdentifier(info);

		for (auto& state : pipelines)
		{
			if (state->getUniqueIdentifier() == ID)
				return state.get();
		}

		pipelines.emplace_back(new mvPipeline(graphics, info));

		return pipelines.back().get();
	}

	mvPipeline::mvPipeline(mvGraphics& graphics, const mvPipelineInfo& info)
		:
		m_id(GenerateUniqueIdentifier(info))
	{
		m_topologyState = new mvTopologyState(info.topology);


		if(info.viewportWidth > 0 || info.viewportHeight > 0)
			m_viewport = new mvViewport(graphics, info.viewportWidth, info.viewportHeight);

		assert(!info.vertexShader.empty());
		m_vertexShader = mvVertexShader::Request(graphics, info.vertexShader);
		
		if (!info.pixelShader.empty())
			m_pixelShader = mvPixelShader::Request(graphics, info.pixelShader);

		assert(info.depthStencilStateFlags != mvDepthStencilStateFlags::NONE);
		switch (info.depthStencilStateFlags)
		{
		case mvDepthStencilStateFlags::OFF:
			m_depthStencilState = mvDepthStencilState::Request(graphics, mvDepthStencilState::Mode::Off);
			break;

		case mvDepthStencilStateFlags::WRITE:
			m_depthStencilState = mvDepthStencilState::Request(graphics, mvDepthStencilState::Mode::Write);
			break;

		case mvDepthStencilStateFlags::MASK:
			m_depthStencilState = mvDepthStencilState::Request(graphics, mvDepthStencilState::Mode::Mask);
			break;

		case mvDepthStencilStateFlags::DEPTH_OFF:
			m_depthStencilState = mvDepthStencilState::Request(graphics, mvDepthStencilState::Mode::DepthOff);
			break;

		case mvDepthStencilStateFlags::DEPTH_REVERSED:
			m_depthStencilState = mvDepthStencilState::Request(graphics, mvDepthStencilState::Mode::DepthReversed);
			break;

		case mvDepthStencilStateFlags::DEPTH_FIRST:
			m_depthStencilState = mvDepthStencilState::Request(graphics, mvDepthStencilState::Mode::DepthFirst);
			break;

		default:
			m_depthStencilState = mvDepthStencilState::Request(graphics, mvDepthStencilState::Mode::Off);
		}

		if (info.rasterizerStateHwPCF)
		{
			assert(info.rasterizerStateDepthBias != 117);
			assert(info.rasterizerStateSlopeBias != 117.0f);
			assert(info.rasterizerStateClamp != 117.0f);
		}
		m_rasterizerState = mvRasterizerState::Request(graphics,
			info.rasterizerStateCull,
			info.rasterizerStateHwPCF,
			info.rasterizerStateDepthBias,
			info.rasterizerStateSlopeBias,
			info.rasterizerStateClamp);

		assert(info.blendStateFlags != mvBlendStateFlags::NONE);
		m_blendState = mvBlendState::Request(graphics, info.blendStateFlags == mvBlendStateFlags::ON);

		for (const auto& sampler : info.samplers)
		{
			assert(sampler.type != mvSamplerStateTypeFlags::NONE);
			assert(sampler.addressing != mvSamplerStateAddressingFlags::NONE);

			mvSamplerState::Type type = mvSamplerState::Type::Anisotropic;
			mvSamplerState::Addressing addressing = mvSamplerState::Addressing::Wrap;

			switch (sampler.type)
			{
			case mvSamplerStateTypeFlags::ANISOTROPIC:
				type = mvSamplerState::Type::Anisotropic;
				break;

			case mvSamplerStateTypeFlags::BILINEAR:
				type = mvSamplerState::Type::Bilinear;
				break;

			case mvSamplerStateTypeFlags::POINT:
				type = mvSamplerState::Type::Point;
				break;
			}

			switch (sampler.addressing)
			{
			case mvSamplerStateAddressingFlags::BORDER:
				addressing = mvSamplerState::Addressing::Border;
				break;

			case mvSamplerStateAddressingFlags::WRAP:
				addressing = mvSamplerState::Addressing::Wrap;
				break;

			case mvSamplerStateAddressingFlags::MIRROR:
				addressing = mvSamplerState::Addressing::Mirror;
				break;
			}

			m_samplerStates.push_back(mvSamplerState::Request(graphics, type, addressing, sampler.slot, sampler.hardwarePCF));
		}

		assert(info.vertexLayout.getElementCount() > 0);
		m_inputLayout = mvInputLayout::Request(graphics, info.vertexLayout, *m_vertexShader);

	}

	void mvPipeline::set(mvGraphics& graphics)
	{
		//---------------------------------------------------------------------
		// input assembler stage
		//---------------------------------------------------------------------
		m_topologyState->set(graphics);
		m_inputLayout->set(graphics);
		
		// * bind vertex buffer (mvDrawable handles this)
		// * bind index buffer  (mvDrawable handles this)

		//---------------------------------------------------------------------
		// vertex shader stage
		//---------------------------------------------------------------------
		m_vertexShader->set(graphics);

		// * bind constant buffers (mvVertexConstantBuffer handles this)

		//---------------------------------------------------------------------
		// hull shader stage
		//---------------------------------------------------------------------
		graphics.getContext()->HSSetShader(nullptr, nullptr, 0);

		//---------------------------------------------------------------------
		// tessellator stage
		//---------------------------------------------------------------------

		//---------------------------------------------------------------------
		// domain shader stage
		//---------------------------------------------------------------------
		graphics.getContext()->DSSetShader(nullptr, nullptr, 0);

		//---------------------------------------------------------------------
		// geometry shader stage
		//---------------------------------------------------------------------
		if (m_geometryShader)
			m_geometryShader->set(graphics);
		else
			graphics.getContext()->GSSetShader(nullptr, nullptr, 0);

		//---------------------------------------------------------------------
		// stream output stage
		//---------------------------------------------------------------------

		//---------------------------------------------------------------------
		// rasterizer stage
		//---------------------------------------------------------------------
		if(m_viewport)
			m_viewport->set(graphics);
		m_rasterizerState->set(graphics);


		//---------------------------------------------------------------------
		// pixel shader stage
		//---------------------------------------------------------------------

		for (auto samplerState : m_samplerStates)
			samplerState->set(graphics);

		if(m_pixelShader)
			m_pixelShader->set(graphics);
		else
			graphics.getContext()->PSSetShader(nullptr, nullptr, 0);

		// * bind textures         (Handled with mvTexture/mvBindable system)
        // * bind constant buffers (mvPixelConstantBuffer handles this)

		//---------------------------------------------------------------------
		// output merger stage
		//---------------------------------------------------------------------
		m_blendState->set(graphics);
		m_depthStencilState->set(graphics);

		// * bind depth stencil buffer (mvRenderGraph/mvDepthStencil handles this)
		// * bind render target        (mvRenderGraph/mvRenderTarget handles this)

		//---------------------------------------------------------------------
		// compute shader
		//---------------------------------------------------------------------
		graphics.getContext()->CSSetShader(nullptr, nullptr, 0);

	}

}