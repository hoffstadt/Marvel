#include "mvTexture.h"
#include <string>
#include <assert.h>
#include "mvGraphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Marvel {

    mvTexture::mvTexture(mvGraphics& graphics, const std::string& path)
    {
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
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        //textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA textureSubresourceData = {};
        textureSubresourceData.pSysMem = testTextureBytes;
        textureSubresourceData.SysMemPitch = texBytesPerRow;


        graphics.getDevice()->CreateTexture2D(&textureDesc, &textureSubresourceData, m_texture.GetAddressOf());
        graphics.getDevice()->CreateShaderResourceView(m_texture.Get(), nullptr, m_textureView.GetAddressOf());

        free(testTextureBytes);
    }

    void mvTexture::bind(mvGraphics& graphics)
    {
        graphics.getContext()->PSSetShaderResources(0, 1, m_textureView.GetAddressOf());
    }

}