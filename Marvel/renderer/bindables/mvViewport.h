#pragma once
#include "mvBindable.h"
#include "mvGraphics.h"

namespace Marvel
{
	class mvViewport : public mvBindable
	{
	public:
		mvViewport(mvGraphics& graphics)
			:
			mvViewport(graphics, (float)graphics.getWidth(), (float)graphics.getHeight())
		{
		}

		mvViewport(mvGraphics& graphics, float width, float height)
		{
			vp.Width = width;
			vp.Height = height;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0.0f;
			vp.TopLeftY = 0.0f;
		}
		void bind(mvGraphics& graphics) override
		{
			graphics.getContext()->RSSetViewports(1u, &vp);
		}

	private:
		D3D11_VIEWPORT vp{};
	};
}
