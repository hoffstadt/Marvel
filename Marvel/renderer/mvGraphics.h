#pragma once
#include <string>
#include <memory>
#include "mvMarvelWin.h"
#include "mvComPtr.h"
#include <d3d11_1.h>
#include "mvMath.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvRenderTarget;
	class mvDepthStencil;

	//-----------------------------------------------------------------------------
	// mvGraphics
	//-----------------------------------------------------------------------------
	class mvGraphics
	{

	public:

		mvGraphics(HWND hwnd, int width, int height, const std::string& root = "../../Marvel/shaders/");
		~mvGraphics();

		void drawIndexed(UINT count);

		// called when the viewport is resized
		//   * recreates render target
		//   * recreates depth stencil
		//   * resizes framebuffer
		void resize(int width, int height);

		// clears render target/depth stencil
		void releaseBuffers();

		// setters
		void setProjection(glm::mat4 proj);
		void setCamera    (glm::mat4 cam);

		// getters
		ID3D11Device*                   getDevice     ();
		ID3D11DeviceContext*            getContext    ();
		IDXGISwapChain*                 getSwapChain  ();
		ID3D11Texture2D*                getFrameBuffer();
		std::shared_ptr<mvRenderTarget> getTarget     ();
		//std::shared_ptr<mvDepthStencil> getDepthBuffer();
		glm::mat4                       getProjection () const;
		glm::mat4                       getCamera     () const;
		const std::string&              getShaderRoot () const { return m_shaderRoot; }
		int                             getWidth      () const { return m_width; }
		int                             getHeight     () const { return m_height; }

	private:

		mvComPtr<ID3D11Device>          m_device;
		mvComPtr<ID3D11DeviceContext>   m_deviceContext;
		mvComPtr<IDXGISwapChain>        m_swapChain;
		mvComPtr<ID3D11Texture2D>       m_frameBuffer;
		std::shared_ptr<mvRenderTarget> m_target = nullptr;
		//std::shared_ptr<mvDepthStencil> m_depthStencil = nullptr;
		glm::mat4                       m_projection;
		glm::mat4                       m_camera;
		std::string                     m_shaderRoot = "../../Marvel/shaders/";
		int                             m_width;
		int                             m_height;
	};

}