#pragma once
#include <d3d11.h>
#include <string>
#include "mvBindable.h"
#include "mvComPtr.h"

namespace Marvel {

	class mvTexture : public mvBindable
	{

	public:

		mvTexture(mvGraphics& graphics, const std::string& path);

		void bind(mvGraphics& graphics) override;

	private:

		mvComPtr<ID3D11Texture2D>          m_texture;
		mvComPtr<ID3D11ShaderResourceView> m_textureView;

	};

}