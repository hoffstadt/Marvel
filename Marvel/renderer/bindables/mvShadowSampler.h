#pragma once
#include <d3d11.h>
#include "mvBindable.h"
#include "mvComPtr.h"

namespace Marvel {

    //-----------------------------------------------------------------------------
    // forward declarations
    //-----------------------------------------------------------------------------
    class mvGraphics;

    //-----------------------------------------------------------------------------
    // mvSampler
    //-----------------------------------------------------------------------------
    class mvShadowSampler : public mvBindable
    {

    public:

        mvShadowSampler(mvGraphics& graphics);

        void bind(mvGraphics& graphics) override;

    private:

        mvComPtr<ID3D11SamplerState> m_samplerState;

    };

}