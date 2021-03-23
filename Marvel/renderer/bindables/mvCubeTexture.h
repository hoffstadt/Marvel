#pragma once

#include <string>
#include <d3d11.h>
#include "mvBindable.h"
#include "mvComPtr.h"

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

}