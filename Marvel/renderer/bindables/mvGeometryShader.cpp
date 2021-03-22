#include "mvGeometryShader.h"
#include <d3dcompiler.h>
#include <assert.h>
#include "mvMarvelUtils.h"
#include "mvGraphics.h"
#include "mvBindable.h"

namespace Marvel {

    mvGeometryShader::mvGeometryShader(mvGraphics& graphics, const char* path)
		{


            Marvel::mvComPtr<ID3DBlob> shaderCompileErrorsBlob;
            HRESULT hResult = D3DCompileFromFile(ToWide(path).c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "gs_5_0", 0, 0,
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

            hResult = graphics.getDevice()->CreateGeometryShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, m_geometryShader.GetAddressOf());
            assert(SUCCEEDED(hResult));

		}

        void mvGeometryShader::bind(mvGraphics& graphics)
        {
            graphics.getContext()->GSSetShader(m_geometryShader.Get(), nullptr, 0);
        }

        ID3DBlob* mvGeometryShader::getBlob()
        { 
            return m_blob.Get(); 
        }

}