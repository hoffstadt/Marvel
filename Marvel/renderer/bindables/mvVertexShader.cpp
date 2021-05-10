#include "mvVertexShader.h"
#include <d3dcompiler.h>
#include <assert.h>
#include "mvGraphics.h"
#include "mvMarvelUtils.h"
#include "mvBindableRegistry.h"

namespace Marvel {

    std::shared_ptr<mvVertexShader> mvVertexShader::Request(mvGraphics& graphics, const std::string& path)
    {
        std::string ID = mvVertexShader::GenerateUniqueIdentifier(path);
        if (auto bindable = mvBindableRegistry::GetBindable(ID))
            return std::dynamic_pointer_cast<mvVertexShader>(bindable);
        auto bindable = std::make_shared<mvVertexShader>(graphics, path);
        mvBindableRegistry::AddBindable(ID, bindable);
        return bindable;
    }

    std::string mvVertexShader::GenerateUniqueIdentifier(const std::string& path)
    {
        return typeid(mvVertexShader).name() + std::string("$") + path;
    }

	mvVertexShader::mvVertexShader(mvGraphics& graphics, const std::string& path)
	{
        m_path = path;
        mvComPtr<ID3DBlob> shaderCompileErrorsBlob;
        HRESULT hResult = D3DCompileFromFile(ToWide(path).c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0, 0,
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

    ID3DBlob* mvVertexShader::getBlob() const
    {
        return m_blob.Get();
    }

    std::string mvVertexShader::getUniqueIdentifier() const
    {
        return GenerateUniqueIdentifier(m_path);
    }

}