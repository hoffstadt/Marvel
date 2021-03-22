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

    public:

        virtual ~mvBindable() = default;

        virtual void bind(mvGraphics& graphics) = 0;

        void setParent(const mvDrawable* parent) { m_parent = parent; }

    protected:

        const mvDrawable* m_parent;

    };

}