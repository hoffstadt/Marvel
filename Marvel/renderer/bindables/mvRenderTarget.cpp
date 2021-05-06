#pragma once
#include <assert.h>
#include "mvGraphics.h"
#include "mvCommonBindables.h"

namespace Marvel {

	mvRenderTarget::mvRenderTarget(mvGraphics& graphics, int width, int height)
		:
		m_width(width),
		m_height(height)
	{

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // never do we not want to bind offscreen RTs as inputs
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		mvComPtr<ID3D11Texture2D> pTexture;
		graphics.getDevice()->CreateTexture2D(&textureDesc, nullptr, pTexture.GetAddressOf());

		// create the target view on the texture
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = textureDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
		HRESULT hResult = graphics.getDevice()->CreateRenderTargetView(pTexture.Get(), &rtvDesc, m_target.GetAddressOf());

		assert(SUCCEEDED(hResult));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		hResult = graphics.getDevice()->CreateShaderResourceView(pTexture.Get(), &srvDesc, m_shaderResource.GetAddressOf());

		assert(SUCCEEDED(hResult));
	}

	mvRenderTarget::mvRenderTarget(mvGraphics& graphics, ID3D11Texture2D* texture, int face)
	{

		// get information from texture about dimensions
		D3D11_TEXTURE2D_DESC textureDesc;
		texture->GetDesc(&textureDesc);
		m_width = textureDesc.Width;
		m_height = textureDesc.Height;

		// create the target view on the texture
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = textureDesc.Format;
		if (face > -1)
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			rtvDesc.Texture2DArray.ArraySize = 1;
			rtvDesc.Texture2DArray.FirstArraySlice = face;
			rtvDesc.Texture2DArray.MipSlice = 0;
		}
		else
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
		}

		HRESULT hResult = graphics.getDevice()->CreateRenderTargetView(texture, &rtvDesc, m_target.GetAddressOf());
		assert(SUCCEEDED(hResult));
	}

	void mvRenderTarget::clear(mvGraphics& graphics)
	{
		static float clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		graphics.getContext()->ClearRenderTargetView(m_target.Get(), (float*)&clear_color);
	}

	void mvRenderTarget::reset()
	{
		m_target->Release();
		m_shaderResource->Release();
	}

	void mvRenderTarget::bindAsBuffer(mvGraphics& graphics)
	{
		ID3D11DepthStencilView* const null = nullptr;
		bindAsBuffer(graphics, null);
	}

	void mvRenderTarget::bindAsBuffer(mvGraphics& graphics, mvBufferResource* depthStencilView)
	{
		bindAsBuffer(graphics, static_cast<mvDepthStencil*>(depthStencilView)->getDepthStencilView());
	}

	void mvRenderTarget::bindAsBuffer(mvGraphics& graphics, ID3D11DepthStencilView* depthStencilView)
	{
		graphics.getContext()->OMSetRenderTargets(1, m_target.GetAddressOf(), depthStencilView);

		// configure viewport
		D3D11_VIEWPORT vp;
		vp.Width = (float)m_width;
		vp.Height = (float)m_height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;

		graphics.getContext()->RSSetViewports(1u, &vp);
	}

	ID3D11RenderTargetView* mvRenderTarget::getTarget()
	{
		return m_target.Get();
	}

	mvComPtr<ID3D11RenderTargetView> mvRenderTarget::getTargetCom()
	{
		return m_target;
	}

	ID3D11ShaderResourceView* mvRenderTarget::getShaderResource()
	{
		return m_shaderResource.Get();
	}

}