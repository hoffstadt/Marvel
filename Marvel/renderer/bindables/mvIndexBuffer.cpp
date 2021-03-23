#include "mvIndexBuffer.h"
#include "mvGraphics.h"

namespace Marvel
{

    mvIndexBuffer::mvIndexBuffer(mvGraphics& graphics, const std::vector<unsigned short>& indices)
    {

        m_count = (UINT)indices.size();
        m_indexBuffer = mvComPtr<ID3D11Buffer>();

        // Fill in a buffer description.
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = UINT(sizeof(unsigned short) * m_count);
        bufferDesc.StructureByteStride = sizeof(unsigned short);
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0u;
        bufferDesc.MiscFlags = 0u;

        // Define the resource data.
        D3D11_SUBRESOURCE_DATA InitData = {};
        InitData.pSysMem = indices.data();

        // Create the buffer with the device.
        graphics.getDevice()->CreateBuffer(&bufferDesc, &InitData, m_indexBuffer.GetAddressOf());

    }

    void mvIndexBuffer::bind(mvGraphics& graphics)
    {
        graphics.getContext()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
    }

    UINT mvIndexBuffer::getCount() const 
    { 
        return m_count; 
    }

}