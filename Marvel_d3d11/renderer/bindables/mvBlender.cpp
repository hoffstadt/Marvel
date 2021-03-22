#include "mvBlender.h"
#include "mvGraphics.h"

namespace Marvel
{
	mvBlender::mvBlender(mvGraphics& graphics, bool blending)
		:
		m_blending(blending)
	{

		D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} };
		auto& brt = blendDesc.RenderTarget[0];
		if (blending)
		{
			brt.BlendEnable = TRUE;
			brt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			brt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		}
		graphics.getDevice()->CreateBlendState(&blendDesc, m_blender.GetAddressOf());
	}

	void mvBlender::bind(mvGraphics& graphics)
	{
		graphics.getContext()->OMSetBlendState(m_blender.Get(), nullptr, 0xFFFFFFFFu);
	}
}