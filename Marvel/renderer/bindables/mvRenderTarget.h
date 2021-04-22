#pragma once
#include <d3d11.h>
#include "mvBindable.h"
#include "mvComPtr.h"
#include "mvDepthStencil.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvRenderTarget
	//-----------------------------------------------------------------------------
	class mvRenderTarget : public mvBindable
	{

	public:

		mvRenderTarget(mvGraphics& graphics, int width, int height);
		mvRenderTarget(mvGraphics& graphics, ID3D11Texture2D* texture);

		void bind(mvGraphics& graphics) override {}

		void bindAsBuffer(mvGraphics& graphics);
		void clear       (mvGraphics& graphics);
		void reset();

		ID3D11RenderTargetView*   getTarget();
		mvComPtr<ID3D11RenderTargetView> getTargetCom();
		ID3D11ShaderResourceView* getShaderResource();

	private:

		UINT                               m_width;
		UINT                               m_height;
		mvComPtr<ID3D11RenderTargetView>   m_target;
		mvComPtr<ID3D11ShaderResourceView> m_shaderResource;
		std::shared_ptr<mvDepthStencil>    m_depthStencil;

	};

}