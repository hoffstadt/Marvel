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
    class mvSampler : public mvBindable
    {

    public:

        enum class Type
        {
            Anisotropic,
            Bilinear,
            Point,
        };

        static std::shared_ptr<mvSampler> Request(mvGraphics& graphics, Type type, bool reflect, UINT slot);
        static std::string                GenerateUniqueIdentifier(Type type, bool reflect, UINT slot);

    public:

        mvSampler(mvGraphics& graphics, Type type = Type::Anisotropic, bool reflect = false, UINT slot = 0u);

        void bind(mvGraphics& graphics) override;
        std::string getUniqueIdentifier() const override;

    private:

        mvComPtr<ID3D11SamplerState> m_samplerState;
        Type                         m_type;
        bool                         m_reflect;
        UINT                         m_slot;

    };

}