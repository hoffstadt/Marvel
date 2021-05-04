#pragma once

#include <vector>
#include <memory>
#include "mvStep.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvDrawable;
	class mvRenderGraph;

	//-----------------------------------------------------------------------------
	// mvTechnique
	//-----------------------------------------------------------------------------
	class mvTechnique
	{

	public:

		// links steps to their target passes
		void link(mvRenderGraph& graph);

		// propagates drawable to all steps to submit their jobs
		// to their target passes
		void submit(const mvDrawable& drawable) const;
	
		void addStep(mvStep step);

	private:

		std::vector<mvStep> m_steps;

	};


}