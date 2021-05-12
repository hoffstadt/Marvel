#include "mvDepthStencil.h"
#include "mvGraphics.h"
#include "mvRenderTarget.h"

namespace Marvel {

    DXGI_FORMAT MapUsageTypeless(mvDepthStencil::Usage usage)
    {
        switch (usage)
        {
        case mvDepthStencil::Usage::DepthStencil:
            return DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
        case mvDepthStencil::Usage::ShadowDepth:
            return DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
        }
    }

    DXGI_FORMAT MapUsageTyped(mvDepthStencil::Usage usage)
    {
        switch (usage)
        {
        case mvDepthStencil::Usage::DepthStencil:
            return DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
        case mvDepthStencil::Usage::ShadowDepth:
            return DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
        }
    }

    DXGI_FORMAT MapUsageColored(mvDepthStencil::Usage usage)
    {
        switch (usage)
        {
        case mvDepthStencil::Usage::DepthStencil:
            return DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        case mvDepthStencil::Usage::ShadowDepth:
            return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
        }
    }

    mvDepthStencil::mvDepthStencil(mvGraphics& graphics, mvComPtr<ID3D11Texture2D> texture, UINT face)
    {
        D3D11_TEXTURE2D_DESC descTex = {};
        texture->GetDesc(&descTex);

        // create target view of depth stensil texture
        D3D11_DEPTH_STENCIL_VIEW_DESC descView = {};
        descView.Format = DXGI_FORMAT_D32_FLOAT;
        descView.Flags = 0;
        descView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
        descView.Texture2DArray.MipSlice = 0;
        descView.Texture2DArray.ArraySize = 1;
        descView.Texture2DArray.FirstArraySlice = face;

        graphics.getDevice()->CreateDepthStencilView(texture.Get(), &descView, m_DSV.GetAddressOf());
    }

	mvDepthStencil::mvDepthStencil(mvGraphics& graphics, int width, int height, bool shaderBinding, Usage usage)
	{

        // create depth stensil texture
        mvComPtr<ID3D11Texture2D> pDepthStencil;
        D3D11_TEXTURE2D_DESC descDepth = {};
        descDepth.Width = width;
        descDepth.Height = height;
        descDepth.MipLevels = 1u;
        descDepth.ArraySize = 1u;
        descDepth.Format = MapUsageTypeless(usage);
        descDepth.SampleDesc.Count = 1u;
        descDepth.SampleDesc.Quality = 0u;
        descDepth.Usage = D3D11_USAGE_DEFAULT;
        descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | (shaderBinding ? D3D11_BIND_SHADER_RESOURCE : 0);
        graphics.getDevice()->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

        // create view of depth stensil texture
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
        descDSV.Format = MapUsageTyped(usage);
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0u;
        descDSV.Flags = 0;
        graphics.getDevice()->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, m_DSV.GetAddressOf());
	}

    ID3D11DepthStencilView* mvDepthStencil::getDepthStencilView()
    {
        return m_DSV.Get();
    }

    void mvDepthStencil::clear(mvGraphics& graphics)
    {
        graphics.getContext()->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
    }

    void mvDepthStencil::reset()
    {
        m_DSV->Release();
    }

    void mvDepthStencil::bindAsBuffer(mvGraphics& graphics)
    {
        graphics.getContext()->OMSetRenderTargets(0, nullptr, m_DSV.Get());
    }

    void mvDepthStencil::bindAsBuffer(mvGraphics& graphics, mvBufferResource* renderTarget)
    {
        bindAsBuffer(graphics, static_cast<mvRenderTarget*>(renderTarget));
    }

    void mvDepthStencil::bindAsBuffer(mvGraphics& graphics, mvRenderTarget* renderTarget)
    {
        renderTarget->bindAsBuffer(graphics, this);
    }

    mvInputDepthStencil::mvInputDepthStencil(mvGraphics& graphics, int width, int height, UINT slot, Usage usage)
        : 
        mvDepthStencil(graphics, width, height, true, usage),
        m_slot(slot)
    {
        mvComPtr<ID3D11Resource> pRes;
        m_DSV->GetResource(pRes.GetAddressOf());

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = MapUsageColored(usage);
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;
        graphics.getDevice()->CreateShaderResourceView(pRes.Get(), &srvDesc, m_shaderResourceView.GetAddressOf());
    }

    mvInputDepthStencil::mvInputDepthStencil(mvGraphics& graphics, UINT slot, Usage usage)
        :
        mvInputDepthStencil(graphics, graphics.getWidth(), graphics.getHeight(), slot, usage)
    {

    }

    void mvInputDepthStencil::bind(mvGraphics& graphics)
    {
        graphics.getContext()->PSSetShaderResources(m_slot, 1u, m_shaderResourceView.GetAddressOf());
    }

    mvOutputDepthStencil::mvOutputDepthStencil(mvGraphics& graphics, int width, int height)
        :
        mvDepthStencil(graphics, width, height)
    {
    }

    mvOutputDepthStencil::mvOutputDepthStencil(mvGraphics& graphics)
        :
        mvDepthStencil(graphics, graphics.getWidth(), graphics.getHeight())
    {
    }

    mvOutputDepthStencil::mvOutputDepthStencil(mvGraphics& graphics, mvComPtr<ID3D11Texture2D> texture, UINT face)
        :
        mvDepthStencil(graphics, std::move(texture), face)
    {

    }
}