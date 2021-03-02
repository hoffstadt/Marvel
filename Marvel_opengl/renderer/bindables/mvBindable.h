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

        virtual void bind(mvGraphics& graphics) = 0;
        virtual void unbind(mvGraphics& graphics) {};

        void setParent(const mvDrawable* parent) { m_parent = parent; }

    protected:

        const mvDrawable* m_parent;

    };

}