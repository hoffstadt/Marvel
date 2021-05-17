#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <assert.h>
#include "mvComPtr.h"
#include "mvBuffer.h"
#include "mvGraphics.h"
#include "mvDynamicBuffer.h"

namespace Marvel {

    class mvConstantBuffer : public mvBuffer
    {

    public:

        mvConstantBuffer(mvGraphics& graphics, const mvBufferLayoutEntry& layoutRoot, UINT slot, const mvDynamicBuffer* buffer)
            :
            m_slot(slot)
        {

            D3D11_BUFFER_DESC cbd;
            cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            cbd.Usage = D3D11_USAGE_DYNAMIC;
            cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            cbd.MiscFlags = 0u;
            cbd.ByteWidth = (UINT)layoutRoot.getSizeInBytes();
            cbd.StructureByteStride = 0u;

            if (buffer != nullptr)
            {
                D3D11_SUBRESOURCE_DATA csd = {};
                csd.pSysMem = buffer->getData();
                graphics.getDevice()->CreateBuffer(&cbd, &csd, m_constantBuffer.GetAddressOf());
            }
            else
                graphics.getDevice()->CreateBuffer(&cbd, nullptr, m_constantBuffer.GetAddressOf());
        }

        template<typename T>
        mvConstantBuffer(mvGraphics& graphics, UINT slot, const T* buffer)
            :
            m_slot(slot)
        {
            D3D11_BUFFER_DESC cbd;
            cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            cbd.Usage = D3D11_USAGE_DYNAMIC;
            cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            cbd.MiscFlags = 0u;
            cbd.ByteWidth = sizeof(T);
            cbd.StructureByteStride = 0u;

            if (buffer != nullptr)
            {
                D3D11_SUBRESOURCE_DATA csd = {};
                csd.pSysMem = &buffer;
                graphics.getDevice()->CreateBuffer(&cbd, &csd, m_constantBuffer.GetAddressOf());
            }
            else
                graphics.getDevice()->CreateBuffer(&cbd, nullptr, m_constantBuffer.GetAddressOf());
        }

        template<typename T>
        void update(mvGraphics& graphics, const T& buffer)
        {
            D3D11_MAPPED_SUBRESOURCE mappedSubresource;
            graphics.getContext()->Map(m_constantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubresource);
            memcpy(mappedSubresource.pData, &buffer, sizeof(T));
            graphics.getContext()->Unmap(m_constantBuffer.Get(), 0u);
        }

        template<>
        void update(mvGraphics& graphics, const mvDynamicBuffer& buffer)
        {
            D3D11_MAPPED_SUBRESOURCE mappedSubresource;
            graphics.getContext()->Map(m_constantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubresource);
            memcpy(mappedSubresource.pData, buffer.getData(), buffer.getSizeInBytes());
            graphics.getContext()->Unmap(m_constantBuffer.Get(), 0u);
        }

    protected:

        mvComPtr<ID3D11Buffer> m_constantBuffer;
        UINT m_slot;
    };

    class mvPixelConstantBuffer : public mvConstantBuffer
    {
    public:
        using mvConstantBuffer::mvConstantBuffer;
        void bind(mvGraphics& graphics) override
        {
            graphics.getContext()->PSSetConstantBuffers(m_slot, 1u, m_constantBuffer.GetAddressOf());
        }
    };

    class mvVertexConstantBuffer : public mvConstantBuffer
    {
    public:
        using mvConstantBuffer::mvConstantBuffer;
        void bind(mvGraphics& graphics) override
        {
            graphics.getContext()->VSSetConstantBuffers(m_slot, 1u, m_constantBuffer.GetAddressOf());
        }
    };

}