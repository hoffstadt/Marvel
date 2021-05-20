#include "mvSamplerState.h"
#include "mvGraphics.h"

namespace Marvel {

	mvSamplerState* mvSamplerState::Request(mvGraphics& graphics, Type type, Addressing addressing, UINT slot, bool hwPcf)
	{
		static std::vector<std::unique_ptr<mvSamplerState>> states;

		std::string ID = GenerateUniqueIdentifier(type, addressing, slot, hwPcf);

		for (auto& state : states)
		{
			if (state->getUniqueIdentifier() == ID)
				return state.get();
		}

		states.emplace_back(new mvSamplerState(graphics, type, addressing, slot, hwPcf));

		return states.back().get();
	}

	std::string mvSamplerState::GenerateUniqueIdentifier(Type type, Addressing addressing, UINT slot, bool hwPcf)
	{
		return typeid(mvSamplerState).name() + std::string("$type") + std::to_string((int)type) + 
			std::string("$addressing") + std::to_string((int)addressing) + 
			std::string("$slot") + std::to_string((int)slot) + std::string("$hwPCF") + std::string(hwPcf ? "true" : "false");
	}

	mvSamplerState::mvSamplerState(mvGraphics& graphics, Type type, Addressing addressing, UINT slot, bool hwPcf)
		:
		m_type(type),
		m_addressing(addressing),
		m_slot(slot),
		m_hwPcf(hwPcf)
	{
		// Create Sampler State
		D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };

		switch (addressing)
		{

		case(Addressing::Border):
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
			break;

		case(Addressing::Wrap):
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			break;

		case(Addressing::Mirror):
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
			break;

		default:
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			break;
		}

		if (hwPcf)
		{
			samplerDesc.BorderColor[0] = 1.0f;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;

			samplerDesc.Filter = [type]() {
				switch (type)
				{
				case Type::Anisotropic:
					return D3D11_FILTER_COMPARISON_ANISOTROPIC;

				case Type::Point:
					return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;

				case Type::Bilinear:
					return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;

				default:
					return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
				}
			}();
		}
		else
		{
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

			samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
		}

		HRESULT hResult = graphics.getDevice()->CreateSamplerState(&samplerDesc, m_state.GetAddressOf());
		assert(SUCCEEDED(hResult));
	}

	void mvSamplerState::set(mvGraphics& graphics)
	{
		graphics.getContext()->PSSetSamplers(m_slot, 1, m_state.GetAddressOf());
	}

	std::string mvSamplerState::getUniqueIdentifier() const
	{
		return GenerateUniqueIdentifier(m_type, m_addressing, m_slot, m_hwPcf);
	}

}