#pragma once

namespace Marvel {

    //-----------------------------------------------------------------------------
    // forward declarations
    //-----------------------------------------------------------------------------
    class mvGraphics;
    class mvDrawable;

    //-----------------------------------------------------------------------------
    // mvBindable
    //-----------------------------------------------------------------------------
    class mvBindable
    {

        // to set parent
        friend class mvStep;

    public:

        virtual ~mvBindable() = default;

        virtual void bind(mvGraphics& graphics) = 0;

    protected:

        const mvDrawable* m_parent;

    };

}