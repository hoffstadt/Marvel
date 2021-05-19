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
	// mvRasterizerState
	//-----------------------------------------------------------------------------
	class mvRasterizerState
	{

	public:

		static mvRasterizerState* Request(mvGraphics& graphics, bool cull, bool hwpcf, int depthBias, float slopeBias, float clamp);
		static std::string GenerateUniqueIdentifier(bool cull, bool hwpcf, int depthBias, float slopeBias, float clamp);

	public:

		void set(mvGraphics& graphics);

	private:

		mvRasterizerState(mvGraphics& graphics, bool cull, bool hwpcf, int depthBias, float slopeBias, float clamp);

		std::string getUniqueIdentifier() const;

	private:

		mvComPtr<ID3D11RasterizerState> m_state;
		bool m_cull = false;

		// pcf
		bool m_hwpcf = false;
		int m_depthBias;
		float m_slopeBias;
		float m_clamp;
	};

}