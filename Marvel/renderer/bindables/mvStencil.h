#pragma once

#include <d3d11.h>
#include "mvBindable.h"
#include "mvComPtr.h"

namespace Marvel{

	class mvStencil : public mvBindable
	{

	public:

		enum class Mode
		{
			Off,
			Write,
			Mask,
			DepthOff,
			DepthReversed,
			DepthFirst, // for skybox render
		};

		mvStencil(mvGraphics& graphics, Mode mode);

		void bind(mvGraphics& graphics) override;

	private:

		Mode                              m_mode;
		mvComPtr<ID3D11DepthStencilState> m_stencil;
	};
}
