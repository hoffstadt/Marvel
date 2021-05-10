#pragma once
#include <d3d11.h>
#include <string>
#include "mvBindable.h"
#include "mvComPtr.h"

namespace Marvel {

	class mvTexture : public mvBindable
	{

	public:

		static std::shared_ptr<mvTexture> Request(mvGraphics& graphics, const std::string& path, UINT slot);
		static std::string                GenerateUniqueIdentifier(const std::string& path, UINT slot);

	public:

		mvTexture(mvGraphics& graphics, const std::string& path, UINT slot = 0);

		void bind(mvGraphics& graphics) override;
		std::string getUniqueIdentifier() const override;
		bool hasAlpha() const;

	private:

		mvComPtr<ID3D11Texture2D>          m_texture;
		mvComPtr<ID3D11ShaderResourceView> m_textureView;
		UINT                               m_slot = 0u;
		bool                               m_alpha = false;
		std::string                        m_path;

	};

}