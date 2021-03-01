#pragma once
#include "mvMarvelWin.h"
#include "mvComPtr.h"
#include <d3d11_1.h>

namespace Marvel {

	class mvRenderTarget;

	class mvGraphics
	{

	public:

		mvGraphics(HWND hwnd, int width, int height);
		~mvGraphics();

		ID3D11Device*           getDevice();
		ID3D11DeviceContext*    getContext();
		IDXGISwapChain*         getSwapChain();
		ID3D11Texture2D*        getFrameBuffer();
		mvRenderTarget*         getTarget();

	private:

		mvComPtr<ID3D11Device>           m_device;
		mvComPtr<ID3D11DeviceContext>    m_deviceContext;
		mvComPtr<IDXGISwapChain>         m_swapChain;
		mvComPtr<ID3D11Texture2D>        m_frameBuffer;
		mvRenderTarget*                  m_target;
	};

}