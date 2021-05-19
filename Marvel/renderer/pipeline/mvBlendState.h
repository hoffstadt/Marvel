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
	// mvBlendState
	//-----------------------------------------------------------------------------
	class mvBlendState
	{

	public:

		static mvBlendState* Request(mvGraphics& graphics, bool blend);
		static std::string GenerateUniqueIdentifier(bool blend);

	public:

		void set(mvGraphics& graphics);

	private:

		mvBlendState(mvGraphics& graphics, bool blend);

		std::string getUniqueIdentifier() const;

	private:

		mvComPtr<ID3D11BlendState> m_state;
		bool m_blend = false;
	};

}