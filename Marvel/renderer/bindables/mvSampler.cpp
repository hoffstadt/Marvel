#pragma once
#include "mvSampler.h"
#include <assert.h>
#include "mvGraphics.h"
#include "mvBindableRegistry.h"

namespace Marvel {

	std::shared_ptr<mvSampler> mvSampler::Request(mvGraphics& graphics, Type type, bool reflect, UINT slot)
	{
		std::string ID = mvSampler::GenerateUniqueIdentifier(type, reflect, slot);
		if (auto bindable = mvBindableRegistry::GetBindable(ID))
			return std::dynamic_pointer_cast<mvSampler>(bindable);
		auto bindable = std::make_shared<mvSampler>(graphics, type, reflect, slot);
		mvBindableRegistry::AddBindable(ID, bindable);
		return bindable;
	}

	std::string mvSampler::GenerateUniqueIdentifier(Type type, bool reflect, UINT slot)
	{
		return typeid(mvSampler).name() + std::string("$Type") + std::to_string((int)type) + std::string("$Reflect") +
			std::string(reflect ? "true" : "false") + std::string("@") + std::to_string(slot);
	}

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
			case Type::Anisotropic: 
				return D3D11_FILTER_ANISOTROPIC;

			case Type::Point: 
				return D3D11_FILTER_MIN_MAG_MIP_POINT;

			case Type::Bilinear: 
				return D3D11_FILTER_MIN_MAG_MIP_LINEAR;

			default:
				return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
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

	std::string mvSampler::getUniqueIdentifier() const
	{
		return GenerateUniqueIdentifier(m_type, m_reflect, m_slot);
	}

}