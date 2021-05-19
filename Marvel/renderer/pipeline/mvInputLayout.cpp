#include "mvInputLayout.h"
#include "mvVertexShader.h"


namespace Marvel {

	mvInputLayout* mvInputLayout::Request(mvGraphics& graphics, const mvVertexLayout& vertexLayout, const mvVertexShader& vertexShader)
	{
		static std::vector<std::unique_ptr<mvInputLayout>> states;

		std::string ID = GenerateUniqueIdentifier(vertexLayout, vertexShader.getUniqueIdentifier());

		for (auto& state : states)
		{
			if (state->getUniqueIdentifier() == ID)
				return state.get();
		}

		states.push_back(std::move(std::make_unique<mvInputLayout>(graphics, vertexLayout, vertexShader)));

		return states.back().get();
	}

	std::string mvInputLayout::GenerateUniqueIdentifier(const mvVertexLayout& vertexLayout, std::string shaderId)
	{
		return typeid(mvInputLayout).name() + std::string("$") + shaderId;
	}

	mvInputLayout::mvInputLayout(mvGraphics& graphics, const mvVertexLayout& vertexLayout, const mvVertexShader& vertexShader)
		:
		m_layout(vertexLayout)
	{

		const auto d3dLayout = m_layout.getD3DLayout();
		const auto pBytecode = vertexShader.getBlob();
		m_shaderId = vertexShader.getUniqueIdentifier();

		HRESULT hResult = graphics.getDevice()->CreateInputLayout(d3dLayout.data(),
			(UINT)d3dLayout.size(),
			pBytecode->GetBufferPointer(),
			pBytecode->GetBufferSize(),
			m_inputLayout.GetAddressOf());
		assert(SUCCEEDED(hResult));
	}

	void mvInputLayout::set(mvGraphics& graphics)
	{
		graphics.getContext()->IASetInputLayout(m_inputLayout.Get());
	}

	std::string mvInputLayout::getUniqueIdentifier() const
	{
		return GenerateUniqueIdentifier(m_layout, m_shaderId);
	}

}