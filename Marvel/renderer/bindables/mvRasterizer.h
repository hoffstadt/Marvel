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
	// mvRasterizer
	//-----------------------------------------------------------------------------
	class mvRasterizer : public mvBindable
	{

	public:

		mvRasterizer(mvGraphics& graphics, bool twoSided);

		void bind(mvGraphics& graphics) override;

	protected:

		mvComPtr<ID3D11RasterizerState> m_rasterizer;
		bool                            m_twoSided;
	};
}
