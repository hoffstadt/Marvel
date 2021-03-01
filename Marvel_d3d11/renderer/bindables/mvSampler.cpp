#pragma once
#include "mvSampler.h"
#include <assert.h>
#include "mvGraphics.h"

namespace Marvel {

    mvSampler::mvSampler(mvGraphics& graphics)
    {
        // Create Sampler State
        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.BorderColor[0] = 1.0f;
        samplerDesc.BorderColor[1] = 1.0f;
        samplerDesc.BorderColor[2] = 1.0f;
        samplerDesc.BorderColor[3] = 1.0f;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;


        HRESULT hResult = graphics.getDevice()->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf());
        assert(SUCCEEDED(hResult));
    }

    void mvSampler::bind(mvGraphics& graphics)
    {
        graphics.getContext()->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
    }

}