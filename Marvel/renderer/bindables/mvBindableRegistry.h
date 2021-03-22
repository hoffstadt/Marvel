#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "mvBindable.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvBindableRegistry
	//-----------------------------------------------------------------------------
	class mvBindableRegistry
	{

	public:

		static void                        Initialize(mvGraphics& graphics);
		static std::shared_ptr<mvBindable> GetBindable(const std::string& ID);
		static std::shared_ptr<mvBindable> AddBindable(const std::string& ID, std::shared_ptr<mvBindable> bindable);

	private:

		static std::unordered_map<std::string, std::shared_ptr<mvBindable>> m_bindables;

	};

}