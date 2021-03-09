#pragma once
#include <d3d11.h>
#include "mvBindable.h"
#include "mvComPtr.h"

namespace Marvel {

    class mvGraphics;

    class mvSampler : public mvBindable
    {

    public:

        mvSampler(mvGraphics& graphics);

        void bind(mvGraphics& graphics) override;

    private:

        mvComPtr<ID3D11SamplerState> m_samplerState;

    };

}