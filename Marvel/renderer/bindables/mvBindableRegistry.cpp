#include "mvCommonBindables.h"
#include <assert.h>
#include "mvGraphics.h"

namespace Marvel {

	std::unordered_map<std::string, std::shared_ptr<mvBindable>> mvBindableRegistry::m_bindables;

	void mvBindableRegistry::Initialize(mvGraphics& graphics)
	{
	}

	std::shared_ptr<mvBindable> mvBindableRegistry::GetBindable(const std::string& ID)
	{
		if (m_bindables.find(ID) == m_bindables.end())
		{
			//assert(false);
			return nullptr;
		}

		return m_bindables.at(ID);
	}

	std::shared_ptr<mvBindable> mvBindableRegistry::AddBindable(const std::string& ID, std::shared_ptr<mvBindable> bindable)
	{
		assert(m_bindables.find(ID) == m_bindables.end());

		m_bindables.try_emplace(ID, bindable);

		return bindable;
	}

}