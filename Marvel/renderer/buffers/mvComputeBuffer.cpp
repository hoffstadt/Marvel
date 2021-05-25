#include "mvComputeBuffer.h"

namespace Marvel {

    mvComputeBuffer::mvComputeBuffer(mvGraphics& graphics, UINT slot, UINT numElements, UINT stride)
        :
        mvComputeBuffer(graphics, slot, nullptr, numElements, stride)
    {

    }

    mvComputeBuffer::mvComputeBuffer(mvGraphics& graphics, UINT slot, void* data, UINT numElements, UINT stride)
        : 
        m_slot(slot)
    {
        D3D11_BUFFER_DESC cbd = {};
        cbd.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
        cbd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        cbd.ByteWidth = numElements*stride;
        cbd.StructureByteStride = stride;

        if (data)
        {
            m_input = true;
            D3D11_SUBRESOURCE_DATA csd = {};
            csd.pSysMem = data;
            HRESULT hResult = graphics.getDevice()->CreateBuffer(&cbd, &csd, m_buffer.GetAddressOf());
            assert(SUCCEEDED(hResult));
        }
        else
        {
            m_input = false;
            HRESULT hResult = graphics.getDevice()->CreateBuffer(&cbd, nullptr, m_buffer.GetAddressOf());
            assert(SUCCEEDED(hResult));
        }

        if(m_input)
        {
            // Now we create a view on the resource. DX11 requires you to send the data
            // to shaders using a "shader view"
            D3D11_BUFFER_DESC descBuf;
            m_buffer->GetDesc(&descBuf);

            D3D11_SHADER_RESOURCE_VIEW_DESC descView;
            descView.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
            descView.BufferEx.FirstElement = 0;

            descView.Format = DXGI_FORMAT_UNKNOWN;
            descView.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;

            HRESULT hResult = graphics.getDevice()->CreateShaderResourceView(m_buffer.Get(),
                &descView, m_shaderResourceView.GetAddressOf());
            assert(SUCCEEDED(hResult));
        }

        else
        {
            // Now we create a view on the resource. DX11 requires you to send the data
            // to shaders using a "shader view"
            D3D11_BUFFER_DESC descBuf;
            m_buffer->GetDesc(&descBuf);

            D3D11_UNORDERED_ACCESS_VIEW_DESC descView = {};
            descView.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
            descView.Buffer.FirstElement = 0;

            descView.Format = DXGI_FORMAT_UNKNOWN;
            descView.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;

            HRESULT hResult = graphics.getDevice()->CreateUnorderedAccessView(m_buffer.Get(),
                &descView, m_unorderedAccessView.GetAddressOf());
            assert(SUCCEEDED(hResult));
        }


    }

    void mvComputeBuffer::bind(mvGraphics& graphics)
    {
        if (m_input)
            graphics.getContext()->CSSetShaderResources(m_slot, 1u, m_shaderResourceView.GetAddressOf());
        else
            graphics.getContext()->CSSetUnorderedAccessViews(m_slot, 1u, m_unorderedAccessView.GetAddressOf(), nullptr);
    }

}