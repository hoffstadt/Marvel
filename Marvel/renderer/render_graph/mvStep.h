#pragma once

#include <vector>
#include <memory>
#include <string>
#include "mvBindable.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;
	class mvDrawable;
	class mvPass;
	class mvRenderGraph;

	//-----------------------------------------------------------------------------
	// mvStep
	//-----------------------------------------------------------------------------
	class mvStep
	{

	public:

		mvStep(const std::string& targetPass);

		// sets the pass this step targets
		void link(mvRenderGraph& graph);

		// adds a job to target pass
		void submit(const mvDrawable& drawable) const;

		// binds all step specific bindables to the pipeline
		void bind(mvGraphics& graphics, const mvDrawable* parent) const;

		void addBindable(std::shared_ptr<mvBindable> bindable);
		
	private:

		std::vector<std::shared_ptr<mvBindable>> m_bindables;
		mvPass*                                  m_pass = nullptr;
		std::string                              m_targetPass;

	};


}