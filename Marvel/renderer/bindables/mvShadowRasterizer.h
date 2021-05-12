#pragma once
#include <array>
#include <d3d11.h>
#include "mvBindable.h"
#include "mvComPtr.h"

namespace Marvel{

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvShadowRasterizer
	//-----------------------------------------------------------------------------
	class mvShadowRasterizer : public mvBindable
	{

	public:

		mvShadowRasterizer(mvGraphics& graphics, int depthBias, float slopeBias, float clamp);

		void bind(mvGraphics& graphics) override;

	protected:

		mvComPtr<ID3D11RasterizerState> m_rasterizer;
		int                             m_depthBias;
		float                           m_slopeBias;
		float                           m_clamp;
	};
}
