#pragma once

namespace Marvel
{

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvBufferResource
	{

	public:

		virtual ~mvBufferResource() = default;
		virtual void bindAsBuffer(mvGraphics&) = 0;
		virtual void bindAsBuffer(mvGraphics&, mvBufferResource*) = 0;
		virtual void clear(mvGraphics&) = 0;
	};
}