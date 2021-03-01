#include "mvVertexShader.h"
#include <d3dcompiler.h>
#include <assert.h>
#include "mvGraphics.h"
#include "mvMarvelUtils.h"

namespace Marvel {

	mvVertexShader::mvVertexShader(mvGraphics& graphics, const char* path)
	{
        mvComPtr<ID3DBlob> shaderCompileErrorsBlob;
        HRESULT hResult = D3DCompileFromFile(ToWide(path).c_str(), nullptr, nullptr, "main", "vs_5_0", 0, 0,
            m_blob.GetAddressOf(), shaderCompileErrorsBlob.GetAddressOf());
        if (FAILED(hResult))
        {
            const char* errorString = NULL;
            if (hResult == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
                errorString = "Could not compile shader; file not found";
            else if (shaderCompileErrorsBlob)
                errorString = (const char*)shaderCompileErrorsBlob->GetBufferPointer();

            MessageBoxA(0, errorString, "Shader Compiler Error", MB_ICONERROR | MB_OK);
        }

        hResult = graphics.getDevice()->CreateVertexShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
        assert(SUCCEEDED(hResult));
	}

    void mvVertexShader::bind(mvGraphics& graphics)
    {
        graphics.getContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    }

    ID3DBlob* mvVertexShader::getBlob()
    {
        return m_blob.Get();
    }

}