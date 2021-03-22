#pragma once

#include <vector>
#include <memory>
#include "mvStep.h"

namespace Marvel {

	class mvGraphics;
	class mvDrawable;

	class mvTechnique
	{

	public:

		void addStep(mvStep step);

	private:

		std::vector<mvStep> m_steps;

	};


}