#pragma once

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvDrawable;
	class mvGraphics;
	class mvStep;

	//-----------------------------------------------------------------------------
	// mvJob
	//-----------------------------------------------------------------------------
	class mvJob
	{

	public:

		mvJob(const mvStep* step, const mvDrawable* drawable);

		void execute(mvGraphics& graphics) const;

	private:

		const mvDrawable* m_drawable;
		const mvStep*     m_step;
	};
}