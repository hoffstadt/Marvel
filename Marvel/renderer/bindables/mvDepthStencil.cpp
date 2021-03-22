#include "mvDepthStencil.h"
#include "mvGraphics.h"

namespace Marvel {

	mvDepthStencil::mvDepthStencil(mvGraphics& graphics, int width, int height)
	{
        // create depth stensil state
        D3D11_DEPTH_STENCIL_DESC dsDesc = {};
        dsDesc.DepthEnable = TRUE;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
        mvComPtr<ID3D11DepthStencilState> pDSState;
        graphics.getDevice()->CreateDepthStencilState(&dsDesc, &pDSState);

        // bind depth state
        graphics.getContext()->OMSetDepthStencilState(pDSState.Get(), 1u);

        // create depth stensil texture
        mvComPtr<ID3D11Texture2D> pDepthStencil;
        D3D11_TEXTURE2D_DESC descDepth = {};
        descDepth.Width = width;
        descDepth.Height = height;
        descDepth.MipLevels = 1u;
        descDepth.ArraySize = 1u;
        descDepth.Format = DXGI_FORMAT_D32_FLOAT;
        descDepth.SampleDesc.Count = 1u;
        descDepth.SampleDesc.Quality = 0u;
        descDepth.Usage = D3D11_USAGE_DEFAULT;
        descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        graphics.getDevice()->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

        // create view of depth stensil texture
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
        descDSV.Format = DXGI_FORMAT_D32_FLOAT;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0u;
        graphics.getDevice()->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &m_DSV);
	}

    ID3D11DepthStencilView* mvDepthStencil::getDepthStencilView()
    {
        return m_DSV.Get();
    }

    void mvDepthStencil::clear(mvGraphics& graphics)
    {
        graphics.getContext()->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
    }

}