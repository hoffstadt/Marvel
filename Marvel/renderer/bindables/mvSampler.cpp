#pragma once
#include "mvSampler.h"
#include <assert.h>
#include "mvGraphics.h"

namespace Marvel {

    mvSampler::mvSampler(mvGraphics& graphics, mvSampler::Type type, bool reflect, UINT slot)
		:
		m_type(type),
		m_reflect(reflect),
		m_slot(slot)
    {
        // Create Sampler State
		D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };
		samplerDesc.Filter = [type]() {
			switch (type)
			{
			case Type::Anisotropic: return D3D11_FILTER_ANISOTROPIC;
			case Type::Point: return D3D11_FILTER_MIN_MAG_MIP_POINT;
			default:
			case Type::Bilinear: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
		}();
		samplerDesc.AddressU = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;


        HRESULT hResult = graphics.getDevice()->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf());
        assert(SUCCEEDED(hResult));
    }

    void mvSampler::bind(mvGraphics& graphics)
    {
        graphics.getContext()->PSSetSamplers(m_slot, 1, m_samplerState.GetAddressOf());
    }

}