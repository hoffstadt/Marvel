#pragma once

#include <d3d11_1.h>
#include <vector>
#include <string>
#include <memory>
#include "mvComPtr.h"


namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvSamplerState
	//-----------------------------------------------------------------------------
	class mvSamplerState
	{

	public:

		enum class Type
		{
			Anisotropic,
			Bilinear,
			Point,
		};

		enum class Addressing
		{
			Border,
			Wrap,
			Mirror,
			Clamp
		};

		static mvSamplerState* Request(mvGraphics& graphics, Type type, Addressing addressing, UINT slot, bool hwPcf);
		static std::string GenerateUniqueIdentifier(Type type, Addressing addressing, UINT slot, bool hwPcf);

	public:

		void set(mvGraphics& graphics);

	private:

		mvSamplerState(mvGraphics& graphics, Type type, Addressing addressing, UINT slot, bool hwPcf);

		std::string getUniqueIdentifier() const;

	private:

		mvComPtr<ID3D11SamplerState> m_state;
		Type                         m_type;
		Addressing                   m_addressing;
		UINT                         m_slot;
		bool                         m_hwPcf;
	};

}