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

		id.append("$rasterizerStateFlags_");
		id.append(std::to_string((int)info.rasterizerStateFlags));

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

		pipelines.push_back(std::move(std::make_unique<mvPipeline>(graphics, info)));

		return pipelines.back().get();
	}

	mvPipeline::mvPipeline(mvGraphics& graphics, const mvPipelineInfo& info)
		:
		m_id(GenerateUniqueIdentifier(info))
	{
		m_topologyState = new mvTopologyState(info.topology);

		m_viewport = new mvViewport(graphics, info.viewportWidth, info.viewportHeight);

		m_vertexShader = mvVertexShader::Request(graphics, info.vertexShader);
		
		if (!info.pixelShader.empty())
			m_pixelShader = mvPixelShader::Request(graphics, info.pixelShader);


		auto dssMode = mvDepthStencilState::Mode::Off;
		if((int)info.depthStencilStateFlags & (int)mvDepthStencilStateFlags::MV_DEPTH_STENCIL_STATE_OFF)
			dssMode = mvDepthStencilState::Mode::Off;
		else if ((int)info.depthStencilStateFlags & (int)mvDepthStencilStateFlags::MV_DEPTH_STENCIL_STATE_WRITE)
			dssMode = mvDepthStencilState::Mode::Write;
		else if ((int)info.depthStencilStateFlags & (int)mvDepthStencilStateFlags::MV_DEPTH_STENCIL_STATE_MASK)
			dssMode = mvDepthStencilState::Mode::Mask;
		else if ((int)info.depthStencilStateFlags & (int)mvDepthStencilStateFlags::MV_DEPTH_STENCIL_STATE_DEPTH_OFF)
			dssMode = mvDepthStencilState::Mode::DepthOff;
		else if ((int)info.depthStencilStateFlags & (int)mvDepthStencilStateFlags::MV_DEPTH_STENCIL_STATE_DEPTH_REVERSED)
			dssMode = mvDepthStencilState::Mode::DepthReversed;
		else if ((int)info.depthStencilStateFlags & (int)mvDepthStencilStateFlags::MV_DEPTH_STENCIL_STATE_DEPTH_FIRST)
			dssMode = mvDepthStencilState::Mode::DepthFirst;
		m_depthStencilState = mvDepthStencilState::Request(graphics, dssMode);

		m_rasterizerState = mvRasterizerState::Request(graphics,
			(int)info.rasterizerStateFlags & (int)mvRasterizerStateFlags::MV_RASTERIZER_STATE_CULL,
			(int)info.rasterizerStateFlags & (int)mvRasterizerStateFlags::MV_RASTERIZER_STATE_HWPCF,
			info.rasterizerStateDepthBias,
			info.rasterizerStateSlopeBias,
			info.rasterizerStateClamp);

		m_blendState = mvBlendState::Request(graphics, (int)info.blendStateFlags & (int)mvBlendStateFlags::MV_BLEND_STATE_BLEND_ON);

		for (const auto& sampler : info.samplers)
		{
			mvSamplerState::Type type;
			mvSamplerState::Addressing addressing;

			if ((int)sampler.type & (int)mvSamplerStateTypeFlags::MV_SAMPLER_STATE_TYPE_ANISOTROPIC)
				type = mvSamplerState::Type::Anisotropic;
			else if ((int)sampler.type & (int)mvSamplerStateTypeFlags::MV_SAMPLER_STATE_TYPE_BILINEAR)
				type = mvSamplerState::Type::Bilinear;
			else if ((int)sampler.type & (int)mvSamplerStateTypeFlags::MV_SAMPLER_STATE_TYPE_POINT)
				type = mvSamplerState::Type::Point;

			if ((int)sampler.addressing & (int)mvSamplerStateAddressingFlags::MV_SAMPLER_STATE_ADDRESS_BORDER)
				addressing = mvSamplerState::Addressing::Border;
			else if ((int)sampler.addressing & (int)mvSamplerStateAddressingFlags::MV_SAMPLER_STATE_ADDRESS_WRAP)
				addressing = mvSamplerState::Addressing::Wrap;
			else if ((int)sampler.addressing & (int)mvSamplerStateAddressingFlags::MV_SAMPLER_STATE_ADDRESS_MIRROR)
				addressing = mvSamplerState::Addressing::Mirror;

			m_samplerStates.push_back(mvSamplerState::Request(graphics, type, addressing, sampler.slot, sampler.hardwarePCF));
		}

		m_inputLayout = mvInputLayout::Request(graphics, info.vertexLayout, *m_vertexShader);

	}

	void mvPipeline::set(mvGraphics& graphics)
	{

		// input assembler stage
		// --------------------------------------------------------------------
		m_topologyState->set(graphics);
		m_inputLayout->set(graphics);

		// vertex shader stage
		// --------------------------------------------------------------------
		m_vertexShader->set(graphics);

		// hull shader stage
		// --------------------------------------------------------------------
		graphics.getContext()->HSSetShader(nullptr, nullptr, 0);

		// tessellator stage
		// --------------------------------------------------------------------

		// domain shader stage
		// --------------------------------------------------------------------
		graphics.getContext()->DSSetShader(nullptr, nullptr, 0);

		// geometry shader stage
		// --------------------------------------------------------------------
		if (m_geometryShader)
			m_geometryShader->set(graphics);
		else
			graphics.getContext()->GSSetShader(nullptr, nullptr, 0);

		// stream output stage
		// --------------------------------------------------------------------

		// rasterizer stage
		// --------------------------------------------------------------------
		m_viewport->set(graphics);
		m_rasterizerState->set(graphics);
		for (auto samplerState : m_samplerStates)
			samplerState->set(graphics);

		// pixel shader stage
		// --------------------------------------------------------------------
		if(m_pixelShader)
			m_pixelShader->set(graphics);
		else
			graphics.getContext()->PSSetShader(nullptr, nullptr, 0);

		// output merger stage
		// --------------------------------------------------------------------
		m_blendState->set(graphics);
		m_depthStencilState->set(graphics);

		// compute shader
		// --------------------------------------------------------------------
		graphics.getContext()->CSSetShader(nullptr, nullptr, 0);

	}

}