#pragma once
#include <d3d11.h>
#include <string>
#include "mvComPtr.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvVertexShader
	//-----------------------------------------------------------------------------
	class mvPixelShader
	{

	public:

		static mvPixelShader* Request(mvGraphics& graphics, const std::string& path);
		static std::string    GenerateUniqueIdentifier(const std::string& path);

	public:

		mvPixelShader(mvGraphics& graphics, const std::string& path);

		void set(mvGraphics& graphics);
		std::string getUniqueIdentifier() const;

		ID3DBlob* getBlob();

	private:

		mvComPtr<ID3D11PixelShader> m_pixelShader;
		mvComPtr<ID3DBlob>          m_blob;
		std::string                 m_path;

	};

}