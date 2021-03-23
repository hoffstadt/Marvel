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
	// mvGeometryShader
	//-----------------------------------------------------------------------------
	class mvGeometryShader : public mvBindable
	{

	public:

		mvGeometryShader(mvGraphics& graphics, const char* path);

        void bind(mvGraphics& graphics) override;

        ID3DBlob* getBlob();


	private:

		mvComPtr<ID3D11GeometryShader> m_geometryShader;
		mvComPtr<ID3DBlob>             m_blob;

	};

}