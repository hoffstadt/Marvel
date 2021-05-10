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

		template<class T, typename...Params>
		static std::shared_ptr<T> Request(mvGraphics& graphics, Params&&...p)
		{
			static_assert(std::is_base_of<mvBindable, T>::value, "Can only resolve classes derived from Bindable");
			return T::Request(graphics, std::forward<Params>(p)...);
		}

	private:

		static std::unordered_map<std::string, std::shared_ptr<mvBindable>> m_bindables;

	};

}