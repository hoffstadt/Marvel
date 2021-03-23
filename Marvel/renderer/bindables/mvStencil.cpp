#include "mvStencil.h"
#include "mvGraphics.h"

namespace Marvel{

	mvStencil::mvStencil(mvGraphics& graphics, Mode mode)
			:
			m_mode(mode)
		{
			D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };

			if (mode == Mode::Write)
			{
				dsDesc.DepthEnable = FALSE;
				dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
				dsDesc.StencilEnable = TRUE;
				dsDesc.StencilWriteMask = 0xFF;
				dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
				dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
			}
			else if (mode == Mode::Mask)
			{
				dsDesc.DepthEnable = FALSE;
				dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
				dsDesc.StencilEnable = TRUE;
				dsDesc.StencilReadMask = 0xFF;
				dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
				dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			}
			else if (mode == Mode::DepthOff)
			{
				dsDesc.DepthEnable = FALSE;
				dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			}
			else if (mode == Mode::DepthReversed)
			{
				dsDesc.DepthFunc = D3D11_COMPARISON_GREATER;
			}
			else if (mode == Mode::DepthFirst)
			{
				dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
				dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			}

			graphics.getDevice()->CreateDepthStencilState(&dsDesc, m_stencil.GetAddressOf());
		}

		void mvStencil::bind(mvGraphics& graphics)
		{
			graphics.getContext()->OMSetDepthStencilState(m_stencil.Get(), 0xFF);
		}
}
