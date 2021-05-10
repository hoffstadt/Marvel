#include "mvPassResource.h"
#include <assert.h>

namespace Marvel {

	mvPassResource::mvPassResource(const std::string& name)
		:
		m_name(name)
	{

	}

	const std::string& mvPassResource::getName() const 
	{ 
		return m_name; 
	}

	const std::string& mvPassResource::getPass() const
	{
		assert(m_prelinked && "not linked yet");
		return m_pass;
	}

	const std::string& mvPassResource::getProduct() const
	{
		assert(m_prelinked && "not linked yet");
		return m_product;
	}

	void mvPassResource::setTarget(const std::string& pass, const std::string& product)
	{
		assert(!m_prelinked && "already linked");

		m_pass = pass;
		m_product = product;

		m_prelinked = true;
	}
}