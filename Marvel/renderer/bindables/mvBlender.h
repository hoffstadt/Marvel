#pragma once

#include <array>
#include <optional>
#include <d3d11.h>
#include "mvBindable.h"
#include "mvComPtr.h"

namespace Marvel{
	
	//-----------------------------------------------------------------------------
	// Forward Declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvBlender
	//-----------------------------------------------------------------------------
	class mvBlender : public mvBindable
	{

	public:

		mvBlender(mvGraphics& graphics, bool blending);
		
		void bind(mvGraphics& graphics) override;

	protected:
		mvComPtr<ID3D11BlendState> m_blender;
		bool                       m_blending;
	};
}
