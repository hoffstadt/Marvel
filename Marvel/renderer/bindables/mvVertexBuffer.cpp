#include "mvVertexBuffer.h"


namespace Marvel
{

	mvVertexBuffer::mvVertexBuffer(mvGraphics& gfx, const std::vector<float>& vbuf, const mvVertexLayout& layout, bool dynamic)
		: 
		m_stride(layout.getSize()),
		m_layout(layout)
	{

		m_data = vbuf;
		m_dynamic = dynamic;

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		if (m_dynamic)
		{
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			m_buffersize = m_data.size() * sizeof(float) + 10000;
		}
		else
		{
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0u;
			m_buffersize = UINT(m_data.size() * sizeof(float));
		}

		bd.MiscFlags = 0u;
		bd.ByteWidth = m_buffersize;
		bd.StructureByteStride = m_layout.getSize();
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = m_data.data();
		gfx.getDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer);
	}

	const mvVertexLayout& mvVertexBuffer::GetLayout() const
	{
		return m_layout;
	}

	void mvVertexBuffer::update(mvGraphics& graphics)
	{
		if (!m_dynamic)
			return;

		if (m_buffersize < m_data.size()*sizeof(float))
		{
			m_vertexBuffer->Release();
			m_buffersize = m_data.size() * sizeof(float) + 10000;
			D3D11_BUFFER_DESC bufferDesc = {};
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.ByteWidth = m_buffersize;
			bufferDesc.StructureByteStride = sizeof(float);
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.MiscFlags = 0u;
			graphics.getDevice()->CreateBuffer(&bufferDesc, nullptr, m_vertexBuffer.GetAddressOf());
		}

		// Upload vertex/index data into a single contiguous GPU buffer
		D3D11_MAPPED_SUBRESOURCE resource;

		if (graphics.getContext()->Map(m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource) != S_OK)
			return;

		memcpy((float*)resource.pData, m_data.data(), m_data.size() * sizeof(float));

		graphics.getContext()->Unmap(m_vertexBuffer.Get(), 0);
	}

	void mvVertexBuffer::bind(mvGraphics& graphics)
	{
		update(graphics);
		const UINT offset = 0u;
		graphics.getContext()->IASetVertexBuffers(0u, 1u, m_vertexBuffer.GetAddressOf(), &m_stride, &offset);
	}

	bool mvVertexBuffer::isDynamic() const
	{
		return m_dynamic;
	}

	std::vector<float>& mvVertexBuffer::getData()
	{
		return m_data;
	}

}
