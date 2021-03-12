#include "mvNullGeometryShader.h"
#include "mvGraphics.h"

namespace Marvel {

    mvNullGeometryShader::mvNullGeometryShader(mvGraphics& graphics) 
    {
    }

    void mvNullGeometryShader::bind(mvGraphics& graphics)
    {
        graphics.getContext()->GSSetShader(nullptr, nullptr, 0);
    }

}