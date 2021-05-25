#include "mvComputeShader.h"
#include <d3dcompiler.h>
#include <assert.h>
#include <vector>
#include "mvMarvelUtils.h"
#include "mvGraphics.h"

namespace Marvel {

    mvComputeShader* mvComputeShader::Request(mvGraphics& graphics, const std::string& path)
    {
        static std::vector<std::unique_ptr<mvComputeShader>> states;

        std::string ID = GenerateUniqueIdentifier(path);

        for (auto& state : states)
        {
            if (state->getUniqueIdentifier() == ID)
                return state.get();
        }

        states.emplace_back(new mvComputeShader(graphics, path));

        return states.back().get();
    }

    std::string mvComputeShader::GenerateUniqueIdentifier(const std::string& path)
    {
        return typeid(mvComputeShader).name() + std::string("$") + path;
    }

    mvComputeShader::mvComputeShader(mvGraphics& graphics, const std::string& path)
    {
        m_path = path;
        Marvel::mvComPtr<ID3DBlob> shaderCompileErrorsBlob;
        HRESULT hResult = D3DCompileFromFile(ToWide(path).c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "cs_5_0", 0, 0,
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

        hResult = graphics.getDevice()->CreateComputeShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, m_computeShader.GetAddressOf());
        assert(SUCCEEDED(hResult));

    }

    void mvComputeShader::set(mvGraphics& graphics)
    {
        graphics.getContext()->CSSetShader(m_computeShader.Get(), nullptr, 0);
    }

    ID3DBlob* mvComputeShader::getBlob()
    { 
        return m_blob.Get(); 
    }

    std::string mvComputeShader::getUniqueIdentifier() const
    {
        return GenerateUniqueIdentifier(m_path);
    }

    void mvComputeShader::dispatch(mvGraphics& graphics, UINT xThreads, UINT yThreads, UINT zThreads)
    {
        set(graphics);
        graphics.getContext()->Dispatch(xThreads, yThreads, zThreads);

        // Some service variables
        ID3D11UnorderedAccessView* ppUAViewNULL[1] = { NULL };
        ID3D11ShaderResourceView* ppSRVNULL[2] = { NULL, NULL };
        graphics.getContext()->CSSetShader(NULL, NULL, 0);
        graphics.getContext()->CSSetUnorderedAccessViews(0, 1, ppUAViewNULL, NULL);
        graphics.getContext()->CSSetShaderResources(0, 2, ppSRVNULL);
    }

}