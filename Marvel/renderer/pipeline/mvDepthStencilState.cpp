#include "mvDepthStencilState.h"
#include "mvGraphics.h"

namespace Marvel {

	std::string mvDepthStencilState::GenerateUniqueIdentifier(Mode mode)
	{
		return typeid(mvDepthStencilState).name() + std::string("mode") + std::to_string((int)mode);
	}

	mvDepthStencilState* mvDepthStencilState::Request(mvGraphics& graphics, Mode mode)
	{
		static std::vector<std::unique_ptr<mvDepthStencilState>> states;

		std::string ID = GenerateUniqueIdentifier(mode);

		for (auto& state : states)
		{
			if (state->getUniqueIdentifier() == ID)
				return state.get();
		}

		states.emplace_back(new mvDepthStencilState(graphics, mode));

		return states.back().get();
	}

	std::string mvDepthStencilState::getUniqueIdentifier() const
	{
		return GenerateUniqueIdentifier(m_mode);
	}

	mvDepthStencilState::mvDepthStencilState(mvGraphics& graphics, Mode mode)
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

		graphics.getDevice()->CreateDepthStencilState(&dsDesc, m_state.GetAddressOf());
	}

	void mvDepthStencilState::set(mvGraphics& graphics)
	{
		graphics.getContext()->OMSetDepthStencilState(m_state.Get(), 0xFF);
	}

}