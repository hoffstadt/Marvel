#pragma once

#include <string>
#include <memory>
#include <assert.h>
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
		virtual std::shared_ptr<mvBindable> getBindable() { assert(false); return nullptr; };
		virtual std::shared_ptr<mvBufferResource> getBuffer() { assert(false); return nullptr; };

		const std::string& getName() const { return m_name; }
		bool isLinked() const { return m_linked; }

	protected:

		std::string m_name;
		bool        m_linked = false;
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
			if (m_linked)
			{
				assert(false && "already bound");
			}
			m_linked = true;
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
			if (m_linked)
			{
				assert(false && "already bound");
			}
			m_linked = true;
			return m_resource;
		}

	private:
		std::shared_ptr<T>& m_resource;
	};

}