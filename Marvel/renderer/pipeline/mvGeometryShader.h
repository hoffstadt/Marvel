#pragma once
#include <d3d11.h>
#include "mvComPtr.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvGeometryShader
	//-----------------------------------------------------------------------------
	class mvGeometryShader
	{

	public:

		mvGeometryShader(mvGraphics& graphics, const char* path);

        void set(mvGraphics& graphics);

        ID3DBlob* getBlob();


	private:

		mvComPtr<ID3D11GeometryShader> m_geometryShader;
		mvComPtr<ID3DBlob>             m_blob;

	};

}