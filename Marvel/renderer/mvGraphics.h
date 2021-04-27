#pragma once
#include <string>
#include "mvMarvelWin.h"
#include "mvComPtr.h"
#include <d3d11_1.h>
#include "mvMath.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvRenderTarget;

	//-----------------------------------------------------------------------------
	// mvGraphics
	//-----------------------------------------------------------------------------
	class mvGraphics
	{

	public:

		mvGraphics(HWND hwnd, int width, int height, const std::string& root = "../../Marvel/shaders/");
		~mvGraphics();

		void drawIndexed(UINT count);

		void resize(int width, int height);

		ID3D11Device*        getDevice      ();
		ID3D11DeviceContext* getContext     ();
		IDXGISwapChain*      getSwapChain   ();
		ID3D11Texture2D*     getFrameBuffer ();
		mvRenderTarget*      getTarget      ();
		glm::mat4            getProjection  () const;
		glm::mat4            getCamera      () const;

		void      setProjection    (glm::mat4 proj);
		void      setCamera        (glm::mat4 cam);

		const std::string& getShaderRoot() const { return m_shaderRoot; }

	private:

		mvComPtr<ID3D11Device>        m_device;
		mvComPtr<ID3D11DeviceContext> m_deviceContext;
		mvComPtr<IDXGISwapChain>      m_swapChain;
		mvComPtr<ID3D11Texture2D>     m_frameBuffer;
		mvRenderTarget*               m_target;

		glm::mat4                     m_projection;
		glm::mat4                     m_camera;

		std::string                   m_shaderRoot = "../../Marvel/shaders/";
	};

}