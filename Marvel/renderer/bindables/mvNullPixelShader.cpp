#include "mvNullPixelShader.h"
#include <d3dcompiler.h>
#include <assert.h>
#include "mvMarvelUtils.h"
#include "mvGraphics.h"

namespace Marvel {

    mvNullPixelShader::mvNullPixelShader(mvGraphics& graphics)
    {


    }

    void mvNullPixelShader::bind(mvGraphics& graphics)
    {
        graphics.getContext()->PSSetShader(nullptr, nullptr, 0u);
    }


}