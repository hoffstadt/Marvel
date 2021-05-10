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

		static std::shared_ptr<mvVertexShader> Request(mvGraphics& graphics, const std::string& path);
		static std::string                     GenerateUniqueIdentifier(const std::string& path);

	public:

		mvVertexShader(mvGraphics& graphics, const std::string& path);

		void bind(mvGraphics& graphics) override;
		std::string getUniqueIdentifier() const override;

		ID3DBlob* getBlob() const;

	private:

		mvComPtr<ID3D11VertexShader> m_vertexShader;
		mvComPtr<ID3DBlob>           m_blob;
		std::string                  m_path;

	};

}