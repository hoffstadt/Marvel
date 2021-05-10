#pragma once

#include <string>
#include <memory>
#include <assert.h>

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

        virtual std::string getUniqueIdentifier() const
        {
            //assert(false);
            return "";
        }

    protected:

        const mvDrawable* m_parent;

    };

}