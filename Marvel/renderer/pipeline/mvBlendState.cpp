#include "mvBlendState.h"
#include "mvGraphics.h"

namespace Marvel {

	std::string mvBlendState::GenerateUniqueIdentifier(bool blend)
	{
		return typeid(mvBlendState).name() + std::string("$blend") + std::string(blend ? "T" : "F");
	}

	mvBlendState* mvBlendState::Request(mvGraphics& graphics, bool blend)
	{
		static std::vector<std::unique_ptr<mvBlendState>> states;

		std::string ID = GenerateUniqueIdentifier(blend);

		for (const auto& state : states)
		{
			if (state->getUniqueIdentifier() == ID)
				return state.get();
		}

		states.emplace_back(new mvBlendState(graphics, blend));

		return states.back().get();
	}

	std::string mvBlendState::getUniqueIdentifier() const
	{
		return GenerateUniqueIdentifier(m_blend);
	}

	mvBlendState::mvBlendState(mvGraphics& graphics, bool blend)
		:
		m_blend(blend)
	{
		D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} };
		auto& brt = blendDesc.RenderTarget[0];
		if (blend)
		{
			brt.BlendEnable = TRUE;
			brt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			brt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		}
		graphics.getDevice()->CreateBlendState(&blendDesc, m_state.GetAddressOf());
	}

	void mvBlendState::set(mvGraphics& graphics)
	{
		graphics.getContext()->OMSetBlendState(m_state.Get(), nullptr, 0xFFFFFFFFu);
	}

}