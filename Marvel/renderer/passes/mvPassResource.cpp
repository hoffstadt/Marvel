#include "mvPassResource.h"

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
		return m_pass;
	}

	const std::string& mvPassResource::getProduct() const
	{
		return m_product;
	}

	void mvPassResource::setTarget(const std::string& pass, const std::string& product)
	{
		m_pass = pass;
		m_product = product;
	}
}