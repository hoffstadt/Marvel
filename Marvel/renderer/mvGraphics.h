#pragma once
#include <string>
#include <thread>
#include <memory>
#include <atomic>
#include "mvMarvelWin.h"
#include "mvImGuiManager.h"
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

		void beginFrame();
		void endFrame();

		void drawIndexed(UINT count);
		void finishRecording();
		void resetCommandList() { m_commandListReady = false; m_commandList = nullptr; }

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
		ID3D11Device*                   getDevice         ();
		ID3D11DeviceContext*            getContext        ();
		ID3D11DeviceContext*            getDeferredContext();
		IDXGISwapChain*                 getSwapChain      ();
		ID3D11Texture2D*                getFrameBuffer    ();
		ID3D11CommandList*              getCommandList    ();
		std::shared_ptr<mvRenderTarget> getTarget         ();
		glm::mat4                       getProjection     () const;
		glm::mat4                       getCamera         () const;
		const std::string&              getShaderRoot     () const { return m_shaderRoot; }
		int                             getWidth          () const { return m_width; }
		int                             getHeight         () const { return m_height; }
		bool                            isCommandListReady() const { return m_commandListReady; }

	private:

		mvComPtr<ID3D11Device>          m_device;
		mvComPtr<ID3D11DeviceContext>   m_deviceContext;
		mvComPtr<ID3D11DeviceContext>   m_deviceDeferredContext;
		mvComPtr<IDXGISwapChain>        m_swapChain;
		mvComPtr<ID3D11Texture2D>       m_frameBuffer;
		std::shared_ptr<mvRenderTarget> m_target = nullptr;
		glm::mat4                       m_projection;
		glm::mat4                       m_camera;
		std::string                     m_shaderRoot = "../../Marvel/shaders/";
		int                             m_width;
		int                             m_height;
		std::unique_ptr<mvImGuiManager> m_imguiManager;
		std::thread::id                 m_mainid;
		mvComPtr<ID3D11CommandList>     m_commandList = nullptr;
		std::atomic_bool                m_commandListReady = false;

	};

}