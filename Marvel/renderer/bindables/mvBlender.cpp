#include "mvBlender.h"
#include "mvGraphics.h"
#include "mvBindableRegistry.h"

namespace Marvel
{
	std::shared_ptr<mvBlender> mvBlender::Request(mvGraphics& graphics, bool blending)
	{
		std::string ID = mvBlender::GenerateUniqueIdentifier(blending);
		if (auto bindable = mvBindableRegistry::GetBindable(ID))
			return std::dynamic_pointer_cast<mvBlender>(bindable);
		auto bindable = std::make_shared<mvBlender>(graphics, blending);
		mvBindableRegistry::AddBindable(ID, bindable);
		return bindable;
	}

	std::string mvBlender::GenerateUniqueIdentifier(bool blending)
	{
		return typeid(mvBlender).name() + std::string("$") + std::string(blending ? "b" : "n");
	}

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

	std::string mvBlender::getUniqueIdentifier() const
	{
		return GenerateUniqueIdentifier(m_blending);
	}
}