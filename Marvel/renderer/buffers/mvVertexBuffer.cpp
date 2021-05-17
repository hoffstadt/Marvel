#include "mvVertexBuffer.h"
#include "mvBufferRegistry.h"

namespace Marvel
{

	std::shared_ptr<mvVertexBuffer> mvVertexBuffer::Request(mvGraphics& graphics, const std::string& name, const std::vector<float>& vbuf, const mvVertexLayout& layout, bool dynamic)
	{
		std::string ID = mvVertexBuffer::GenerateUniqueIdentifier(name, layout, dynamic);
		if (auto buffer = mvBufferRegistry::GetBuffer(ID))
			return std::dynamic_pointer_cast<mvVertexBuffer>(buffer);
		auto buffer = std::make_shared<mvVertexBuffer>(graphics, name, vbuf, layout, dynamic);
		mvBufferRegistry::AddBuffer(ID, buffer);
		return buffer;
	}

	std::string mvVertexBuffer::GenerateUniqueIdentifier(const std::string& name, const mvVertexLayout& layout, bool dynamic)
	{
		return typeid(mvVertexBuffer).name() + std::string("$") + name + std::string("$") + std::string(dynamic ? "T" : "F");
	}

	mvVertexBuffer::mvVertexBuffer(mvGraphics& graphics, const std::string& name, const std::vector<float>& vbuf, const mvVertexLayout& layout, bool dynamic)
		: 
		m_stride(layout.getSize()),
		m_layout(layout)
	{

		m_data = vbuf;
		m_dynamic = dynamic;
		m_name = name;

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		if (m_dynamic)
		{
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			m_buffersize = m_data.size() * sizeof(float) + 5000;
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
		graphics.getDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer);
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
			m_vertexBuffer = nullptr;
			m_buffersize = m_data.size() * sizeof(float) + 5000;
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
