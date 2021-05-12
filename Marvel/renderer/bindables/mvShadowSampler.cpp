#include "mvShadowSampler.h"
#include <assert.h>
#include "mvGraphics.h"
#include "mvBindableRegistry.h"

namespace Marvel {

	mvShadowSampler::mvShadowSampler(mvGraphics& graphics)
    {

        D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };

        samplerDesc.BorderColor[0] = 1.0f;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;

        bool hwPcf = true;
        bool bilin = true;

        if (hwPcf)
        {
            samplerDesc.Filter = bilin ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR : D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
            samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
        }
        else
        {
            samplerDesc.Filter = bilin ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
        }


        HRESULT hResult = graphics.getDevice()->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf());
        assert(SUCCEEDED(hResult));
    }

    void mvShadowSampler::bind(mvGraphics& graphics)
    {
        graphics.getContext()->PSSetSamplers(1u, 1, m_samplerState.GetAddressOf());
    }


}