#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <assert.h>
#include "mvComPtr.h"
#include "mvBindable.h"
#include "mvGraphics.h"

namespace Marvel {

    template <typename T>
    class mvConstantBuffer : public mvBindable
    {

    public:

        mvConstantBuffer(mvGraphics& graphics, const T& cbuffer, UINT slot = 0u)
            :
            m_slot(slot)
        {
            D3D11_BUFFER_DESC constantBufferDesc = {};
            // ByteWidth must be a multiple of 16, per the docs
            constantBufferDesc.ByteWidth = sizeof(cbuffer) + 0xf & 0xfffffff0;
            constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
            constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

            HRESULT hResult = graphics.getDevice()->CreateBuffer(&constantBufferDesc, nullptr, m_buffer.GetAddressOf());
            assert(SUCCEEDED(hResult));
        }

        mvConstantBuffer(mvGraphics& graphics, UINT slot = 0u)
            :
            m_slot(slot)
        {
            D3D11_BUFFER_DESC constantBufferDesc = {};
            // ByteWidth must be a multiple of 16, per the docs
            constantBufferDesc.ByteWidth = sizeof(T);
            constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
            constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

            HRESULT hResult = graphics.getDevice()->CreateBuffer(&constantBufferDesc, nullptr, m_buffer.GetAddressOf());
            assert(SUCCEEDED(hResult));
        }

        void update(mvGraphics& graphics, const T& cbuffer)
        {
            D3D11_MAPPED_SUBRESOURCE mappedSubresource;
            graphics.getContext()->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
            T* constants = (T*)(mappedSubresource.pData);
            memcpy(mappedSubresource.pData, &cbuffer, sizeof(cbuffer));
            graphics.getContext()->Unmap(m_buffer.Get(), 0);
        }

    protected:

        Marvel::mvComPtr<ID3D11Buffer> m_buffer;
        UINT                           m_slot;

    };

    template <typename T>
    class mvVertexConstantBuffer : public mvConstantBuffer<T>
    {
        using mvConstantBuffer<T>::m_buffer;
        using mvConstantBuffer<T>::m_slot;
    public:

        using mvConstantBuffer<T>::mvConstantBuffer;
        void bind(mvGraphics& graphics) override
        {
            graphics.getContext()->VSSetConstantBuffers(m_slot, 1u, m_buffer.GetAddressOf());
        }

    };

    template <typename T>
    class mvPixelConstantBuffer : public mvConstantBuffer<T>
    {
        using mvConstantBuffer<T>::m_buffer;
        using mvConstantBuffer<T>::m_slot;
    public:

        using mvConstantBuffer<T>::mvConstantBuffer;
        void bind(mvGraphics& graphics) override
        {
            graphics.getContext()->PSSetConstantBuffers(m_slot, 1u, m_buffer.GetAddressOf());
        }

    };

    template <typename T>
    class mvGeometryConstantBuffer : public mvConstantBuffer<T>
    {
        using mvConstantBuffer<T>::m_buffer;
        using mvConstantBuffer<T>::m_slot;
    public:

        using mvConstantBuffer<T>::mvConstantBuffer;
        void bind(mvGraphics& graphics) override
        {
            graphics.getContext()->GSSetConstantBuffers(m_slot, 1u, m_buffer.GetAddressOf());
        }

    };

    template <typename T>
    class mvGlobalConstantBuffer : public mvConstantBuffer<T>
    {
        using mvConstantBuffer<T>::m_buffer;
        using mvConstantBuffer<T>::m_slot;
    public:

        using mvConstantBuffer<T>::mvConstantBuffer;
        void bind(mvGraphics& graphics) override
        {
            graphics.getContext()->PSSetConstantBuffers(m_slot, 1u, m_buffer.GetAddressOf());
            graphics.getContext()->VSSetConstantBuffers(m_slot, 1u, m_buffer.GetAddressOf());
            graphics.getContext()->GSSetConstantBuffers(m_slot, 1u, m_buffer.GetAddressOf());
        }

    };

}