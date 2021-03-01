#include "mvVertexBuffer.h"


namespace Marvel
{

	mvVertexBuffer::mvVertexBuffer(mvGraphics& gfx, const mvDynamicVertexBuffer& vbuf)
		: 
		m_stride((UINT)vbuf.GetLayout().Size()), 
		m_layout(vbuf.GetLayout())
	{

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(vbuf.SizeBytes());
		bd.StructureByteStride = m_stride;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vbuf.GetData();
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
