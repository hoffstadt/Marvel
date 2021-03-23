#pragma once

#include <vector>
#include <memory>
#include "mvBindable.h"

namespace Marvel {

	class mvGraphics;
	class mvDrawable;
	class mvPass;

	class mvStep
	{

	public:

		void addBindable(std::shared_ptr<mvBindable> bindable);
		void bind       (mvGraphics& graphics, const mvDrawable* parent) const;
		void submit     (const mvDrawable& drawable) const;
		void setPass    (mvPass* pass) const;


	private:

		std::vector<std::shared_ptr<mvBindable>> m_bindables;
		mutable mvPass*                          m_pass;

	};


}