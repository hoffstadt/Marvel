#include "mvInputLayout.h"
#include "mvVertexShader.h"

namespace Marvel {

	mvInputLayout::mvInputLayout(mvGraphics& graphics, mvVertexLayout vertexLayout, mvVertexShader* vertexShader)
		:
		m_layout(std::move(vertexLayout))
	{

		const auto d3dLayout = m_layout.getD3DLayout();
		const auto pBytecode = vertexShader->getBlob();

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