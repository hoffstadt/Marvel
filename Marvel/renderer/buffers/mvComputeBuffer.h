#pragma once
#include <d3d11.h>
#include <assert.h>
#include "mvComPtr.h"
#include "mvBuffer.h"
#include "mvGraphics.h"

namespace Marvel {

    class mvComputeBuffer : public mvBuffer
    {

    public:

        mvComputeBuffer(mvGraphics& graphics, UINT slot, UINT numElements, UINT stride);
        mvComputeBuffer(mvGraphics& graphics, UINT slot, void* data, UINT numElements, UINT stride);

        void bind(mvGraphics& graphics) override;

    protected:

        UINT                                m_slot;
        mvComPtr<ID3D11Buffer>              m_buffer = nullptr;
        mvComPtr<ID3D11ShaderResourceView>  m_shaderResourceView = nullptr;
        mvComPtr<ID3D11UnorderedAccessView> m_unorderedAccessView = nullptr;
        bool                                m_input = true;
    };

    template<typename T>
    class mvComputeInputBuffer : public mvComputeBuffer
    {

    public:

        mvComputeInputBuffer(mvGraphics& graphics, UINT slot, void* data, UINT numElements)
            :
            mvComputeBuffer(graphics, slot, data, numElements, sizeof(T))
        {

        }

    };

    template<typename T>
    class mvComputeOutputBuffer : public mvComputeBuffer
    {

    public:

        mvComputeOutputBuffer(mvGraphics& graphics, UINT slot, UINT numElements)
            :
            mvComputeBuffer(graphics, slot, numElements, sizeof(T))
        {

        }

        T* getDataFromGPU(mvGraphics& graphics)
        {
            mvComPtr<ID3D11Buffer> stagingBuffer;

            D3D11_BUFFER_DESC cbd;
            m_buffer.Get()->GetDesc(&cbd);
            cbd.BindFlags = 0;
            cbd.MiscFlags = 0;
            cbd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
            cbd.Usage = D3D11_USAGE_STAGING;


            HRESULT hResult = graphics.getDevice()->CreateBuffer(&cbd, nullptr, stagingBuffer.GetAddressOf());
            assert(SUCCEEDED(hResult));

            graphics.getContext()->CopyResource(stagingBuffer.Get(), m_buffer.Get());

            D3D11_MAPPED_SUBRESOURCE MappedResource;
            T* p;
            graphics.getContext()->Map(stagingBuffer.Get(), 0, D3D11_MAP_READ, 0, &MappedResource);

            p = (T*)MappedResource.pData;

            return p;
        }

    };

}