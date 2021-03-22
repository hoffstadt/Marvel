#pragma once
#include <d3d11.h>
#include "mvComPtr.h"
#include "mvBindable.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvVertexShader
	//-----------------------------------------------------------------------------
	class mvPixelShader : public mvBindable
	{

	public:

		mvPixelShader(mvGraphics& graphics, const char* path);

		void bind(mvGraphics& graphics) override;

		ID3DBlob* getBlob();

	private:

		mvComPtr<ID3D11PixelShader> m_pixelShader;
		mvComPtr<ID3DBlob>          m_blob;

	};

}