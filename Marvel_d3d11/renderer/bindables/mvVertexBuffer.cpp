#include "mvVertexBuffer.h"


namespace Marvel
{

	mvVertexBuffer::mvVertexBuffer(mvGraphics& gfx, const std::vector<float>& vbuf, const mvVertexLayout& layout)
		: 
		m_stride(layout.getSize()),
		m_layout(layout)
	{

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(vbuf.size()*sizeof(float));
		bd.StructureByteStride = m_layout.getSize();
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vbuf.data();
		gfx.getDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer);
	}

	const mvVertexLayout& mvVertexBuffer::GetLayout() const
	{
		return m_layout;
	}

	void mvVertexBuffer::bind(mvGraphics& gfx)
	{
		const UINT offset = 0u;
		gfx.getContext()->IASetVertexBuffers(0u, 1u, m_vertexBuffer.GetAddressOf(), &m_stride, &offset);
	}

}
