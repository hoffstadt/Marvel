#pragma once

#include <vector>
#include <memory>
#include "mvBindable.h"

namespace Marvel {

	class mvGraphics;
	class mvDrawable;

	class mvStep
	{

	public:

		void addBindable(std::shared_ptr<mvBindable> bindable);
		void bind       (mvGraphics& graphics, const mvDrawable* parent) const;

	private:

		std::vector<std::shared_ptr<mvBindable>> m_bindables;

	};


}