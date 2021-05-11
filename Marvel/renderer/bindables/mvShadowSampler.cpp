#include "mvShadowSampler.h"
#include <assert.h>
#include "mvGraphics.h"
#include "mvBindableRegistry.h"

namespace Marvel {

	mvShadowSampler::mvShadowSampler(mvGraphics& graphics)
    {
        D3D11_SAMPLER_DESC comparisonSamplerDesc;
        ZeroMemory(&comparisonSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
        comparisonSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        comparisonSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        comparisonSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        comparisonSamplerDesc.BorderColor[0] = 1.0f;
        comparisonSamplerDesc.BorderColor[1] = 1.0f;
        comparisonSamplerDesc.BorderColor[2] = 1.0f;
        comparisonSamplerDesc.BorderColor[3] = 1.0f;
        comparisonSamplerDesc.MinLOD = 0.f;
        comparisonSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
        comparisonSamplerDesc.MipLODBias = 0.f;
        comparisonSamplerDesc.MaxAnisotropy = 0;
        comparisonSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
        comparisonSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;


        HRESULT hResult = graphics.getDevice()->CreateSamplerState(&comparisonSamplerDesc, m_samplerState.GetAddressOf());
        assert(SUCCEEDED(hResult));
    }

    void mvShadowSampler::bind(mvGraphics& graphics)
    {
        graphics.getContext()->PSSetSamplers(1u, 1, m_samplerState.GetAddressOf());
    }


}