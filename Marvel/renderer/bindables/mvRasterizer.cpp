#include "mvRasterizer.h"
#include "mvGraphics.h"
#include "mvBindableRegistry.h"

namespace Marvel
{
	std::shared_ptr<mvRasterizer> mvRasterizer::Request(mvGraphics& graphics, bool twoSided)
	{
		std::string ID = mvRasterizer::GenerateUniqueIdentifier(twoSided);
		if (auto bindable = mvBindableRegistry::GetBindable(ID))
			return std::dynamic_pointer_cast<mvRasterizer>(bindable);
		auto bindable = std::make_shared<mvRasterizer>(graphics, twoSided);
		mvBindableRegistry::AddBindable(ID, bindable);
		return bindable;
	}

	std::string mvRasterizer::GenerateUniqueIdentifier(bool twoSided)
	{
		return typeid(mvRasterizer).name() + std::string("$") + std::string(twoSided ? "b" : "n");
	}

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

	std::string mvRasterizer::getUniqueIdentifier() const
	{
		return GenerateUniqueIdentifier(m_twoSided);
	}

}