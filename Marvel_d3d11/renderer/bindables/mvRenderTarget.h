#pragma once
#include <d3d11.h>
#include "mvBindable.h"
#include "mvComPtr.h"

namespace Marvel {

	class mvGraphics;
	class mvDepthStencil;

	class mvRenderTarget : public mvBindable
	{

	public:

		mvRenderTarget(mvGraphics& graphics, int width, int height);
		mvRenderTarget(mvGraphics& graphics, ID3D11Texture2D* texture);

		void bind(mvGraphics& graphics) override {}
		void mvRenderTarget::bindAsBuffer(mvGraphics& graphics);

		void clear(mvGraphics& graphics);

		ID3D11RenderTargetView*   getTarget();
		ID3D11ShaderResourceView* getShaderResource();

	private:

		UINT                             m_width;
		UINT                             m_height;
		mvComPtr<ID3D11RenderTargetView> m_target;
		mvComPtr<ID3D11ShaderResourceView> m_shaderResource;

	};

}