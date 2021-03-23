#pragma once

#include <vector>
#include <memory>
#include <string>
#include "mvJob.h"
#include "mvBindable.h"

namespace Marvel{

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvPass
	//-----------------------------------------------------------------------------
	class mvPass
	{

	public:

		mvPass(const std::string& name);

		virtual void execute(mvGraphics& graphics) const = 0;

		void               addJob     (mvJob job);
		void               reset      ();
		void               addBindable(std::shared_ptr<mvBindable> bindable);
		const std::string& getName    () const;

	protected:

		std::vector<mvJob>                       m_jobs;
		std::vector<std::shared_ptr<mvBindable>> m_bindables;
		std::string                              m_name;
	};
}