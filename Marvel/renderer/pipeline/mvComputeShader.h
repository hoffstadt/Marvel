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
	// mvComputeShader
	//-----------------------------------------------------------------------------
	class mvComputeShader
	{

	public:

		static mvComputeShader* Request(mvGraphics& graphics, const std::string& path);
		static std::string      GenerateUniqueIdentifier(const std::string& path);

	public:

		mvComputeShader(mvGraphics& graphics, const std::string& path);

		void set(mvGraphics& graphics);
		void dispatch(mvGraphics& graphics, UINT xThreads, UINT yThreads, UINT zThreads);
		std::string getUniqueIdentifier() const;

		ID3DBlob* getBlob();

	private:

		mvComPtr<ID3D11ComputeShader> m_computeShader;
		mvComPtr<ID3DBlob>          m_blob;
		std::string                 m_path;

	};

}