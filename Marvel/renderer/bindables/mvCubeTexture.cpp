#include "mvCubeTexture.h"
#include "mvGraphics.h"
#include <vector>

#include "stb_image.h"

namespace Marvel {

	mvCubeTexture::mvCubeTexture(mvGraphics& graphics, const std::string& path, UINT slot)
		:
		m_path(path),
		m_slot(slot)
	{

		// Load Image
		int texWidth, texHeight, texNumChannels, textBytesPerRow;

		// load 6 surfaces for cube faces
		std::vector<unsigned char*> surfaces;
		for (int i = 0; i < 6; i++)
		{
			// Load Image
			int texForceNumChannels = 4;
			std::string file = path + "\\" + std::to_string(i) + ".png";
			unsigned char* testTextureBytes = stbi_load(file.c_str(), &texWidth, &texHeight,
				&texNumChannels, texForceNumChannels);
			assert(testTextureBytes);
			textBytesPerRow = 4 * texWidth;

			surfaces.push_back(testTextureBytes);
		}

		// texture descriptor
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = texWidth;
		textureDesc.Height = texHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 6;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		// subresource data
		D3D11_SUBRESOURCE_DATA data[6];
		for (int i = 0; i < 6; i++)
		{
			data[i].pSysMem = surfaces[i];
			data[i].SysMemPitch = textBytesPerRow;
			data[i].SysMemSlicePitch = 0;
		}
		// create the texture resource
		mvComPtr<ID3D11Texture2D> pTexture;
		graphics.getDevice()->CreateTexture2D(&textureDesc, data, &pTexture);

		// create the resource view on the texture
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		graphics.getDevice()->CreateShaderResourceView(pTexture.Get(), &srvDesc, m_textureView.GetAddressOf());
	}

	void mvCubeTexture::bind(mvGraphics& graphics)
	{
		graphics.getContext()->PSSetShaderResources(m_slot, 1u, m_textureView.GetAddressOf());
	}


}