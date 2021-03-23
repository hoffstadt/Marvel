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

		void addBindable(std::shared_ptr<mvBindable> bindable);
		void bind       (mvGraphics& graphics, const mvDrawable* parent) const;
		void submit     (const mvDrawable& drawable) const;
		void link       (mvRenderGraph& graph);


	private:

		std::vector<std::shared_ptr<mvBindable>> m_bindables;
		mvPass*                                  m_pass = nullptr;
		std::string                              m_targetPass;

	};


}