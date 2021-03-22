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
	class mvVertexShader : public mvBindable
	{

	public:

		mvVertexShader(mvGraphics& graphics, const char* path);

		void bind(mvGraphics& graphics) override;

		ID3DBlob* getBlob();


	private:

		mvComPtr<ID3D11VertexShader> m_vertexShader;
		mvComPtr<ID3DBlob>           m_blob;

	};

}