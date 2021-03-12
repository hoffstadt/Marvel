#pragma once
#include "mvMarvelWin.h"
#include "mvComPtr.h"
#include <d3d11_1.h>
#include "mvMath.h"

namespace Marvel {

	class mvRenderTarget;

	class mvGraphics
	{

	public:

		mvGraphics(HWND hwnd, int width, int height);
		~mvGraphics();

		void drawIndexed(UINT count);

		ID3D11Device*        getDevice      ();
		ID3D11DeviceContext* getContext     ();
		IDXGISwapChain*      getSwapChain   ();
		ID3D11Texture2D*     getFrameBuffer ();
		mvRenderTarget*      getTarget      ();
		glm::mat4            getProjection  () const;
		glm::mat4            getCamera      () const;

		void      setProjection    (glm::mat4 proj);
		void      setCamera        (glm::mat4 cam);

	private:

		mvComPtr<ID3D11Device>        m_device;
		mvComPtr<ID3D11DeviceContext> m_deviceContext;
		mvComPtr<IDXGISwapChain>      m_swapChain;
		mvComPtr<ID3D11Texture2D>     m_frameBuffer;
		mvRenderTarget*               m_target;

		glm::mat4                     m_projection;
		glm::mat4                     m_camera;
	};

}