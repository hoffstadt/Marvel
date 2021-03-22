#include "mvRasterizer.h"
#include "mvGraphics.h"

namespace Marvel
{
	mvRasterizer::mvRasterizer(mvGraphics& graphics, bool twoSided)
		:
		m_twoSided(twoSided)
	{

		D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		rasterDesc.CullMode = twoSided ? D3D11_CULL_NONE : D3D11_CULL_BACK;

		graphics.getDevice()->CreateRasterizerState(&rasterDesc, m_rasterizer.GetAddressOf());
	}

	void mvRasterizer::bind(mvGraphics& graphics)
	{
		graphics.getContext()->RSSetState(m_rasterizer.Get());
	}

}