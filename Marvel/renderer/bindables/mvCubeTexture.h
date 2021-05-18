#pragma once

#include <string>
#include <d3d11.h>
#include <memory>
#include <vector>
#include "mvBindable.h"
#include "mvComPtr.h"
#include "mvRenderTarget.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvCubeTexture
	//-----------------------------------------------------------------------------
	class mvCubeTexture : public mvBindable
	{

	public:

		mvCubeTexture(mvGraphics& graphics, const std::string& path, UINT slot = 0);

		void bind(mvGraphics& graphics) override;
	
	private:

		unsigned int                       m_slot;
		std::string                        m_path;
		mvComPtr<ID3D11ShaderResourceView> m_textureView;
	};

	//-----------------------------------------------------------------------------
	// mvCubeTargetTexture
	//-----------------------------------------------------------------------------
	class mvCubeTargetTexture : public mvBindable
	{

	public:

		mvCubeTargetTexture(mvGraphics& graphics, UINT width, UINT height, UINT slot = 0, DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);

		void bind(mvGraphics& graphics) override;

		std::shared_ptr<mvRenderTarget> getRenderTarget(size_t index) const;
	
	private:

		unsigned int m_slot;
		mvComPtr<ID3D11ShaderResourceView> m_textureView;
		std::vector<std::shared_ptr<mvRenderTarget>> m_renderTargets;
	};

	//-----------------------------------------------------------------------------
	// mvCubeDepthTexture
	//-----------------------------------------------------------------------------
	class mvCubeDepthTexture : public mvBindable
	{

	public:

		mvCubeDepthTexture(mvGraphics& graphics, UINT size, UINT slot = 0);

		void bind(mvGraphics& graphics) override;

		std::shared_ptr<mvOutputDepthStencil> getDepthBuffer(size_t index) const;

	private:

		unsigned int m_slot;
		mvComPtr<ID3D11ShaderResourceView> m_textureView;
		std::vector<std::shared_ptr<mvOutputDepthStencil>> m_depthBuffers;
	};

	//-----------------------------------------------------------------------------
	// mvDepthTexture
	//-----------------------------------------------------------------------------
	class mvDepthTexture : public mvBindable
	{

	public:

		mvDepthTexture(mvGraphics& graphics, UINT size, UINT slot = 0);

		void bind(mvGraphics& graphics) override;

		std::shared_ptr<mvOutputDepthStencil> getDepthBuffer()const;

	private:

		unsigned int m_slot;
		mvComPtr<ID3D11ShaderResourceView> m_textureView;
		std::shared_ptr<mvOutputDepthStencil> m_depthBuffer;
	};

}