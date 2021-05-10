#include "mvPixelShader.h"
#include <d3dcompiler.h>
#include <assert.h>
#include "mvMarvelUtils.h"
#include "mvGraphics.h"
#include "mvBindableRegistry.h"

namespace Marvel {

    std::shared_ptr<mvPixelShader> mvPixelShader::Request(mvGraphics& graphics, const std::string& path)
    {
        std::string ID = mvPixelShader::GenerateUniqueIdentifier(path);
        if (auto bindable = mvBindableRegistry::GetBindable(ID))
            return std::dynamic_pointer_cast<mvPixelShader>(bindable);
        auto bindable = std::make_shared<mvPixelShader>(graphics, path);
        mvBindableRegistry::AddBindable(ID, bindable);
        return bindable;
    }

    std::string mvPixelShader::GenerateUniqueIdentifier(const std::string& path)
    {
        return typeid(mvPixelShader).name() + std::string("$") + path;
    }

    mvPixelShader::mvPixelShader(mvGraphics& graphics, const std::string& path)
    {
        m_path = path;
        Marvel::mvComPtr<ID3DBlob> shaderCompileErrorsBlob;
        HRESULT hResult = D3DCompileFromFile(ToWide(path).c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", 0, 0,
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

        hResult = graphics.getDevice()->CreatePixelShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf());
        assert(SUCCEEDED(hResult));

    }

    void mvPixelShader::bind(mvGraphics& graphics)
    {
        graphics.getContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0);
    }

    ID3DBlob* mvPixelShader::getBlob() 
    { 
        return m_blob.Get(); 
    }

    std::string mvPixelShader::getUniqueIdentifier() const
    {
        return GenerateUniqueIdentifier(m_path);
    }

}