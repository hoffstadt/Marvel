#pragma once

#include <string>
#include <memory>
#include "mvBufferResource.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvBindable;

	class mvPassProduct
	{

	public:

		mvPassProduct(const std::string& name)
			:
			m_name(name)
		{

		}

		virtual ~mvPassProduct() = default;
		virtual std::shared_ptr<mvBindable> getBindable() { return nullptr; };
		virtual std::shared_ptr<mvBufferResource> getBuffer() { return nullptr; };

		const std::string& getName() const { return m_name; }

	protected:

		std::string m_name;
	};

	template<typename T>
	class mvBindPassProduct : public mvPassProduct
	{

	public:

		mvBindPassProduct(const std::string& name, std::shared_ptr<T>& resource)
			:
			mvPassProduct(name),
			m_resource(resource)
		{

		}

		std::shared_ptr<mvBindable> getBindable() override
		{
			return m_resource;
		};

	private:
		std::shared_ptr<T>& m_resource;
	};

	template<typename T>
	class mvBufferPassProduct : public mvPassProduct
	{

	public:

		mvBufferPassProduct(const std::string& name, std::shared_ptr<T>& resource)
			:
			mvPassProduct(name),
			m_resource(resource)
		{

		}

		std::shared_ptr<mvBufferResource> getBuffer() override
		{
			return m_resource;
		}

	private:
		std::shared_ptr<T>& m_resource;
	};

}