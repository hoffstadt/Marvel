#include "mvTexture.h"
#include <string>
#include <assert.h>
#include "mvGraphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Marvel {

    mvTexture::mvTexture(mvGraphics& graphics, const std::string& path, UINT slot)
    {
        m_slot = slot;

        // Load Image
        int texWidth, texHeight, texNumChannels;
        int texForceNumChannels = 4;
        unsigned char* testTextureBytes = stbi_load(path.c_str(), &texWidth, &texHeight,
            &texNumChannels, texForceNumChannels);
        assert(testTextureBytes);
        int texBytesPerRow = 4 * texWidth;

        // Create Texture
        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.Width = texWidth;
        textureDesc.Height = texHeight;
        textureDesc.MipLevels = 0;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

        graphics.getDevice()->CreateTexture2D(&textureDesc, nullptr, m_texture.GetAddressOf());

        graphics.getContext()->UpdateSubresource(m_texture.Get(), 0u, nullptr, testTextureBytes, texBytesPerRow, 0u);

        // create the resource view on the texture
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;


        graphics.getDevice()->CreateShaderResourceView(m_texture.Get(), &srvDesc, m_textureView.GetAddressOf());

        graphics.getContext()->GenerateMips(m_textureView.Get());

        free(testTextureBytes);
    }

    void mvTexture::bind(mvGraphics& graphics)
    {
        graphics.getContext()->PSSetShaderResources(m_slot, 1, m_textureView.GetAddressOf());
    }

}