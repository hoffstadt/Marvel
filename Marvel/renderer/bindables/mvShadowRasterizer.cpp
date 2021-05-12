#include "mvShadowRasterizer.h"
#include "mvGraphics.h"
#include "mvBindableRegistry.h"

namespace Marvel
{

	mvShadowRasterizer::mvShadowRasterizer(mvGraphics& graphics, int depthBias, float slopeBias, float clamp)
		:
		m_depthBias(depthBias),
		m_slopeBias(slopeBias),
		m_clamp(clamp)
	{

		D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		rasterDesc.DepthBias = m_depthBias;
		rasterDesc.SlopeScaledDepthBias = m_slopeBias;
		rasterDesc.DepthBiasClamp = m_clamp;

		graphics.getDevice()->CreateRasterizerState(&rasterDesc, m_rasterizer.GetAddressOf());
	}

	void mvShadowRasterizer::bind(mvGraphics& graphics)
	{
		graphics.getContext()->RSSetState(m_rasterizer.Get());
	}

}