#include "mvRasterizerState.h"
#include "mvGraphics.h"

namespace Marvel {

	std::string mvRasterizerState::GenerateUniqueIdentifier(bool cull, bool hwpcf, int depthBias, float slopeBias, float clamp)
	{
		return typeid(mvRasterizerState).name() + std::string("$cull") + std::string(cull ? "T" : "F") +
			std::string("$hwpcf") + std::string(hwpcf ? "T" : "F") +
			std::string("$depthBias") + std::to_string(depthBias) + std::string("$slopeBias") + std::to_string(slopeBias)
			+ std::string("$clamp") + std::to_string(clamp);
	}

	mvRasterizerState* mvRasterizerState::Request(mvGraphics& graphics, bool cull, bool hwpcf, int depthBias, float slopeBias, float clamp)
	{
		static std::vector<std::unique_ptr<mvRasterizerState>> states;

		std::string ID = GenerateUniqueIdentifier(cull, hwpcf, depthBias, slopeBias, clamp);

		for (auto& state : states)
		{
			if (state->getUniqueIdentifier() == ID)
				return state.get();
		}

		states.emplace_back(new mvRasterizerState(graphics, cull, hwpcf, depthBias, slopeBias, clamp));

		return states.back().get();
	}

	std::string mvRasterizerState::getUniqueIdentifier() const
	{
		return GenerateUniqueIdentifier(m_cull, m_hwpcf, m_depthBias, m_slopeBias, m_clamp);
	}

	mvRasterizerState::mvRasterizerState(mvGraphics& graphics, bool cull, bool hwpcf, int depthBias, float slopeBias, float clamp)
		:
		m_cull(cull),
		m_hwpcf(hwpcf),
		m_depthBias(depthBias),
		m_slopeBias(slopeBias),
		m_clamp(clamp)
	{

		D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		
		if (m_hwpcf)
		{
			rasterDesc.DepthBias = m_depthBias;
			rasterDesc.SlopeScaledDepthBias = m_slopeBias;
			rasterDesc.DepthBiasClamp = m_clamp;
		}
		else
		{
			rasterDesc.CullMode = cull ? D3D11_CULL_BACK : D3D11_CULL_NONE;
		}

		graphics.getDevice()->CreateRasterizerState(&rasterDesc, m_state.GetAddressOf());
	}

	void mvRasterizerState::set(mvGraphics& graphics)
	{
		graphics.getContext()->RSSetState(m_state.Get());
	}

}