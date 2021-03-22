#pragma once
#include "mvSampler.h"
#include <assert.h>
#include "mvGraphics.h"

namespace Marvel {

    mvSampler::mvSampler(mvGraphics& graphics)
    {
        // Create Sampler State
        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MinLOD = 0.0f;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
        samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;


        HRESULT hResult = graphics.getDevice()->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf());
        assert(SUCCEEDED(hResult));
    }

    void mvSampler::bind(mvGraphics& graphics)
    {
        graphics.getContext()->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
    }

}