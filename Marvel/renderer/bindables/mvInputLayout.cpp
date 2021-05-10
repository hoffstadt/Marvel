#include "mvInputLayout.h"
#include "mvVertexShader.h"
#include "mvBindableRegistry.h"


namespace Marvel {

	std::shared_ptr<mvInputLayout> mvInputLayout::Request(mvGraphics& graphics, const mvVertexLayout& vertexLayout, const mvVertexShader& vertexShader)
	{
		std::string ID = GenerateUniqueIdentifier(vertexLayout, vertexShader);
		if (auto bindable = mvBindableRegistry::GetBindable(ID))
			return std::dynamic_pointer_cast<mvInputLayout>(bindable);
		auto bindable = std::make_shared<mvInputLayout>(graphics, vertexLayout, vertexShader);
		mvBindableRegistry::AddBindable(ID, bindable);
		return bindable;
	}

	std::string mvInputLayout::GenerateUniqueIdentifier(const mvVertexLayout& vertexLayout, const mvVertexShader& vertexShader)
	{
		return typeid(mvInputLayout).name() + std::string("$") + vertexShader.getUniqueIdentifier();
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

	void mvInputLayout::bind(mvGraphics& graphics)
	{
		graphics.getContext()->IASetInputLayout(m_inputLayout.Get());
	}

}